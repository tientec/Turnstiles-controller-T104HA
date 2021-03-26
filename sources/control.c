/*******************************************************************************
********************************************************************************
* File Name          : control.c
* Author            : Tomasz Nowak / GASTOP
* Version            : V1.00A
* Date               : 04/08/201
1
* Description        : Controllng inputs and outputs - algorithm handling
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
* Local Function Prototypes
*******************************************************************************/
static void Keyboard_Stand_by ( void );
static void Keyboard_Stand_by_Fwd ( void );
static void Keyboard_Stand_by_Bck ( void );
static void Release_Kbd_Mem ( void );
static void Enc_Positioning ( void );
static void Enc_Positioning_Stop ( void );
static void Forward_Check ( void );
static void Backwords_Check ( void );
static void Forward_Move ( void );
static void Backwords_Move ( void );
static void Test_Loop ( void );
static void Self_Test_Loop ( void );
static void Standby_Time_Wait ( void );
static void Stop_Fwd_Fun ( void );
static void Stop_Bck_Fun ( void );

static void Keyboard_Standby ( void );
static void Release_Kbd_Mem ( void );
static void Enc_Positioning ( void );
static void Enc_Positioning_Stop ( void );
static void Forward_Check ( void );
static void Backwords_Check ( void );
static void Forward_Move ( void );
static void Backwords_Move ( void );
static void Test_Check ( void );
static void Test_Interval_Fun ( void );
static void Manual_Test ( void );
static void Test_Loop ( void );
static void Self_Test_Loop ( void );
static void Standby_Time_Wait ( void );
static void Standby_Zwora_Fwd_Fun ( void );
static void Standby_Zwora_Bck_Fun ( void );
static void Forcing_Wait ( void );

static void Forcing_Loop ( void );
static void Reset_Counters_Fun ( void );

static void Forward_Go ( void );
static void Backwords_Go ( void );

static void Stop_Ind_Fwd_Fun ( void );
static void Stop_Ind_Bck_Fun ( void );
static void Start_Ind_Fun ( void );
/*******************************************************************************
* Local Object Definitions
*******************************************************************************/
/* Variables for system timers */

typedef struct {
	uint32_t Timer;
} _Tmr_Cntr;

_Tmr_Cntr Wait_4_Move_Timer;
_Tmr_Cntr Pos_100ms_Timer;
_Tmr_Cntr Sig_500ms_Timer;
_Tmr_Cntr Test_100ms_Timer;
_Tmr_Cntr Test_Interval_Timer;
_Tmr_Cntr Arm_Window_Timer;
_Tmr_Cntr Forcing_Timer;
_Tmr_Cntr Standby_Pwm_Timer;;
_Tmr_Cntr Zwora_Timer;
_Tmr_Cntr Stop_Fwd_Bck_Timer;
_Tmr_Cntr Standby_NB_Timer;
_Tmr_Cntr Transition_Timer;
_Tmr_Cntr Normal_Exit_Timer;
_Tmr_Cntr Free_Exit_Timer;
_Tmr_Cntr Ind_Timer;

typedef enum {
	LEDS_OFF = 0,
	LEDS_FORWARD,
	LEDS_BACKWORDS,
	LEDS_ON,
	LEDS_ALL,
	LEDS_BLANK,
	LEDS_PULSE_GREEN_FWD,
	LEDS_PULSE_GREEN_BCK,
	LEDS_PULSE_RED_FWD,
	LEDS_PULSE_RED_BCK,
	LEDS_PULSE_GREEN,
	LEDS_PULSE_RED,
	LEDS_PULSE_ALL,
} _Pictogram_Status;

typedef struct {
    bool Motor;
	bool Brake;
	bool Bolt_Fwd;
	bool Bolt_Bck;
	Motor_Dir Motor_Direction;
    bool Brake_PWM;
    bool Bolts_PWM;
} _Outputs_Status;

typedef struct {
	uint16_t * Time_Value;
	_Sys_Tmr_Intervals Resolution;
	_Machine_State Next;
	_Tmr_Cntr * Tmr_Cntr_Ptr;
} _Time_Connection;

typedef struct {
	void (*Encoder_fun)(void);
	_Machine_State Next[2];
} _Motor_Enc_Ctrl;

_Motor_Enc_Ctrl Enc_Pos = { Enc_Positioning, { FORCING, FORCING } };
_Motor_Enc_Ctrl Enc_Pos_Stop = { Enc_Positioning_Stop, { STANDBY, STANDBY } };
_Motor_Enc_Ctrl Enc_Wait_Fwd = { Forward_Check, { MOVE_FORWARD, MOVE_FORWARD } };
_Motor_Enc_Ctrl Enc_Wait_Bck = { Backwords_Check, { MOVE_BACK, MOVE_BACK } };
_Motor_Enc_Ctrl Enc_Move_Fwd = { Forward_Move, { FWD_SIGNAL, FWD_SIG_NOBRAKE } };
_Motor_Enc_Ctrl Enc_Move_Bck = { Backwords_Move, { BCK_SIGNAL, BCK_SIG_NOBRAKE } };

_Motor_Enc_Ctrl Enc_Arm_Fwd  = { Stop_Fwd_Fun, { MOVE_FORWARD, MOVE_FORWARD } };
_Motor_Enc_Ctrl Enc_Arm_Bck  = { Stop_Bck_Fun, { MOVE_BACK, MOVE_BACK } };

_Motor_Enc_Ctrl Enc_Test_Loop = { Test_Loop, { TEST_BOLTS, TEST_BOLTS } };
_Motor_Enc_Ctrl Enc_Test_Interval = { Test_Interval_Fun, { TEST_START, TEST_START } };

_Motor_Enc_Ctrl Enc_Self_Test = { Self_Test_Loop, { STANDBY, STANDBY } };
_Motor_Enc_Ctrl Enc_Forcing = { Forcing_Wait, { WAIT_FOR_BCK, WAIT_FOR_FWD} };
_Motor_Enc_Ctrl Enc_Forcing_Loop = { Forcing_Loop, { FORCING, STANDBY} };
_Motor_Enc_Ctrl Standby_Wait = { Standby_Time_Wait, { STANDBY_PWM, STANDBY_PWM } };
_Motor_Enc_Ctrl Stop_Fwd = { Stop_Fwd_Fun, { MOVE_FORWARD, MOVE_FORWARD } };
_Motor_Enc_Ctrl Stop_Bck = { Stop_Bck_Fun, { MOVE_BACK, MOVE_BACK } };
_Motor_Enc_Ctrl Standby_Zwora_Fwd = { Standby_Zwora_Fwd_Fun, { WAIT_FORWARD,WAIT_FORWARD} };
_Motor_Enc_Ctrl Standby_Zwora_Bck = { Standby_Zwora_Bck_Fun, { WAIT_BACK, WAIT_BACK} };

_Motor_Enc_Ctrl Enc_Go_Fwd = { Forward_Go, { FWD_SIGNAL, FWD_SIG_NOBRAKE } };
_Motor_Enc_Ctrl Enc_Go_Bck = { Backwords_Go, { BCK_SIGNAL, BCK_SIG_NOBRAKE } };

_Motor_Enc_Ctrl Stop_Ind_Fwd = { Stop_Ind_Fwd_Fun, { IND_BCK } };
_Motor_Enc_Ctrl Stop_Ind_Bck = { Stop_Ind_Bck_Fun, { IND_STOP } };
_Motor_Enc_Ctrl Start_Ind = { Start_Ind_Fun, { IND_FWD } };

typedef struct {
	_Pictogram_Status Pictogram_Status;
	const _Outputs_Status * Outputs_Status_Ptr;
	const _Time_Connection * Time_Connection_Ptr;
	const _Motor_Enc_Ctrl * Motor_Enc_Ptr;
	void (*Keyboard_fun)(void);
} _Control_Variables;



_Time_Connection Wait_4_Move = { &Controlling_Variables.Ctrl_Vars.Czas_otwarcia_bramki, T_1sec, RESET_COUNTERS, &Wait_4_Move_Timer };
_Time_Connection Sig_500ms = { &Controlling_Variables.Ctrl_Vars.Czas_syg_zwr, T_100msec, STANDBY, &Sig_500ms_Timer };
_Time_Connection Sig_No_Brake = { &Controlling_Variables.Ctrl_Vars.Czas_syg_zwr, T_100msec, STANDBY_NOBRAKE, &Sig_500ms_Timer };
_Time_Connection Test_100ms_Fwd = { &Service_Variables.Serv_Vars.Serwis_014, T_10msec, TEST_LOOP_F, &Test_100ms_Timer };
_Time_Connection Test_100ms_Bck = { &Service_Variables.Serv_Vars.Serwis_014, T_10msec, TEST_LOOP_B, &Test_100ms_Timer };
_Time_Connection Test_Bolts_On = { &Service_Variables.Serv_Vars.Serwis_014, T_10msec, TEST_RES, &Test_100ms_Timer };
_Time_Connection Forcing_Fwd = { &Service_Variables.Serv_Vars.Serwis_016, T_10msec, MOVE_FOR_FWD, &Test_100ms_Timer };
_Time_Connection Forcing_Bck = { &Service_Variables.Serv_Vars.Serwis_016, T_10msec, MOVE_FOR_BCK, &Test_100ms_Timer };
_Time_Connection Zwora_Fwd = { &Service_Variables.Serv_Vars.Serwis_017, T_10msec, FREE_ENTER_FWD, &Zwora_Timer };
_Time_Connection Zwora_Bck = { &Service_Variables.Serv_Vars.Serwis_017, T_10msec, FREE_ENTER_BCK, &Zwora_Timer };

_Time_Connection Ind_Fwd = { &Service_Variables.Serv_Vars.Serwis_023, T_10msec, IND_STOP_FWD, &Ind_Timer };
_Time_Connection Ind_Bck = { &Service_Variables.Serv_Vars.Serwis_023, T_10msec, IND_STOP_BCK, &Ind_Timer };

_Time_Connection Standby_NB_Wait = { &Service_Variables.Serv_Vars.Standby_No_Brake_Wait, T_1sec, STANDBY, &Standby_NB_Timer };

/* Constant definition */
//                                            Motor  Brake  BoFor  BoBck  MtrDir   BrPWM  BoPWM*/
const _Outputs_Status Outputs_Off =         { FALSE, TRUE , FALSE, FALSE, FORWARD, FALSE, FALSE };
const _Outputs_Status Outputs_Off_Pwm =     { FALSE, TRUE , FALSE, FALSE, FORWARD, TRUE , TRUE  };
const _Outputs_Status Outputs_No_Brake_Fwd ={ FALSE, FALSE, FALSE, FALSE, FORWARD, FALSE, FALSE };//R
const _Outputs_Status Outputs_No_Brake_Bck ={ FALSE, FALSE, FALSE, FALSE, BACK   , FALSE, FALSE };//R

const _Outputs_Status Outputs_No_Brake =    { FALSE, FALSE, FALSE, FALSE, FORWARD, FALSE, FALSE };
const _Outputs_Status Outputs_Bolts_On =    { FALSE, TRUE , TRUE , TRUE , FORWARD, FALSE, FALSE };

const _Outputs_Status Outputs_Wait_Fwd =    { FALSE, FALSE, TRUE , FALSE, FORWARD, FALSE, FALSE };//R
const _Outputs_Status Outputs_On_Fwd =      { TRUE , FALSE, TRUE , TRUE , FORWARD, FALSE, FALSE };//R
const _Outputs_Status Outputs_Wait_Bckwds = { FALSE, FALSE, FALSE, TRUE , BACK   , FALSE, FALSE };//R
const _Outputs_Status Outputs_On_Bckwds =   { TRUE , FALSE, TRUE , TRUE , BACK   , FALSE, FALSE };//R
const _Outputs_Status Outputs_Signal_Fwd =  { FALSE, TRUE , FALSE, FALSE, FORWARD, FALSE, FALSE };//N
const _Outputs_Status Outputs_Signal_Bck =  { FALSE, TRUE , FALSE, FALSE, BACK   , FALSE, FALSE };//N

