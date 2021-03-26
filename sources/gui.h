#ifndef _GUI_H
#define _GUI_H

/*******************************************************************************
********************************************************************************
* File Name          : gui.h
* Author             : Tomasz Nowak
* Version            : V1.00a
* Date               : 05/07/2011
* Description        : Header file for gui.c
********************************************************************************
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/

/*******************************************************************************
* Exported Preprocessor #define Constants
*******************************************************************************/
#define STRINGS_COUNT   255

#define STRING_LICENCE_LOCKED    0b0000000001110111
#define STRING_LICENCE_UNLOCKED  0b0000000000001011
#define VAR_LICENCE_UNLOCKED  0b0000000000000000
#define VAR_LICENCE_LOCKED    0b0000000000000111

#define ENABLE_STRING_WW        0b0000000000010000
#define ENABLE_STRING_SP        0b0000000000100000
#define ENABLE_STRING_SAP       0b0000000001000000
#define ENABLE_STRING_BLO       0b0000000010000000
#define ENABLE_STRING_P         0b0000000100000000
#define ENABLE_STRING           0b0000000111111111

#define IND_MASK_STATUS         0b0000000000000011
#define IND_MASK_LINES          0b0000000000000111
#define IND_MASK_TEST           0b0000000000001011
#define IND_MASK_OK             0b0000000000010011
#define IND_MASK_FALSE          0b0000000000100011

#define TEST_STARTED            0b0000000000011111
#define TEST_STOPPED            0b0000000000101111

#define STER_ODBLOKOWANY        0
#define STER_ZABLOKOWANY        1
#define STER_BLOKADA_CALKOWITA  2
#define STER_BLOKOWANY          3

#define NUM_OF_LANG             1

#define EEPROM_BASE_ADDRESS     ((uint32_t)0x0801FC00)
#define EEPROM_END_ADDRESS      ((uint32_t)0x08020000)


#define CONTROLLER_PROGRAMMED   0xa5
#define GO_2_TEST_PROGRAM       0xa5a5

#define SERVICE_PASSWORD        86635
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/  
extern volatile uint32_t Flash_Address;
extern volatile uint32_t NbrOfPage;
extern volatile FLASH_Status FLASHStatus;

//USB DEFINES
#define USB_READ_DATA           0x81
#define USB_WEITE_DATA          0x82
#define USB_UPDATE_MCU_FLASH    0x83
#define USB_STATUS_WR_OK        0x84
#define USB_STATUS_WR_NOK       0x85
#define USB_WRITE_SN            0x86
#define USB_WRITE_TIME          0x87
#define USB_UNLOCK_FROM_TEST    0x88

#define USB_SERVICE_VARIABLES_OFFSET_1      0x0000//60 bytes
#define USB_SERVICE_VARIABLES_OFFSET_2      0x003C//60 bytes
#define USB_SERVICE_VARIABLES_OFFSET_3      0x0078//60 bytes
#define USB_CONTROLLING_VARIABLES_OFFSET    0x00B4//60 bytes
#define USB_CONTROLLER_VARIABLES_OFFSET     0x00F0//60 bytes
#define USB_COUNTERS_OFFSET                 0x012C//60 bytes
#define USB_OTHER_VARIABLES_OFFSET          0x0168//60 bytes
#define USB_END_TABLE                       0x01A4//60 bytes

#define SOFTWARE_VER_OFFSET     8
#define SOFTWARE_VER_SIZE       14

#define PAGE_1                  0x00
#define PAGE_2                  0x01
#define PAGE_3                  0x02
#define PAGE_4                  0x03
#define PAGE_5                  0x04
#define PAGE_6                  0x05
#define PAGE_7                  0x06

#define TOTAL_PAGES             0x06

#define START_YEAR              2015
/*******************************************************************************
* Exported Preprocessor #define MACROS
*******************************************************************************/

/*******************************************************************************
* Exported Type Declarations
*******************************************************************************/
typedef enum {
    TEXT_VAR = 0,
    DIGITS_VAR,
} _Var_Type;

typedef enum {
    VAR_U8 = 0,
    VAR_U16,
    VAR_U32,
    VAR_S8,
    VAR_S16,
    VAR_S32,
} _Var_Size;

