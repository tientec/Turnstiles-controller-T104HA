/*******************************************************************************
********************************************************************************
* File Name          : sys.c
* Author             : Tomasz Nowak
* Version            : V1.00a
* Date               : 27/01/2012
* Description        : System functions handling
********************************************************************************
*******************************************************************************/


/*******************************************************************************
* Header Files
*******************************************************************************/
#include  "includes.h"


/*******************************************************************************
* Local Preprocessor #define Constants
*******************************************************************************/

/*******************************************************************************
* Local Preprocessor #define MACROS
*******************************************************************************/


/*******************************************************************************
* Local Type Declarations
*******************************************************************************/

/*******************************************************************************
* Exported Const Object Definitions
*******************************************************************************/


/*******************************************************************************
* Local Object Definitions
*******************************************************************************/
_Timers System_Timers;

_System_Timers Inputs_Timer;
_System_Timers Keyboard_Timer;
_System_Timers Startup_Timer;
_System_Timers Encoder_Timer;
_System_Timers Pulse_Leds_Timer;
_System_Timers Screen_Reload_Timer;
_System_Timers Temperature_Timer;
_System_Timers Trans_Timer;

_Keyboard Keyboard;
_Keyboard Inputs;

volatile uint16_t Arm_Pos;

volatile uint32_t Interval;
volatile uint32_t Interval_Sec;

volatile uint16_t Inputs_Level = 0;
volatile uint16_t Inputs_Level_Test = 0;

uint16_t Fast_Pass_Marker = 0;
uint16_t Free_Exit_Marker = 2;
uint16_t Free_Exit_Mem = 2;

const uint8_t * SYS1 = "SYS-OK/";

uint16_t Zbyszek = 0;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/


/*******************************************************************************
* Local Inline Function Definitions and Function-Like Macros
*******************************************************************************/


/*******************************************************************************
* Function Definitions
*******************************************************************************/

/*******************************************************************************
* Function Name  : Sys_Init
* Description    : System initialization
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Sys_Init ( void )
{
	System_Timer_Setting ( &Startup_Timer.Timer, STARTUP_TIME, T_10msec );
	while ( System_Timer_Check ( &Startup_Timer.Timer, T_10msec ) == FALSE );
	System_Timer_Setting ( &Keyboard_Timer.Timer, KBD_SAMPLE_TIME, T_10msec );
    System_Timer_Setting ( &Encoder_Timer.Timer, ENCODER_SAMPLE_TIME, T_10msec );
	System_Timer_Setting ( &Pulse_Leds_Timer.Timer, TIME_05_SEC, T_10msec );
    System_Timer_Setting ( &Inputs_Timer.Timer, INPUTS_SAMPLE_TIME, T_10msec );
    System_Timer_Setting ( &Screen_Reload_Timer.Timer, SCREEN_RELOAD_TIME, T_10msec );
    System_Timer_Setting ( &Temperature_Timer.Timer, T_SAMPLE_TIME, T_10msec );

    Stop_Angles_Set();

    Test_Cntr = 0;
    
    if ( Service_Variables.Serv_Vars.Marker_Blokady == STER_ZABLOKOWANY )
    {
        Machine_State = SW_LOCKED;
    }
    else if ( Service_Variables.Serv_Vars.Marker_Blokady == STER_BLOKADA_CALKOWITA )
    {
        Machine_State = SW_LOCKED;    
    }
    else
    {
        if ( Controller_Variables.Controller_Vars.Test_Marker == GO_2_TEST_PROGRAM )
        {
            Controller_Variables.Controller_Vars.Test_Status = 0;
            Machine_State = MANUAL_TEST;             
        }
        else
        {
            if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
            {
                if ( Service_Variables.Serv_Vars.Typ_Bramki < 2 )
                {
                    Machine_State = SELF_TEST;     
                }
                else
                {
                    Machine_State = STANDBY;     
                }
            }
            else//Inductive
            {
                Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 0;//STOP            
                Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET + 10;            
                Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET + 10;

                if ( GPIO_ReadInputDataBit(PORT_IN2, PIN_IN2) == 1 )
                    Controller_Variables.Controller_Vars.Sensor_Edge_Flag = 1;
                else
                    Controller_Variables.Controller_Vars.Sensor_Edge_Flag = 0;

                Machine_State = STANDBY;             
            }
        }
    }

    Lcd_Write_Text ( SYS1 );           

}

/*******************************************************************************
* Function Name  : Sys_sys
* Description    : System functions handling
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Sys_sys ( void )
{
    Arm_Pos = ENC_OFFSET + Encoder_Pos_Read();
    Controller_Variables.Controller_Vars.Encoder_Var = Arm_Pos - ENC_OFFSET;

    Other_Variables.Other_Variables_Tab16[0] = Persons_Counter_Fwd;
    Other_Variables.Other_Variables_Tab16[1] = Persons_Counter_Bck;

    Analog_Measure();
    Keyboard_Handling();
    Inputs_Handling();
    RTC_Handler();
    WWDT_Handler();
    Sap_Handling();
    Internal_Fan_Control();
    Periods_Handling();
    Back_Signals_Control();
    Output_1_Control();
    Output_2_Control();
    Output_3_Control();
    Output_4_Control();
    Input_Output_Test();
}

/*******************************************************************************
* Function Name  : System_Timer_Setting
* Description    : Setting timer to count to programmed value
* Input1         : Timer_Counter - pointer to 16 bit register
* Input2         : Time_Value - Set time interval to count
* Input3         : Enable_Flag - pointer to BOOL variable
* Input4         : Sys_Tmr_Intervals - base time interval
* Output         : None
* Return         : None
*******************************************************************************/
void System_Timer_Setting ( uint32_t * Timer_Counter, uint16_t Time_Value, _Sys_Tmr_Intervals Sys_Tmr_Intervals )
{
	*Timer_Counter = System_Timers.Time_Table[Sys_Tmr_Intervals] + Time_Value;
}

