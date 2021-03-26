#ifndef _SYS_H
#define _SYS_H

/*******************************************************************************
********************************************************************************
* File Name          : sys.h
* Author             : Tomasz Nowak
* Version            : V1.00a
* Date               : 27/01/2012
* Description        : sys.c header file
********************************************************************************
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/


/*******************************************************************************
* Exported Preprocessor #define Constants
*******************************************************************************/
#define ENC_OFFSET          1000
#define ENC_MAX             1023
#define IND_OFFSET          100
#define IND_TEST_VALUE      20

#define IND_MARKER_FWD      0x01
#define IND_MARKER_BCK      0x02

#define T_SAMPLE_TIME       5

#define ENCODER_SAMPLE_TIME 2
#define STARTUP_TIME		80
#define KBD_SAMPLE_TIME		5
#define INPUTS_SAMPLE_TIME  2
#define SCREEN_RELOAD_TIME  100

#define TIME_05_SEC         50

#define SPI_CS_ENCODER_PORT GPIOA
#define SPI_CS_ENCODER_PIN  GPIO_Pin_4

#define KBD_PORT_ENTER		GPIOA
#define KBD_RELEASED_ENTER	GPIO_Pin_13
#define KBD_PORT_ESCAPE		GPIOC
#define KBD_RELEASED_ESCAPE	GPIO_Pin_11
#define KBD_PORT_LEFT		GPIOC
#define KBD_RELEASED_LEFT	GPIO_Pin_12
#define KBD_PORT_UP		    GPIOB
#define KBD_RELEASED_UP 	GPIO_Pin_4
#define KBD_PORT_RIGHT		GPIOA
#define KBD_RELEASED_RIGHT	GPIO_Pin_14
#define KBD_PORT_DOWN		GPIOC
#define KBD_RELEASED_DOWN	GPIO_Pin_10

#define KEY_ENTER           0x01
#define KEY_ESCAPE          0x02
#define KEY_LEFT            0x04
#define KEY_UP              0x08
#define KEY_RIGHT           0x10
#define KEY_DOWN            0x20

#define		INPUT_FORWARD	0x01
#define		INPUT_BACK		0x02
#define     INPUT_TEST      0x04

#define		SAP_INPUT		    0x0001
#define     LIGHT_SENSOR_INPUT  0x0002
#define     IND_SENSOR1         0x0004
#define     IND_SENSOR2         0x0008
#define     IND_SENSOR3         0x0010
#define     FORWARD_ENABLE      0x0020
#define     BACK_ENABLE         0x0040

#define     INPUT1              0x0001
#define     INPUT2              0x0002
#define     INPUT3              0x0004
#define     INPUT4              0x0008
#define     INPUT5              0x0010
#define     INPUT6              0x0020
#define     INPUT7              0x0040
#define     INPUT8              0x0080

#define PORT_LED_RED_FWD    GPIOA
#define PORT_LED_GRN_FWD    GPIOB
#define PORT_LED_RED_BCK    GPIOB
#define PORT_LED_GRN_BCK    GPIOB

#define PIN_LED_RED_FWD     GPIO_Pin_15
#define PIN_LED_GRN_FWD     GPIO_Pin_2
#define PIN_LED_RED_BCK     GPIO_Pin_10
#define PIN_LED_GRN_BCK     GPIO_Pin_3

#define PORT_MOTOR          GPIOA
#define PORT_MOTOR_DIR      GPIOA
#define PORT_BRAKE          GPIOA
#define PORT_BOLT1          GPIOC
#define PORT_BOLT2          GPIOC
#define PORT_AD_OUT1        GPIOC
#define PORT_AD_OUT2        GPIOC

#define MOTOR_TIMER         TIM1
#define BRAKE_TIMER         TIM1
#define BOLT1_TIMER         TIM3
#define BOLT2_TIMER         TIM3
#define AOUT1_TIMER         TIM3
#define AOUT2_TIMER         TIM3

#define PIN_MOTOR           GPIO_Pin_9
#define PIN_MOTOR_DIR       GPIO_Pin_10
#define PIN_BRAKE           GPIO_Pin_8
#define PIN_BOLT1           GPIO_Pin_9
#define PIN_BOLT2           GPIO_Pin_7
#define PIN_AOUT1           GPIO_Pin_8
#define PIN_AOUT2           GPIO_Pin_6