typedef enum {
    STANDBY_SCR = 0,
    CONTROLLER_ON,
    MENU,
    PARAM_STER,
    UST_STER,
    WYB_BRAM,
    DATA_I_CZAS,
    LICENCJA,
    TEST,
    HASLO,
    WYB_JEZ,
    HAS_MENU,
    HAS_BLOK,
    KAL_RTC, 
    PARAM_MECH1,
    PARAM_MECH2,
    PARAM_MECH3,
    PARAM_MECH4,
    PARAM_STER1,
    PARAM_STER2,
    PARAM_STER3,
    HEAT_COOL1,
    OSWIETLENIE,
    FUNCTIONS1,
    FUNCTIONS2,
    FUNCTIONS3,
    FUNCTIONS4,
    FUNCTIONS5,
    PROG_TESTOWY,
    UST_ENKODERA,
    BLOK_CALK,
    STER_BLOK,
    ODBLOKUJ,
    UST_FABRYCZNE,
    ZER_LICZNIKI,
    UST_SERWISOWE1,
    UST_SERWISOWE2,
    UST_SERWISOWE3,
    UST_SERWISOWE4,    
    ODCZYT_SERWISOWY,
    IND_TEST,
    TEST_WEWY,
    INFO,
} _Screen_Number;
/*******************************************************************************
* Exported Const Object Declarations
*******************************************************************************/

/*******************************************************************************
* Exported Object Declarations
*******************************************************************************/

typedef union {
    uint8_t Controlling_Tab8[32];
    uint32_t Variables_Tab[8];
    struct {
//1
        uint8_t Predkosc_Silnika1;
        uint8_t Predkosc_Silnika2;
        uint16_t Czas_otwarcia_bramki;

        uint16_t Czas_syg_zwr;
        uint16_t Encoder;
 
        uint8_t Predkosc_Silnika3;
        uint8_t Predkosc_Silnika4;
        uint16_t Heat_Min;

        uint16_t Heat_Max;
        uint16_t Vent_Min;
//5
        uint16_t Vent_Max;
        uint8_t Heat_Marker;
        uint8_t Vent_Marker;
//6
        uint16_t Transition_time;
        uint16_t Free_Exit_Time;
//7        
        uint16_t Normal_Exit_Time;
        uint8_t Exit_Var;
        uint8_t Back_Signal_Type;
//8
        uint16_t Trans_time;

    } Ctrl_Vars;
} _Controlling_Variables;

typedef union {
    uint8_t Service_Tab8[128];
    uint32_t Service_Tab[32];
    struct {
//0
        uint32_t Haslo_Dostepu;
        uint32_t Haslo_Blokady;
    
        uint8_t Blokada;
        uint8_t Ilosc_Prob_Dostepu;
        uint8_t Ilosc_Prob_Blokady;
        uint8_t PWM_Lewy_Rygiel;
    
        uint8_t PWM_Prawy_Rygiel;
        uint8_t PWM_Zwora;
        uint8_t PWM_Zbijak;    
        uint8_t PWM_LR_Var;
    
        uint8_t PWM_PR_Var;
        uint8_t PWM_Ham_Var;
        uint8_t PWM_Zbijak_Var;
        uint8_t Lewy_Rygiel;
    
        uint8_t Prawy_Rygiel;
        uint8_t Hamulec;
        uint8_t KOlowrotek;
        uint8_t Rygle_var;
//5        
        uint8_t Jezyk;
        uint8_t Sap;
        uint8_t W_wyjscie;
        uint8_t Szybkie_wejscie;
        
        uint8_t Blokada_dzien;
        uint8_t Blokada_miesiac;
        uint16_t Blokada_rok;
        
        uint8_t RTC_Cal_Var;       
        uint8_t Marker_Blokady;
        uint8_t Blok_Dostep_Pozostalo;
        uint8_t Blok_Prog_Pozostalo;
        
        uint16_t Lock_Interval;
        uint16_t Lock_Counter;
        
        uint8_t Typ_Bramki;
        uint8_t First_On_Marker;
        uint16_t Serwis_001;
//10        
        uint16_t Serwis_002;
        uint16_t Serwis_003;
        
        uint16_t Serwis_004;
        uint16_t Serwis_005;
        
        uint16_t Serwis_006;
        uint16_t Serwis_007;
        
        uint16_t Serwis_008;
        uint16_t Serwis_009;
        
        uint16_t Serwis_010;
        uint16_t Serwis_011;
 //15       
        uint16_t Serwis_012;
        uint16_t Serwis_013;
        
        uint16_t Serwis_014;
        uint16_t Serwis_015;
        
        uint16_t Serwis_016;
        uint16_t Serwis_017;
        
        uint16_t Serwis_018;
        uint8_t Motor_On_Off;
        uint8_t Free_Marker_1;
        
        uint8_t Free_Marker_2;
        uint8_t Free_Hour_11;
        uint8_t Free_Minute_11;
        uint8_t Free_Hour_12;
//20        
        uint8_t Free_Minute_12;
        uint8_t Free_Hour_21;
        uint8_t Free_Minute_21;
        uint8_t Free_Hour_22;
        
        uint8_t Free_Minute_22;        
        uint8_t Fast_Marker_1;
        uint8_t Fast_Marker_2;
        uint8_t Fast_Hour_11;
        
        uint8_t Fast_Minute_11;
        uint8_t Fast_Hour_12;
        uint8_t Fast_Minute_12;
        uint8_t Fast_Hour_21;
        
        uint8_t Fast_Minute_21;
        uint8_t Fast_Hour_22;
        uint8_t Fast_Minute_22;
        uint8_t Standby_No_Brake;
        
        uint16_t Standby_No_Brake_Wait;
        uint8_t Priority;
        uint8_t Lights_Sensor_Marker;
//25        
        uint8_t Lights_Period_Marker;
        uint8_t Lights_Hour1;
        uint8_t Lights_Hour2;
        uint8_t Lights_Minute1;
        
        uint8_t Lights_Minute2;
        uint8_t Sensor_Type;
        uint8_t Sap_Type;
        uint8_t Sensor_Counter_Move;
        
        uint8_t Sensor_Counter_Speed;
        uint8_t Temp_for_8_bit_value;//temporary
        uint16_t Serwis_019;
        
        uint16_t Serwis_020;
        uint16_t Serwis_021;
       
        uint16_t Serwis_022;
        uint16_t Serwis_023;
//30
        uint16_t Serwis_024;
    } Serv_Vars;
} _Service_Variables;


