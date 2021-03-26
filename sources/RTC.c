/******************** (C) COPYRIGHT 2012 TIENTEC *******************************
* File Name          : RTC.c
* Author             : Tomasz Nowak
* Version            : V1.00
* Date               : 05/07/2012
* Description        : RTC program body
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
_Date Date;
_Date Date_Set;
_Time Time;
_Time Time_Set;
  
volatile uint32_t LSE_Timeout = 0;
bool LSE_FAILED_FLAG = FALSE;

const uint8_t * RTC_Text = "    LSE clock failed";
const uint8_t * RTC_Text1 = "    RTC OFF/";
const uint8_t * RTC1 = "    RTC-OK/";

/* Private function prototypes -----------------------------------------------*/
uint8_t IsLeapYear(uint16_t Year);
static void Set_Start_Date ( _Date * Date_Ptr );


/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : RTC_Init
* Description    : Init RTC clock
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Init(void)
{
    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);
    
    LSE_FAILED_FLAG = FALSE;
    LSE_Timeout = 0;

    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
        LSE_Timeout++;
    
        if ( LSE_Timeout >= 5000000 )
        {           
            LSE_FAILED_FLAG = TRUE;
        }
    
        if ( LSE_Timeout >= 5000000 )
        {
            break;
        }
    }
  
    if ( LSE_FAILED_FLAG == FALSE )
    { 
        /* Select the RTC Clock Source */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

        /* Enable the RTC Clock */
        RCC_RTCCLKCmd(ENABLE);

        /* Wait for RTC registers synchronization */
        RTC_WaitForSynchroTimeout();

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTaskTimeout();

        /* Enable the RTC Second */  
        //RTC_ITConfig(RTC_IT_SEC, ENABLE);

        /* Wait until last write operation on RTC registers has finished */
        //RTC_WaitForLastTaskTimeout();
   
        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTaskTimeout();  
    }
        
    if ( LSE_FAILED_FLAG == TRUE )
    {
        RTC_SetCounter(0x00000000);
        Lcd_Write_Text(RTC_Text1);
    }
    else
    {
        Lcd_Write_Text ( RTC1 );           
    }

    if ( LSE_FAILED_FLAG == FALSE )
    {     
        /* Read data from backup registers to clock structure */
        Read_Date ( &Date );

        /* update date time if RTC counter is above */
        Set_Start_Date ( &Date );

        /* read time from counter */
        Read_Time ( &Time );
    }
}


/*******************************************************************************
* Function Name  : RTC_Handler
* Description    : WHandling the date and time in main loop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Handler ( void )
{
    uint16_t Counter_Tmp = 0;

    _Time Time_Temp;

    Time_Temp = Time;

    if ( RTC_GetCounter() >= 86399 )
    {
        RTC_WaitForLastTask();
        RTC_SetCounter(0x00000000);
        RTC_WaitForLastTask();
 
        Update_Date ( &Date );
        Write_Date ( &Date );

        Counter_Tmp = 1;
        Update_Lock_Counter(Counter_Tmp);
    }

 
    Read_Date ( &Date );
    Read_Time ( &Time );

    if ( Time_Temp.Time_Vars.Minute != Time.Time_Vars.Minute )
    {
        Reload_Screen = TRUE;
    }
}

/*******************************************************************************
* Function Name  : Set_Date
* Description    : Setting the date in system
* Input          : Pointer to structure which keeps tha date values
* Output         : None
* Return         : None
*******************************************************************************/
void Set_Date ( _Date * Date_Ptr )
{

}

/*******************************************************************************
* Function Name  : Set_Time
* Description    : Setting the time in system
* Input          : Pointer to structure which keeps tha time values
* Output         : None
* Return         : None
*******************************************************************************/
void Set_Time ( _Time * Time_Ptr )
{

}