/*******************************************************************************
* Function Name  : System_Timer_Check
* Description    : Checking if programed timer has reached programmed value
* Input1         : Timer_Counter - pointer to 16 bit register
* Input2         : Enable_Flag - pointer to BOOL variable
* Input3         : Sys_Tmr_Intervals - base time interval
* Output         : None
* Return         : TRUE - Time match / FALSE Time no match
*******************************************************************************/
uint8_t System_Timer_Check ( uint32_t * Timer_Counter, _Sys_Tmr_Intervals Sys_Tmr_Intervals )
{
		if ( *Timer_Counter == System_Timers.Time_Table[Sys_Tmr_Intervals] )
		{
			return TRUE;	
		}
		else
		{
			return FALSE;
		}
}

/*******************************************************************************
* Function Name  : Keyboard_Handling
* Description    : Checking the keys
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Keyboard_Handling ( void )
{
	if ( System_Timer_Check ( &Keyboard_Timer.Timer, T_10msec ) == TRUE )
	{
        Keyboard.Keyboard_State = 0;
   

        if ( GPIO_ReadInputDataBit(KBD_PORT_ENTER, KBD_RELEASED_ENTER) == 0 )    
            Keyboard.Keyboard_State |= KEY_ENTER;
        if ( GPIO_ReadInputDataBit(KBD_PORT_ESCAPE, KBD_RELEASED_ESCAPE) == 0 )    
            Keyboard.Keyboard_State |= KEY_ESCAPE;
        if ( GPIO_ReadInputDataBit(KBD_PORT_LEFT, KBD_RELEASED_LEFT) == 0 )    
            Keyboard.Keyboard_State |= KEY_LEFT;
        if ( GPIO_ReadInputDataBit(KBD_PORT_UP, KBD_RELEASED_UP) == 0 )    
            Keyboard.Keyboard_State |= KEY_UP;
        if ( GPIO_ReadInputDataBit(KBD_PORT_RIGHT, KBD_RELEASED_RIGHT) == 0 )    
            Keyboard.Keyboard_State |= KEY_RIGHT;
        if ( GPIO_ReadInputDataBit(KBD_PORT_DOWN, KBD_RELEASED_DOWN) == 0 )    
            Keyboard.Keyboard_State |= KEY_DOWN;
    
    
        if ( Keyboard.Keyboard_Memory == 0 && Keyboard.Keyboard_Flag == 0 )
        {
            if ( !(GPIO_ReadInputDataBit(KBD_PORT_ENTER, KBD_RELEASED_ENTER)) )
            {
                Keyboard.Keyboard_Memory |= KEY_ENTER;
                Keyboard.Keyboard_Flag = 1;
            }            
            else if ( !(GPIO_ReadInputDataBit(KBD_PORT_ESCAPE, KBD_RELEASED_ESCAPE)) )
            {
                Keyboard.Keyboard_Memory |= KEY_ESCAPE;
                Keyboard.Keyboard_Flag = 1;
            }                 
            else if ( !(GPIO_ReadInputDataBit(KBD_PORT_LEFT, KBD_RELEASED_LEFT)) )
            {
                Keyboard.Keyboard_Memory |= KEY_LEFT;
                Keyboard.Keyboard_Flag = 1;
            }                 
            else if ( !(GPIO_ReadInputDataBit(KBD_PORT_UP, KBD_RELEASED_UP)) )
            {
                Keyboard.Keyboard_Memory |= KEY_UP;
                Keyboard.Keyboard_Flag = 1;
            }                 
            else if ( !(GPIO_ReadInputDataBit(KBD_PORT_RIGHT, KBD_RELEASED_RIGHT)) )
            {
                Keyboard.Keyboard_Memory |= KEY_RIGHT;
                Keyboard.Keyboard_Flag = 1;
            }                 
            else if ( !(GPIO_ReadInputDataBit(KBD_PORT_DOWN, KBD_RELEASED_DOWN)) )
            {            
                Keyboard.Keyboard_Memory |= KEY_DOWN;
                Keyboard.Keyboard_Flag = 1;   
            }
        }

		System_Timer_Setting ( &Keyboard_Timer.Timer, KBD_SAMPLE_TIME, T_10msec );
    }

}

/*******************************************************************************
* Function Name  : Keyboard_Handling
* Description    : Checking the keys
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Inputs_Handling ( void )
{
	if ( System_Timer_Check ( &Inputs_Timer.Timer, T_10msec ) == TRUE )
	{
        Inputs.Keyboard_State = 0;
   
//Edge handling
        if ( GPIO_ReadInputDataBit(PORT_IN4, PIN_IN4) == 0 )    
            Inputs.Keyboard_State |= INPUT_FORWARD;
        if ( GPIO_ReadInputDataBit(PORT_IN5, PIN_IN5) == 0 )    
            Inputs.Keyboard_State |= INPUT_BACK;
        if ( GPIO_ReadInputDataBit(PORT_IN8, PIN_IN8) == 0 )    
            Inputs.Keyboard_State |= INPUT_TEST;

        
        
//level handling         
        if ( GPIO_ReadInputDataBit(PORT_IN1, PIN_IN1) == 1 )
            Inputs_Level |= IND_SENSOR1;
        else
            Inputs_Level &= ~IND_SENSOR1;

        if ( GPIO_ReadInputDataBit(PORT_IN2, PIN_IN2) == 1 )
            Inputs_Level |= IND_SENSOR2;
        else
            Inputs_Level &= ~IND_SENSOR2;

        if ( GPIO_ReadInputDataBit(PORT_IN3, PIN_IN3) == 1 )
            Inputs_Level |= IND_SENSOR3;
        else
            Inputs_Level &= ~IND_SENSOR3;

        if ( GPIO_ReadInputDataBit(PORT_IN4, PIN_IN4) == 0 )
            Inputs_Level |= FORWARD_ENABLE;
        else
            Inputs_Level &= ~FORWARD_ENABLE;

        if ( GPIO_ReadInputDataBit(PORT_IN5, PIN_IN5) == 0 )
            Inputs_Level |= BACK_ENABLE;
        else
            Inputs_Level &= ~BACK_ENABLE;
        
        if ( GPIO_ReadInputDataBit(PORT_IN6, PIN_IN6) == 0 )
            Inputs_Level &= ~SAP_INPUT;
        else
            Inputs_Level |= SAP_INPUT;

        if ( GPIO_ReadInputDataBit(PORT_IN7, PIN_IN7) != 0 )
            Inputs_Level &= ~LIGHT_SENSOR_INPUT;
        else
            Inputs_Level |= LIGHT_SENSOR_INPUT;

//Level handling for test function
        if ( GPIO_ReadInputDataBit(PORT_IN1, PIN_IN1) == 1 )
            Inputs_Level_Test |= INPUT1;
        else
            Inputs_Level_Test &= ~INPUT1;

        if ( GPIO_ReadInputDataBit(PORT_IN2, PIN_IN2) == 1 )
            Inputs_Level_Test |= INPUT2;
        else
            Inputs_Level_Test &= ~INPUT2;

        if ( GPIO_ReadInputDataBit(PORT_IN3, PIN_IN3) == 1 )
            Inputs_Level_Test |= INPUT3;
        else
            Inputs_Level_Test &= ~INPUT3;
     
        if ( GPIO_ReadInputDataBit(PORT_IN4, PIN_IN4) == 0 )
            Inputs_Level_Test |= INPUT4;
        else
            Inputs_Level_Test &= ~INPUT4;

        if ( GPIO_ReadInputDataBit(PORT_IN5, PIN_IN5) == 0 )
            Inputs_Level_Test |= INPUT5;
        else
            Inputs_Level_Test &= ~INPUT5;
        
       if ( GPIO_ReadInputDataBit(PORT_IN6, PIN_IN6) == 0 )
            Inputs_Level_Test |= INPUT6;
        else
            Inputs_Level_Test &= ~INPUT6;
        
        if ( GPIO_ReadInputDataBit(PORT_IN7, PIN_IN7) == 0 )
            Inputs_Level_Test |= INPUT7;
        else
            Inputs_Level_Test &= ~INPUT7;
        
        if ( GPIO_ReadInputDataBit(PORT_IN8, PIN_IN8) == 0 )
            Inputs_Level_Test |= INPUT8;
        else
            Inputs_Level_Test &= ~INPUT8;
        
//End of level handling for test
        
        if ( Inputs.Keyboard_Memory == 0 && Inputs.Keyboard_Flag == 0 )
        {
            if ( !(GPIO_ReadInputDataBit(PORT_IN4, PIN_IN4)) )
            {
                Inputs.Keyboard_Memory |= INPUT_FORWARD;
                Inputs.Keyboard_Flag = 1;
            }            
            if ( !(GPIO_ReadInputDataBit(PORT_IN5, PIN_IN5)) )
            {
                Inputs.Keyboard_Memory |= INPUT_BACK;
                Inputs.Keyboard_Flag = 1;
            }
            if ( !(GPIO_ReadInputDataBit(PORT_IN8, PIN_IN8)) )
            {
                Inputs.Keyboard_Memory |= INPUT_TEST;
                Inputs.Keyboard_Flag = 1;
            }         
        }

		System_Timer_Setting ( &Inputs_Timer.Timer, Service_Variables.Serv_Vars.Serwis_018, T_10msec );
    }

    if ( Controller_Variables.Controller_Vars.Sensor_Edge_Flag == 0 )
    {
//Rising edge detection
        if ( Inputs_Level & IND_SENSOR3 )
        {
            if ( Inputs_Level & IND_SENSOR2 )
            {
                Controller_Variables.Controller_Vars.Sensor_Counter++;
                Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 1;//FORWARD            
            }
            else
            {
                Controller_Variables.Controller_Vars.Sensor_Counter--;
                Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 2;//BACK 
            }
        
        
            Controller_Variables.Controller_Vars.Sensor_Edge_Flag = 1;
        }
    }
//Falling edge detection
    else if ( Controller_Variables.Controller_Vars.Sensor_Edge_Flag == 1 )
    {
        if ( !(Inputs_Level & IND_SENSOR3) )
        {
            if ( !(Inputs_Level & IND_SENSOR2) )
            {
                Controller_Variables.Controller_Vars.Sensor_Counter++;
                Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 1;//FORWARD            
            }
            else
            {
                Controller_Variables.Controller_Vars.Sensor_Counter--;
                Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 2;//BACK 
            }

            Controller_Variables.Controller_Vars.Sensor_Edge_Flag = 0;
        }    
    }
}
/*******************************************************************************
* Function Name  : Encoder_Pos_Read
* Description    : Software SPI for reading the position from an encoder
* Input          : None
* Output         : None
* Return         : Position of encoder in arbitrary units ( 0 to 1023)
*******************************************************************************/
uint16_t Encoder_Pos_Read ( void )
{
	static uint16_t Position = 0;

    if ( System_Timer_Check ( &Encoder_Timer.Timer, T_10msec ) == TRUE )
	{
        GPIO_ResetBits(SPI_CS_ENCODER_PORT, SPI_CS_ENCODER_PIN);      
    	Software_Delay ( 3 );

        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, 0x0000);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
        Position = SPI_I2S_ReceiveData(SPI1);
	    Software_Delay ( 3 );
 
        GPIO_SetBits(SPI_CS_ENCODER_PORT, SPI_CS_ENCODER_PIN);      

        Position >>= 5;
		Position &= 0x03FF;

        System_Timer_Setting ( &Encoder_Timer.Timer, ENCODER_SAMPLE_TIME, T_10msec );
	}

	return Position;
}

