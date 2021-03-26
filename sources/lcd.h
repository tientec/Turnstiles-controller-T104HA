#ifndef _LCD_H
#define _LCD_H

/*******************************************************************************
********************************************************************************
* File Name          : lcd.h
* Author             : Tomasz Nowak
* Version            : V1.00a
* Date               : 05/07/2012
* Description        : Header file for lcd.c
********************************************************************************
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/

/*******************************************************************************
* Exported Preprocessor #define Constants
*******************************************************************************/
#define LCD_PORT_RS	GPIOB
#define LCD_PIN_RS	GPIO_Pin_8

#define LCD_PORT_E	GPIOB
#define LCD_PIN_E	GPIO_Pin_9

#define LCD_PORT_D0	GPIOB
#define LCD_PIN_D0	GPIO_Pin_12
#define LCD_PORT_D1	GPIOB
#define LCD_PIN_D1	GPIO_Pin_13
#define LCD_PORT_D2	GPIOB
#define LCD_PIN_D2	GPIO_Pin_14
#define LCD_PORT_D3	GPIOB
#define LCD_PIN_D3	GPIO_Pin_15

#define LCD_V_SIZE      4
#define LCD_H_SIZE      20

#define LCD_SIZE	80

#define LINE1_ADDRESS   0x80
#define LINE2_ADDRESS   0xC0
#define LINE3_ADDRESS   0x94
#define LINE4_ADDRESS   0xD4

/*******************************************************************************
* Exported Preprocessor #define MACROS
*******************************************************************************/

/*******************************************************************************
* Exported Type Declarations
*******************************************************************************/

typedef union {
	volatile uint8_t Buffer[80];
	volatile uint8_t BufferTab[4][20];
} _Lcd_Buffer;

typedef struct {
    uint8_t Object_X_Pos;
    uint8_t Object_Y_Pos;
} _Object_Pos;


/*******************************************************************************
* Exported Const Object Declarations
*******************************************************************************/
extern const uint8_t Lcd_Address_Base[4];


/*******************************************************************************
* Exported Object Declarations
*******************************************************************************/
extern _Lcd_Buffer Lcd_Buffer;
extern _Object_Pos Object_Pos;
extern _Object_Pos Object_Pos_Temp;


/*******************************************************************************
* Exported Function Prototypes
*******************************************************************************/
void Lcd_Init 	( void );
void Lcd_Clear 	( void );
void Lcd_Delay 	( uint16_t n );
void Lcd_Data 	( uint8_t data );
void Lcd_Cmd 	( uint8_t command );
void Lcd_Write_Text ( const uint8_t * const string );
void Lcd_Set_Address ( uint8_t X_Pos, uint8_t Y_Pos );
void Lcd_Load_Buffer ( _Lcd_Buffer * Buffer );
void Lcd_Clear_Buffer ( _Lcd_Buffer * Buffer );
void Lcd_Write_Buffer ( _Lcd_Buffer * Buffer, const uint8_t * string );

/*******************************************************************************
 * Exported Inline Function Definitions and #define Function-Like Macros
*******************************************************************************/

/*******************************************************************************
* End fo file lcd.h
*******************************************************************************/
#endif