const _Outputs_Status Outputs_Sig_Fwd_NB =  { FALSE, FALSE, FALSE, FALSE, FORWARD, FALSE, FALSE };//N
const _Outputs_Status Outputs_Sig_Bck_NB =  { FALSE, FALSE, FALSE, FALSE, BACK   , FALSE, FALSE };//N

const _Outputs_Status Outputs_Hand_Posit =  { FALSE, FALSE, TRUE , TRUE , FORWARD, FALSE, FALSE };
const _Outputs_Status Outputs_Test_Bckwds = { TRUE , FALSE, TRUE , TRUE , BACK   , FALSE, FALSE };
const _Outputs_Status Outputs_Test_Fwd =    { TRUE , FALSE, TRUE , TRUE , FORWARD, FALSE, FALSE };
const _Outputs_Status Outputs_Self_Test =   { TRUE , FALSE, TRUE , TRUE , FORWARD, FALSE, FALSE };
const _Outputs_Status Outputs_Wait_Test_Fwd={ FALSE, FALSE, TRUE , TRUE , FORWARD, FALSE, FALSE };//R
const _Outputs_Status Outputs_Wait_Test_Bck={ FALSE, FALSE, TRUE , TRUE , BACK   , FALSE, FALSE };//R

const _Outputs_Status Outputs_Wait_Fwd_Pos ={ FALSE, FALSE, TRUE , FALSE, FORWARD, FALSE, FALSE };
const _Outputs_Status Outputs_Wait_Bck_Pos ={ FALSE, FALSE, FALSE, TRUE , BACK   , FALSE, FALSE };

const _Outputs_Status Outputs_On_Fwd_Pos =  { TRUE , FALSE, TRUE , TRUE , FORWARD, FALSE, FALSE };
const _Outputs_Status Outputs_On_Bck_Pos =  { TRUE , FALSE, TRUE , TRUE , BACK   , FALSE, FALSE };
const _Outputs_Status Outputs_Stop_Fwd =    { FALSE, FALSE, TRUE , TRUE , FORWARD, FALSE, FALSE };
const _Outputs_Status Outputs_Stop_Bckwds = { FALSE, FALSE, TRUE , TRUE , BACK   , FALSE, FALSE };
const _Outputs_Status Outputs_Alert =       { FALSE, FALSE, TRUE , TRUE , FORWARD, FALSE, FALSE };
const _Outputs_Status Outputs_Alert_Fwd =   { FALSE, FALSE, FALSE, TRUE , FORWARD, FALSE, FALSE };
const _Outputs_Status Outputs_Alert_Bck =   { FALSE, FALSE, TRUE , FALSE, FORWARD, FALSE, FALSE };


const _Outputs_Status Outputs_Arm_Stop_Fwd ={ TRUE , FALSE, TRUE , FALSE, FORWARD, FALSE, FALSE };//R
const _Outputs_Status Outputs_Arm_Stop_Bck ={ TRUE , FALSE, FALSE, TRUE , BACK   , FALSE, FALSE };//R

const _Outputs_Status Outputs_Ind_Off =     { FALSE, FALSE, TRUE , TRUE , FORWARD, FALSE, FALSE };
const _Outputs_Status Outputs_Ind_Off_Bck = { FALSE, FALSE, TRUE , TRUE , BACK   , FALSE, FALSE };
const _Outputs_Status Outputs_Ind_Fwd =     { TRUE , FALSE, TRUE , TRUE , FORWARD, TRUE , FALSE };
const _Outputs_Status Outputs_Ind_Bck =     { TRUE , FALSE, TRUE , TRUE , BACK   , TRUE , FALSE };


_Control_Variables Control_Variables[] = {
/*                Pictogram   Outputs  Tmr_Con   Pos_Flag Keyboard*/
/*STANDBY*/		    { LEDS_OFF, &Outputs_Off, NULL, &Standby_Wait, Release_Kbd_Mem},
/*STANDBY_PWM*/	    { LEDS_OFF, &Outputs_Off_Pwm, NULL, &Enc_Pos, Keyboard_Stand_by},
/*RESET CNTRS*/     { LEDS_OFF, &Outputs_Off, NULL, NULL, Reset_Counters_Fun},

/*ZWORA_FWD*/       { LEDS_OFF, &Outputs_No_Brake_Fwd, &Zwora_Fwd, NULL, Keyboard_Stand_by_Fwd},
/*ZWORA_BCK*/		{ LEDS_OFF, &Outputs_No_Brake_Bck, &Zwora_Bck, NULL, Keyboard_Stand_by_Bck},

/*STNBY_ZWORA_FWD*/	{ LEDS_OFF, &Outputs_No_Brake_Fwd, NULL, &Standby_Zwora_Fwd, Release_Kbd_Mem},
/*STNBY_ZWORA_BCK*/	{ LEDS_OFF, &Outputs_No_Brake_Bck, NULL, &Standby_Zwora_Bck, Release_Kbd_Mem},

/*FREE_ENT_FWD*/    { LEDS_FORWARD, &Outputs_Wait_Fwd, NULL, &Enc_Wait_Fwd, Keyboard_Stand_by_Fwd},
/*FREE_ENT_BCK*/    { LEDS_BACKWORDS, &Outputs_Wait_Bckwds, NULL, &Enc_Wait_Bck, Keyboard_Stand_by_Bck},

/*WAIT_FORWARD*/    { LEDS_FORWARD, &Outputs_Wait_Fwd, &Wait_4_Move, &Enc_Wait_Fwd, Release_Kbd_Mem},
/*MOVE_FORWARD*/    { LEDS_FORWARD, &Outputs_On_Fwd, NULL, &Enc_Move_Fwd, Release_Kbd_Mem},
/*WAIT_BACK*/	    { LEDS_BACKWORDS, &Outputs_Wait_Bckwds, &Wait_4_Move, &Enc_Wait_Bck, Release_Kbd_Mem},
/*MOVE_BACK*/	    { LEDS_BACKWORDS, &Outputs_On_Bckwds, NULL, &Enc_Move_Bck, Release_Kbd_Mem},

/*ARM_STOP_FWD*/    { LEDS_FORWARD, &Outputs_Arm_Stop_Fwd, NULL, &Enc_Arm_Fwd, Release_Kbd_Mem},
/*ARM_STOP_FWD*/    { LEDS_BACKWORDS, &Outputs_Arm_Stop_Bck, NULL, &Enc_Arm_Bck, Release_Kbd_Mem},

/*TEST_START*/	    { LEDS_OFF, &Outputs_Off, NULL, NULL, Test_Check},
/*TEST_FWD*/	    { LEDS_FORWARD, &Outputs_Wait_Test_Fwd, &Test_100ms_Fwd, NULL, Manual_Test},
/*TEST_LOOP_F*/	    { LEDS_FORWARD, &Outputs_Test_Fwd, NULL, &Enc_Test_Loop, Manual_Test},
/*TEST_BACK*/	    { LEDS_BACKWORDS, &Outputs_Wait_Test_Bck, &Test_100ms_Bck, NULL, Manual_Test},
/*TEST_LOOP_B*/	    { LEDS_BACKWORDS, &Outputs_Test_Bckwds, NULL, &Enc_Test_Loop, Manual_Test},
/*TEST_RES*/	    { LEDS_OFF, &Outputs_Off, NULL, &Enc_Test_Interval, NULL},
/*TEST_BOLTS*/    	{ LEDS_OFF, &Outputs_Bolts_On, &Test_Bolts_On, NULL, NULL},

/*FWD_SIGNAL*/	    { LEDS_OFF, &Outputs_Signal_Fwd, &Sig_500ms, NULL, Release_Kbd_Mem},
/*BCK_SIGNAL*/	    { LEDS_OFF, &Outputs_Signal_Bck, &Sig_500ms, NULL, Release_Kbd_Mem},

/*HAND_POSIT*/	    { LEDS_ALL, &Outputs_Hand_Posit, NULL, NULL, NULL},

/*SELF_TEST*/       { LEDS_ALL, &Outputs_Self_Test, NULL, &Enc_Self_Test, Release_Kbd_Mem},

/*FORCING*/ 	    { LEDS_OFF, &Outputs_Off, NULL, &Enc_Forcing, Keyboard_Stand_by},
/*WIAI_FOR_FWD*/    { LEDS_OFF, &Outputs_Wait_Fwd_Pos, &Forcing_Fwd, NULL, Keyboard_Stand_by},
/*MOVE_FOR_FWD*/    { LEDS_OFF, &Outputs_On_Fwd_Pos, NULL, &Enc_Forcing_Loop, Keyboard_Stand_by},
/*WAIT_FOR BCK*/    { LEDS_OFF, &Outputs_Wait_Bck_Pos, &Forcing_Bck, NULL, Keyboard_Stand_by},
/*MOVE_FOR_BCK*/    { LEDS_OFF, &Outputs_On_Bck_Pos, NULL, &Enc_Forcing_Loop, Keyboard_Stand_by},

/*SAP_ALERT*/	    { LEDS_ON, &Outputs_Alert, NULL, NULL, Release_Kbd_Mem},
/*SAP_ALERT_FWD*/	{ LEDS_FORWARD, &Outputs_Alert_Fwd, NULL, NULL, Release_Kbd_Mem},
/*SAP_ALERT_BCK*/   { LEDS_BACKWORDS, &Outputs_Alert_Bck, NULL, NULL, Release_Kbd_Mem},

/*SW LOCKED*/	    { LEDS_PULSE_RED, &Outputs_Off_Pwm, NULL, NULL, NULL},

/*STANDBY_NOBRAKE*/ { LEDS_OFF, &Outputs_No_Brake, NULL, &Standby_Wait, Release_Kbd_Mem},
/*FWD_SIG_NOBRAKE*/	{ LEDS_OFF, &Outputs_Sig_Fwd_NB, &Sig_No_Brake, NULL, Release_Kbd_Mem},
/*BCK_SIG_NOBRAKE*/	{ LEDS_OFF, &Outputs_Sig_Bck_NB, &Sig_No_Brake, NULL, Release_Kbd_Mem},
/*STANDBY_WAIT_NB*/ { LEDS_OFF, &Outputs_No_Brake, &Standby_NB_Wait, NULL, Keyboard_Stand_by},

/*GO_FORWARD*/      { LEDS_PULSE_GREEN, &Outputs_Arm_Stop_Fwd, NULL, &Enc_Go_Fwd, Release_Kbd_Mem},
/*GO_FORWARD*/      { LEDS_PULSE_GREEN, &Outputs_Arm_Stop_Bck, NULL, &Enc_Go_Bck, Release_Kbd_Mem},
/*MANUAL_TEST*/		{ LEDS_OFF, &Outputs_Off, NULL, NULL, NULL},

/*IND_STOP*/      { LEDS_PULSE_GREEN, &Outputs_Ind_Off, NULL, NULL, NULL},
/*IND_START*/     { LEDS_PULSE_GREEN, &Outputs_Ind_Off, NULL, &Start_Ind, NULL},
/*IND_FWD*/       { LEDS_PULSE_GREEN, &Outputs_Ind_Fwd, &Ind_Fwd, NULL, NULL},
/*IND_STOP_FWD*/  { LEDS_PULSE_GREEN, &Outputs_Ind_Off_Bck, NULL, &Stop_Ind_Fwd, NULL},
/*IND_BCK*/       { LEDS_PULSE_GREEN, &Outputs_Ind_Bck, &Ind_Bck, NULL, NULL},
/*IND_STOP_BCK*/  { LEDS_PULSE_GREEN, &Outputs_Ind_Off, NULL, &Stop_Ind_Bck, NULL},

};