/*******************************************************************************
* Function Name  : Read_Date
* Description    : Reading the date in system
* Input          : Pointer to structure which keeps tha date values
* Output         : None
* Return         : None
*******************************************************************************/
void Read_Date ( _Date * Date_Ptr )
{
    Date_Ptr->Date_Vars.Day = (BKP_ReadBackupRegister(BKP_DR2) & 0xFF00) >> 8;
    Date_Ptr->Date_Vars.Month = (BKP_ReadBackupRegister(BKP_DR2) & 0x00FF);
    Date_Ptr->Date_Vars.Year = BKP_ReadBackupRegister(BKP_DR3);
    Date_Ptr->Date_Vars.Day_Of_Week = (BKP_ReadBackupRegister(BKP_DR4) & 0xFF00) >> 8;
}
/*******************************************************************************
* Function Name  : Read_Time
* Description    : reading the time in system
* Input          : Pointer to structure which keeps tha time values
* Output         : None
* Return         : None
*******************************************************************************/
void Read_Time ( _Time * Time_Ptr )
{
    Time_Ptr->Time_Vars.Hour = (RTC_GetCounter()/3600);
    Time_Ptr->Time_Vars.Minute = (RTC_GetCounter()%3600)/60;
    Time_Ptr->Time_Vars.Second = (RTC_GetCounter()%3600)%60;
}
/*******************************************************************************
* Function Name  : Update_Date
* Description    : Write date to backup registers
* Input          : Pointer to structure which keeps tha date values
* Output         : None
* Return         : none
*******************************************************************************/
void Update_Date ( _Date * Date_Ptr )
{
  Date_Ptr->Date_Vars.Day_Of_Week++;

  if ( Date_Ptr->Date_Vars.Day_Of_Week > 6 )
  {
    Date_Ptr->Date_Vars.Day_Of_Week = 0;
  }

  if(Date_Ptr->Date_Vars.Month == 1 || Date_Ptr->Date_Vars.Month == 3 || Date_Ptr->Date_Vars.Month == 5 || Date_Ptr->Date_Vars.Month == 7 ||
     Date_Ptr->Date_Vars.Month == 8 || Date_Ptr->Date_Vars.Month == 10 || Date_Ptr->Date_Vars.Month == 12)
  {
    if(Date_Ptr->Date_Vars.Day < 31)
    {
      Date_Ptr->Date_Vars.Day++;
    }
    /* Date structure member: date_s.day = 31 */
    else
    {
      if(Date_Ptr->Date_Vars.Month != 12)
      {
        Date_Ptr->Date_Vars.Month++;
        Date_Ptr->Date_Vars.Day = 1;
      }
      /* Date structure member: date_s.day = 31 & date_s.month =12 */
      else
      {
        Date_Ptr->Date_Vars.Day = 1;
        Date_Ptr->Date_Vars.Month = 1;
        Date_Ptr->Date_Vars.Year++;
      }
    }
  }
  else if(Date_Ptr->Date_Vars.Month == 4 ||
          Date_Ptr->Date_Vars.Month == 6 ||
          Date_Ptr->Date_Vars.Month == 9 ||
          Date_Ptr->Date_Vars.Month == 11)
  {
    if(Date_Ptr->Date_Vars.Day < 30)
    {
      Date_Ptr->Date_Vars.Day++;
    }
    /* Date structure member: date_s.day = 30 */
    else
    {
      Date_Ptr->Date_Vars.Month++;
      Date_Ptr->Date_Vars.Day = 1;
    }
  }
  else if(Date_Ptr->Date_Vars.Month == 2)
  {
    if(Date_Ptr->Date_Vars.Day < 28)
    {
      Date_Ptr->Date_Vars.Day++;
    }
    else if(Date_Ptr->Date_Vars.Day == 28)
    {
      /* Leap year */
      if(IsLeapYear(Date_Ptr->Date_Vars.Year))
      {
        Date_Ptr->Date_Vars.Day++;
      }
      else
      {
        Date_Ptr->Date_Vars.Month++;
        Date_Ptr->Date_Vars.Day = 1;
      }
    }
    else if(Date_Ptr->Date_Vars.Day == 29)
    {
      Date_Ptr->Date_Vars.Month++;
      Date_Ptr->Date_Vars.Day = 1;
    }
  }
}
/*******************************************************************************
* Function Name  : Update_Time
* Description    : Write time to backup registers
* Input          : Pointer to structure which keeps tha time values
* Output         : None
* Return         : None
*******************************************************************************/
void Update_Time ( _Time * Time_Ptr )
{
    uint32_t Time_Temp;

    RTC_WaitForLastTask();  
    Time_Temp = Time_Ptr->Time_Vars.Hour*3600 + Time_Ptr->Time_Vars.Minute*60 + Time_Ptr->Time_Vars.Second;
    RTC_SetCounter ( Time_Temp );
}