/*******************************************************************************
* Function Name  : Sap_Handling
* Description    : FSystem handling during sap sibnal - alert signal
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Sap_Handling ( void )
{
    if ( Machine_State != MANUAL_TEST )
    {
        if ( Service_Variables.Serv_Vars.Sap == 0 )//Yes - Normally opened
        {
            if ( !(Inputs_Level & SAP_INPUT) )
            {
                if ( Service_Variables.Serv_Vars.Sap_Type == 0 ) //Both directions
                {
                    Machine_State = SAP_ALERT;        
                }
                else if ( Service_Variables.Serv_Vars.Sap_Type == 1 ) //Left direction
                {
                    Machine_State = SAP_ALERT_FWD;        
                }
                else if ( Service_Variables.Serv_Vars.Sap_Type == 2 ) //Right direction
                {
                    Machine_State = SAP_ALERT_BCK;                  
                }        
            }
            else
            {
                if ( Machine_State == SAP_ALERT ||
                    Machine_State == SAP_ALERT_FWD ||
                    Machine_State == SAP_ALERT_BCK )
                {
                    Machine_State = STANDBY;
                    STANDBY_PWM_WAIT = FALSE;
                }
            }
        }
        else if( Service_Variables.Serv_Vars.Sap == 1 )//YES_NORMALLY closed
        {
            if ( Inputs_Level & SAP_INPUT )
            {
                if ( Service_Variables.Serv_Vars.Sap_Type == 0 ) //Both directions
                {
                    Machine_State = SAP_ALERT;        
                }
                else if ( Service_Variables.Serv_Vars.Sap_Type == 1 ) //Left direction
                {
                    Machine_State = SAP_ALERT_FWD;        
                }
                else if ( Service_Variables.Serv_Vars.Sap_Type == 2 ) //Right direction
                {
                    Machine_State = SAP_ALERT_BCK;                  
                }  
            }
            else
            {
                if ( Machine_State == SAP_ALERT ||
                    Machine_State == SAP_ALERT_FWD ||
                    Machine_State == SAP_ALERT_BCK )
                {
                    Machine_State = STANDBY;
                    STANDBY_PWM_WAIT = FALSE;
                }
            }
        }
        else
        {
            if ( Machine_State == SAP_ALERT ||
                 Machine_State == SAP_ALERT_FWD ||
                 Machine_State == SAP_ALERT_BCK )
            {
                Machine_State = STANDBY;
                STANDBY_PWM_WAIT = FALSE;
            }
        }
    }
}

/*******************************************************************************
* Function Name  : Stop_Angles_Set
* Description    : sets three of four angles of arm
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Stop_Angles_Set ( void )
{
    static uint16_t Encoder_Tmp;

    Encoder_Tmp = Controlling_Variables.Ctrl_Vars.Encoder;
    Encoder_Tmp += ENC_OFFSET;

    if ( Encoder_Tmp > (ENC_OFFSET+ENC_MAX) )
    {
        Encoder_Tmp = ENC_OFFSET + 500;
    }

    if ( Service_Variables.Serv_Vars.KOlowrotek == 1 )
    {
		if ( Encoder_Tmp > (ENC_OFFSET+768) && Encoder_Tmp <= (ENC_OFFSET+1023) )
		{
			Encoder_Tmp -= 256;
		}

		if ( Encoder_Tmp > (ENC_OFFSET+512) && Encoder_Tmp <= (ENC_OFFSET+768) )
		{
			Encoder_Tmp -= 256;		
		}  

		if ( Encoder_Tmp > (ENC_OFFSET+256) && Encoder_Tmp <= (ENC_OFFSET+512) )
		{
			Encoder_Tmp -= 256;		
		}  
    
		Arms_Stop_Pos.Min_Pos1 = Encoder_Tmp-Service_Variables.Serv_Vars.Serwis_010;
		Arms_Stop_Pos.Max_Pos1 = Encoder_Tmp+Service_Variables.Serv_Vars.Serwis_010;
		Arms_Stop_Pos.Min_Pos2 = Encoder_Tmp-Service_Variables.Serv_Vars.Serwis_010+256;
		Arms_Stop_Pos.Max_Pos2 = Encoder_Tmp+Service_Variables.Serv_Vars.Serwis_010+256;
		Arms_Stop_Pos.Min_Pos3 = Encoder_Tmp-Service_Variables.Serv_Vars.Serwis_010+512;
		Arms_Stop_Pos.Max_Pos3 = Encoder_Tmp+Service_Variables.Serv_Vars.Serwis_010+512;
		Arms_Stop_Pos.Min_Pos4 = Encoder_Tmp-Service_Variables.Serv_Vars.Serwis_010+768;
		Arms_Stop_Pos.Max_Pos4 = Encoder_Tmp+Service_Variables.Serv_Vars.Serwis_010+768;
    }
    else
    {
		if ( Encoder_Tmp > (ENC_OFFSET+ENC_MAX) )
		{
			Encoder_Tmp = ENC_OFFSET + 500;
		}

		if ( Encoder_Tmp > (ENC_OFFSET+681) && Encoder_Tmp <= (ENC_OFFSET+1023) )
		{
			Encoder_Tmp -= 341;
		}

		if ( Encoder_Tmp > (ENC_OFFSET+341) && Encoder_Tmp <= (ENC_OFFSET+681) )
		{
			Encoder_Tmp -= 341;		
		}   

		Arms_Stop_Pos.Min_Pos1 = Encoder_Tmp-Service_Variables.Serv_Vars.Serwis_010;
		Arms_Stop_Pos.Max_Pos1 = Encoder_Tmp+Service_Variables.Serv_Vars.Serwis_010;
		Arms_Stop_Pos.Min_Pos2 = Encoder_Tmp-Service_Variables.Serv_Vars.Serwis_010+341;
		Arms_Stop_Pos.Max_Pos2 = Encoder_Tmp+Service_Variables.Serv_Vars.Serwis_010+341;
		Arms_Stop_Pos.Min_Pos3 = Encoder_Tmp-Service_Variables.Serv_Vars.Serwis_010+682;
		Arms_Stop_Pos.Max_Pos3 = Encoder_Tmp+Service_Variables.Serv_Vars.Serwis_010+682;
		Arms_Stop_Pos.Min_Pos4 = Encoder_Tmp-Service_Variables.Serv_Vars.Serwis_010+682;
		Arms_Stop_Pos.Max_Pos4 = Encoder_Tmp+Service_Variables.Serv_Vars.Serwis_010+682;
    }
}

/*******************************************************************************
* Function Name  : Temperature_Measure
* Description    : Measuring the internal temperature
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Analog_Measure ( void )
{
    static const int16_t T25 = 1750;
    static const int16_t Avg_Slope = 5;

	if ( System_Timer_Check ( &Temperature_Timer.Timer, T_10msec ) == TRUE )
	{    
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));
        DMA_ClearFlag(DMA1_FLAG_TC1);

        Controller_Variables.Controller_Vars.AD_7 = ADC_ConvertedValueTab[0];
        Controller_Variables.Controller_Vars.AD_16 = ADC_ConvertedValueTab[1];

        Controller_Variables.Controller_Vars.Internal_Temperature = (int16_t)(T25-Controller_Variables.Controller_Vars.AD_16)/Avg_Slope + 25;
  
        System_Timer_Setting ( &Temperature_Timer.Timer, T_SAMPLE_TIME, T_10msec );
    }
}


/*******************************************************************************
* Function Name  : Heat_Cool
* Description    : Heater and ventilation handling
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Internal_Fan_Control ( void )
{
    if ( Machine_State != MANUAL_TEST )
    {
//FAN HANDLING
        if ( Controller_Variables.Controller_Vars.Internal_Temperature > (int16_t)Controlling_Variables.Ctrl_Vars.Vent_Max )
        {
            if ( Controlling_Variables.Ctrl_Vars.Vent_Marker == 0 )
            {
                GPIO_SetBits(FAN_PORT, FAN_PIN );
            }
            else
            {
                GPIO_ResetBits(FAN_PORT, FAN_PIN);
            }
        }
        else if ( Controller_Variables.Controller_Vars.Internal_Temperature < (int16_t)Controlling_Variables.Ctrl_Vars.Vent_Min )
        {
            GPIO_ResetBits(FAN_PORT, FAN_PIN);
        }
    }
}

/*******************************************************************************
* Function Name  : Periods_Handling
* Description    : Free exit and fast pass periuods handling
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Periods_Handling ( void )
{
    uint32_t Period_1;
    uint32_t Period_2;
    uint32_t Period_3;
    uint32_t Period_4;

//FREE EXIT HANDLING
    if ( Controlling_Variables.Ctrl_Vars.Exit_Var == 1 ) //Input on edge detection
    {
        if ( Service_Variables.Serv_Vars.W_wyjscie == 2 )
        {
            Period_1 = ( Service_Variables.Serv_Vars.Free_Hour_11 * 3600) + ( Service_Variables.Serv_Vars.Free_Minute_11 * 60 );
            Period_2 = ( Service_Variables.Serv_Vars.Free_Hour_12 * 3600) + ( Service_Variables.Serv_Vars.Free_Minute_12 * 60 );
            Period_3 = ( Service_Variables.Serv_Vars.Free_Hour_21 * 3600) + ( Service_Variables.Serv_Vars.Free_Minute_21 * 60 );
            Period_4 = ( Service_Variables.Serv_Vars.Free_Hour_22 * 3600) + ( Service_Variables.Serv_Vars.Free_Minute_22 * 60 );
    
            Free_Exit_Marker = 2;
    
            if ( Service_Variables.Serv_Vars.Free_Marker_1 < 2 )
            {
                if ( Period_1 < Period_2 )
                {
                    if ( (RTC_GetCounter() >= Period_1) && (RTC_GetCounter() <= Period_2) )
                    {
                        Free_Exit_Marker = Service_Variables.Serv_Vars.Free_Marker_1;
                    }
                }
                else if ( Period_1 > Period_2 )
                {
                    if ( (RTC_GetCounter() >= Period_1) || (RTC_GetCounter() <= Period_2) )
                    {
                        Free_Exit_Marker = Service_Variables.Serv_Vars.Free_Marker_1;
                    }            
                }
            }
  
            if ( Service_Variables.Serv_Vars.Free_Marker_2 < 2 )
            {        
                if ( Period_3 < Period_4 )
                {
                    if ( (RTC_GetCounter() >= Period_3) && (RTC_GetCounter() <= Period_4) )
                    {
                        Free_Exit_Marker = Service_Variables.Serv_Vars.Free_Marker_2;
                    }
                }
                else if ( Period_3 > Period_4 )
                {
                    if ( (RTC_GetCounter() >= Period_3) || (RTC_GetCounter() <= Period_4) )
                    {
                        Free_Exit_Marker = Service_Variables.Serv_Vars.Free_Marker_2;
                    }            
                }   
            }
        }
        else
        {
            Free_Exit_Marker = 2;
        }
    }
        if ( Free_Exit_Marker != Free_Exit_Mem )
        {
            if ( Machine_State == STANDBY_PWM || Machine_State == FREE_ENTER_FWD || Machine_State == FREE_ENTER_BCK )
            {
                Machine_State = STANDBY_NOBRAKE;
                Free_Exit_Mem = Free_Exit_Marker;
            }
        }


//FAST PASS HANDLING
    if ( Service_Variables.Serv_Vars.Szybkie_wejscie == 1 )
    {
        Period_1 = ( Service_Variables.Serv_Vars.Fast_Hour_11 * 3600) + ( Service_Variables.Serv_Vars.Fast_Minute_11 * 60 );
        Period_2 = ( Service_Variables.Serv_Vars.Fast_Hour_12 * 3600) + ( Service_Variables.Serv_Vars.Fast_Minute_12 * 60 );
        Period_3 = ( Service_Variables.Serv_Vars.Fast_Hour_21 * 3600) + ( Service_Variables.Serv_Vars.Fast_Minute_21 * 60 );
        Period_4 = ( Service_Variables.Serv_Vars.Fast_Hour_22 * 3600) + ( Service_Variables.Serv_Vars.Fast_Minute_22 * 60 );
 
        Fast_Pass_Marker = 1;
    
        if ( Service_Variables.Serv_Vars.Fast_Marker_1 == 0 )
        {
            if ( Period_1 < Period_2 )
            {
                if ( (RTC_GetCounter() >= Period_1) && (RTC_GetCounter() <= Period_2) )
                {
                    Fast_Pass_Marker = 0;
                }
            }
            else if ( Period_1 > Period_2 )
            {
                if ( (RTC_GetCounter() >= Period_1) || (RTC_GetCounter() <= Period_2) )
                {
                    Fast_Pass_Marker = 0;
                }          
            }
  
        }
    
        if ( Service_Variables.Serv_Vars.Fast_Marker_2 == 0 )
        {
            if ( Period_3 < Period_4 )
            {
                if ( (RTC_GetCounter() >= Period_3) && (RTC_GetCounter() <= Period_4) )
                {
                    Fast_Pass_Marker = 0;
                }
            }
            else if ( Period_3 > Period_4 )
            {
                if ( (RTC_GetCounter() >= Period_3) || (RTC_GetCounter() <= Period_4) )
                {
                    Fast_Pass_Marker = 0;
                }           
            }   
        }
    }
    else
    {
        Fast_Pass_Marker = 1;
    }
}



/*******************************************************************************
* Function Name  : Output_1_Control
* Description    : Controling mosfet5 = output1
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Output_1_Control ( void )
{

    uint32_t Period_1;
    uint32_t Period_2;

    Period_1 = ( Service_Variables.Serv_Vars.Lights_Hour1 * 3600) + ( Service_Variables.Serv_Vars.Lights_Minute1 * 60 );
    Period_2 = ( Service_Variables.Serv_Vars.Lights_Hour2 * 3600) + ( Service_Variables.Serv_Vars.Lights_Minute2 * 60 );

//No sensor noperiod
    if ( Machine_State != MANUAL_TEST )
    {
        if ( Service_Variables.Serv_Vars.Lights_Sensor_Marker == 1 &&
            Service_Variables.Serv_Vars.Lights_Period_Marker == 1 )
        {
            TIM_SetCompare3(AOUT1_TIMER, PWM_OFF);
        }
//Only sensor
        else if ( Service_Variables.Serv_Vars.Lights_Sensor_Marker == 0 &&
                  Service_Variables.Serv_Vars.Lights_Period_Marker == 1 )
        {
            if ( Inputs_Level & LIGHT_SENSOR_INPUT )    
                TIM_SetCompare3(AOUT1_TIMER, PWM_FULL);
            else    
                TIM_SetCompare3(AOUT1_TIMER, PWM_OFF);
        }
//Only period
        else if ( Service_Variables.Serv_Vars.Lights_Sensor_Marker == 1 &&
                  Service_Variables.Serv_Vars.Lights_Period_Marker == 0 )
        {
            if ( Period_1 < Period_2 )
            {
                if ( (RTC_GetCounter() >= Period_1) && (RTC_GetCounter() <= Period_2) )
                {
                    TIM_SetCompare3(AOUT1_TIMER, PWM_FULL);
                }
                else
                {
                    TIM_SetCompare3(AOUT1_TIMER, PWM_OFF);
                }
            }
            else if ( Period_1 > Period_2 )
            {
                if ( (RTC_GetCounter() >= Period_1) || (RTC_GetCounter() <= Period_2) )
                {
                    TIM_SetCompare3(AOUT1_TIMER, PWM_FULL);
                }
                else
                {
                    TIM_SetCompare3(AOUT1_TIMER, PWM_OFF);
                }
            }   
        }
//Both
        else if ( Service_Variables.Serv_Vars.Lights_Sensor_Marker == 0 &&
                  Service_Variables.Serv_Vars.Lights_Period_Marker == 0 )
        {
            if ( Period_1 < Period_2 )
            {
                if ( (RTC_GetCounter() >= Period_1) && (RTC_GetCounter() <= Period_2) )
                {
                    if ( Inputs_Level & LIGHT_SENSOR_INPUT )    
                        TIM_SetCompare3(AOUT1_TIMER, PWM_FULL);
                    else    
                        TIM_SetCompare3(AOUT1_TIMER, PWM_OFF);
                }
                else
                {
                    TIM_SetCompare3(AOUT1_TIMER, PWM_OFF);
                }
            }
            else if ( Period_1 > Period_2 )
            {
                if ( (RTC_GetCounter() >= Period_1) || (RTC_GetCounter() <= Period_2) )
                {
                    if ( Inputs_Level & LIGHT_SENSOR_INPUT )    
                        TIM_SetCompare3(AOUT1_TIMER, PWM_FULL);
                    else    
                        TIM_SetCompare3(AOUT1_TIMER, PWM_OFF);
                }
                else
                {
                    TIM_SetCompare3(AOUT1_TIMER, PWM_OFF);
                }
            } 
        }
    }
}

/*******************************************************************************
* Function Name  : Output_3_Control
* Description    : Controling mosfet6 = output6
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Output_2_Control ( void )
{
    if ( Machine_State != MANUAL_TEST )
    {
        TIM_SetCompare1(AOUT2_TIMER, PWM_FULL);
    }
}
/*******************************************************************************
* Function Name  : Output_3_Control
* Description    : Controling Output3 = Relay3
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Output_3_Control ( void )
{
    if ( Machine_State != MANUAL_TEST )
    {
        if ( Machine_State == FORCING )
/*        
            Machine_State == ZWORA_FWD ||
            Machine_State == STANDBY_ZWORA_FWD ||
            Machine_State == FREE_ENTER_FWD ||
            Machine_State == WAIT_FORWARD ||
            Machine_State == MOVE_FORWARD ||
            Machine_State == ARM_STOP_FWD ||
            Machine_State == ZWORA_BCK ||
            Machine_State == STANDBY_ZWORA_BCK ||
            Machine_State == FREE_ENTER_BCK ||
            Machine_State == WAIT_BACK ||
            Machine_State == MOVE_BACK ||
            Machine_State == ARM_STOP_BCK )
*/
        {
            GPIO_SetBits(PORT_RELAY3, PIN_RELAY3);
        }
        else   
        {        
            GPIO_ResetBits(PORT_RELAY3, PIN_RELAY3 );  
        }
    }
}

