/******************** (C) COPYRIGHT 2012 TIENTEC *******************************
* File Name          : main.c
* Author             : Tomasz Nowak
* Version            : V1.00
* Date               : 05/07/2012
* Description        : Main program body
*******************************************************************************/

//ZMIANY TEMP
// hardware.c - struktura do pwm
// PWM_MIN z 200 na 0

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
  
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
#ifdef DEBUG
    debug();
#endif

    Registers_Init();
    Hardware_Init();
    Set_USBClock();
    Lcd_Init();
    Gui_Init();
    RTC_Init();
    Sys_Init();
    WWDT_Init ();
    USB_Init();

    while (1)
    {   
        Gui();
        Sys_sys();
        Control();
    }
}


/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