volatile _Machine_State Machine_State;
volatile _Encoder_Angle Encoder_Angle;
volatile _Encoder_Angle Actual_Angle;
volatile _Arms_Stop_Pos Arms_Stop_Pos;

volatile uint16_t Arm_Mem_Pos;
volatile uint16_t Arm_Window_Pos;

uint8_t Test_Cntr;
uint8_t Ind_Test_Marker;

volatile uint16_t Min_Pos_Tmp;
volatile uint16_t Max_Pos_Tmp;
volatile uint16_t Arm_Pos_Tmp;

bool STANDBY_PWM_WAIT = FALSE;
bool STOP_FWD_BCK_FLAG = FALSE;
bool PULSE_ON;
bool IND_SENSOR1_POSITION = TRUE;
bool TEST_INTERVAL_FLAG = FALSE;
bool EXIT_VAR_FLAG = FALSE;
bool FORWARD_ENABLE_FLAG = FALSE;
bool BACK_ENABLE_FLAG = FALSE;
bool IND_TEST_FLAG = FALSE;
bool FORCING_ONCE_FLAG = TRUE;

bool EXIT_FWD_BCK_FLAG = FALSE;
bool ENABLE_MEMORY_FLAG = FALSE;


uint16_t Persons_Counter_Fwd = 0;
uint16_t Persons_Counter_Bck = 0;

uint16_t Arm_Pos_Memory;
/*******************************************************************************
* Local Inline Function Definitions and Function-Like Macros
*******************************************************************************/


/*******************************************************************************
* Function Definitions
*******************************************************************************/

/*******************************************************************************
* Function Name  : Control
* Description    : Control functions - algorithm handling
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Control ( void )
{
    static uint16_t Motor_Pwm;
    static uint16_t Brake_Pwm;
    static uint16_t Bolt_Fwd_Pwm;
    static uint16_t Bolt_Bck_Pwm;
    static uint16_t Coil_Pwm;

/*Keyboard handling */
    if ( Control_Variables[Machine_State].Keyboard_fun != NULL )
    {
        (*Control_Variables[Machine_State].Keyboard_fun)();
    }

	if ( Control_Variables[Machine_State].Motor_Enc_Ptr != NULL )
	{
		if ( ( Control_Variables[Machine_State].Motor_Enc_Ptr->Encoder_fun) != NULL )
		{
			(*Control_Variables[Machine_State].Motor_Enc_Ptr->Encoder_fun)();
		}
	}

/* System timer handling - wait for arm mow to enter gate */
    if ( Control_Variables[Machine_State].Time_Connection_Ptr != NULL )
    {
		if ( System_Timer_Check ( &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
							       Control_Variables[Machine_State].Time_Connection_Ptr->Resolution ) == TRUE )
		{
			Machine_State = Control_Variables[Machine_State].Time_Connection_Ptr->Next;
		}
	}

/*Pictogram controlling*/
	if ( System_Timer_Check ( &Pulse_Leds_Timer.Timer, T_10msec ) == TRUE )
	{
		if ( PULSE_ON == TRUE )
		{
			PULSE_ON = FALSE;
		}
		else
		{
			PULSE_ON = TRUE;
		}
		System_Timer_Setting ( &Pulse_Leds_Timer.Timer, TIME_05_SEC, T_10msec );
	}

if ( Machine_State != MANUAL_TEST )
{
	if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_OFF )
	{
        GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
        GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
        GPIO_SetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
        GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
   	}
	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_FORWARD )
	{
        GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
        GPIO_SetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
        GPIO_SetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
        GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
	}
	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_BACKWORDS )
	{
        GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
        GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
        GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
        GPIO_SetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
	}
	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_ON )
	{
        GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
        GPIO_SetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
        GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
        GPIO_SetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
	}
	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_ALL )
	{
        GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
        GPIO_SetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
        GPIO_SetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
        GPIO_SetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
	}
	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_BLANK )
	{
        GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
        GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
        GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
        GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
	}

	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_PULSE_GREEN_FWD )
	{
		if ( PULSE_ON == TRUE )
		{
            GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_SetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_SetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
		else
		{
            GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_SetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
	}
	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_PULSE_GREEN_BCK )
	{
		if ( PULSE_ON == TRUE )
		{
            GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_SetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
		else
		{
            GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
	}
	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_PULSE_RED_FWD )
	{
		if ( PULSE_ON == TRUE )
		{
            GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_SetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
		else
		{
            GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
	}
	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_PULSE_RED_BCK )
	{
		if ( PULSE_ON == TRUE )
		{
            GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_SetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
		else
		{
            GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
	}
	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_PULSE_GREEN )
	{
		if ( PULSE_ON == TRUE )
		{
            GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_SetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_SetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
		else
		{
            GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
	}
	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_PULSE_RED )
	{
		if ( PULSE_ON == TRUE )
		{
            GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_SetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
		else
		{
            GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
	}
	else if ( Control_Variables[Machine_State].Pictogram_Status == LEDS_PULSE_ALL )
	{
		if ( PULSE_ON == TRUE )
		{
            GPIO_SetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_SetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_SetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_SetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
		else
		{
            GPIO_ResetBits(PORT_LED_RED_FWD, PIN_LED_RED_FWD);
            GPIO_ResetBits(PORT_LED_GRN_FWD, PIN_LED_GRN_FWD );
            GPIO_ResetBits(PORT_LED_RED_BCK, PIN_LED_RED_BCK);
            GPIO_ResetBits(PORT_LED_GRN_BCK, PIN_LED_GRN_BCK );
		}
	}
/* End of pictogram handling */

/*Motor control*/
    if ( Service_Variables.Serv_Vars.Motor_On_Off == 0 )
    {
        if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Motor == TRUE )
        {
            Arm_Pos_Tmp = Arm_Pos;

            if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Motor_Direction == FORWARD )
            {
                if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
                {
                    if ( Arm_Pos_Tmp > Arms_Stop_Pos.Max_Pos4 )
                    {
                        Arm_Pos_Tmp -= 1023;
                    }
        
                    if ( (Arm_Pos_Tmp > Min_Pos_Tmp - Service_Variables.Serv_Vars.Serwis_009) && (Arm_Pos_Tmp < Min_Pos_Tmp) )
                    {
                        Motor_Pwm = PWM_MIN + (Controlling_Variables.Ctrl_Vars.Predkosc_Silnika2 * ((PWM_FULL-PWM_MIN)/PWM_RESOLUTION));
                        TIM_SetCompare2(MOTOR_TIMER, Motor_Pwm);
                    }
                    else
                    {
                        Motor_Pwm = PWM_MIN + (Controlling_Variables.Ctrl_Vars.Predkosc_Silnika1 * ((PWM_FULL-PWM_MIN)/PWM_RESOLUTION));
                        TIM_SetCompare2(MOTOR_TIMER, Motor_Pwm);
                    }  
                }
                else//Inductive
                {
                    if ( Controller_Variables.Controller_Vars.Sensor_Counter <= (IND_OFFSET+Service_Variables.Serv_Vars.Sensor_Counter_Speed) )
                    {
                        Motor_Pwm = PWM_MIN + (Controlling_Variables.Ctrl_Vars.Predkosc_Silnika1 * ((PWM_FULL-PWM_MIN)/PWM_RESOLUTION));
                        TIM_SetCompare2(MOTOR_TIMER, Motor_Pwm);
                    }
                    else
                    {
                        Motor_Pwm = PWM_MIN + (Controlling_Variables.Ctrl_Vars.Predkosc_Silnika2 * ((PWM_FULL-PWM_MIN)/PWM_RESOLUTION));
                        TIM_SetCompare2(MOTOR_TIMER, Motor_Pwm);
                    }
                }      
            }
            else
            {
                if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
                {
                    if ( Arm_Pos_Tmp < Arms_Stop_Pos.Min_Pos1 )
                    {
                        Arm_Pos_Tmp += 1023;
                    }

                    if ( Arm_Pos_Tmp > Max_Pos_Tmp && (Arm_Pos_Tmp < Max_Pos_Tmp + Service_Variables.Serv_Vars.Serwis_009) )
                    {
                        Motor_Pwm = PWM_MIN + Controlling_Variables.Ctrl_Vars.Predkosc_Silnika4 * (PWM_FULL-PWM_MIN)/PWM_RESOLUTION;
                        TIM_SetCompare2(MOTOR_TIMER, Motor_Pwm);
                    }
                    else
                    {
                        Motor_Pwm = PWM_MIN + Controlling_Variables.Ctrl_Vars.Predkosc_Silnika3 * (PWM_FULL-PWM_MIN)/PWM_RESOLUTION;
                        TIM_SetCompare2(MOTOR_TIMER, Motor_Pwm);
                    } 
                }
                else//INDUCTIVE
                {
                    if ( Controller_Variables.Controller_Vars.Sensor_Counter >= (IND_OFFSET-Service_Variables.Serv_Vars.Sensor_Counter_Speed) )
                    {
                        Motor_Pwm = PWM_MIN + (Controlling_Variables.Ctrl_Vars.Predkosc_Silnika1 * ((PWM_FULL-PWM_MIN)/PWM_RESOLUTION));
                        TIM_SetCompare2(MOTOR_TIMER, Motor_Pwm);
                    }
                    else
                    {
                        Motor_Pwm = PWM_MIN + (Controlling_Variables.Ctrl_Vars.Predkosc_Silnika2 * ((PWM_FULL-PWM_MIN)/PWM_RESOLUTION));
                        TIM_SetCompare2(MOTOR_TIMER, Motor_Pwm);
                    }                }
            }  
        }
        else
        {
            TIM_SetCompare2(MOTOR_TIMER, PWM_OFF);
        }
    }
    else
    {
        TIM_SetCompare2(MOTOR_TIMER, PWM_OFF);
    }
/* Brake control*/
    if ( Service_Variables.Serv_Vars.Hamulec == 0 )
    {
        if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Brake == TRUE )
        {
            if ( Service_Variables.Serv_Vars.PWM_Zwora == 0 )
            {
                Brake_Pwm = PWM_MIN + Service_Variables.Serv_Vars.PWM_Ham_Var * (PWM_FULL-PWM_MIN)/PWM_RESOLUTION;
            }
            else
            {
                Brake_Pwm = PWM_FULL;
            }
    
            if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Brake_PWM == TRUE )
            {
                TIM_SetCompare1(BRAKE_TIMER, Brake_Pwm);
            }
            else
            {
                TIM_SetCompare1(BRAKE_TIMER, PWM_FULL);       
            }
        }
        else
        {
            TIM_SetCompare1(BRAKE_TIMER, PWM_OFF);
        }
    }
    else
    {
        TIM_SetCompare1(BRAKE_TIMER, PWM_OFF);
    }

/* Left Bolt Control*/
    if ( Service_Variables.Serv_Vars.Rygle_var == 0 )
    {
        if ( Service_Variables.Serv_Vars.PWM_Lewy_Rygiel == 0 )
        {
            if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Bolts_PWM == TRUE )
            {
                Bolt_Fwd_Pwm = PWM_MIN + Service_Variables.Serv_Vars.PWM_LR_Var * (PWM_FULL-PWM_MIN)/PWM_RESOLUTION;
            }
            else
            {
                Bolt_Fwd_Pwm = PWM_FULL;           
            }
        }
        else
        {
            Bolt_Fwd_Pwm = PWM_FULL;
        }

        if ( Service_Variables.Serv_Vars.Lewy_Rygiel == 0 )
        {
            if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Bolt_Fwd == TRUE )
            {
                TIM_SetCompare4(BOLT1_TIMER, PWM_OFF);
            }
            else
            {
                if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Bolts_PWM == TRUE )
                {
                    TIM_SetCompare4(BOLT1_TIMER, Bolt_Fwd_Pwm);
                }
                else
                {
                    TIM_SetCompare4(BOLT1_TIMER, PWM_FULL);       
                }
            }         
        }
        else
        {
            if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Bolt_Fwd == TRUE )
            {
                if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Bolts_PWM == TRUE )
                {
                    TIM_SetCompare4(BOLT1_TIMER, Bolt_Fwd_Pwm);
                }
                else
                {
                    TIM_SetCompare4(BOLT1_TIMER, PWM_FULL);       
                }
            }
            else
            {
                TIM_SetCompare4(BOLT1_TIMER, PWM_OFF);
            }    
        }