/*******************************************************************************
* Function Name  : Output_4_Control
* Description    : Controling output4 = Relay4
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Output_4_Control ( void )
{
/*
    if ( Machine_State != MANUAL_TEST )
    {
        if ( Machine_State == ZWORA_BCK ||
            Machine_State == STANDBY_ZWORA_BCK ||
            Machine_State == FREE_ENTER_BCK ||
            Machine_State == WAIT_BACK ||
            Machine_State == MOVE_BACK ||
            Machine_State == ARM_STOP_BCK )
        {
            GPIO_SetBits(PORT_RELAY4, PIN_RELAY4);
        }
        else   
        {        
            GPIO_ResetBits(PORT_RELAY4, PIN_RELAY4 );  
        }
    }
*/
    if (    Machine_State == WAIT_FORWARD ||
            Machine_State == MOVE_FORWARD ||
            Machine_State == WAIT_BACK ||
            Machine_State == MOVE_BACK ||
            Machine_State == ARM_STOP_FWD ||
            Machine_State == ARM_STOP_BCK ||
            Machine_State == GO_FORWARD ||
            Machine_State == GO_BACK )
    {
        if ( System_Timer_Check ( &Trans_Timer.Timer, T_1sec ) == TRUE )
        {
            GPIO_SetBits(PORT_RELAY4, PIN_RELAY4 );  
        }   
    }
    else
    { 
        GPIO_ResetBits(PORT_RELAY4, PIN_RELAY4);
    }



}