#define PORT_RELAY1         GPIOB
#define PORT_RELAY2         GPIOC
#define PORT_RELAY3         GPIOB
#define PORT_RELAY4         GPIOA

#define PIN_RELAY1          GPIO_Pin_11
#define PIN_RELAY2          GPIO_Pin_13
#define PIN_RELAY3          GPIO_Pin_5
#define PIN_RELAY4          GPIO_Pin_2

#define PORT_BUZZER         GPIOD

#define PIN_BUZZER          GPIO_Pin_2

#define FAN_PORT            GPIOA

#define FAN_PIN             GPIO_Pin_3

#define PORT_IN1            GPIOC
#define PORT_IN2            GPIOC
#define PORT_IN3            GPIOC
#define PORT_IN4            GPIOC
#define PORT_IN5            GPIOC
#define PORT_IN6            GPIOC
#define PORT_IN7            GPIOB
#define PORT_IN8            GPIOB

#define PIN_IN1             GPIO_Pin_3
#define PIN_IN2             GPIO_Pin_2
#define PIN_IN3             GPIO_Pin_1
#define PIN_IN4             GPIO_Pin_0
#define PIN_IN5             GPIO_Pin_4
#define PIN_IN6             GPIO_Pin_5
#define PIN_IN7             GPIO_Pin_0
#define PIN_IN8             GPIO_Pin_1
/*******************************************************************************
* Exported Preprocessor #define MACROS
*******************************************************************************/


/*******************************************************************************
* Exported Type Declarations
*******************************************************************************/
typedef struct {
    uint8_t Keyboard_Memory;
    uint8_t Keyboard_State;
    uint8_t Keyboard_Flag;
} _Keyboard;

typedef enum {
	T_1msec = 0,
	T_10msec,
	T_100msec,
	T_1sec,
	T_1min,
} _Sys_Tmr_Intervals;

typedef union {
	volatile uint32_t Time_Table[5];
	struct {
		volatile uint32_t Timer_1ms;
		volatile uint32_t Timer_10ms;
		volatile uint32_t Timer_100ms;
		volatile uint32_t Timer_1s;
		volatile uint32_t Timer_1min;
	} Counters;
} _Timers;

typedef struct {
	uint32_t Timer;
} _System_Timers;
/*******************************************************************************
* Exported Const Object Declarations
*******************************************************************************/


/*******************************************************************************
* Exported Object Declarations
*******************************************************************************/
extern _Timers System_Timers;
extern _System_Timers Keyboard_Timer;
extern _System_Timers Inputs_Timer;
extern _System_Timers Startup_Timer;
extern _System_Timers Encoder_Timer;
extern _System_Timers Pulse_Leds_Timer;

extern _System_Timers Screen_Reload_Timer;

extern _System_Timers Trans_Timer;

extern _Keyboard Keyboard;
extern _Keyboard Inputs;

extern volatile uint16_t Arm_Pos;

extern volatile uint32_t Interval;
extern volatile uint32_t Interval_Sec;

extern volatile uint16_t Inputs_Level;
extern volatile uint16_t Inputs_Level_Test;

extern uint16_t Fast_Pass_Marker;
extern uint16_t Free_Exit_Marker;
extern uint16_t Zbyszek;

/*******************************************************************************
* Function Name  : Heat_Cool
* Description    : Heater and ventilation handling
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/*******************************************************************************
* Exported Function Prototypes
*******************************************************************************/
void Sys_Init ( void );
void Sys_sys ( void );
void System_Timer_Setting ( uint32_t * Timer_Counter, uint16_t Time_Value, _Sys_Tmr_Intervals Sys_Tmr_Intervals );
uint8_t System_Timer_Check ( uint32_t * Timer_Counter, _Sys_Tmr_Intervals Sys_Tmr_Intervals );
void Keyboard_Handling ( void );
void Inputs_Handling ( void );
uint16_t Encoder_Pos_Read ( void );
void Sap_Handling ( void );
void Stop_Angles_Set ( void );
void Analog_Measure ( void );
void Internal_Fan_Control ( void );
void Periods_Handling ( void );
void Back_Signals_Control(void);
void Output_1_Control(void);
void Output_2_Control(void);
void Output_3_Control(void);
void Output_4_Control(void);
void Input_Output_Test ( void );
/*******************************************************************************
 * Exported Inline Function Definitions and #define Function-Like Macros
*******************************************************************************/


/*******************************************************************************
* End fo file sys.h
*******************************************************************************/
#endif /*_SYS_H*/