/*Right Bolt Control*/
        if ( Service_Variables.Serv_Vars.PWM_Prawy_Rygiel == 0 )
        {
            if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Bolts_PWM == TRUE )
            {
                Bolt_Bck_Pwm = PWM_MIN + Service_Variables.Serv_Vars.PWM_PR_Var * (PWM_FULL-PWM_MIN)/PWM_RESOLUTION;
            }
            else
            {
                Bolt_Bck_Pwm = PWM_FULL;            
            }
        }
        else
        {
            Bolt_Bck_Pwm = PWM_FULL;
        }

        if ( Service_Variables.Serv_Vars.Prawy_Rygiel == 0 )
        {
            if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Bolt_Bck == TRUE )
            {
            TIM_SetCompare2(BOLT2_TIMER, PWM_OFF);
            }
            else
            {
                if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Bolts_PWM == TRUE )
                {
                    TIM_SetCompare2(BOLT2_TIMER, Bolt_Bck_Pwm);
                }
                else
                {
                    TIM_SetCompare2(BOLT2_TIMER, PWM_FULL);       
                }
            }         
        }
        else
        {
            if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Bolt_Bck == TRUE )
            {
                if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Bolts_PWM == TRUE )
                {
                    TIM_SetCompare2(BOLT2_TIMER, Bolt_Bck_Pwm);
                }
                else
                {
                    TIM_SetCompare2(BOLT2_TIMER, PWM_FULL);       
                }
            }
            else
            {
                TIM_SetCompare2(BOLT2_TIMER, PWM_OFF);
            }    
        }
    }
    else
    {
        TIM_SetCompare4(BOLT1_TIMER, PWM_OFF);
        TIM_SetCompare2(BOLT2_TIMER, PWM_OFF);   
    }
/* Motor direction control */
	if ( Control_Variables[Machine_State].Outputs_Status_Ptr->Motor_Direction == FORWARD )
	{
        GPIO_ResetBits(PORT_MOTOR_DIR, PIN_MOTOR_DIR);
	}
	else
	{
        GPIO_SetBits(PORT_MOTOR_DIR, PIN_MOTOR_DIR);
	}
}//end of manual test excluding
}

/*******************************************************************************
* Function Name  : Position_Check
* Description    : checking the position of the arms if correct
* Input          : None
* Output         : None
* Return         : TRUE - position correct / FALSE - position incorrect
*******************************************************************************/
uint8_t Position_Check ( uint16_t Histeresis )
{
    
    if ( Service_Variables.Serv_Vars.KOlowrotek == 1 )
    {
        if ( (Arm_Pos >= (Arms_Stop_Pos.Min_Pos1 - Histeresis)) && (Arm_Pos <= (Arms_Stop_Pos.Max_Pos1 + Histeresis)) )
        {
            Min_Pos_Tmp = Arms_Stop_Pos.Min_Pos2;
            Max_Pos_Tmp = Arms_Stop_Pos.Max_Pos4;
            Encoder_Angle = Angle_1;
            return TRUE;
        }
        else if ( (Arm_Pos >= (Arms_Stop_Pos.Min_Pos2 - Histeresis)) && (Arm_Pos <= (Arms_Stop_Pos.Max_Pos2 + Histeresis)) )
        {
            Min_Pos_Tmp = Arms_Stop_Pos.Min_Pos3;
            Max_Pos_Tmp = Arms_Stop_Pos.Max_Pos1;
            Encoder_Angle = Angle_2;
            return TRUE;
        }
        else if ( (Arm_Pos >= (Arms_Stop_Pos.Min_Pos3 - Histeresis)) && (Arm_Pos <= (Arms_Stop_Pos.Max_Pos3 + Histeresis)) )
        {
            Min_Pos_Tmp = Arms_Stop_Pos.Min_Pos4;
            Max_Pos_Tmp = Arms_Stop_Pos.Max_Pos2;
            Encoder_Angle = Angle_3;
            return TRUE;
        }
        else if ( (Arm_Pos >= (Arms_Stop_Pos.Min_Pos4 - Histeresis)) && (Arm_Pos <= (Arms_Stop_Pos.Max_Pos4 + Histeresis)) )
        {
            Min_Pos_Tmp = Arms_Stop_Pos.Min_Pos1;
            Max_Pos_Tmp = Arms_Stop_Pos.Max_Pos3;	
            Encoder_Angle = Angle_4;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if ( (Arm_Pos >= (Arms_Stop_Pos.Min_Pos1 - Histeresis)) && (Arm_Pos <= (Arms_Stop_Pos.Max_Pos1 + Histeresis)) )
        {
            Min_Pos_Tmp = Arms_Stop_Pos.Min_Pos2;
            Max_Pos_Tmp = Arms_Stop_Pos.Max_Pos3;
            Encoder_Angle = Angle_1;
            return TRUE;
        }
        else if ( (Arm_Pos >= (Arms_Stop_Pos.Min_Pos2 - Histeresis)) && (Arm_Pos <= (Arms_Stop_Pos.Max_Pos2 + Histeresis)) )
        {
            Min_Pos_Tmp = Arms_Stop_Pos.Min_Pos3;
            Max_Pos_Tmp = Arms_Stop_Pos.Max_Pos1;
            Encoder_Angle = Angle_2;
            return TRUE;
        }
        else if ( (Arm_Pos >= (Arms_Stop_Pos.Min_Pos3 - Histeresis)) && (Arm_Pos <= (Arms_Stop_Pos.Max_Pos3 + Histeresis)) )
        {
            Min_Pos_Tmp = Arms_Stop_Pos.Min_Pos1;
            Max_Pos_Tmp = Arms_Stop_Pos.Max_Pos2;
            Encoder_Angle = Angle_3;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    
    Actual_Angle = Encoder_Angle;  
}

/*******************************************************************************
* Function Name  : Enc_Positioning
* Description    : Changing Machine_State
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Enc_Positioning ( void )
{
    if ( FORCING_ONCE_FLAG == TRUE )
    {
        if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
        {
            if ( Position_Check(Service_Variables.Serv_Vars.Serwis_004) == FALSE )
            {
                FORCING_ONCE_FLAG = FALSE;
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                System_Timer_Setting ( &Forcing_Timer.Timer, Service_Variables.Serv_Vars.Serwis_013, T_10msec );
            }
        }
        else
        {
            if ( Controller_Variables.Controller_Vars.Sensor_Counter < (IND_OFFSET - 1) ||
                 Controller_Variables.Controller_Vars.Sensor_Counter > (IND_OFFSET + 1) )
            {
                if ( !(Inputs_Level & IND_SENSOR1) )
                {
                    FORCING_ONCE_FLAG = FALSE;
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                    System_Timer_Setting ( &Forcing_Timer.Timer, Service_Variables.Serv_Vars.Serwis_013, T_10msec );
                }
            }
        }
    }
}

/*******************************************************************************
* Function Name  : Enc_Positioning_Stop
* Description    : Check if position is set during ositioning
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Enc_Positioning_Stop ( void )
{
	if ( Position_Check(Service_Variables.Serv_Vars.Serwis_003) == TRUE )
	{
		Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];	
	}
}

/*******************************************************************************
* Function Name  : Forward_Check
* Description    : Chcecking if arm has been moved during forward entering throught gate
* Input          : None
* Output         : None
* Return         : Nono
*******************************************************************************/
static void Forward_Check ( void )
{
	static uint16_t Arm_Pos_Tmp1;
	static uint16_t Arm_Pos_Tmp2;

	Arm_Pos_Tmp1 = Arm_Pos;
	Arm_Pos_Tmp2 = Arm_Mem_Pos;

	if ( (Arm_Pos_Tmp2 - 512) > Arm_Pos_Tmp1 )
	{
		Arm_Pos_Tmp1 += 1023;
	}

	if ( (Arm_Pos_Tmp2 + 512) < Arm_Pos_Tmp1 )
	{
		Arm_Pos_Tmp1 -= 1023;
	}

        if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
        {
            if ( (Arm_Pos_Tmp2 + Service_Variables.Serv_Vars.Serwis_001) < Arm_Pos_Tmp1 )
            {
                Arm_Window_Pos = Arm_Pos;
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }
        }
        else//Inductive
        {
            if ( !(Inputs_Level & IND_SENSOR1) )
            {
                if ( Controller_Variables.Controller_Vars.Sensor_Counter >= (IND_OFFSET+Service_Variables.Serv_Vars.Sensor_Counter_Move) )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                    System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
                    System_Timer_Setting ( &Transition_Timer.Timer, Controlling_Variables.Ctrl_Vars.Transition_time, T_100msec );
                    Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
                }
            }
        }
}

/*******************************************************************************
* Function Name  : Forward_Move
* Description    : Chcecking if arm is moving correctly and if move has finished
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Forward_Move ( void )
{
	uint16_t Arm_Pos_Tmp3;

    if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
    {
        if ( Position_Check(Service_Variables.Serv_Vars.Serwis_003) == TRUE )
        { 
            if ( Encoder_Angle != Actual_Angle )
            {
                Counters_Variables.Counters_Vars.No_Goin++;
                Persons_Counter_Fwd--;
                Update_Counters();
                Reload_Screen = TRUE;
            
                if ( Service_Variables.Serv_Vars.Standby_No_Brake == 0 )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
                }
                else
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
        
                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );

                Actual_Angle = Encoder_Angle;
            }

            if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
            {
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }
        }
        else
        {
            if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
            {
                Arm_Pos_Tmp3 = Arm_Pos;
                if ( Arm_Pos_Tmp3 < Arms_Stop_Pos.Min_Pos1 )
                {
                    Arm_Pos_Tmp3 += 1023;
                }

                if ( Arm_Window_Pos < Arms_Stop_Pos.Min_Pos1 )
                {
                    Arm_Window_Pos += 1023;
                }

                if ( (Arm_Window_Pos + Service_Variables.Serv_Vars.Serwis_006) >= Arm_Pos_Tmp3 )
                {
                    Arm_Window_Pos = Arm_Pos;
                    STOP_FWD_BCK_FLAG = FALSE;           
                    Machine_State = ARM_STOP_FWD;
           
                }   
                else
                {
                    Arm_Window_Pos = Arm_Pos;
                }

                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }
        }

    }
    else//Inductive
    {
        if ( Inputs_Level & IND_SENSOR1  )
        {
            if ( Controller_Variables.Controller_Vars.Sensor_Counter > (IND_OFFSET+Service_Variables.Serv_Vars.Serwis_021) )
            {
                Counters_Variables.Counters_Vars.No_Goin++;
                Persons_Counter_Fwd--;
                Update_Counters();
                Reload_Screen = TRUE;
            
                if ( Service_Variables.Serv_Vars.Standby_No_Brake == 0 )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
                }
                else
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
        
                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
            }

            if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
            {
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }      
        }
        else
        {
            if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
            {
                if ( Controller_Variables.Controller_Vars.Sensor_Counter_Copy >= Controller_Variables.Controller_Vars.Sensor_Counter )
                {
                    STOP_FWD_BCK_FLAG = FALSE;           
                    Machine_State = ARM_STOP_FWD;
                    Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
                }
                else
                {
                    Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;                
                }
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }

            if ( Controller_Variables.Controller_Vars.Sensor_Direction_Detect != 1 )
            {
                STOP_FWD_BCK_FLAG = FALSE;           
                Machine_State = ARM_STOP_FWD;
                Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }
        
            if ( System_Timer_Check ( &Transition_Timer.Timer, T_100msec ) == TRUE )
            {
                Machine_State = GO_FORWARD;
            }
        }
    }

/*
    else//Inductive
    {
        if ( Inputs_Level & IND_SENSOR1 )
        {
            if ( Controller_Variables.Controller_Vars.Sensor_Counter > (IND_OFFSET+Service_Variables.Serv_Vars.Serwis_021) )
            {
                Counters_Variables.Counters_Vars.No_Goin++;
                Persons_Counter_Fwd--;
                Update_Counters();
                Reload_Screen = TRUE;
            
                if ( Service_Variables.Serv_Vars.Standby_No_Brake == 0 )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
                }
                else
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
        
                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
            }
            else
            {
                Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
                Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
            }
        
            if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
            {
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }      
        }
        else
        {
            if ( Controller_Variables.Controller_Vars.Sensor_Counter > (IND_OFFSET+Service_Variables.Serv_Vars.Serwis_024) )
            {
                Counters_Variables.Counters_Vars.No_Goin++;
                Persons_Counter_Fwd--;
                Update_Counters();
                Reload_Screen = TRUE;
            
                if ( Service_Variables.Serv_Vars.Standby_No_Brake == 0 )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
                }
                else
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
        
                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
     
                Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 2;//BACK          
            }
            else
            {
                if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
                {
                    if ( Controller_Variables.Controller_Vars.Sensor_Counter_Copy >= Controller_Variables.Controller_Vars.Sensor_Counter )
                    {
                        STOP_FWD_BCK_FLAG = FALSE;           
                        Machine_State = ARM_STOP_FWD;
                        Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
                    }
                    else
                    {
                        Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;                
                    }
                    System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
                }

                if ( Controller_Variables.Controller_Vars.Sensor_Direction_Detect != 1 )
                {
                    STOP_FWD_BCK_FLAG = FALSE;           
                    Machine_State = ARM_STOP_FWD;
                    Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
                    System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
                }
        
                if ( System_Timer_Check ( &Transition_Timer.Timer, T_100msec ) == TRUE )
                {
                    Machine_State = GO_FORWARD;
                }
            }
        }
    }
*/
}

/*******************************************************************************
* Function Name  : Backwords_Check
* Description    : Chcecking if arm has been moved during backword entering throught gate
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Backwords_Check ( void )
{
	static uint16_t Arm_Pos_Tmp6;
	static uint16_t Arm_Pos_Tmp7;

	Arm_Pos_Tmp6 = Arm_Mem_Pos;
	Arm_Pos_Tmp7 = Arm_Pos;

	if ( (Arm_Pos_Tmp6 + 512) < Arm_Pos_Tmp7)
	{
		Arm_Pos_Tmp7 -= 1023;
	}

	if ( (Arm_Pos_Tmp6 - 512) > Arm_Pos_Tmp7 )
	{
		Arm_Pos_Tmp7 += 1023;
	}

    if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
    {
        if ( (Arm_Pos_Tmp6 - Service_Variables.Serv_Vars.Serwis_002) > Arm_Pos_Tmp7 )
        {
            Arm_Window_Pos = Arm_Pos;
            Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
            System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
        }

    }
    else//Inductive
    {
        if ( !(Inputs_Level & IND_SENSOR1) )
        {
            if ( Controller_Variables.Controller_Vars.Sensor_Counter <= (IND_OFFSET-Service_Variables.Serv_Vars.Sensor_Counter_Move) )
            {
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
                System_Timer_Setting ( &Transition_Timer.Timer, Controlling_Variables.Ctrl_Vars.Transition_time, T_100msec );
                Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
            }
        }
    }
}
/*******************************************************************************
* Function Name  : Backwords_Move
* Description    : Chcecking if arm is moving correctly and if move has finished
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Backwords_Move ( void )
{
	uint16_t Arm_Pos_Tmp8;

    if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
    {
        if ( Position_Check(Service_Variables.Serv_Vars.Serwis_003) == TRUE )
        {
            if ( Encoder_Angle != Actual_Angle )
            {
                Counters_Variables.Counters_Vars.No_Goout++;
                Persons_Counter_Bck--;
                Update_Counters();
                Reload_Screen = TRUE;       

                if ( Service_Variables.Serv_Vars.Standby_No_Brake == 0)
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
                }
                else
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );

                Actual_Angle = Encoder_Angle;
            }

            if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
            {
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }

        }
        else
        {
            if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
            {
                Arm_Pos_Tmp8 = Arm_Pos;
                if ( Arm_Pos_Tmp8 > Arms_Stop_Pos.Max_Pos4 )
                {
                    Arm_Pos_Tmp8 -= 1023;
                }

                if ( Arm_Window_Pos > Arms_Stop_Pos.Max_Pos4 )
                {
                    Arm_Window_Pos -= 1023;
                }

                if ( (Arm_Window_Pos - Service_Variables.Serv_Vars.Serwis_006) <= Arm_Pos_Tmp8 )
                {
                    Arm_Window_Pos = Arm_Pos;
                    STOP_FWD_BCK_FLAG = FALSE;
                    Machine_State = ARM_STOP_BCK;
                }
                else
                {
                    Arm_Window_Pos = Arm_Pos;
                }

                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }   
        }
    }
    else//Inductive
    {
        if ( Inputs_Level & IND_SENSOR1 )
        {
            if ( Controller_Variables.Controller_Vars.Sensor_Counter < (IND_OFFSET-Service_Variables.Serv_Vars.Serwis_021) )
            {
                Counters_Variables.Counters_Vars.No_Goout++;
                Persons_Counter_Bck--;
                Update_Counters();
                Reload_Screen = TRUE;

                if ( Service_Variables.Serv_Vars.Standby_No_Brake == 0 )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
                }
                else
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
        
                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
            }

            if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
            {
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }      
        }
        else
        {
            if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
            {
                if ( Controller_Variables.Controller_Vars.Sensor_Counter_Copy <= Controller_Variables.Controller_Vars.Sensor_Counter )
                {
                    STOP_FWD_BCK_FLAG = FALSE;
                    Machine_State = ARM_STOP_BCK;
                    Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
                }
                else
                {
                    Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;                
                }
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }

            if ( Controller_Variables.Controller_Vars.Sensor_Direction_Detect != 2 )
            {
                STOP_FWD_BCK_FLAG = FALSE;           
                Machine_State = ARM_STOP_BCK;
                Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }
        
            if ( System_Timer_Check ( &Transition_Timer.Timer, T_100msec ) == TRUE )
            {
                Machine_State = GO_BACK;
            }
        }
    }



/*
    else//Inductive
    {
        if ( Inputs_Level & IND_SENSOR1 )
        {
            if ( Controller_Variables.Controller_Vars.Sensor_Counter < (IND_OFFSET-Service_Variables.Serv_Vars.Serwis_021) )
            {
                Counters_Variables.Counters_Vars.No_Goout++;
                Persons_Counter_Bck--;
                Update_Counters();
                Reload_Screen = TRUE;

                if ( Service_Variables.Serv_Vars.Standby_No_Brake == 0 )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
                }
                else
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
        
                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
            }
            else
            {
                Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
                Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
            }
        
            if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
            {
                System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
            }      
        }
        else
        {
            if ( Controller_Variables.Controller_Vars.Sensor_Counter < (IND_OFFSET-Service_Variables.Serv_Vars.Serwis_024) )
            {
                Counters_Variables.Counters_Vars.No_Goout++;
                Persons_Counter_Bck--;
                Update_Counters();
                Reload_Screen = TRUE;

                if ( Service_Variables.Serv_Vars.Standby_No_Brake == 0 )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
                }
                else
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
        
                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );            

                Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 1;//FORWARD            
            }
            else
            {
                if ( System_Timer_Check ( &Arm_Window_Timer.Timer, T_10msec ) == TRUE )
                {
                    if ( Controller_Variables.Controller_Vars.Sensor_Counter_Copy <= Controller_Variables.Controller_Vars.Sensor_Counter )
                    {
                        STOP_FWD_BCK_FLAG = FALSE;
                        Machine_State = ARM_STOP_BCK;
                        Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
                    }
                    else
                    {
                        Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;                
                    }
                    System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
                }

                if ( Controller_Variables.Controller_Vars.Sensor_Direction_Detect != 2 )
                {
                    STOP_FWD_BCK_FLAG = FALSE;           
                    Machine_State = ARM_STOP_BCK;
                    Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
                    System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
                }
        
                if ( System_Timer_Check ( &Transition_Timer.Timer, T_100msec ) == TRUE )
                {
                    Machine_State = GO_BACK;
                }
            }
        }
    }
*/
}