/*******************************************************************************
* Function Name  : Back_signals_Control
* Description    : Controling output4 = Relay4
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Back_Signals_Control ( void )
{
    if ( Machine_State != MANUAL_TEST )
    {
        if ( Controlling_Variables.Ctrl_Vars.Back_Signal_Type == 0 )//Standard back signals
        {
            if ( Machine_State == FWD_SIGNAL ||
                Machine_State == FWD_SIG_NOBRAKE )       
            {
                GPIO_SetBits(PORT_RELAY1, PIN_RELAY1 );
            }
            else
            {
                GPIO_ResetBits(PORT_RELAY1, PIN_RELAY1 );
            }

            if ( Machine_State == BCK_SIGNAL ||
                Machine_State == BCK_SIG_NOBRAKE )       
            {
                GPIO_SetBits(PORT_RELAY2, PIN_RELAY2 );
            }
            else
            {
                GPIO_ResetBits(PORT_RELAY2, PIN_RELAY2 );
            }
        }
        else if ( Controlling_Variables.Ctrl_Vars.Back_Signal_Type == 1 )//Door back signals
        {
            if ( Machine_State == MOVE_FORWARD ||
                Machine_State == ARM_STOP_FWD ||
                Machine_State == FWD_SIGNAL ||         
                Machine_State == FWD_SIG_NOBRAKE )       
            {
                GPIO_SetBits(PORT_RELAY1, PIN_RELAY1 );
            }
            else
            {
                GPIO_ResetBits(PORT_RELAY1, PIN_RELAY1 );
            }

            if ( Machine_State == MOVE_BACK ||
                Machine_State == ARM_STOP_BCK ||
                Machine_State == BCK_SIGNAL ||
                Machine_State == BCK_SIG_NOBRAKE )       
            {
                GPIO_SetBits(PORT_RELAY2, PIN_RELAY2 );
            }
            else
            {
                GPIO_ResetBits(PORT_RELAY2, PIN_RELAY2 );
            }    
        }
    }
}

/*******************************************************************************
* Function Name  : Input_Output_Test
* Description    : Manual testing of inputs and outputs
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Input_Output_Test ( void )
{
    if ( Machine_State == MANUAL_TEST )
    {
        if ( Controller_Variables.Controller_Vars.Test_Status == 0 )
        {
            if ( Inputs_Level_Test & INPUT4 )
            {
                TIM_SetCompare2(MOTOR_TIMER, PWM_FULL);
            }
            else
            {
                TIM_SetCompare2(MOTOR_TIMER, PWM_OFF); 
            }
        
            if ( Inputs_Level_Test & INPUT5 )
            {
                TIM_SetCompare1(BRAKE_TIMER, PWM_FULL);       
            }
            else
            {
                TIM_SetCompare1(BRAKE_TIMER, PWM_OFF);    
            }        
   
            if ( Inputs_Level_Test & INPUT6 )
            {
                TIM_SetCompare4(BOLT1_TIMER, PWM_FULL);       
            }
            else
            {
                TIM_SetCompare4(BOLT1_TIMER, PWM_OFF);    
            }  
 
            if ( Inputs_Level_Test & INPUT7 )
            {
                TIM_SetCompare2(BOLT2_TIMER, PWM_FULL);       
            }
            else
            {
                TIM_SetCompare2(BOLT2_TIMER, PWM_OFF);    
            } 
  
            if ( Inputs_Level_Test & INPUT8 )
            {
                TIM_SetCompare3(AOUT1_TIMER, PWM_FULL);       
            }
            else
            {
                TIM_SetCompare3(AOUT1_TIMER, PWM_OFF);    
            } 

            if ( Inputs_Level_Test & INPUT4 )
            {
                TIM_SetCompare1(AOUT2_TIMER, PWM_FULL);       
            }
            else
            {
                TIM_SetCompare1(AOUT2_TIMER, PWM_OFF);    
            }         

            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD);
            GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK);
            GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_RELAY1, PIN_RELAY1 );
            GPIO_ResetBits(PORT_RELAY2, PIN_RELAY2 );
            GPIO_ResetBits(PORT_RELAY3, PIN_RELAY3 );
            GPIO_ResetBits(PORT_RELAY4, PIN_RELAY4 );
            GPIO_ResetBits(PORT_MOTOR_DIR, PIN_MOTOR_DIR);
            GPIO_ResetBits(FAN_PORT, FAN_PIN);
        
         }
        else if ( Controller_Variables.Controller_Vars.Test_Status == 1 )
        {
            TIM_SetCompare2(MOTOR_TIMER, PWM_OFF);

            if ( Inputs_Level_Test & INPUT4 )
            {
                GPIO_SetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD);
            }
            else
            {
                GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD);
            }
        
            if ( Inputs_Level_Test & INPUT5 )
            {
                GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            }
            else
            {
                GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            }
        
            if ( Inputs_Level_Test & INPUT6 )
            {
                GPIO_SetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK);
            }
            else
            {
                GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK);
            }  

            if ( Inputs_Level_Test & INPUT7 )
            {
                GPIO_SetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            }
            else
            {
                GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            }
        
      
        }
        else if ( Controller_Variables.Controller_Vars.Test_Status == 2 )
        {
            TIM_SetCompare2(MOTOR_TIMER, PWM_FULL);

            if ( Inputs_Level_Test & INPUT4 )
            {
                GPIO_SetBits(PORT_RELAY1, PIN_RELAY1 );
            }
            else
            {
                GPIO_ResetBits(PORT_RELAY1, PIN_RELAY1 );
            } 
        
            if ( Inputs_Level_Test & INPUT5 )
            {
                GPIO_SetBits(PORT_RELAY2, PIN_RELAY2 );
            }
            else
            {
                GPIO_ResetBits(PORT_RELAY2, PIN_RELAY2 );
            }  
        
            if ( Inputs_Level_Test & INPUT6 )
            {
                GPIO_SetBits(PORT_RELAY3, PIN_RELAY3 );
            }
            else
            {
                GPIO_ResetBits(PORT_RELAY3, PIN_RELAY3 );
            }
        
            if ( Inputs_Level_Test & INPUT7 )
            {
                GPIO_SetBits(PORT_RELAY4, PIN_RELAY4 );
            }
            else
            {
                GPIO_ResetBits(PORT_RELAY4, PIN_RELAY4 );
            }  

            if ( Inputs_Level_Test & INPUT8 )        
            {
                GPIO_SetBits(PORT_MOTOR_DIR, PIN_MOTOR_DIR);
            }
            else
            {
                GPIO_ResetBits(PORT_MOTOR_DIR, PIN_MOTOR_DIR);
            }        
        }
        else if ( Controller_Variables.Controller_Vars.Test_Status == 3 )
        {
            TIM_SetCompare2(MOTOR_TIMER, PWM_OFF);
 
            if ( Inputs_Level_Test & INPUT4 )
            {
                GPIO_SetBits(FAN_PORT, FAN_PIN );
            }
            else
            {
                GPIO_ResetBits(FAN_PORT, FAN_PIN);
            }        
        }
        else if ( Controller_Variables.Controller_Vars.Test_Status == 4 )
        {
            TIM_SetCompare2(MOTOR_TIMER, PWM_OFF); 
            TIM_SetCompare1(BRAKE_TIMER, PWM_OFF);    
            TIM_SetCompare4(BOLT1_TIMER, PWM_OFF);    
            TIM_SetCompare2(BOLT2_TIMER, PWM_OFF);    
            TIM_SetCompare3(AOUT1_TIMER, PWM_OFF);    
            TIM_SetCompare1(AOUT2_TIMER, PWM_OFF);    
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD);
            GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK);
            GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_RELAY1, PIN_RELAY1 );
            GPIO_ResetBits(PORT_RELAY2, PIN_RELAY2 );
            GPIO_ResetBits(PORT_RELAY3, PIN_RELAY3 );
            GPIO_ResetBits(PORT_RELAY4, PIN_RELAY4 );
            GPIO_ResetBits(PORT_MOTOR_DIR, PIN_MOTOR_DIR);
            GPIO_ResetBits(FAN_PORT, FAN_PIN);       
        }
    }
}
/*******************************************************************************
* End file sys.c
*******************************************************************************/









