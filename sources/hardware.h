#ifndef _HARDWARE_H
#define _HARDWARE_H

/*******************************************************************************
********************************************************************************
* File Name          : hardware.h
* Author             : Tomasz Nowak
* Version            : V1.00a
* Date               : 06/07/2012
* Description        : hardware.c header file
********************************************************************************
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/


/*******************************************************************************
* Exported Preprocessor #define Constants
*******************************************************************************/
#define PWM_OFF         0
#define PWM_FULL        1000
#define PWM_MIN         200
#define PWM_RESOLUTION  50

/*******************************************************************************
* Exported Preprocessor #define MACROS
*******************************************************************************/


/*******************************************************************************
* Exported Type Declarations
*******************************************************************************/
typedef enum _Motor_Dir {
	FORWARD = 0,
	BACK = 1,
} Motor_Dir;

extern NVIC_InitTypeDef NVIC_InitStructure;
extern SPI_InitTypeDef  SPI_InitStructure;
extern GPIO_InitTypeDef GPIO_InitStructure;
extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
extern TIM_OCInitTypeDef  TIM_OCInitStructure;
extern ADC_InitTypeDef ADC_InitStructure;

extern volatile uint16_t ADC_ConvertedValueTab[2];

extern uint32_t while_timeout;

extern ErrorStatus HSEStartUpStatus;

/*******************************************************************************
* Exported Const Object Declarations
*******************************************************************************/


/*******************************************************************************
* Exported Object Declarations
*******************************************************************************/


/*******************************************************************************
* Exported Function Prototypes
*******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void NVIC_Enable ( void );
void Tim_Coufiguration(void);
void SPI_Configuration(void);
void Adc_Configuration ( void );
void WWDT_Init( void );
void WWDT_Handler( void );
void FLASH_Init( void );
void DMA_Configuration(void);
void EXTI_Configuration ( void );


void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);

//void Software_Delay ( uint32_t Delay_var );


/*******************************************************************************
 * Exported Inline Function Definitions and #define Function-Like Macros
*******************************************************************************/


/*******************************************************************************
* End fo file hardware.h
*******************************************************************************/
#endif /*_HARDWARE_H*/



