/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Usb_Rx_Buffer[64];
uint8_t Usb_Tx_Buffer[64];
uint16_t i, j;
uint16_t Bkp_Reg_Temp;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_OUT_Callback(void)
{  
    PMAToUserBufferCopy(Usb_Rx_Buffer, ENDP1_RXADDR, 64);

    if (Usb_Rx_Buffer[0] == USB_READ_DATA)
    {
        Usb_Tx_Buffer[0] = USB_READ_DATA;
        Usb_Tx_Buffer[2] = TOTAL_PAGES;
        Usb_Tx_Buffer[3] = 0;

        for ( i=USB_SERVICE_VARIABLES_OFFSET_1, j=0;i<(USB_SERVICE_VARIABLES_OFFSET_1+sizeof(Service_Variables.Service_Tab8));i++, j++ )
        {
            Usb_Tx_Buf_Tab.Usb_Byte_Tab[i] = Service_Variables.Service_Tab8[j];
        }
  
        for ( i=USB_CONTROLLING_VARIABLES_OFFSET, j=0;i<(USB_CONTROLLING_VARIABLES_OFFSET+sizeof(Controlling_Variables.Controlling_Tab8));i++, j++ )
        {
            Usb_Tx_Buf_Tab.Usb_Byte_Tab[i] = Controlling_Variables.Controlling_Tab8[j];
        } 
     
        for ( i=USB_CONTROLLER_VARIABLES_OFFSET,j=0;i<(USB_CONTROLLER_VARIABLES_OFFSET+sizeof(Controller_Variables.Controller_Tab8));i++, j++ )
        {
            Usb_Tx_Buf_Tab.Usb_Byte_Tab[i] = Controller_Variables.Controller_Tab8[j];
        }   
  
        for ( i=USB_COUNTERS_OFFSET,j=0;i<(USB_COUNTERS_OFFSET+sizeof(Counters_Variables.Counters_Tab8));i++, j++ )
        {
            Usb_Tx_Buf_Tab.Usb_Byte_Tab[i] = Counters_Variables.Counters_Tab8[j];
        } 
  
        for ( i=USB_OTHER_VARIABLES_OFFSET,j=0;i<(USB_OTHER_VARIABLES_OFFSET+sizeof(Other_Variables.Other_Variables_Tab8));i++, j++ )
        {
            Usb_Tx_Buf_Tab.Usb_Byte_Tab[i] = Other_Variables.Other_Variables_Tab8[j];
        } 
    
        if ( Usb_Rx_Buffer[1] == PAGE_1 )
        {    
            Usb_Tx_Buffer[1] = PAGE_1;

            for ( i=4, j=USB_SERVICE_VARIABLES_OFFSET_1 ;i<64;i++, j++ )
            {
                Usb_Tx_Buffer[i] = Usb_Tx_Buf_Tab.Usb_Byte_Tab[j];
            }
        }
        else if ( Usb_Rx_Buffer[1] == PAGE_2 )
        {    
            Usb_Tx_Buffer[1] = PAGE_2;

            for ( i=4, j=USB_SERVICE_VARIABLES_OFFSET_2 ;i<64;i++, j++ )
            {
                Usb_Tx_Buffer[i] = Usb_Tx_Buf_Tab.Usb_Byte_Tab[j];
            }
        }
        else if ( Usb_Rx_Buffer[1] == PAGE_3 )
        {    
            Usb_Tx_Buffer[1] = PAGE_3;

            for ( i=4, j=USB_SERVICE_VARIABLES_OFFSET_3 ;i<64;i++, j++ )
            {
                Usb_Tx_Buffer[i] = Usb_Tx_Buf_Tab.Usb_Byte_Tab[j];
            }
        }    
        else if ( Usb_Rx_Buffer[1] == PAGE_4 )
        {    
            Usb_Tx_Buffer[1] = PAGE_4;

            for ( i=4, j=USB_CONTROLLING_VARIABLES_OFFSET; i<64; i++, j++ )
            {
                Usb_Tx_Buffer[i] = Usb_Tx_Buf_Tab.Usb_Byte_Tab[j];
            }
        }   
        else if ( Usb_Rx_Buffer[1] == PAGE_5 )
        {    
            Usb_Tx_Buffer[1] = PAGE_5;

            for ( i=4, j=USB_CONTROLLER_VARIABLES_OFFSET; i<64; i++, j++ )
            {
                Usb_Tx_Buffer[i] = Usb_Tx_Buf_Tab.Usb_Byte_Tab[j];
            }
        } 
        else if ( Usb_Rx_Buffer[1] == PAGE_6 )
        {    
            Usb_Tx_Buffer[1] = PAGE_6;

            for ( i=4, j=USB_COUNTERS_OFFSET; i<64; i++, j++ )
            {
                Usb_Tx_Buffer[i] = Usb_Tx_Buf_Tab.Usb_Byte_Tab[j];
            }
        } 
        else if ( Usb_Rx_Buffer[1] == PAGE_7 )
        {    
            Usb_Tx_Buffer[1] = PAGE_7;

            for ( i=4, j=USB_OTHER_VARIABLES_OFFSET; i<64; i++, j++ )
            {
                Usb_Tx_Buffer[i] = Usb_Tx_Buf_Tab.Usb_Byte_Tab[j];
            }
        }  
    
        UserToPMABufferCopy(Usb_Tx_Buffer, ENDP1_TXADDR, 64);
        SetEPTxCount(ENDP1, 64);
        SetEPTxValid(ENDP1);
    }

    else if (Usb_Rx_Buffer[0] == USB_WRITE_SN)
    {
        for( i = SOFTWARE_VER_OFFSET, j = (SOFTWARE_VER_OFFSET + 1); i < (SOFTWARE_VER_OFFSET + SOFTWARE_VER_SIZE); i++, j++)
        {
            Controller_Variables.Controller_Tab8[i] = Usb_Rx_Buffer[j];
        }
        Bkp_Reg_Temp = 0;
        Bkp_Reg_Temp =  Controller_Variables.Controller_Vars.Software_Version[0] << 8;
        Bkp_Reg_Temp |= Controller_Variables.Controller_Vars.Software_Version[1]; 
        BKP_WriteBackupRegister(BKP_DR9, Bkp_Reg_Temp);
        Bkp_Reg_Temp = 0;
        Bkp_Reg_Temp =  Controller_Variables.Controller_Vars.Software_Version[2] << 8;
        Bkp_Reg_Temp |= Controller_Variables.Controller_Vars.Controller_Date[0]; 
        BKP_WriteBackupRegister(BKP_DR10, Bkp_Reg_Temp);
        Bkp_Reg_Temp = 0;
        Bkp_Reg_Temp =  Controller_Variables.Controller_Vars.Controller_Date[1] << 8;
        Bkp_Reg_Temp |= Controller_Variables.Controller_Vars.Controller_Date[2]; 
        BKP_WriteBackupRegister(BKP_DR11, Bkp_Reg_Temp);
        BKP_WriteBackupRegister(BKP_DR12, Controller_Variables.Controller_Vars.Serial_Number);

    }

    else if (Usb_Rx_Buffer[0] == USB_WRITE_TIME)
    {
        for( i = 1, j = 0; j < sizeof(Date_Set.Date_Tab); i++, j++)
        {
            Date_Set.Date_Tab[j] = Usb_Rx_Buffer[i];
        }
        for( i = (sizeof(Date_Set.Date_Tab)+1), j = 0; j < sizeof(Time_Set.Time_Tab); i++, j++)
        {
            Time_Set.Time_Tab[j] = Usb_Rx_Buffer[i];
        }
    
        Date_Set.Date_Vars.Day_Of_Week--;
        Write_Date ( &Date_Set );
        Update_Time ( &Time_Set );    
    }
    else if (Usb_Rx_Buffer[0] == USB_UNLOCK_FROM_TEST)
    {
        Controller_Variables.Controller_Vars.Test_Marker = 0;
        BKP_WriteBackupRegister(BKP_DR13, Controller_Variables.Controller_Vars.Test_Marker);
        Machine_State = STANDBY;
        Screen_Number = CONTROLLER_ON;
        Reload_Screen = TRUE;
    }
    
    SetEPRxStatus(ENDP1, EP_RX_VALID);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