/*******************************************************************************
* Function Name  : IsLeapYear
* Description    : Check whether the passed year is Leap or not.
* Input          : None
* Output         : None
* Return         : 1: leap year
*                  0: not leap year
*******************************************************************************/
uint8_t IsLeapYear(uint16_t Year)
{
  if(Year % 4 != 0) return 0;
  if(Year % 100 != 0) return 1;
  return (uint8_t)(Year % 400 == 0);
}

static void Set_Start_Date ( _Date * Date_Ptr )
{
    uint32_t i = 0;

    Lock_Counter = 0;

    if(RTC_GetCounter() / 86399 != 0)
    {
        for(i = 0; i < (RTC_GetCounter() / 86399); i++)
        {
          Update_Date( &Date );
          Lock_Counter++;
        }
        RTC_SetCounter(RTC_GetCounter() % 86399);
        Write_Date ( &Date ); 
    }
}

void Write_Date ( _Date * Date_Ptr )
{
    uint16_t Bkp_Tmp;

    Bkp_Tmp = Date_Ptr->Date_Vars.Day << 8;
    Bkp_Tmp |= Date_Ptr->Date_Vars.Month; 
    BKP_WriteBackupRegister(BKP_DR2, Bkp_Tmp);
    BKP_WriteBackupRegister(BKP_DR3, Date_Ptr->Date_Vars.Year);
    Bkp_Tmp = Date_Ptr->Date_Vars.Day_Of_Week << 8;
    BKP_WriteBackupRegister(BKP_DR4, Bkp_Tmp);

}

/*******************************************************************************
* Function Name  : IsLeapYear
* Description    : Check whether the passed year is Leap or not.
* Input          : None
* Output         : None
* Return         : 1: leap year
*                  0: not leap year
*******************************************************************************/
uint32_t Date_2_Days ( uint16_t Year, uint8_t Month, uint8_t Day )
{
    uint16_t i;
    uint32_t No_Of_Days;
    uint8_t Month_Tab[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31 ,30, 31 };

    No_Of_Days = 0;

    if ( Year > START_YEAR )
    {
        for ( i = START_YEAR; i < Year; i++ )
        {
            if ( IsLeapYear(i) == 1 )
            {
                No_Of_Days += 366;
            }
            else
            {
                No_Of_Days += 365;                        
            }
        }  
    }

    for ( i = 0; i < (Month-1); i++ )
    {
        No_Of_Days += Month_Tab[i];
    }
    
    if ( Month > 1 && IsLeapYear(Year) == 1 )
    {
        No_Of_Days += 1;                     
    } 
                
    No_Of_Days += Day;

    return No_Of_Days;
}

/*******************************************************************************
* Function Name  : RTC_WaitForLastTaskTimeout
* Description    : Waits until last write operation on RTC registers has finished.
*                  This function must be called before any write to RTC registers.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_WaitForLastTaskTimeout(void)
{
    while_timeout = 0;

    /* Loop until RTOFF flag is set */
    while ((RTC->CRL & RTC_FLAG_RTOFF) == (u16)RESET)
    {
        while_timeout++;
        if ( while_timeout >= 5000000)
        {
          LSE_FAILED_FLAG = TRUE;
        }
        if ( while_timeout >= 5000000)
        {
          break;
        }
    }
}

/*******************************************************************************
* Function Name  : RTC_WaitForSynchroTimeout
* Description    : Waits until the RTC registers (RTC_CNT, RTC_ALR and RTC_PRL)
*                  are synchronized with RTC APB clock.
*                  This function must be called before any read operation after
*                  an APB reset or an APB clock stop.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_WaitForSynchroTimeout(void)
{
    while_timeout = 0;

    /* Clear RSF flag */
    RTC->CRL &= (u16)~RTC_FLAG_RSF;

    /* Loop until RSF flag is set */
    while ((RTC->CRL & RTC_FLAG_RSF) == (u16)RESET)
    {
    
        while_timeout++;
        if ( while_timeout >= 5000000)
        {
          LSE_FAILED_FLAG = TRUE;
        }
        if ( while_timeout >= 5000000)
        {
          break;
        }
    }
}

/*******************************************************************************
* End file RTC.c
*******************************************************************************/