/*******************************************************************************
* Function Name  : Release_Kbd_Mem
* Description    : Release keyboard memory if keyboard not used and button pressed
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Release_Kbd_Mem ( void )
{
    if ( Controlling_Variables.Ctrl_Vars.Exit_Var == 1 ) //Input on edge detection
    {
        if ( Inputs.Keyboard_Memory != 0 && Inputs.Keyboard_Flag == 1 )
        {
            switch ( Inputs.Keyboard_Memory )
            {
                case INPUT_FORWARD:
                    Persons_Counter_Fwd++;
                break;

                case INPUT_BACK:
                    Persons_Counter_Bck++;
                break;
            }
            Inputs.Keyboard_Flag = 0;
        }

        if ( Inputs.Keyboard_State == 0 )
        {
            Inputs.Keyboard_Memory = 0;
        }
    }
    else
    {  
        if ( Free_Exit_Marker == 0 )
        {
            if ( ENABLE_MEMORY_FLAG == FALSE )
            {
                if ( EXIT_FWD_BCK_FLAG == FALSE )
                {
                    if ( Inputs_Level & BACK_ENABLE )
                    {
                        EXIT_FWD_BCK_FLAG = TRUE;
                        System_Timer_Setting ( &Normal_Exit_Timer.Timer, Controlling_Variables.Ctrl_Vars.Normal_Exit_Time , T_100msec );
                    }
                }
                else
                {
                    if ( System_Timer_Check ( &Normal_Exit_Timer.Timer, T_100msec ) == TRUE )
                    {
                        if ( !(Inputs_Level & BACK_ENABLE) )
                        {
                            Persons_Counter_Bck++;
                        }
                        EXIT_FWD_BCK_FLAG = FALSE;                 
                    }                 
                }
            }
            else
            {
                if ( !(Inputs_Level & BACK_ENABLE) )
                {
                    ENABLE_MEMORY_FLAG = FALSE;
                }
            }
        }
        else if ( Free_Exit_Marker == 1 )
        {
            if ( ENABLE_MEMORY_FLAG == FALSE )
            {
                if ( EXIT_FWD_BCK_FLAG == FALSE )
                {
                    if ( Inputs_Level & FORWARD_ENABLE )
                    {
                        EXIT_FWD_BCK_FLAG = TRUE;
                        System_Timer_Setting ( &Normal_Exit_Timer.Timer, Controlling_Variables.Ctrl_Vars.Normal_Exit_Time , T_100msec );
                    }
                }
                else
                {
                    if ( System_Timer_Check ( &Normal_Exit_Timer.Timer, T_100msec ) == TRUE )
                    {
                        if ( !(Inputs_Level & FORWARD_ENABLE) )
                        {
                            Persons_Counter_Fwd++;
                        }
                        EXIT_FWD_BCK_FLAG = FALSE;                 
                    }                 
                }
            }
            else
            {
                if ( !(Inputs_Level & FORWARD_ENABLE) )
                {
                    ENABLE_MEMORY_FLAG = FALSE;
                }
            }
        }
        else
        {
            if ( Inputs.Keyboard_Memory != 0 && Inputs.Keyboard_Flag == 1 )
            {
                switch ( Inputs.Keyboard_Memory )
                {
                    case INPUT_FORWARD:
                        Persons_Counter_Fwd++;
                    break;

                    case INPUT_BACK:
                        Persons_Counter_Bck++;
                    break;
                }
                Inputs.Keyboard_Flag = 0;
            }

            if ( Inputs.Keyboard_State == 0 )
            {
                Inputs.Keyboard_Memory = 0;
            }
        }
    }
}

/*******************************************************************************
* Function Name  : Test_Loop
* Description    : Test handling funvtion
* Input          : None
* Output         : None
* Return         : TNone
*******************************************************************************/
static void Test_Loop ( void )
{
    if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
    {
        if ( Position_Check(Service_Variables.Serv_Vars.Serwis_003) == TRUE )
        {
            if ( Encoder_Angle != Actual_Angle )
            {
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];

                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
            }
            Actual_Angle = Encoder_Angle;
        }
    }
    else//Inductive
    {
        if ( IND_SENSOR1_POSITION == FALSE )
        {
            if ( (Inputs_Level & IND_SENSOR1) != 0 )
            {
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                IND_SENSOR1_POSITION = TRUE;
                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );            
            }
        }
        else
        {
            if ( (Inputs_Level & IND_SENSOR1) == 0 )
            {   
                IND_SENSOR1_POSITION = FALSE;
            }
        }
    }    

}