typedef union {
    uint8_t Controller_Tab8[32];
    uint32_t Controller_Tab[8];
    struct {
//0
        uint16_t Encoder_Var;
        uint16_t Machine_State_Var;
//1
        uint8_t Sensor_Counter;
        uint8_t Sensor_Edge_Flag;
        uint8_t Sensor_Counter_Copy;
        uint8_t Sensor_Direction_Detect;
//2   
        uint16_t Software_Version[3];
        uint16_t Controller_Date[3];
//3    
        uint16_t Serial_Number;
        uint16_t Test_Marker;
//4
        uint8_t Test_Status;
//5
        uint16_t AD_7;
        uint16_t AD_16;
//6
        int16_t Internal_Temperature;
        volatile uint16_t Sine;
//7
        volatile uint16_t Cosine;
        int16_t Angle;

    } Controller_Vars;
} _Controller_Variables;

typedef union {
    uint8_t Counters_Tab8[16];
    uint16_t Counters_Tab[8];
    struct {
        uint32_t No_Goin;
        uint32_t No_Goout;
        uint32_t No_Goin_Test;
        uint32_t No_Goout_Test;
    
    } Counters_Vars;
} _Counters_Variables;

typedef union {
    uint8_t Other_Variables_Tab8[60];
    uint16_t Other_Variables_Tab16[30];
    uint32_t Other_Variables_Tab32[15];
} _Other_Variables;

typedef union {
    uint8_t  Usb_Byte_Tab[512];
    uint16_t Usb_Word_Tab[256];
    uint32_t Usb_Long_Tab[128];
} _Usb_buffer;

extern _Screen_Number Screen_Number;
extern _Screen_Number Screen_Before_Password;

extern _Keyboard Keyboard;

extern _Controlling_Variables Controlling_Variables_Set;
extern _Service_Variables Service_Variables_Set;
extern _Counters_Variables Counters_Variables_Set;
extern _Controller_Variables Controller_Variables_Set;
extern _Other_Variables Other_Variables;

extern _Controlling_Variables Controlling_Variables;
extern _Service_Variables Service_Variables;
extern _Counters_Variables Counters_Variables;
extern _Controller_Variables Controller_Variables;

extern _Usb_buffer Usb_Tx_Buf_Tab;
extern _Usb_buffer Usb_Rx_Buf_Tab;

extern bool Reload_Screen;
extern bool Enable_Lcd_Write;
extern bool Lcd_Write_Strobe;
extern uint8_t Lcd_Write_Ptr;

extern uint8_t Count_Add;
extern uint32_t Div_Temp;

extern uint16_t Lock_Counter;

extern uint32_t Test_String_Mask;
extern uint32_t Manual_Test_Mask;
extern uint32_t Ind_Test_Mask;


extern bool TEST_ENABLE;

/*******************************************************************************
* Exported Function Prototypes
*******************************************************************************/
void Keyboard_Edit_Variable ( void );
void First_Editable_Variable ( void );
uint8_t Next_Editable_Variable ( void );


void Menu_Keyboard ( void );
void Gui_Init ( void );
void Gui ( void );
void Bin2String ( int32_t Source_Count, uint8_t * Result, uint8_t Digits_Count, uint8_t Resolution, uint8_t Zero_Blank );

void Write_Settings ( void );
void Read_Settings ( void );
void Update_Counters ( void );
void Update_Lock_Counter ( uint16_t Update_Counter );
void Lock_Init ( void );
void Lock_Handling ( void );
/*******************************************************************************
 * Exported Inline Function Definitions and #define Function-Like Macros
*******************************************************************************/

/*******************************************************************************
* End fo file gui.h
*******************************************************************************/
#endif



