/*******************************************************************************
********************************************************************************
* File Name          : lcd.c
* Author             : Tomasz Nowak
* Version            : V1.00A
* Date               : 05/07/2012
* Description        : Lcd handling
********************************************************************************
*******************************************************************************/


/*******************************************************************************
* Header Files
*******************************************************************************/
#include "includes.h"


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
const uint8_t Lcd_Address_Base[4] = { LINE1_ADDRESS, LINE2_ADDRESS, LINE3_ADDRESS, LINE4_ADDRESS };


/*******************************************************************************
* Local Object Definitions
*******************************************************************************/
uint8_t i;
volatile uint8_t j, k;

_Lcd_Buffer Lcd_Buffer;
_Object_Pos Object_Pos;
_Object_Pos Object_Pos_Temp;

const uint8_t * Init_Text = "Hardware init...";
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
* Function Name  : Lcd_Init
* Description    : Lcd initialization
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Lcd_Init (void)
{
    Lcd_Delay ( 65000 );//Wait for hardware LCD stabilization
	Lcd_Cmd ( 0x28 );	//4bit / 2lines / big_letters
	Lcd_Cmd ( 0x06 );	//inc / no_scroll
	Lcd_Cmd ( 0x01 );	//clear & reset
	Lcd_Delay ( 5000 );	//delay 1,6ms
	Lcd_Cmd ( 0x0C );	//screen / cursor / blink
    Lcd_Write_Text ( Init_Text );
}

/*******************************************************************************
* Function Name  : Lcd_Clear
* Description    : Clear the display command
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Lcd_Clear ( void )
{
	Lcd_Cmd ( 0x01 );
	Lcd_Delay ( 5000 );	//delay 1,6ms
	Lcd_Cmd ( 0x0C );
}

/*******************************************************************************
* Function Name  : Lcd_Delay
* Description    : Delay
* Input          : delay value - in arbitrary units
* Output         : None
* Return         : None
*******************************************************************************/
void Lcd_Delay ( volatile uint16_t n )
{
	volatile uint16_t decreg;
	decreg = n;
	while(--decreg!=0);  

}

/*******************************************************************************
* Function Name  : Lcd_Data
* Description    : Send data to LCD
* Input          : 8 bit data value
* Output         : None
* Return         : None
*******************************************************************************/
void Lcd_Data ( uint8_t Data )
{
    GPIO_SetBits(LCD_PORT_RS, LCD_PIN_RS);

    GPIO_ResetBits(GPIOB, LCD_PIN_D0 | LCD_PIN_D1 | LCD_PIN_D2 | LCD_PIN_D3 );

	i = ((Data >> 4) & 0x0F);		//send higher nibble

	if ( i & 0x01)
        GPIO_SetBits(LCD_PORT_D0, LCD_PIN_D0);
	if ( i & 0x02)
        GPIO_SetBits(LCD_PORT_D1, LCD_PIN_D1);
	if ( i & 0x04)
        GPIO_SetBits(LCD_PORT_D2, LCD_PIN_D2);
	if ( i & 0x08)
        GPIO_SetBits(LCD_PORT_D3, LCD_PIN_D3);

    GPIO_SetBits(LCD_PORT_E, LCD_PIN_E);
    GPIO_ResetBits(LCD_PORT_E, LCD_PIN_E);
   
    GPIO_ResetBits(GPIOB, LCD_PIN_D0 | LCD_PIN_D1 | LCD_PIN_D2 | LCD_PIN_D3 );

	i = (Data & 0x0F);				//send lower nibble

	if ( i & 0x01)
        GPIO_SetBits(LCD_PORT_D0, LCD_PIN_D0);
	if ( i & 0x02)
        GPIO_SetBits(LCD_PORT_D1, LCD_PIN_D1);
	if ( i & 0x04)
        GPIO_SetBits(LCD_PORT_D2, LCD_PIN_D2);
	if ( i & 0x08)
        GPIO_SetBits(LCD_PORT_D3, LCD_PIN_D3);

    GPIO_SetBits(LCD_PORT_E, LCD_PIN_E);
    GPIO_ResetBits(LCD_PORT_E, LCD_PIN_E);

	Lcd_Delay ( 240 );
}