/*******************************************************************************
* Function Name  : Keyboard_stand_by
* Description    : Keyboard handling in STANDBY state
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Stand_by ( void )
{
if ( Controlling_Variables.Ctrl_Vars.Exit_Var == 1 ) //Input on edge detection
{
    if ( Inputs.Keyboard_Memory != 0 && Inputs.Keyboard_Flag == 1 )
    {
        switch ( Inputs.Keyboard_Memory )
        {
            case INPUT_FORWARD:
				Machine_State = STANDBY_ZWORA_FWD;
				Persons_Counter_Fwd++;
				Arm_Mem_Pos = Arm_Pos;
                //Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
                //Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
                Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 0;
                FORCING_ONCE_FLAG = TRUE;

                System_Timer_Setting ( &Trans_Timer.Timer, Controlling_Variables.Ctrl_Vars.Trans_time, T_1sec );
            break;

            case INPUT_BACK:
 				Machine_State = STANDBY_ZWORA_BCK;
				Persons_Counter_Bck++;
				Arm_Mem_Pos = Arm_Pos;
                //Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
                //Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
                Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 0;
                FORCING_ONCE_FLAG = TRUE;

                System_Timer_Setting ( &Trans_Timer.Timer, Controlling_Variables.Ctrl_Vars.Trans_time, T_1sec );
            break;
           
            case INPUT_TEST:
                Test_String_Mask = TEST_STARTED;
                Test_Cntr = 0;
                Machine_State = TEST_START;
                TEST_ENABLE = TRUE;
                Screen_Number = ODCZYT_SERWISOWY;//PROG_TESTOWY;
                Reload_Screen = TRUE; 
                Counters_Variables.Counters_Vars.No_Goin_Test = 0;
                Counters_Variables.Counters_Vars.No_Goout_Test = 0;            
            break;
        }
        Inputs.Keyboard_Flag = 0;
    }

    if ( Inputs.Keyboard_State == 0 )
    {
        Inputs.Keyboard_Memory = 0;
    }
}
else
{
        if ( EXIT_VAR_FLAG == FALSE )
        {
            if ( Inputs_Level & FORWARD_ENABLE )
            {
                System_Timer_Setting ( &Normal_Exit_Timer.Timer, Controlling_Variables.Ctrl_Vars.Normal_Exit_Time , T_100msec );
                System_Timer_Setting ( &Free_Exit_Timer.Timer, Controlling_Variables.Ctrl_Vars.Free_Exit_Time , T_100msec );
                FORWARD_ENABLE_FLAG = TRUE;
                EXIT_VAR_FLAG = TRUE;
            }
            else if ( Inputs_Level & BACK_ENABLE )
            {
                System_Timer_Setting ( &Normal_Exit_Timer.Timer, Controlling_Variables.Ctrl_Vars.Normal_Exit_Time , T_100msec );
                System_Timer_Setting ( &Free_Exit_Timer.Timer, Controlling_Variables.Ctrl_Vars.Free_Exit_Time , T_100msec );
                BACK_ENABLE_FLAG = TRUE;
                EXIT_VAR_FLAG = TRUE;
            }
        }
        else
        {
            if ( Inputs_Level & FORWARD_ENABLE )
            {
               FORWARD_ENABLE_FLAG = TRUE;
            }

            if ( Inputs_Level & BACK_ENABLE )
            {
               BACK_ENABLE_FLAG = TRUE;
            }
    
            if ( System_Timer_Check ( &Normal_Exit_Timer.Timer, T_100msec ) == TRUE )
            {
                if ( (FORWARD_ENABLE_FLAG == TRUE) && (BACK_ENABLE_FLAG == FALSE) )
                {
                    if ( !(Inputs_Level & FORWARD_ENABLE) )
                    {
                        Machine_State = STANDBY_ZWORA_FWD;
                        Persons_Counter_Fwd++;
                        Arm_Mem_Pos = Arm_Pos;
                        //Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
                        //Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
                        Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 0;
                        FORCING_ONCE_FLAG = TRUE;

                        System_Timer_Setting ( &Trans_Timer.Timer, Controlling_Variables.Ctrl_Vars.Trans_time, T_1sec );

                        FORWARD_ENABLE_FLAG = FALSE;
                        EXIT_VAR_FLAG = FALSE;
                    }
                }

                else if ( (BACK_ENABLE_FLAG == TRUE) && (FORWARD_ENABLE_FLAG == FALSE) )
                {
                    if ( !(Inputs_Level & BACK_ENABLE) )
                    {
                        Machine_State = STANDBY_ZWORA_BCK;
                        Persons_Counter_Bck++;
                        Arm_Mem_Pos = Arm_Pos;
                        //Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
                        //Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
                        Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 0;
                        FORCING_ONCE_FLAG = TRUE;

                        System_Timer_Setting ( &Trans_Timer.Timer, Controlling_Variables.Ctrl_Vars.Trans_time, T_1sec );

                        BACK_ENABLE_FLAG = FALSE;
                        EXIT_VAR_FLAG = FALSE;
                    }                
                }
                else
                {
                    FORWARD_ENABLE_FLAG = FALSE;
                    BACK_ENABLE_FLAG = FALSE;
                    EXIT_VAR_FLAG = FALSE;                   
                }
            }  

            if ( System_Timer_Check ( &Free_Exit_Timer.Timer, T_100msec ) == TRUE )
            {
                if ( (FORWARD_ENABLE_FLAG == TRUE) && (BACK_ENABLE_FLAG == FALSE) )
                {
                    if ( Inputs_Level & FORWARD_ENABLE )
                    {
                        Free_Exit_Marker = 0;
                    }
                }
            
                else if ( (BACK_ENABLE_FLAG == TRUE) && (FORWARD_ENABLE_FLAG == FALSE) )
                {
                    if ( Inputs_Level & BACK_ENABLE )
                    {
                        Free_Exit_Marker = 1;
                    }                
                }
                else
                {
                        Free_Exit_Marker = 2;                
                }
                FORWARD_ENABLE_FLAG = FALSE;
                BACK_ENABLE_FLAG = FALSE;
                EXIT_VAR_FLAG = FALSE;
            }  
        }
    
    Inputs.Keyboard_Flag = 0;
    Inputs.Keyboard_Memory = 0;    
    }


}

/*******************************************************************************
* Function Name  : Keyboard_standby_Fwd
* Description    : Keyboard handling in FREE_ENTER state
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Stand_by_Fwd ( void )
{
if ( Controlling_Variables.Ctrl_Vars.Exit_Var == 1 ) //Input on edge detection
{
    if ( Inputs.Keyboard_Memory != 0 && Inputs.Keyboard_Flag == 1 )
    {
        switch ( Inputs.Keyboard_Memory )
        {
            case INPUT_BACK:
 				Machine_State = STANDBY_ZWORA_BCK;
				Persons_Counter_Bck++;
				Arm_Mem_Pos = Arm_Pos;
                //Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
                //Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
                Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 0;
                FORCING_ONCE_FLAG = TRUE;

                System_Timer_Setting ( &Trans_Timer.Timer, Controlling_Variables.Ctrl_Vars.Trans_time, T_1sec );
            break;
            
            case INPUT_TEST:
                Test_String_Mask = TEST_STARTED;
                Test_Cntr = 0;
                Machine_State = TEST_START;
                TEST_ENABLE = TRUE;
                Screen_Number = PROG_TESTOWY;
                Reload_Screen = TRUE; 
                Counters_Variables.Counters_Vars.No_Goin_Test = 0;
                Counters_Variables.Counters_Vars.No_Goout_Test = 0;            
            break;
        }
        Inputs.Keyboard_Flag = 0;
    }

    if ( Inputs.Keyboard_State == 0 )
    {
        Inputs.Keyboard_Memory = 0;
    }
}
else
{
    if ( Controlling_Variables.Ctrl_Vars.Exit_Var == 0 )
    {
        if ( !(Inputs_Level & FORWARD_ENABLE) )   
        {
            EXIT_VAR_FLAG = FALSE;
            Free_Exit_Marker = 2;//STANDBY
        }
    
        if ( Inputs_Level & BACK_ENABLE )
        {
            Machine_State = STANDBY_ZWORA_BCK;
            Persons_Counter_Bck++;
            Arm_Mem_Pos = Arm_Pos;
            //Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
            //Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
            Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 0;
            FORCING_ONCE_FLAG = TRUE;
            EXIT_FWD_BCK_FLAG = FALSE; 
            ENABLE_MEMORY_FLAG = TRUE;
            System_Timer_Setting ( &Trans_Timer.Timer, Controlling_Variables.Ctrl_Vars.Trans_time, T_1sec );
        } 
    }
    Inputs.Keyboard_Flag = 0;
    Inputs.Keyboard_Memory = 0;
}
}
/*******************************************************************************
* Function Name  : Keyboard_standby_Bck
* Description    : Keyboard handling in FREE_ENTER state
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Stand_by_Bck ( void )
{
if ( Controlling_Variables.Ctrl_Vars.Exit_Var == 1 ) //Input on edge detection
{
    if ( Inputs.Keyboard_Memory != 0 && Inputs.Keyboard_Flag == 1 )
    {
        switch ( Inputs.Keyboard_Memory )
        {
            case INPUT_FORWARD:
				Machine_State = STANDBY_ZWORA_FWD;
				Persons_Counter_Fwd++;
				Arm_Mem_Pos = Arm_Pos;
                //Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
                //Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
                Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 0;
                FORCING_ONCE_FLAG = TRUE;

                System_Timer_Setting ( &Trans_Timer.Timer, Controlling_Variables.Ctrl_Vars.Trans_time, T_1sec );
            break;
            
            case INPUT_TEST:
                Test_String_Mask = TEST_STARTED;
                Test_Cntr = 0;
                Machine_State = TEST_START;
                TEST_ENABLE = TRUE;
                Screen_Number = PROG_TESTOWY;
                Reload_Screen = TRUE;  
                Counters_Variables.Counters_Vars.No_Goin_Test = 0;
                Counters_Variables.Counters_Vars.No_Goout_Test = 0;            
            break;
        }
   
        Inputs.Keyboard_Flag = 0;
    }

    if ( Inputs.Keyboard_State == 0 )
    {
        EXIT_VAR_FLAG = FALSE;
        Inputs.Keyboard_Memory = 0;
    }
}
else
{
    if ( Controlling_Variables.Ctrl_Vars.Exit_Var == 0 )
    { 
        if ( !(Inputs_Level & BACK_ENABLE) )
        {
            Free_Exit_Marker = 2;//STANDBY
        }

        if ( Inputs_Level & FORWARD_ENABLE )
        {
            Machine_State = STANDBY_ZWORA_FWD;
            Persons_Counter_Fwd++;
            Arm_Mem_Pos = Arm_Pos;
            //Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
            //Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
            Controller_Variables.Controller_Vars.Sensor_Direction_Detect = 0;
            FORCING_ONCE_FLAG = TRUE;
            EXIT_FWD_BCK_FLAG = FALSE;
            ENABLE_MEMORY_FLAG = TRUE;
            System_Timer_Setting ( &Trans_Timer.Timer, Controlling_Variables.Ctrl_Vars.Trans_time, T_1sec );
        }
    }
    Inputs.Keyboard_Flag = 0;
    Inputs.Keyboard_Memory = 0;
}
}

/*******************************************************************************
* Function Name  : Self_Test_Loop
* Description    : Testing an arm after power on
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Self_Test_Loop ( void )
{
    if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
    {
        if ( Position_Check(Service_Variables.Serv_Vars.Serwis_003) == TRUE )
        {
            if ( Encoder_Angle != Actual_Angle )
            {
                if ( ++Test_Cntr == 5 )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }

                Actual_Angle = Encoder_Angle;
            }
        }
    }
    else//Inductive
    {
        if ( Inputs_Level & IND_SENSOR1  )
        {
            if ( ++Test_Cntr == 5 )
            {
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
            }
        }
    }
}

/*******************************************************************************
* Function Name  : Test_Pin_Check
* Description    : Checking if test pin is pulled down
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Test_Check ( void )
{
		if ( ++Test_Cntr > Service_Variables.Serv_Vars.Serwis_008 )
			Test_Cntr = 0;

		if ( Test_Cntr <= Service_Variables.Serv_Vars.Serwis_007 )
        {
			Machine_State = TEST_FWD;
            Counters_Variables.Counters_Vars.No_Goin_Test++;
            Reload_Screen = TRUE;
            Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
            Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
		}
        else if ( Test_Cntr > Service_Variables.Serv_Vars.Serwis_007 && Test_Cntr <= Service_Variables.Serv_Vars.Serwis_008 )
		{
            Machine_State = TEST_BCK;
            Counters_Variables.Counters_Vars.No_Goout_Test++;
            Reload_Screen = TRUE;
            Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
            Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
        }
			System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
									*Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
									Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );;
}

/*******************************************************************************
* Function Name  : Manual_Test
* Description    : Turning off test from input number 8
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Manual_Test ( void )
{
    if ( Inputs.Keyboard_Memory != 0 && Inputs.Keyboard_Flag == 1 )
    {
        switch ( Inputs.Keyboard_Memory )
        {        
            case INPUT_TEST:
                    Screen_Before_Password = TEST;
                    Test_Cntr = 0;
                    Machine_State = STANDBY;
                    TEST_ENABLE = FALSE;
                    Screen_Number = CONTROLLER_ON;
                    Reload_Screen = TRUE;            
            break;
        }
        Inputs.Keyboard_Flag = 0;
    }

    if ( Inputs.Keyboard_State == 0 )
    {
        Inputs.Keyboard_Memory = 0;
    }

}
/*******************************************************************************
* Function Name  : Forcing_Wait
* Description    : wait 200 msec time to enable the motor to positioning in static point
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Forcing_Wait ( void )
{
	static uint16_t Arm_Pos_Temp;
	static uint16_t Half_Cycle;

    if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
    {
        if ( Service_Variables.Serv_Vars.KOlowrotek == 0 )
        {
            Half_Cycle = 170;
        }
        else
        {
            Half_Cycle = 130;
        }

        if ( System_Timer_Check ( &Forcing_Timer.Timer, T_10msec ) == TRUE )
        {
            Arm_Pos_Temp = Arm_Pos;

            if ( Arm_Pos_Temp < Arms_Stop_Pos.Min_Pos1 )
            {
                Arm_Pos_Temp += 1023;
            }

            if ( Position_Check(Service_Variables.Serv_Vars.Serwis_004) == FALSE )
            {
                if ( Arm_Pos_Temp > Arms_Stop_Pos.Max_Pos1 && Arm_Pos_Temp < (Arms_Stop_Pos.Max_Pos1 + Half_Cycle) )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
                else if ( Arm_Pos_Temp > Arms_Stop_Pos.Max_Pos2 && Arm_Pos_Temp < (Arms_Stop_Pos.Max_Pos2 + Half_Cycle) )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
                else if ( Arm_Pos_Temp > Arms_Stop_Pos.Max_Pos3 && Arm_Pos_Temp < (Arms_Stop_Pos.Max_Pos3 + Half_Cycle) )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
                else if ( Arm_Pos_Temp > Arms_Stop_Pos.Max_Pos4 && Arm_Pos_Temp < (Arms_Stop_Pos.Max_Pos4 + Half_Cycle) )
                {
                    if ( Service_Variables.Serv_Vars.KOlowrotek == 1 )
                    {
                        Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                    }
                }
                else
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
                }

                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
            }
            System_Timer_Setting ( &Forcing_Timer.Timer, Service_Variables.Serv_Vars.Serwis_022, T_10msec );
        }
    }
    else//INDUCTIVE
    {
        if ( System_Timer_Check ( &Forcing_Timer.Timer, T_10msec ) == TRUE )
        {
            if ( !(Inputs_Level & IND_SENSOR1) )
            {
                if ( Controller_Variables.Controller_Vars.Sensor_Direction_Detect == 1)
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
                else if ( Controller_Variables.Controller_Vars.Sensor_Direction_Detect == 2 )
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
                }
                else 
                {
                    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
                }
            
                System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                        *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                        Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
            }
            System_Timer_Setting ( &Forcing_Timer.Timer, Service_Variables.Serv_Vars.Serwis_022, T_10msec );
        }        
    }
}

/*******************************************************************************
* Function Name  : Forcing_Loop
* Description    : Go to forcing state after 20 second of positioning
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Forcing_Loop ( void )
{
	if ( System_Timer_Check ( &Forcing_Timer.Timer, T_10msec ) == TRUE )
	{
//Dla ponownego pozycjonowania
//		Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
//		System_Timer_Setting ( &Forcing_Timer.Timer, Service_Variables.Serv_Vars.Serwis_013, T_10msec );

   		Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];

	}

    if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
    {
        if ( Position_Check(Service_Variables.Serv_Vars.Serwis_003) == TRUE )
        {
            Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
        }
    }
    else//INDUCTIVE
    {
        if ( Inputs_Level & IND_SENSOR1 )
        {
            Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
        }
    }
}
/*******************************************************************************
* Function Name  : Standby_Time_Wait
* Description    : Function defines how long the controlller is in STANDBY state after which it goes to STANDBY_PWM mode
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Standby_Time_Wait ( void )
{
	if ( STANDBY_PWM_WAIT == FALSE )
	{
		System_Timer_Setting ( &Standby_Pwm_Timer.Timer, Service_Variables.Serv_Vars.Serwis_011, T_10msec );
		STANDBY_PWM_WAIT = TRUE;
	}
	else
	{
		if ( System_Timer_Check ( &Standby_Pwm_Timer.Timer, T_10msec ) == TRUE )
		{
            if ( Service_Variables.Serv_Vars.Szybkie_wejscie == 0 || Fast_Pass_Marker == 0 )
            {
                if ( Service_Variables.Serv_Vars.W_wyjscie == 0 || Free_Exit_Marker == 0 )
                {
                    Persons_Counter_Fwd = 0;
        
                    if ( Persons_Counter_Bck != 0 )
                    {
                        Machine_State = STANDBY_ZWORA_BCK;             
                    }
                    else
                    {
                        Machine_State = ZWORA_FWD;
                        System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                                *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                                Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
                    } 
                }
                else if ( Service_Variables.Serv_Vars.W_wyjscie == 1 || Free_Exit_Marker == 1 )
                {
                    Persons_Counter_Bck = 0;
                
                    if ( Persons_Counter_Fwd != 0 )
                    {
                        Machine_State = STANDBY_ZWORA_FWD;             
                    }
                    else
                    {
                        Machine_State = ZWORA_BCK;
                        System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                                *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                                Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
                    } 
                }
                else if ( Service_Variables.Serv_Vars.W_wyjscie == 2 || Free_Exit_Marker == 2 )
                {                
                    if ( Persons_Counter_Fwd != 0 && Persons_Counter_Bck == 0 )
                    {
                        Machine_State = STANDBY_ZWORA_FWD;
                    }
          
                    if ( Persons_Counter_Bck != 0 && Persons_Counter_Fwd == 0 )
                    {
                        Machine_State = STANDBY_ZWORA_BCK;
                    }
        
                    if ( Persons_Counter_Fwd != 0 && Persons_Counter_Bck != 0 )
                    {
                        if ( Service_Variables.Serv_Vars.Priority == 0 )
                        {
                            Machine_State = STANDBY_ZWORA_FWD;
                        }
                        else
                        {
                            Machine_State = STANDBY_ZWORA_BCK;                       
                        }
                    }

                    if ( Persons_Counter_Fwd == 0 && Persons_Counter_Bck == 0 )
                    {
                        if ( Machine_State == STANDBY )
                        {
                            Machine_State = STANDBY_PWM;
                        }
                        else if ( Machine_State == STANDBY_NOBRAKE )
                        {
                            Machine_State = STANDBY_WAIT_NB;
                            System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                                    *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                                    Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );

                        }
                    }
                }
			}
            else
            {
                Persons_Counter_Fwd = 0;
                Persons_Counter_Bck = 0;
            
                if ( Service_Variables.Serv_Vars.W_wyjscie == 0 || Free_Exit_Marker == 0 )
                {
                    Machine_State = ZWORA_FWD;
                    System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                            *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                            Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );

                }
                else if ( Service_Variables.Serv_Vars.W_wyjscie == 1 || Free_Exit_Marker == 1 )
                {
                    Machine_State = ZWORA_BCK;
                    System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                            *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                            Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );

                }
                else if ( Service_Variables.Serv_Vars.W_wyjscie == 2 || Free_Exit_Marker == 2 )
                {
                    if ( Machine_State == STANDBY )
                    {
                        Machine_State = STANDBY_PWM;
                    }
                    else if ( Machine_State == STANDBY_NOBRAKE )
                    {
                        Machine_State = STANDBY_WAIT_NB;
                        System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                                *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                                Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );

                    }
                }
            }
            
            Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
            Controller_Variables.Controller_Vars.Sensor_Counter_Copy = IND_OFFSET;
       
        
			STANDBY_PWM_WAIT = FALSE;
            EXIT_VAR_FLAG = FALSE;

		}
	}


    Arm_Mem_Pos = Arm_Pos;
}

/*******************************************************************************
* Function Name  : Stop_Fwd_Fun
* Description    : Brake turn on after arm stop during entering through gate
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Stop_Fwd_Fun ( void )
{
    uint16_t Arm_Pos_Tmp_Fwd_Fun;

    if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
    {
        if ( Position_Check(Service_Variables.Serv_Vars.Serwis_003) == TRUE )
        { 
            if ( Encoder_Angle != Actual_Angle )
            {      
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
            }
        }
        else
        {
//..............................................
            Arm_Pos_Tmp_Fwd_Fun = Arm_Pos;
            if ( Arm_Pos_Tmp_Fwd_Fun < Arms_Stop_Pos.Min_Pos1 )
            {
                Arm_Pos_Tmp_Fwd_Fun += 1023;
            }

            if ( Arm_Window_Pos < Arms_Stop_Pos.Min_Pos1 )
            {
                Arm_Window_Pos += 1023;
            }
    
            if ( Service_Variables.Serv_Vars.Rygle_var == 1 )
            {
                if ( STOP_FWD_BCK_FLAG == FALSE )
                {
                    System_Timer_Setting ( &Stop_Fwd_Bck_Timer.Timer, 100, T_10msec );
                    STOP_FWD_BCK_FLAG = TRUE;
                }
                else
                {
                    if ( System_Timer_Check ( &Stop_Fwd_Bck_Timer.Timer, T_10msec ) == TRUE )
                    {
                        Machine_State = MOVE_FORWARD;
                        STOP_FWD_BCK_FLAG = FALSE;
                        Arm_Window_Pos = Arm_Pos;
                        System_Timer_Setting ( &Arm_Window_Timer.Timer, 300, T_10msec );
                    }
                }
            }
            else
            {
                if ( (Arm_Window_Pos + Service_Variables.Serv_Vars.Serwis_006) < Arm_Pos_Tmp_Fwd_Fun )
                {
                    Machine_State = MOVE_FORWARD;
                    STOP_FWD_BCK_FLAG = FALSE;
                    Arm_Window_Pos = Arm_Pos;
                    System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
                }
            }
//...............................................
        }
    }
    else//Inductive
    {
        if ( Inputs_Level & IND_SENSOR1 )
        {     
            if ( Controller_Variables.Controller_Vars.Sensor_Counter > (IND_OFFSET+Service_Variables.Serv_Vars.Serwis_021) )
            {
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
            }
        }
        else
        {
            if ( Service_Variables.Serv_Vars.Rygle_var == 1 )
            {
                if ( STOP_FWD_BCK_FLAG == FALSE )
                {
                    System_Timer_Setting ( &Stop_Fwd_Bck_Timer.Timer, 100, T_10msec );
                    STOP_FWD_BCK_FLAG = TRUE;
                }
                else
                {
                    if ( System_Timer_Check ( &Stop_Fwd_Bck_Timer.Timer, T_10msec ) == TRUE )
                    {
                        Machine_State = MOVE_FORWARD;
                        STOP_FWD_BCK_FLAG = FALSE;
                        System_Timer_Setting ( &Arm_Window_Timer.Timer, 300, T_10msec );
                    }
                }
            }
            else
            {
                if ( (Controller_Variables.Controller_Vars.Sensor_Counter_Copy+Service_Variables.Serv_Vars.Serwis_020) <= Controller_Variables.Controller_Vars.Sensor_Counter )
                {
                    Machine_State = MOVE_FORWARD;
                    STOP_FWD_BCK_FLAG = FALSE;
                    System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_019, T_10msec );
                    Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
                }
            }
        
            if ( System_Timer_Check ( &Transition_Timer.Timer, T_100msec ) == TRUE )
            {
                Machine_State = GO_FORWARD;
            }
        }
    }
}

/*******************************************************************************
* Function Name  : Stop_Bck_Fun
* Description    : Brake turn on after arm stop during entering through gate
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Stop_Bck_Fun ( void )
{
    uint16_t Arm_Pos_Tmp_Bck_Fun;

    if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
    {
        if ( Position_Check(Service_Variables.Serv_Vars.Serwis_003) == TRUE )
        {
            if ( Encoder_Angle != Actual_Angle )
            {     
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
            }
        }
        else
        {
            Arm_Pos_Tmp_Bck_Fun = Arm_Pos;
            if ( Arm_Pos_Tmp_Bck_Fun > Arms_Stop_Pos.Max_Pos4 )
            {
                Arm_Pos_Tmp_Bck_Fun -= 1023;
            }

            if ( Arm_Window_Pos > Arms_Stop_Pos.Max_Pos4 )
            {
                Arm_Window_Pos -= 1023;
            }
    
            if ( Service_Variables.Serv_Vars.Rygle_var == 1 )
            {
                if ( STOP_FWD_BCK_FLAG == FALSE )
                {
                    System_Timer_Setting ( &Stop_Fwd_Bck_Timer.Timer, 100, T_10msec );
                    STOP_FWD_BCK_FLAG = TRUE;
                }
                else
                {
                    if ( System_Timer_Check ( &Stop_Fwd_Bck_Timer.Timer, T_10msec ) == TRUE )
                    {
                        Machine_State = MOVE_BACK;
                        STOP_FWD_BCK_FLAG = FALSE;
                        Arm_Window_Pos = Arm_Pos;
                        System_Timer_Setting ( &Arm_Window_Timer.Timer, 300, T_10msec );
                    }
                }
            }
            else
            {
                if ( (Arm_Window_Pos - Service_Variables.Serv_Vars.Serwis_006) > Arm_Pos_Tmp_Bck_Fun )
                {
                    Machine_State = MOVE_BACK;
                    STOP_FWD_BCK_FLAG = FALSE;
                    Arm_Window_Pos = Arm_Pos;
                    System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_005, T_10msec );
                }
            }
        }
    }
    else//INDUCTIVE
    {
        if ( Inputs_Level & IND_SENSOR1 )
        {     
            if ( Controller_Variables.Controller_Vars.Sensor_Counter < (IND_OFFSET-Service_Variables.Serv_Vars.Serwis_021) )
            {
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
            }
        }
        else
        {
            if ( Service_Variables.Serv_Vars.Rygle_var == 1 )
            {
                if ( STOP_FWD_BCK_FLAG == FALSE )
                {
                    System_Timer_Setting ( &Stop_Fwd_Bck_Timer.Timer, 100, T_10msec );
                    STOP_FWD_BCK_FLAG = TRUE;
                }
                else
                {
                    if ( System_Timer_Check ( &Stop_Fwd_Bck_Timer.Timer, T_10msec ) == TRUE )
                    {
                        Machine_State = MOVE_FORWARD;
                        STOP_FWD_BCK_FLAG = FALSE;
                        System_Timer_Setting ( &Arm_Window_Timer.Timer, 300, T_10msec );
                    }
                }
            }
            else
            {
                if ( (Controller_Variables.Controller_Vars.Sensor_Counter_Copy-Service_Variables.Serv_Vars.Serwis_020) >= Controller_Variables.Controller_Vars.Sensor_Counter )
                {
                    Machine_State = MOVE_BACK;
                    STOP_FWD_BCK_FLAG = FALSE;
                    System_Timer_Setting ( &Arm_Window_Timer.Timer, Service_Variables.Serv_Vars.Serwis_019, T_10msec );
                    Controller_Variables.Controller_Vars.Sensor_Counter_Copy = Controller_Variables.Controller_Vars.Sensor_Counter;            
                }
            }
        
            if ( System_Timer_Check ( &Transition_Timer.Timer, T_100msec ) == TRUE )
            {
                Machine_State = GO_BACK;
            }

        }    
    }
}

/*******************************************************************************
* Function Name  : Reset_Counters
* Description    : Resets the persons counters
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Reset_Counters_Fun ( void )
{
    Persons_Counter_Fwd = 0;
    Persons_Counter_Bck = 0;
    Machine_State = STANDBY;
    STANDBY_PWM_WAIT = FALSE;
}

/*******************************************************************************
* Function Name  : Standby_Zwora_Fwd_Fun
* Description    : Wait till brake free
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Standby_Zwora_Fwd_Fun ( void )
{
	if ( STANDBY_PWM_WAIT == FALSE )
	{
		System_Timer_Setting ( &Standby_Pwm_Timer.Timer, Service_Variables.Serv_Vars.Serwis_012, T_10msec );
		STANDBY_PWM_WAIT = TRUE;
	}
	else
	{
		if ( System_Timer_Check ( &Standby_Pwm_Timer.Timer, T_10msec ) == TRUE )
		{
			STANDBY_PWM_WAIT = FALSE;
			Arm_Mem_Pos = Arm_Pos;

			Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];

			System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
									*Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
									Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
		}
	}
}

/*******************************************************************************
* Function Name  : Standby_Zwora_Bck_Fun
* Description    : Wait till brake free
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Standby_Zwora_Bck_Fun ( void )
{
	if ( STANDBY_PWM_WAIT == FALSE )
	{
		System_Timer_Setting ( &Standby_Pwm_Timer.Timer, Service_Variables.Serv_Vars.Serwis_012, T_10msec );
		STANDBY_PWM_WAIT = TRUE;
	}
	else
	{
		if ( System_Timer_Check ( &Standby_Pwm_Timer.Timer, T_10msec ) == TRUE )
		{
			STANDBY_PWM_WAIT = FALSE;
			Arm_Mem_Pos = Arm_Pos;

			Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];

			System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
									*Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
									Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
		}
	}
}



/*******************************************************************************
* Function Name  : Forward_Go
* Description    : Checking if arm is on zero position after longtransition
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Forward_Go ( void )
{
	uint16_t Arm_Pos_Tmp3;

    if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
    {

    }
    else//Inductive
    {
        if ( Inputs_Level & IND_SENSOR1  )
        {
            Counters_Variables.Counters_Vars.No_Goin++;
            Persons_Counter_Fwd--;
            Update_Counters();
            Reload_Screen = TRUE;
     
            if ( Service_Variables.Serv_Vars.Standby_No_Brake == 0 )
            {
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
            }
            else
            {
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
            }
        
            System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                    *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                    Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
        }
    }
}



/*******************************************************************************
* Function Name  : Backwords_Go
* Description    : Checking if arm is on zero position after longtransition
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Backwords_Go ( void )
{
	uint16_t Arm_Pos_Tmp8;

    if ( Service_Variables.Serv_Vars.Sensor_Type == 0 )//ENKODER
    {

    }
    else//Inductive
    {
        if ( Inputs_Level & IND_SENSOR1 )
        {
            Counters_Variables.Counters_Vars.No_Goout++;
            Persons_Counter_Bck--;
            Update_Counters();
            Reload_Screen = TRUE;
           
            if ( Service_Variables.Serv_Vars.Standby_No_Brake == 0 )
            {
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[1];
            }
            else
            {
                Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
            }
        
            System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                    *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                    Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
        }
    }
}
/*******************************************************************************
* Function Name  : Test_Interval_Fun
* Description    : Interval time between each move in test
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Test_Interval_Fun ( void )
{
	if ( TEST_INTERVAL_FLAG == FALSE )
	{
		System_Timer_Setting ( &Test_Interval_Timer.Timer, Service_Variables.Serv_Vars.Serwis_015, T_10msec );
		TEST_INTERVAL_FLAG= TRUE;
	}
	else
	{
		if ( System_Timer_Check ( &Test_Interval_Timer.Timer, T_10msec ) == TRUE )
		{
            Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
			TEST_INTERVAL_FLAG = FALSE;
		}
	}
}

static void Stop_Ind_Fwd_Fun ( void )
{
	if ( IND_TEST_FLAG == FALSE )
	{
        if ( Controller_Variables.Controller_Vars.Sensor_Counter > ( IND_OFFSET + IND_TEST_VALUE ) )
        {
            Ind_Test_Marker |= IND_MARKER_FWD;
        }

        Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;

		System_Timer_Setting ( &Ind_Timer.Timer, Service_Variables.Serv_Vars.Serwis_015, T_10msec );
		IND_TEST_FLAG = TRUE;
	}
	else
	{
		if ( System_Timer_Check ( &Ind_Timer.Timer, T_10msec ) == TRUE )
		{
            Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];

            System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                                    *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                                    Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );
        }
    }
}

static void Stop_Ind_Bck_Fun ( void )
{
    if ( Controller_Variables.Controller_Vars.Sensor_Counter < ( IND_OFFSET - IND_TEST_VALUE ) )
    {
            Ind_Test_Marker |= IND_MARKER_BCK;
    }

    if ( (Ind_Test_Marker == (IND_MARKER_FWD | IND_MARKER_BCK )))
    {
        Ind_Test_Mask = IND_MASK_OK;
    }
    else
    {
        Ind_Test_Mask = IND_MASK_FALSE;
    }

    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];
}

static void Start_Ind_Fun ( void )
{
    Machine_State = Control_Variables[Machine_State].Motor_Enc_Ptr->Next[0];

    Ind_Test_Marker = 0;
    IND_TEST_FLAG = FALSE;

    System_Timer_Setting (  &Control_Variables[Machine_State].Time_Connection_Ptr->Tmr_Cntr_Ptr->Timer,
                            *Control_Variables[Machine_State].Time_Connection_Ptr->Time_Value,
                             Control_Variables[Machine_State].Time_Connection_Ptr->Resolution );

}
/*******************************************************************************
* End file control.c
*******************************************************************************/









