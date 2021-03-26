#ifndef _RTC_H
#define _RTC_H

/*******************************************************************************
********************************************************************************
* File Name          : rtc.h
* Author             : Tomasz Nowak
* Version            : V1.00a
* Date               : 06/07/2012
* Description        : rtc header file
********************************************************************************
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/


/*******************************************************************************
* Exported Preprocessor #define Constants
*******************************************************************************/


/*******************************************************************************
* Exported Preprocessor #define MACROS
*******************************************************************************/


/*******************************************************************************
* Exported Type Declarations
*******************************************************************************/


/*******************************************************************************
* Exported Const Object Declarations
*******************************************************************************/


/*******************************************************************************
* Exported Object Declarations
*******************************************************************************/
typedef union {
    uint8_t Date_Tab[5];
    struct {
        uint16_t Year; 
        uint8_t Day_Of_Week;
        uint8_t Day;
        uint8_t Month;

    } Date_Vars;
} _Date;

typedef union {
    uint8_t Time_Tab[3];
    struct {
        uint8_t Second;
        uint8_t Minute;
        uint8_t Hour;
    } Time_Vars;
} _Time;


extern _Date Date;
extern _Date Date_Set;
extern _Time Time;
extern _Time Time_Set;

/*******************************************************************************
* Exported Function Prototypes
*******************************************************************************/
void RTC_Init ( void );
void RTC_Handler ( void );
void Set_Date ( _Date * Date_Ptr );
void Set_Time ( _Time * Time_Ptr );
void Read_Date ( _Date * Date_Ptr );
void Read_Time ( _Time * Time_Ptr );
void Update_Date ( _Date * Date_Ptr );
void Update_Time ( _Time * Time_Ptr );
void Write_Date ( _Date * Date_Ptr );
uint8_t IsLeapYear(uint16_t Year);
uint32_t Date_2_Days ( uint16_t Year, uint8_t Month, uint8_t Day );

void RTC_WaitForLastTaskTimeout(void);
void RTC_WaitForSynchroTimeout(void);
/*******************************************************************************
 * Exported Inline Function Definitions and #define Function-Like Macros
*******************************************************************************/


/*******************************************************************************
* End fo file RTC.h
*******************************************************************************/
#endif /*_RTC_H*/