/*******************************************************************************
* Function Name  : Lcd_Command
* Description    : Send command to LCD
* Input          : 8 bit command value
* Output         : None
* Return         : None
*******************************************************************************/
void Lcd_Cmd ( uint8_t Command )
{
    GPIO_ResetBits(LCD_PORT_RS, LCD_PIN_RS);

    GPIO_ResetBits(GPIOB, LCD_PIN_D0 | LCD_PIN_D1 | LCD_PIN_D2 | LCD_PIN_D3 );

	i = ((Command >> 4) & 0x0F);		//send higher nibble

	if ( i & 0x01)
        GPIO_SetBits(LCD_PORT_D0, LCD_PIN_D0);
	if ( i & 0x02)
        GPIO_SetBits(LCD_PORT_D1, LCD_PIN_D1);
	if ( i & 0x04)
        GPIO_SetBits(LCD_PORT_D2, LCD_PIN_D2);
	if ( i & 0x08)
        GPIO_SetBits(LCD_PORT_D3, LCD_PIN_D3);

    GPIO_SetBits(LCD_PORT_E, LCD_PIN_E);
    GPIO_ResetBits(LCD_PORT_E, LCD_PIN_E);
   
    GPIO_ResetBits(GPIOB, LCD_PIN_D0 | LCD_PIN_D1 | LCD_PIN_D2 | LCD_PIN_D3 );

	i = (Command & 0x0F);				//send lower nibble

	if ( i & 0x01)
        GPIO_SetBits(LCD_PORT_D0, LCD_PIN_D0);
	if ( i & 0x02)
        GPIO_SetBits(LCD_PORT_D1, LCD_PIN_D1);
	if ( i & 0x04)
        GPIO_SetBits(LCD_PORT_D2, LCD_PIN_D2);
	if ( i & 0x08)
        GPIO_SetBits(LCD_PORT_D3, LCD_PIN_D3);

    GPIO_SetBits(LCD_PORT_E, LCD_PIN_E);
    GPIO_ResetBits(LCD_PORT_E, LCD_PIN_E);

	Lcd_Delay ( 5000 );							//delay 40 us
}

/*******************************************************************************
* Function Name  : Lcd_write_Text
* Description    : Send string to LCD
* Input          : pointer to string
* Output         : None
* Return         : None
*******************************************************************************/
void Lcd_Write_Text ( const uint8_t * string )
{
    while (*string != 0) 
	{
       	Lcd_Data(*(string++));
   	}

}

/*******************************************************************************
* Function Name  : Lcd_Set_Address
* Description    : Set buffer position
* Input          : 8 bit positions value - X and Y
* Output         : None
* Return         : None
*******************************************************************************/
void Lcd_Set_Address ( uint8_t X_Pos, uint8_t Y_Pos )
{
    if ( X_Pos >= LCD_H_SIZE )
    {
        X_Pos = 0;
    }

    if ( Y_Pos >= LCD_V_SIZE )
    {
        Y_Pos = 0;
    }

    Object_Pos.Object_X_Pos = X_Pos;
    Object_Pos.Object_Y_Pos = Y_Pos;
}

/*******************************************************************************
* Function Name  : Lcd_Write_Buffer
* Description    : Load buffer with data
* Input          : Pointer to buffer, Pointer to string to be loaded
* Output         : None
* Return         : None
*******************************************************************************/
void Lcd_Write_Buffer ( _Lcd_Buffer * Buffer, const uint8_t * string )
{
    j = 0;

    while (*string != 0) 
	{
        Buffer->BufferTab[Object_Pos.Object_Y_Pos][Object_Pos.Object_X_Pos+j] = *(string++);
        j++;
    }

}

/*******************************************************************************
* Function Name  : Lcd_Load_buffer
* Description    : Send buffer to LCD
* Input          : Pointer to buffer to be sent
* Output         : None
* Return         : None
*******************************************************************************/
void Lcd_Load_Buffer ( _Lcd_Buffer * Buffer )
{
    if ( Lcd_Write_Strobe == TRUE )
    {
        Lcd_Cmd(Lcd_Address_Base[Lcd_Write_Ptr]);    
    
        for ( j = 0; j < LCD_H_SIZE; j++ )
        {
            Lcd_Data ( Buffer->BufferTab[Lcd_Write_Ptr][j] );
        }     

     
        Lcd_Write_Strobe = FALSE;

        if (++Lcd_Write_Ptr == LCD_V_SIZE )
        {
            Enable_Lcd_Write = FALSE;
        }
    }
}

/*******************************************************************************
* Function Name  : Lcd_Clear_Buffer
* Description    : Clear buffer - load with " " value
* Input          : Pointer to buffer to be cleared
* Output         : None
* Return         : None
*******************************************************************************/
void Lcd_Clear_Buffer ( _Lcd_Buffer * Buffer )
{
    for ( j = 0; j < LCD_SIZE; j++ )
    {
        Buffer->Buffer[j] = ' ';
    }
}

/*******************************************************************************
* End file lcd.c.c
*******************************************************************************/










