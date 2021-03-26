/*******************************************************************************
********************************************************************************
* File Name          : gui.c
* Author             : Tomasz Nowak
* Version            : V1.00A
* Date               : 05/07/2011
* Description        : General user interface
********************************************************************************
*******************************************************************************/


/*******************************************************************************
* Header Files
*******************************************************************************/
#include "includes.h"

#include "Languages/Texts.h"

/*******************************************************************************
* Local Preprocessor #define Constants
*******************************************************************************/

/*******************************************************************************
* Local Preprocessor #define MACRO
*******************************************************************************/


/*******************************************************************************
* Local Type Declarations
*******************************************************************************/
typedef enum {
    Gastop = 0,
    Gate,
    Version,
    Pn,
    Wt,
    Sr,
    Cz,
    Pt,
    So,
    Nd,
    Menu_List,
    Menu_Param,
    Menu_Set,
    Menu_Fun,
    Menu_Gate,
    Menu_Date,
    Menu_Lic,
    Menu_Test,
    Menu_1,
    Menu_2,
    Menu_3,
    Menu_4,
    Menu_5,
    Menu_6,
    Menu_7,
    Menu_8,
    Menu_9,
    Mechanism,
    Controlling,
    Heat_Cool_Text,
    Param_mechanizm,
    Param_sterowanie,
    Ust_hasel,
    Ust_blokady,
    RTC_cal,
    Wybor_jezyka,
    Zerowanie,
    Haslo_menu,
    Haslo_blokady,
    Sterownik,
    Zablokowany,
    Blokada_calkowita,
    Data_blokady,
    Ilosc_prob,
    Lic_Programu,
    Bezterminowo,
    Do_dnia,
    Odblokuj,
    Polski,
    English,
    Tak,
    Nie,
    Wl,
    Wyl,
    St_R,
    St_L,
    Kropki,
    Kropki_1,
    Kropka,
    Dwukropek,
    L_C_R,
    P_C_R,
    Kolowrot,
    Hamulec,
    Rygle,
    Zbijak,
    PWM_L_C,
    PWM_P_C,
    PWM_Hamulec,
    Predkosc1,
    Predkosc2,    
    Sap,
    Wo_Wy,
    Szybkie,
    Tak_NO,
    Tak_NC,
    W_lewo,
    W_prawo,
    Swobodny,
    Brak,
    Czas_otw,
    Czas_sygn_zwr,
    R_3,
    R_4,
    We,
    Wy,
    Haslo,
    Proby,
    Dostepu,
    Blokady,
    Blok,
    Blok_data,
    Prog_testowy,
    Test_We_Wy,
    Ust_enkodera,
    Start_Stop,
    OK_Tak,
    ESC_Nie,
    Br2_Chin,
    Br2_Hiszp,
    Br3,
    Br4,
    Br3_Stadion,
    Br2_Basic,
    Br2_Ga,
    Kreski,
    Sap_text,
    Ww_text,
    Sp_text,
    Blo_text,
    Test_Start,
    Test_Stop,
    Ust_serwis,
    Odczyt_serwis,
    _1z,
    _2z,
    _3z,
    _4z,
    _5z,
    z1,
    z2,
    z3,
    z4,
    z5,
    Heating,
    Cooling,
    Min_Val,
    Max_Val,
    Heat_Cool_Param,
    Motor_On_Off_String,
    Czas_WW,
    Czas_SP,
    SP_Hamulec,
    SP_Hamulec_Czas,
    Par1,
    Par2,
    Par3,
    Par4,
    Par5,
    Par6,
    Par7,
    Par8,
    Par9,
    Par10,
    Par11,
    Par12,
    Priorytet_przejscia,
    Przedzial,
    Swiatla,
    P_Czasowy,
    Sensor,
    Typ_Czujnika,
    Enkoder,
    Indukcyjny,
    Sig_Typ,
    Std,
    Door,
    Trans_time,
    Angle1,
    Angle2,
    Ind_test,
    Test_M,
    Test_OC,
    Test_P,
    Test_W,
    Test_In,
    In1,
    In2,
    In3,
    In4,
    In5,
    In6,
    In7,
    In8, 
    Soft_Version,
    Manufactured,
    S_N,
    Edge_Time,
    Signal_Time,
    FE_Time,
    Tamper_Relay,
    Ind_status,
    Ind_lines,
    Ind_testing,
    Ind_ok,
    Ind_false,
} _String_Number;

typedef enum {
    POLISH = 0,
    ENGLISH,
    DEUTSCH,

} _Language_Num;

typedef struct _Languages_ {
    const uint8_t ** Languages_Tab[8];
} _Languages;


typedef struct _Strings_ {
    uint8_t String_Index;
    uint8_t String_X_Pos;
    uint8_t String_Y_Pos;
    _String_Number String_Num;
    const struct _Languages_ * Languages_Ptr;
    const struct _Strings_ * Next_String;
} _Strings;

typedef struct _Menu_ {
    uint8_t Menu_X_Pos;
    uint8_t Menu_Y_Pos;
    uint8_t Lines_On_Screen;
    uint8_t Total_Lines;
    const struct _Languages_ * Languages_Ptr;
    _String_Number Menu_String_Num[10];
} _Menu;

typedef struct {
    uint8_t First_On_Screen;
    uint8_t Active_String;
    uint8_t Active_On_Screen;
} _Menu_Keys_Ptrs;

typedef struct _Var_Param_ {
    _Var_Type Variable_Type;
    _Var_Size Variable_Size;
    uint8_t Variable_Digits_Count;
    uint8_t Variable_Resolution;
    _String_Number Variable_String_Num[8];
    bool Variable_Hide_Zeros;
    uint8_t Variable_Accuracy;
    const struct _Var_Ranges_ * Ranges;
} _Var_Param;

typedef struct _Variables_ {
    uint8_t Variable_Index;
    uint8_t Variable_X_Pos;
    uint8_t Variable_Y_Pos;
    void * Var_Ptr;
    bool Edit_Var;
    const struct _Var_Param_ * Var_Param_Ptr;
    const struct _Languages_ * Languages_Ptr;
    const struct _Variables_ * Next_Variable;
} _Variables;

typedef struct _Var_Ranges_ {
    int32_t Var_min;
    int32_t Var_max;
} _Var_Ranges;

typedef struct {
    const _Strings * String_Ptr;
    const _Menu * Menu_Ptr;
    const _Variables * Variable_Ptr;
    uint32_t * String_Mask;
    uint32_t * Menu_Mask;
    uint32_t * Variable_Mask;
} _Screen_Ptrs_Masks;

typedef struct {
    void (*Keybord_Ptr)(void);
    const _Screen_Ptrs_Masks * Screen_Objects;
} _Screen_Lcd;

    uint8_t k;
    uint8_t Count_Add;
    uint32_t Div_Temp;

    const int32_t Add_Tab[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };
/*******************************************************************************
* Exported Const Object Definitions
*******************************************************************************/


/*******************************************************************************
* Local Object Definitions
*******************************************************************************/
bool Reload_Screen;
bool Enable_Lcd_Write = FALSE;
bool Lcd_Write_Strobe = FALSE;
uint8_t Lcd_Write_Ptr;

static uint32_t Password_Set;
static uint32_t Lock_Interval_Tmp;
static uint32_t Lock_Date_Now_Tmp;
static uint32_t Lock_Date_Set_Tmp;

uint32_t String_Lic_Mask;
uint32_t Var_Lic_Mask;
uint32_t Enable_String_Mask;
uint32_t Test_String_Mask;
uint32_t Manual_Test_Mask;
uint32_t Ind_Test_Mask;

_Language_Num Language_Num;
_Screen_Number Screen_Number;
_Screen_Number Screen_After_Password;
_Screen_Number Screen_Before_Password;
_Menu_Keys_Ptrs Menu_Keys = { 0 ,0 ,0 };
_Menu_Keys_Ptrs Menu_Backup_Keys;
_Menu_Keys_Ptrs Menu_Backup_Keys_Set;

_Controlling_Variables Controlling_Variables;
_Service_Variables Service_Variables;
_Counters_Variables Counters_Variables;
_Controller_Variables Controller_Variables;
_Other_Variables Other_Variables;

_Controlling_Variables Controlling_Variables_Set;
_Service_Variables Service_Variables_Set;
_Counters_Variables Counters_Variables_Set;
_Controller_Variables Controller_Variables_Set;

_Usb_buffer Usb_Tx_Buf_Tab;
_Usb_buffer Usb_Rx_Buf_Tab;

const _Languages Languages = { { Polish_Tab, English_Tab } };

const _Variables * Variable_Editing_Ptr;

uint16_t Lock_Counter = 0;

volatile uint32_t Flash_Address;
volatile uint32_t NbrOfPage = 0;
volatile FLASH_Status FLASHStatus;

bool TEST_ENABLE = FALSE;

const uint32_t Settings_Default[32] = {
12345,/*Haslo dostepu*/
12345,/*Haslo blokady*/
0x01050A01, /*Blokada,Il.P.Dost,Il.P.Blo,PWM_LR*/
0x19000001,/*PWM_PR,PWM_Zwora,PWM_Zbijak,PWM_LR_Var*/
0x01190A19,/*PWM_PR_Var,PWM_Ham_Var,PWM_Zbijak_Var,Lewy_Rygiel*/
0x00000001,/*Prawy_Rygiel,Hamulec,KOlowrotek,Rygle_var*/
0x01020201,/*,Jezyk,Sap,W_wyjscie,Szybkie_wejscie*/
0x00000000,/*,Blokada_dzien,Blokada_miesiac,Blokada_rok-16*/
0x050A0000,/*RTC_Cal_Var,Marker_Blokady,Blok_Dostep_Pozostalo,Blok_Prog_Pozostalo*/
0x00000000,/*Lock_Interval-16,Lock_Counter-16*/
0x0014A502,/*Typ_Bramki,First_On_Marker,S001-16*/
0x00000014,/*S002-16,S003-16*/
0x000F0014 ,/*S004-16,S005-16*/
0x000A0005,/*S006-16,S007-16*/
0x00820014,/*S008-16,S009-16*/
0x000A0005,/*S010-16,S011-16*/
0x03E80032,/*S012-16,S013-16*/
0x0096001E,/*S014-16,S015-16*/
0x00320014,/*S016-16,S017-16*/
0x02000001,/*S018-16,Motor_On_Off,Free_Marker1*/
0x09000602,/*Free_Marker_2,Free_Hour_11,Free_Minute_11,Free_Hour_12*/
0x12000F00,/*Free_Minute_12,Free_Hour_21,Free_Minute_21,Free_Hour_22*/
0x06010100,/*Free_Minute_22,Fast_Marker1,Fast_Marker_2,Fast_Hour_11*/
0x0F000900,/*Fast_Minute_11,Fast_Hour_12,Fast_Minute_12,Fast_Hour_21*/
0x01001200,/*Fast_Minute_21,Fast_Hour_22,Fast_Minute_22,Standby_No_Brake*/
0x0000000A,/*Light_Sensor_Marker, Priority, Standby_No_Brake_Wait-16*/
0x00080F01,/*Lights_Minute1,Lights_Hour2,Lights_Hour1, Lights_Period_Marker*/
0x02000000,/*Lights_Minute, Sensor type, Sap_Type, Sensor_Counter_Rot*/
0x001E0008,/*Sensor_Counter_Speed, Temp, Serwis_019*/
0x00060002,/*Serwis_20, Serwis_021*/
0x01f40258,/*Serwis_22, Serwis_023*/
0x0000000e,/*Serwis_24, */

};
                                                                                                               
const uint32_t Controlling_Default[8] = {
0x000F141E,/*Predkosc_Silnika1,Predkosc_Silnika2,Czas_otwarcia_bramki-16*/
0x01F40005,/*Czas_syg_zwr-16,Encoder-16**/
0x000A141E,/*Predkosc_Silnika3,Predkosc_Silnika4, Heat_Min-16*/
0x00160014,/*Heat_Max-16, Air_Min-16*/
0x00010019,/*Air_Max-16, Heat_Marker, Air_Marker*/
0x0032005A,/*Transition_time, Free_Exit_Time*/
0x0001000A,/*Normal_Exit_Var, Exit_Var,Back_Signal_Type, */
0x0000000A,/*Trans_time */
};

/*******************************************************************************
* Handling functions for variables' editing
*******************************************************************************/

/*******************************************************************************
* Keyboard functions declarations
*******************************************************************************/
static void Keyboard_Standby ( void );
static void Keyboard_Enable ( void );
static void Keyboard_Menu ( void );
static void Keyboard_Param1 ( void );
static void Keyboard_Serwis ( void );
static void Keyboard_Heat_Cool ( void );
static void Keyboard_Lights ( void );
static void Keyboard_Alarm ( void );
static void Keyboard_Date ( void );
static void Keyboard_Param ( void );
static void Keyboard_Set ( void );
static void Keyboard_Gate ( void );
static void Keyboard_Lic ( void );
static void Keyboard_Kontakt ( void );
static void Keyboard_Test ( void );
static void Keyboard_Haslo ( void );
static void Keyboard_Passwd ( void );
static void Keyboard_Lock ( void );
static void Keyboard_Functions1 ( void );
static void Keyboard_RTC ( void );

static void Keyboard_Language ( void );
static void Keyboard_Param_Mech ( void );
static void Keyboard_Param_Ster ( void );

static void Keyboard_Prog_Test ( void );
static void Keyboard_Ust_Enk ( void );

static void Keyboard_Ster_Blok ( void );
static void Keyboard_Odblokuj ( void );

static void Keyboard_Fabryczne ( void );
static void Keyboard_Zerowanie ( void );
static void Keyboard_Info ( void );

static void Keyboard_Ust_Serwis ( void );
static void Keyboard_Odczyt_Serwis ( void );
static void Keyboard_Ind_Test ( void );
static void Keyboard_Test_WeWy ( void );

void Write_Settings ( void );
void Read_Settings ( void );

const _Var_Ranges Ranges_Lx = { 10, 250 };
const _Var_Ranges Ranges_Tryb = { 0, 1 };
const _Var_Ranges Ranges_Rtc = { 0, 100000 };

const _Var_Param Var_Tryb =    { TEXT_VAR, VAR_U8, 1, 0, { Tak, Nie }, TRUE, 1, &Ranges_Tryb };
const _Var_Param Var_Czas_Lx = { DIGITS_VAR, VAR_U8, 3, 0, { Pn, Wt }, TRUE, 1, &Ranges_Lx };
const _Var_Param Var_rtc = { DIGITS_VAR, VAR_U32, 5, 0, { Pn, Wt }, FALSE, 1, &Ranges_Rtc };

const _Var_Ranges Ranges_Sec = { 0, 59 };
const _Var_Ranges Ranges_Minute = { 0, 59 };
const _Var_Ranges Ranges_Hour= { 0, 23 };
const _Var_Ranges Ranges_Day = { 1, 31 };
const _Var_Ranges Ranges_Month = { 1, 12 };
const _Var_Ranges Ranges_Year = { 2013, 2099 };
const _Var_Ranges Ranges_DOW = { 0, 6 };
const _Var_Ranges Ranges_Pass = { 0, 99999 };
const _Var_Ranges Ranges_Prob = { 3, 20 };
const _Var_Ranges Ranges_Lang = { 0, 1 };
const _Var_Ranges Ranges_PWM = { 1, 50 };
const _Var_Ranges Ranges_Tak_Nie = { 0, 1 };

const _Var_Ranges Ranges_Sap = { 0, 2 };
const _Var_Ranges Ranges_Wo_wy = { 0, 2 };

const _Var_Ranges Ranges_czas_otw = { 5, 60 };
const _Var_Ranges Ranges_czas_syg = { 2, 20 };

const _Var_Ranges Ranges_counters = { 0, 50000000 };
const _Var_Ranges Ranges_RTC = { 0, 127 };
const _Var_Ranges Ranges_Enk = { 0, 65535 };
const _Var_Ranges Ranges_Serwis = { 0, 65000 };

const _Var_Ranges Ranges_Gate_Type = { 0, 3 };
const _Var_Ranges Ranges_TempC = { 0, 99 };

const _Var_Ranges Ranges_Vent_Min = { 15, 24 };
const _Var_Ranges Ranges_Vent_Max = { 25, 40 };
const _Var_Ranges Ranges_Heat_Min = { 0, 14 };
const _Var_Ranges Ranges_Heat_Max = { 16, 25 };
const _Var_Ranges Ranges_SNBW = { 2, 20 };
const _Var_Ranges Ranges_Back_Sig_Type = { 0, 1 };
const _Var_Ranges Ranges_Sap_Type = { 0, 2 };
const _Var_Ranges Ranges_Sensor = { 1, 16 };
const _Var_Ranges Ranges_Transition = { 5, 99 };
const _Var_Ranges Ranges_TestWeWy = { 0, 4 };

const _Var_Ranges Ranges_Free_Exit = { 50, 250 };
const _Var_Ranges Ranges_Normal_Exit = { 5, 100 };

const _Var_Ranges Ranges_Trans = { 5, 60 };


const _Var_Param Clock_Sec = { DIGITS_VAR, VAR_U8, 2, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_Sec };
const _Var_Param Clock_Minute = { DIGITS_VAR, VAR_U8, 2, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_Minute };
const _Var_Param Clock_Hour = { DIGITS_VAR, VAR_U8, 2, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_Hour };
const _Var_Param Clock_Day = { DIGITS_VAR, VAR_U8, 2, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_Day };
const _Var_Param Clock_Month = { DIGITS_VAR, VAR_U8, 2, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_Month };
const _Var_Param Clock_Year = { DIGITS_VAR, VAR_U16, 4, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_Year };
const _Var_Param Clock_DOW =   { TEXT_VAR, VAR_U8, 1, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_DOW };
const _Var_Param Passwd = { DIGITS_VAR, VAR_U32, 5, 0, { Pn }, FALSE, 1, &Ranges_Pass };
const _Var_Param Lang_Param =   { TEXT_VAR, VAR_U8, 1, 0, { Polski, English }, FALSE, 1, &Ranges_Lang };
const _Var_Param TN_Param =   { TEXT_VAR, VAR_U8, 1, 0, { Tak, Nie }, FALSE, 1, &Ranges_Tak_Nie };
const _Var_Param PWM_Param = { DIGITS_VAR, VAR_U8, 2, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_PWM };
const _Var_Param Silnik_Param = { DIGITS_VAR, VAR_U8, 2, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_PWM };
const _Var_Param Counters = { DIGITS_VAR, VAR_U32, 7, 0, { Pn }, TRUE, 1, &Ranges_counters };
const _Var_Param Param_Prob = { DIGITS_VAR, VAR_U8, 2, 0, { Pn }, FALSE, 1, &Ranges_Prob };
const _Var_Param Param_2h = { DIGITS_VAR, VAR_U8, 2, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, TRUE, 1, &Ranges_Enk };
const _Var_Param Param_Enk = { DIGITS_VAR, VAR_U16, 4, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_Enk };
const _Var_Param Sap_param =   { TEXT_VAR, VAR_U8, 1, 0, { Tak_NO, Tak_NC, Nie }, FALSE, 1, &Ranges_Sap };
const _Var_Param Wowy_param =   { TEXT_VAR, VAR_U8, 1, 0, { W_lewo, W_prawo, Brak }, FALSE, 1, &Ranges_Wo_wy };
const _Var_Param Kolowrot_param =   { TEXT_VAR, VAR_U8, 1, 0, { R_3, R_4 }, FALSE, 1, &Ranges_Tak_Nie };
const _Var_Param Czas_otw_param = { DIGITS_VAR, VAR_U16, 2, 0, { Tak, Nie }, FALSE, 1, &Ranges_czas_otw };
const _Var_Param Czas_syg_zwr_param = { DIGITS_VAR, VAR_U16, 2, 1, { Tak, Nie }, FALSE, 1, &Ranges_czas_syg };
const _Var_Param Param_RTC = { DIGITS_VAR, VAR_U8, 3, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_RTC };
const _Var_Param Param_Gate_Type = { TEXT_VAR, VAR_U8, 1, 0, { Br2_Chin, Br2_Hiszp, Br3, Br4, Br3_Stadion, Br2_Basic, Br2_Ga }, FALSE, 1, &Ranges_Gate_Type };
const _Var_Param Param_Serwis = { DIGITS_VAR, VAR_U16, 5, 0, { Tak, Nie }, FALSE, 1, &Ranges_Serwis };

const _Var_Param Param_Vent_Min = { DIGITS_VAR, VAR_U16, 4, 0, { Tak, Nie }, TRUE, 1, &Ranges_Vent_Min };
const _Var_Param Param_Vent_Max = { DIGITS_VAR, VAR_U16, 4, 0, { Tak, Nie }, TRUE, 1, &Ranges_Vent_Max };
const _Var_Param Param_Heat_Min = { DIGITS_VAR, VAR_U16, 4, 0, { Tak, Nie }, TRUE, 1, &Ranges_Heat_Min };
const _Var_Param Param_Heat_Max = { DIGITS_VAR, VAR_U16, 4, 0, { Tak, Nie }, TRUE, 1, &Ranges_Heat_Max };
const _Var_Param Param_TempC = { DIGITS_VAR, VAR_S8, 2, 0, { Pn }, FALSE, 1, &Ranges_TempC };

const _Var_Param SNBW_Param = { DIGITS_VAR, VAR_U16, 2, 0, { Tak, Nie }, TRUE, 1, &Ranges_SNBW };
const _Var_Param Priorytet_Param =   { TEXT_VAR, VAR_U8, 1, 0, { W_lewo, W_prawo }, FALSE, 1, &Ranges_Tak_Nie };
const _Var_Param Sensor_Param =   { TEXT_VAR, VAR_U8, 1, 0, { Enkoder, Indukcyjny }, FALSE, 1, &Ranges_Tak_Nie };
const _Var_Param Back_Signal_Type_Param =   { TEXT_VAR, VAR_U8, 1, 0, { Std, Door }, FALSE, 1, &Ranges_Back_Sig_Type };
const _Var_Param Sap_Type_Param =   { TEXT_VAR, VAR_U8, 1, 0, { Swobodny, W_lewo, W_prawo }, FALSE, 1, &Ranges_Sap_Type };

const _Var_Param Param_Sensor = { DIGITS_VAR, VAR_U8, 2, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, TRUE, 1, &Ranges_Sensor };

const _Var_Param Transition_time_param = { DIGITS_VAR, VAR_U16, 2, 1, { Tak, Nie }, FALSE, 1, &Ranges_Transition };

const _Var_Param TestWeWy_Param =   { TEXT_VAR, VAR_U8, 1, 0, { Test_M, Test_OC, Test_P, Test_W, Test_In }, FALSE, 1, &Ranges_TestWeWy };

const _Var_Param Param_Serial = { DIGITS_VAR, VAR_U16, 2, 0, { Tak, Nie }, FALSE, 1, &Ranges_Serwis };

const _Var_Param Free_Exit_Param = { DIGITS_VAR, VAR_U16, 3, 1, { Tak, Nie }, TRUE, 1, &Ranges_Free_Exit };
const _Var_Param Normal_Exit_Param = { DIGITS_VAR, VAR_U16, 3, 1, { Tak, Nie }, TRUE, 1, &Ranges_Normal_Exit };

const _Var_Param Param_Angle = { DIGITS_VAR, VAR_S16, 4, 0, { Pn, Wt, Sr, Cz, Pt, So, Nd }, FALSE, 1, &Ranges_Enk };


const _Var_Param Trans_time_param = { DIGITS_VAR, VAR_U16, 2, 0, { Tak, Nie }, FALSE, 1, &Ranges_Trans };
/*******************************************************************************
* StandBy screen objects definition - SCREEN 0
*******************************************************************************/
const _Strings String_Standby3 = { 2, 3, 3, Version, &Languages, NULL };
const _Strings String_Standby2 = { 1, 2, 1, Gate, &Languages, &String_Standby3 };
const _Strings String_Standby1 = { 0, 5, 0, Gastop, &Languages, &String_Standby2 };

const _Variables Var_Standby3 = { 2, 14, 3, &Controller_Variables.Controller_Vars.Software_Version[2], FALSE, &Param_Serial, &Languages, NULL };
const _Variables Var_Standby2 = { 1, 11,  3, &Controller_Variables.Controller_Vars.Software_Version[1], FALSE, &Param_Serial, &Languages, &Var_Standby3 };
const _Variables Var_Standby1 = { 0, 8,  3, &Controller_Variables.Controller_Vars.Software_Version[0], FALSE, &Param_Serial, &Languages, &Var_Standby2 };

const _Screen_Ptrs_Masks Screen_Standby = { &String_Standby1, NULL, &Var_Standby1, NULL, NULL, NULL };
/*******************************************************************************
* Enable screen objects definition - SCREEN 1
*******************************************************************************/
const _Variables Var_Enable10 = { 9, 5, 1, &Controller_Variables.Controller_Vars.Internal_Temperature, FALSE, &Param_TempC, &Languages, NULL };
const _Variables Var_Enable9 = { 8, 11, 1, &Service_Variables.Serv_Vars.Typ_Bramki, FALSE, &Param_Gate_Type, &Languages, &Var_Enable10 };//NULL };
const _Variables Var_Enable8 = { 7, 3,  3, &Counters_Variables.Counters_Vars.No_Goin, FALSE, &Counters, &Languages, &Var_Enable9 };
const _Variables Var_Enable7 = { 6, 3,  2, &Counters_Variables.Counters_Vars.No_Goout, FALSE, &Counters, &Languages, &Var_Enable8 };
const _Variables Var_Enable6 = { 5, 18,  0, &Time.Time_Vars.Minute, FALSE, &Clock_Minute, &Languages, &Var_Enable7 };
const _Variables Var_Enable5 = { 4, 15,  0, &Time.Time_Vars.Hour, FALSE, &Clock_Hour, &Languages, &Var_Enable6 };
const _Variables Var_Enable4 = { 3, 10,  0, &Date.Date_Vars.Year, FALSE, &Clock_Year, &Languages, &Var_Enable5 };
const _Variables Var_Enable3 = { 2, 7,  0, &Date.Date_Vars.Month, FALSE, &Clock_Month, &Languages,&Var_Enable4 };
const _Variables Var_Enable2 = { 1, 4,  0, &Date.Date_Vars.Day, FALSE, &Clock_Day, &Languages, &Var_Enable3 };
const _Variables Var_Enable1 = { 0, 0,  0, &Date.Date_Vars.Day_Of_Week, FALSE, &Clock_DOW, &Languages, &Var_Enable2 };

const _Strings String_Enable9 = {8, 19, 2, Przedzial, &Languages, NULL };
const _Strings String_Enable8 = {7, 15, 3, Blo_text, &Languages, &String_Enable9 };
const _Strings String_Enable7 = {6, 15, 2, Sap_text, &Languages, &String_Enable8 };
const _Strings String_Enable6 = {5, 12, 3, Sp_text, &Languages, &String_Enable7 };
const _Strings String_Enable5 = {4, 12, 2, Ww_text, &Languages, &String_Enable6 };
const _Strings String_Enable4 = {3, 0, 1, Kreski, &Languages, &String_Enable5 };
const _Strings String_Enable3 = { 2, 0, 3, Wy, &Languages, &String_Enable4 };
const _Strings String_Enable2 = { 1, 0, 2, We, &Languages, &String_Enable3 };
const _Strings String_Enable1 = { 0, 6, 0, Kropki, &Languages, &String_Enable2 };

const _Screen_Ptrs_Masks Screen_Enable = { &String_Enable1, NULL, &Var_Enable1, &Enable_String_Mask, NULL, NULL };

/*******************************************************************************
* MENU screen objects definition - SCREEN 2
*******************************************************************************/

const _Strings String_Menu1 = { 0, 0, 0, Menu_List, &Languages, NULL };

const _Menu Menu_Menu1 = { 1, 1, 3, 9, &Languages, { Menu_1, Menu_2, Menu_3, Menu_4, Menu_5, Menu_6, Menu_7, Menu_8, Menu_9 } };

const _Screen_Ptrs_Masks Screen_Menu1 = { &String_Menu1, &Menu_Menu1, NULL, NULL, NULL, NULL };

/*******************************************************************************
* Parameters screen objects definition - SCREEN 3
*******************************************************************************/
const _Menu Menu_Param1 = { 1, 1, 3, 4, &Languages, { Mechanism, Controlling, Heat_Cool_Text, Swiatla } };

const _Strings String_Param1 = { 0, 0, 0, Menu_Param, &Languages, NULL };

const _Screen_Ptrs_Masks Screen_Param = { &String_Param1, &Menu_Param1, NULL, NULL, NULL, NULL };
/*******************************************************************************
* Settings screen objects definition - SCREEN 4
*******************************************************************************/
const _Menu Menu_Set1 = { 1, 1, 3, 5, &Languages, { Wybor_jezyka, Ust_hasel, Ust_blokady, RTC_cal, Zerowanie } };

const _Strings String_Set1 = { 0, 0, 0, Menu_Set, &Languages, NULL };

const _Screen_Ptrs_Masks Screen_Set = { &String_Set1, &Menu_Set1, NULL, NULL, NULL, NULL };

/*******************************************************************************
* Gate select screen objects definition - SCREEN 5
*******************************************************************************/

const _Variables Var_Gate1 = { 0, 6, 2, &Service_Variables_Set.Serv_Vars.Typ_Bramki, TRUE, &Param_Gate_Type, &Languages, NULL };

const _Strings String_Gate1 = { 0, 0, 0, Menu_Gate, &Languages, NULL };

const _Screen_Ptrs_Masks Screen_Gate = { &String_Gate1, NULL, &Var_Gate1, NULL, NULL, NULL };


/*******************************************************************************
* Date screen objects definition - SCREEN 6
*******************************************************************************/

const _Variables Var_Date6 = { 5, 11,  3, &Date_Set.Date_Vars.Year, TRUE, &Clock_Year, &Languages,NULL };
const _Variables Var_Date5 = { 4, 8,  3, &Date_Set.Date_Vars.Month, TRUE, &Clock_Month, &Languages, &Var_Date6 };
const _Variables Var_Date4 = { 3, 5,  3, &Date_Set.Date_Vars.Day, TRUE, &Clock_Day, &Languages, &Var_Date5 };
const _Variables Var_Date3 = { 2, 11,  2, &Time_Set.Time_Vars.Minute, TRUE, &Clock_Minute, &Languages,&Var_Date4 };
const _Variables Var_Date2 = { 1, 8,  2, &Time_Set.Time_Vars.Hour, TRUE, &Clock_Hour, &Languages, &Var_Date3 };
const _Variables Var_Date1 = { 0, 5,  2, &Date_Set.Date_Vars.Day_Of_Week, TRUE, &Clock_DOW, &Languages, &Var_Date2 };

const _Strings String_Date4 = { 3, 10, 3, Kropka, &Languages, NULL };
const _Strings String_Date3 = { 2, 7, 3, Kropka, &Languages, &String_Date4 };
const _Strings String_Date2 = { 1, 10, 2, Dwukropek, &Languages, &String_Date3 };
const _Strings String_Date1 = { 0, 0, 0, Menu_Date, &Languages, &String_Date2 };

const _Screen_Ptrs_Masks Screen_Date = { &String_Date1, NULL, &Var_Date1, NULL, NULL, NULL };

/*******************************************************************************
* Licence screen objects definition - SCREEN 7
*******************************************************************************/
const _Strings String_Lic7 = { 6, 14, 2, Kropka, &Languages, NULL };
const _Strings String_Lic6 = { 5, 11, 2, Kropka, &Languages, &String_Lic7 };
const _Strings String_Lic5 = { 4, 0, 3, Odblokuj, &Languages, &String_Lic6 };
const _Strings String_Lic4 = { 3, 0, 2, Bezterminowo, &Languages, &String_Lic5 };
const _Strings String_Lic3 = { 2, 0, 2, Do_dnia, &Languages, &String_Lic4 };
const _Strings String_Lic2 = { 1, 0, 1, Lic_Programu, &Languages, &String_Lic3 };
const _Strings String_Lic1 = { 0, 0, 0, Menu_Lic, &Languages, &String_Lic2 };

const _Variables Var_Lic3 = { 2, 15, 2, &Service_Variables_Set.Serv_Vars.Blokada_rok, FALSE, &Clock_Year, &Languages, NULL };
const _Variables Var_Lic2 = { 1, 12, 2, &Service_Variables_Set.Serv_Vars.Blokada_miesiac, FALSE, &Clock_Month, &Languages, &Var_Lic3 };
const _Variables Var_Lic1 = { 0, 9, 2, &Service_Variables_Set.Serv_Vars.Blokada_dzien, FALSE, &Clock_Day, &Languages, &Var_Lic2 };

const _Screen_Ptrs_Masks Screen_Lic = { &String_Lic1, NULL, &Var_Lic1, &String_Lic_Mask, NULL, &Var_Lic_Mask };

/*******************************************************************************
* Test screen objects definition - SCREEN 8
*******************************************************************************/
const _Menu Menu_Test1 = { 1, 1, 3, 6, &Languages, { Prog_testowy, Test_We_Wy, Ust_enkodera, Ust_serwis, Odczyt_serwis, Ind_test } };

const _Strings String_Test1 = { 0, 0, 0, Menu_Test, &Languages, NULL };

const _Screen_Ptrs_Masks Screen_Test = { &String_Test1, &Menu_Test1, NULL, NULL, NULL, NULL };

/*******************************************************************************
* Password screen objects definition - SCREEN 9
*******************************************************************************/
const _Variables Var_Password2 = { 0, 18,  3, &Service_Variables_Set.Serv_Vars.Blok_Dostep_Pozostalo, FALSE, &Param_2h, &Languages, NULL };
const _Variables Var_Password1 = { 0, 7,  2, &Password_Set, TRUE, &Passwd, &Languages, &Var_Password2 };

const _Strings String_Password2 = { 1, 0, 3, Ilosc_prob, &Languages, NULL};
const _Strings String_Password1 = { 0, 0, 0, Haslo_menu, &Languages, &String_Password2};

const _Screen_Ptrs_Masks Screen_Haslo = { &String_Password1, NULL, &Var_Password1, NULL, NULL, NULL };

/*******************************************************************************
* Language screen objects definition - SCREEN 10
*******************************************************************************/
const _Variables Var_Language1 = { 0, 7,  2, &Service_Variables_Set.Serv_Vars.Jezyk, TRUE, &Lang_Param, &Languages, NULL };

const _Strings String_Language1 = { 0, 0, 0, Wybor_jezyka, &Languages, NULL};

const _Screen_Ptrs_Masks Screen_Language = { &String_Language1, NULL, &Var_Language1, NULL, NULL, NULL };

/*******************************************************************************
* Param1 screen objects definition - SCREEN 11
*******************************************************************************/
const _Variables Var_Parameters3 = { 2, 13,  3, &Service_Variables_Set.Serv_Vars.KOlowrotek, TRUE, &Kolowrot_param, &Languages, NULL };
const _Variables Var_Parameters2 = { 1, 13,  2, &Service_Variables_Set.Serv_Vars.Prawy_Rygiel, TRUE, &TN_Param, &Languages, &Var_Parameters3 };
const _Variables Var_Parameters1 = { 0, 13,  1, &Service_Variables_Set.Serv_Vars.Lewy_Rygiel, TRUE, &TN_Param, &Languages, &Var_Parameters2 };

const _Strings String_Parameters6 = { 5, 18, 0, z4, &Languages, NULL};
const _Strings String_Parameters5 = { 4, 16, 0, _1z, &Languages, &String_Parameters6};
const _Strings String_Parameters4 = { 3, 0, 3, Kolowrot, &Languages, &String_Parameters5};
const _Strings String_Parameters3 = { 2, 0, 2, P_C_R, &Languages, &String_Parameters4};
const _Strings String_Parameters2 = { 1, 0, 1, L_C_R, &Languages, &String_Parameters3};
const _Strings String_Parameters1 = { 0, 0, 0, Param_mechanizm, &Languages, &String_Parameters2};

const _Screen_Ptrs_Masks Screen_Param_Mech1 = { &String_Parameters1, NULL, &Var_Parameters1, NULL, NULL, NULL };
/*******************************************************************************
* Param2 screen objects definition - SCREEN 12
*******************************************************************************/
const _Variables Var_Parameters9 = { 5, 17,  3, &Service_Variables_Set.Serv_Vars.PWM_Ham_Var, TRUE, &PWM_Param, &Languages, NULL };
const _Variables Var_Parameters8 = { 4, 17,  2, &Service_Variables_Set.Serv_Vars.PWM_PR_Var, TRUE, &PWM_Param, &Languages, &Var_Parameters9 };
const _Variables Var_Parameters7 = { 3, 17,  1, &Service_Variables_Set.Serv_Vars.PWM_LR_Var, TRUE, &PWM_Param, &Languages, &Var_Parameters8 };
const _Variables Var_Parameters6 = { 2, 13,  3, &Service_Variables_Set.Serv_Vars.PWM_Zwora, TRUE, &TN_Param, &Languages, &Var_Parameters7 };
const _Variables Var_Parameters5 = { 1, 13,  2, &Service_Variables_Set.Serv_Vars.PWM_Prawy_Rygiel, TRUE, &TN_Param, &Languages, &Var_Parameters6 };
const _Variables Var_Parameters4 = { 0, 13,  1, &Service_Variables_Set.Serv_Vars.PWM_Lewy_Rygiel, TRUE, &TN_Param, &Languages, &Var_Parameters5 };

const _Strings String_Parameters12 = { 5, 18, 0, z4, &Languages, NULL};
const _Strings String_Parameters11 = { 4, 16, 0, _2z, &Languages, &String_Parameters12};
const _Strings String_Parameters10 = { 3, 0, 3, PWM_Hamulec, &Languages, &String_Parameters11};
const _Strings String_Parameters9 = { 2, 0, 2, PWM_P_C, &Languages, &String_Parameters10};
const _Strings String_Parameters8 = { 1, 0, 1, PWM_L_C, &Languages, &String_Parameters9};
const _Strings String_Parameters7 = { 0, 0, 0, Param_mechanizm, &Languages, &String_Parameters8};

const _Screen_Ptrs_Masks Screen_Param_Mech2 = { &String_Parameters7, NULL, &Var_Parameters4, NULL, NULL, NULL };

/*******************************************************************************
* Param3 screen objects definition - SCREEN 13
*******************************************************************************/
const _Variables Var_Parameters12 = { 2, 13,  3, &Service_Variables_Set.Serv_Vars.Rygle_var, TRUE, &TN_Param, &Languages, NULL };
const _Variables Var_Parameters11 = { 1, 13,  2, &Service_Variables_Set.Serv_Vars.Hamulec, TRUE, &TN_Param, &Languages, &Var_Parameters12 };
//const _Variables Var_Parameters11 = { 1, 17,  1, &Service_Variables_Set.Serv_Vars.PWM_Zbijak_Var, TRUE, &PWM_Param, &Languages, &Var_Parameters12 };
//const _Variables Var_Parameters10 = { 0, 13,  1, &Service_Variables_Set.Serv_Vars.PWM_Zbijak, TRUE, &TN_Param, &Languages, &Var_Parameters11 };
const _Variables Var_Parameters10 = { 0, 13,  1, &Service_Variables_Set.Serv_Vars.Motor_On_Off, TRUE, &TN_Param, &Languages, &Var_Parameters11 };

const _Strings String_Parameters18 = { 5, 18, 0, z4, &Languages, NULL};
const _Strings String_Parameters17 = { 4, 16, 0, _3z, &Languages, &String_Parameters18};
const _Strings String_Parameters16 = { 3, 0, 1, Motor_On_Off_String, &Languages, &String_Parameters17};
const _Strings String_Parameters15 = { 2, 0, 2, Hamulec, &Languages, &String_Parameters16};
const _Strings String_Parameters14 = { 1, 0, 3, Rygle, &Languages, &String_Parameters15};
const _Strings String_Parameters13 =  { 0, 0, 0, Param_mechanizm, &Languages, &String_Parameters14};

const _Screen_Ptrs_Masks Screen_Param_Mech3 = { &String_Parameters13, NULL, &Var_Parameters10, NULL, NULL, NULL };

/*******************************************************************************
* Param4 screen objects definition - SCREEN 13-1
*******************************************************************************/
const _Variables Var_Parameters15 = { 2, 18, 3, &Service_Variables_Set.Serv_Vars.Sensor_Counter_Speed, TRUE, &Param_Sensor, &Languages, NULL };
const _Variables Var_Parameters14 = { 1, 18, 2, &Service_Variables_Set.Serv_Vars.Sensor_Counter_Move, TRUE, &Param_Sensor, &Languages, &Var_Parameters15 };
const _Variables Var_Parameters13 = { 0, 11, 1, &Service_Variables_Set.Serv_Vars.Sensor_Type, TRUE, &Sensor_Param, &Languages, &Var_Parameters14 };

const _Strings String_Parameters24 = { 5, 18, 0, z4, &Languages, NULL};
const _Strings String_Parameters23 = { 4, 16, 0, _4z, &Languages, &String_Parameters24};
const _Strings String_Parameters22 = { 3, 0 , 3, Angle2, &Languages, &String_Parameters23};
const _Strings String_Parameters21 = { 2, 0 , 2, Angle1, &Languages, &String_Parameters22};
const _Strings String_Parameters20 = { 1, 0 , 1, Typ_Czujnika, &Languages, &String_Parameters21};
const _Strings String_Parameters19 = { 0, 0 , 0, Param_mechanizm, &Languages, &String_Parameters20};

const _Screen_Ptrs_Masks Screen_Param_Mech4 = { &String_Parameters19, NULL, &Var_Parameters13, NULL, NULL, NULL };


/*******************************************************************************
* Ster1 screen objects definition - SCREEN 14
*******************************************************************************/
const _Variables Var_Settings5 = { 4, 15,  3, &Controlling_Variables_Set.Ctrl_Vars.Czas_otwarcia_bramki, TRUE, &Czas_otw_param, &Languages, NULL };
const _Variables Var_Settings4 = { 3, 18,  2, &Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika4, TRUE, &Silnik_Param, &Languages, &Var_Settings5 };
const _Variables Var_Settings3 = { 2, 15,  2, &Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika3, TRUE, &Silnik_Param, &Languages, &Var_Settings4 };
const _Variables Var_Settings2 = { 1, 18,  1, &Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika2, TRUE, &Silnik_Param, &Languages, &Var_Settings3 };
const _Variables Var_Settings1 = { 0, 15,  1, &Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika1, TRUE, &Silnik_Param, &Languages, &Var_Settings2 };

const _Strings String_Settings6 = { 5, 18, 0, z3, &Languages, NULL};
const _Strings String_Settings5 = { 4, 16, 0, _1z, &Languages, &String_Settings6};
const _Strings String_Settings4 = { 3, 0, 3, Czas_otw, &Languages, &String_Settings5};
const _Strings String_Settings3 = { 2, 0, 2, Predkosc2, &Languages, &String_Settings4};
const _Strings String_Settings2 = { 1, 0, 1, Predkosc1, &Languages, &String_Settings3};
const _Strings String_Settings1 = { 0, 0, 0, Param_sterowanie, &Languages, &String_Settings2};

const _Screen_Ptrs_Masks Screen_Param_Ster1 = { &String_Settings1, NULL, &Var_Settings1, NULL, NULL, NULL };

/*******************************************************************************
* Ster1 screen objects definition - SCREEN 14
*******************************************************************************/
const _Variables Var_Settings8 = { 2, 14,  3, &Controlling_Variables_Set.Ctrl_Vars.Back_Signal_Type, TRUE, &Back_Signal_Type_Param, &Languages, NULL };
const _Variables Var_Settings7 = { 1, 17,  2, &Controlling_Variables_Set.Ctrl_Vars.Czas_syg_zwr, TRUE, &Czas_syg_zwr_param, &Languages, &Var_Settings8 };
const _Variables Var_Settings6 = { 0, 17,  1, &Controlling_Variables_Set.Ctrl_Vars.Transition_time, TRUE, &Transition_time_param, &Languages, &Var_Settings7 };

const _Strings String_Settings12 = { 5, 18, 0, z3, &Languages, NULL};
const _Strings String_Settings11 = { 4, 16, 0, _2z, &Languages, &String_Settings12};
const _Strings String_Settings10 = { 3, 0, 3, Sig_Typ, &Languages, &String_Settings11};
const _Strings String_Settings9 = { 2, 0, 2, Czas_sygn_zwr, &Languages, &String_Settings10};
const _Strings String_Settings8 = { 1, 0, 1, Trans_time, &Languages, &String_Settings9};
const _Strings String_Settings7 = { 0, 0, 0, Param_sterowanie, &Languages, &String_Settings8};

const _Screen_Ptrs_Masks Screen_Param_Ster2 = { &String_Settings7, NULL, &Var_Settings6, NULL, NULL, NULL };

/*******************************************************************************
* Ster1 screen objects definition - SCREEN 14-1
*******************************************************************************/
const _Variables Var_Settings9 = { 0, 17,  1, &Controlling_Variables_Set.Ctrl_Vars.Trans_time, TRUE, &Trans_time_param, &Languages, NULL };

const _Strings String_Settings16 = { 3, 18, 0, z3, &Languages, NULL};
const _Strings String_Settings15 = { 2, 16, 0, _3z, &Languages, &String_Settings16};
const _Strings String_Settings14 = { 1, 0, 1, Tamper_Relay, &Languages, &String_Settings15};
const _Strings String_Settings13 = { 0, 0, 0, Param_sterowanie, &Languages, &String_Settings14};

const _Screen_Ptrs_Masks Screen_Param_Ster3 = { &String_Settings13, NULL, &Var_Settings9, NULL, NULL, NULL };

/*******************************************************************************
* Functions1 screen objects definition - SCREEN 15
*******************************************************************************/
const _Variables Var_Fun4 = { 3, 14,  3, &Service_Variables_Set.Serv_Vars.Szybkie_wejscie, TRUE, &TN_Param, &Languages, NULL };
const _Variables Var_Fun3 = { 2, 14,  2, &Service_Variables_Set.Serv_Vars.W_wyjscie, TRUE, &Wowy_param, &Languages, &Var_Fun4 };
const _Variables Var_Fun2 = { 1, 11,  1, &Service_Variables_Set.Serv_Vars.Sap_Type, TRUE, &Sap_Type_Param, &Languages, &Var_Fun3 };
const _Variables Var_Fun1 = { 0, 4,  1, &Service_Variables_Set.Serv_Vars.Sap, TRUE, &Sap_param, &Languages, &Var_Fun2 };

const _Strings String_Fun6 = { 5, 0, 3, Szybkie, &Languages, NULL};
const _Strings String_Fun5 = { 4, 0, 2, Wo_Wy, &Languages, &String_Fun6};
const _Strings String_Fun4 = { 3, 0, 1, Sap, &Languages, &String_Fun5};
const _Strings String_Fun3 = { 2, 18, 0, z5, &Languages, &String_Fun4};
const _Strings String_Fun2 = { 1, 16, 0, _1z, &Languages, &String_Fun3};
const _Strings String_Fun1 = { 0, 0, 0, Menu_Fun, &Languages, &String_Fun2};

const _Screen_Ptrs_Masks Screen_Functions1 = { &String_Fun1, NULL, &Var_Fun1, NULL, NULL, NULL };



/*******************************************************************************
* Functions2 screen objects definition - SCREEN 15-1
*******************************************************************************/
const _Variables Var_Fun7 = { 2, 15,  3, &Service_Variables_Set.Serv_Vars.Priority, TRUE, &Priorytet_Param, &Languages, NULL };
const _Variables Var_Fun6 = { 1, 17,  2, &Service_Variables_Set.Serv_Vars.Standby_No_Brake_Wait, TRUE, &SNBW_Param, &Languages, &Var_Fun7 };
const _Variables Var_Fun5 = { 0, 17,  1, &Service_Variables_Set.Serv_Vars.Standby_No_Brake, TRUE, &TN_Param, &Languages, &Var_Fun6 };

const _Strings String_Fun12 = { 3, 0, 3, Priorytet_przejscia, &Languages, NULL};
const _Strings String_Fun11 = { 3, 0, 2, SP_Hamulec_Czas, &Languages, &String_Fun12};
const _Strings String_Fun10 = { 3, 0, 1, SP_Hamulec, &Languages, &String_Fun11};
const _Strings String_Fun9 = { 2, 18, 0, z5, &Languages, &String_Fun10};
const _Strings String_Fun8 = { 1, 16, 0, _2z, &Languages, &String_Fun9};
const _Strings String_Fun7 = { 0, 0, 0, Menu_Fun, &Languages, &String_Fun8};

const _Screen_Ptrs_Masks Screen_Functions2 = { &String_Fun7, NULL, &Var_Fun5, NULL, NULL, NULL };
/*******************************************************************************
* Functions3 screen objects definition - SCREEN 15-2
*******************************************************************************/
const _Variables Var_Fun17 = { 10, 18, 3, &Service_Variables_Set.Serv_Vars.Free_Minute_22, TRUE, &Clock_Minute, &Languages, NULL };
const _Variables Var_Fun16 = { 8, 15,  3, &Service_Variables_Set.Serv_Vars.Free_Hour_22, TRUE, &Clock_Hour, &Languages, &Var_Fun17 };
const _Variables Var_Fun15 = { 7, 10,  3, &Service_Variables_Set.Serv_Vars.Free_Minute_21, TRUE, &Clock_Minute, &Languages, &Var_Fun16 };
const _Variables Var_Fun14 = { 6, 7,  3, &Service_Variables_Set.Serv_Vars.Free_Hour_21, TRUE, &Clock_Hour, &Languages, &Var_Fun15 };
const _Variables Var_Fun13 = { 5, 1,  3, &Service_Variables_Set.Serv_Vars.Free_Marker_2, TRUE, &Wowy_param, &Languages, &Var_Fun14 };
const _Variables Var_Fun12 = { 4, 18,  2, &Service_Variables_Set.Serv_Vars.Free_Minute_12, TRUE, &Clock_Minute, &Languages, &Var_Fun13 };
const _Variables Var_Fun11 = { 3, 15,  2, &Service_Variables_Set.Serv_Vars.Free_Hour_12, TRUE, &Clock_Hour, &Languages, &Var_Fun12 };
const _Variables Var_Fun10 = { 2, 10,  2, &Service_Variables_Set.Serv_Vars.Free_Minute_11, TRUE, &Clock_Minute, &Languages, &Var_Fun11 };
const _Variables Var_Fun9 = { 1, 7,  2, &Service_Variables_Set.Serv_Vars.Free_Hour_11, TRUE, &Clock_Hour, &Languages, &Var_Fun10 };
const _Variables Var_Fun8 = { 0, 1,  2, &Service_Variables_Set.Serv_Vars.Free_Marker_1, TRUE, &Wowy_param, &Languages, &Var_Fun9 };

const _Strings String_Fun18 = { 3, 9, 3, Kropki_1, &Languages, NULL};
const _Strings String_Fun17 = { 3, 9, 2, Kropki_1, &Languages, &String_Fun18};
const _Strings String_Fun16 = { 3, 0, 1, Czas_WW, &Languages, &String_Fun17};
const _Strings String_Fun15 = { 2, 18, 0, z5, &Languages, &String_Fun16};
const _Strings String_Fun14 = { 1, 16, 0, _3z, &Languages, &String_Fun15};
const _Strings String_Fun13 = { 0, 0, 0, Menu_Fun, &Languages, &String_Fun14};

const _Screen_Ptrs_Masks Screen_Functions3 = { &String_Fun13, NULL, &Var_Fun8, NULL, NULL, NULL };



/*******************************************************************************
* Functions4 screen objects definition - SCREEN 15-3
*******************************************************************************/

const _Variables Var_Fun27 = { 10, 18, 3, &Service_Variables_Set.Serv_Vars.Fast_Minute_22, TRUE, &Clock_Minute, &Languages, NULL };
const _Variables Var_Fun26 = { 8, 15,  3, &Service_Variables_Set.Serv_Vars.Fast_Hour_22, TRUE, &Clock_Hour, &Languages, &Var_Fun27 };
const _Variables Var_Fun25 = { 7, 10,  3, &Service_Variables_Set.Serv_Vars.Fast_Minute_21, TRUE, &Clock_Minute, &Languages, &Var_Fun26 };
const _Variables Var_Fun24 = { 6, 7,  3, &Service_Variables_Set.Serv_Vars.Fast_Hour_21, TRUE, &Clock_Hour, &Languages, &Var_Fun25 };
const _Variables Var_Fun23 = { 5, 1,  3, &Service_Variables_Set.Serv_Vars.Fast_Marker_2, TRUE, &TN_Param, &Languages, &Var_Fun24 };
const _Variables Var_Fun22 = { 4, 18,  2, &Service_Variables_Set.Serv_Vars.Fast_Minute_12, TRUE, &Clock_Minute, &Languages, &Var_Fun23 };
const _Variables Var_Fun21 = { 3, 15,  2, &Service_Variables_Set.Serv_Vars.Fast_Hour_12, TRUE, &Clock_Hour, &Languages, &Var_Fun22 };
const _Variables Var_Fun20 = { 2, 10,  2, &Service_Variables_Set.Serv_Vars.Fast_Minute_11, TRUE, &Clock_Minute, &Languages, &Var_Fun21 };
const _Variables Var_Fun19 = { 1, 7,  2, &Service_Variables_Set.Serv_Vars.Fast_Hour_11, TRUE, &Clock_Hour, &Languages, &Var_Fun20 };
const _Variables Var_Fun18 = { 0, 1,  2, &Service_Variables_Set.Serv_Vars.Fast_Marker_1, TRUE, &TN_Param, &Languages, &Var_Fun19 };

const _Strings String_Fun24 = { 3, 9, 3, Kropki_1, &Languages, NULL};
const _Strings String_Fun23 = { 3, 9, 2, Kropki_1, &Languages, &String_Fun24};
const _Strings String_Fun22 = { 3, 0, 1, Czas_SP, &Languages, &String_Fun23};
const _Strings String_Fun21 = { 2, 18, 0, z5, &Languages, &String_Fun22};
const _Strings String_Fun20 = { 1, 16, 0, _4z, &Languages, &String_Fun21};
const _Strings String_Fun19 = { 0, 0, 0, Menu_Fun, &Languages, &String_Fun20};

const _Screen_Ptrs_Masks Screen_Functions4 = { &String_Fun19, NULL, &Var_Fun18, NULL, NULL, NULL };
/*******************************************************************************
* Functions5 screen objects definition - SCREEN 15-4
*******************************************************************************/

const _Variables Var_Fun30 = { 2, 16, 3, &Controlling_Variables_Set.Ctrl_Vars.Free_Exit_Time, TRUE, &Free_Exit_Param, &Languages, NULL };
const _Variables Var_Fun29 = { 1, 16, 2, &Controlling_Variables_Set.Ctrl_Vars.Normal_Exit_Time, TRUE, &Normal_Exit_Param, &Languages, &Var_Fun30 };
const _Variables Var_Fun28 = { 0, 17, 1, &Controlling_Variables_Set.Ctrl_Vars.Exit_Var, TRUE, &TN_Param, &Languages, &Var_Fun29 };

const _Strings String_Fun30 = { 3, 0, 3, FE_Time, &Languages, NULL};
const _Strings String_Fun29 = { 3, 0, 2, Signal_Time, &Languages, &String_Fun30};
const _Strings String_Fun28 = { 3, 0, 1, Edge_Time, &Languages, &String_Fun29};
const _Strings String_Fun27 = { 2, 18, 0, z5, &Languages, &String_Fun28};
const _Strings String_Fun26 = { 1, 16, 0, _5z, &Languages, &String_Fun27};
const _Strings String_Fun25 = { 0, 0, 0, Menu_Fun, &Languages, &String_Fun26};

const _Screen_Ptrs_Masks Screen_Functions5 = { &String_Fun25, NULL, &Var_Fun28, NULL, NULL, NULL };



/*******************************************************************************
* Passwords settings screen objects definition - SCREEN 16
*******************************************************************************/
const _Variables Var_Passwd4 = { 3, 18, 3, &Service_Variables_Set.Serv_Vars.Ilosc_Prob_Dostepu, TRUE, &Param_Prob, &Languages, NULL };
const _Variables Var_Passwd3 = { 2, 8,  3, &Service_Variables_Set.Serv_Vars.Haslo_Dostepu, TRUE, &Passwd, &Languages, &Var_Passwd4 };
const _Variables Var_Passwd2 = { 1, 18, 2, &Service_Variables_Set.Serv_Vars.Ilosc_Prob_Blokady, TRUE, &Param_Prob, &Languages, &Var_Passwd3 };
const _Variables Var_Passwd1 = { 0, 8,  2, &Service_Variables_Set.Serv_Vars.Haslo_Blokady, TRUE, &Passwd, &Languages, &Var_Passwd2 };

const _Strings String_Passwd5 = { 4, 0, 3, Dostepu, &Languages, NULL};
const _Strings String_Passwd4 = { 3, 0, 2, Blokady, &Languages, &String_Passwd5};
const _Strings String_Passwd3 = { 2, 14, 1, Proby, &Languages, &String_Passwd4};
const _Strings String_Passwd2 = { 1, 5, 1, Haslo, &Languages, &String_Passwd3};
const _Strings String_Passwd1 = { 0, 0, 0, Ust_hasel, &Languages, &String_Passwd2};

const _Screen_Ptrs_Masks Screen_Passwd = { &String_Passwd1, NULL, &Var_Passwd1, NULL, NULL, NULL };


/*******************************************************************************
* Passwords settings screen objects definition - SCREEN 17
*******************************************************************************/
const _Variables Var_Lock4 = { 3, 16, 3, &Service_Variables_Set.Serv_Vars.Blokada_rok, TRUE, &Clock_Year, &Languages, NULL };
const _Variables Var_Lock3 = { 2, 13, 3, &Service_Variables_Set.Serv_Vars.Blokada_miesiac, TRUE, &Clock_Month, &Languages, &Var_Lock4 };
const _Variables Var_Lock2 = { 1, 10, 3, &Service_Variables_Set.Serv_Vars.Blokada_dzien, TRUE, &Clock_Day, &Languages, &Var_Lock3 };
const _Variables Var_Lock1 = { 0, 10, 2, &Service_Variables_Set.Serv_Vars.Blokada, TRUE, &TN_Param, &Languages, &Var_Lock2 };

const _Strings String_Lock5 = { 3, 15, 3, Kropka, &Languages, NULL};
const _Strings String_Lock4 = { 3, 12, 3, Kropka, &Languages, &String_Lock5};
const _Strings String_Lock3 = { 2, 0, 3, Blok_data, &Languages, &String_Lock4};
const _Strings String_Lock2 = { 1, 0, 2, Blok, &Languages, &String_Lock3};
const _Strings String_Lock1 = { 0, 0, 0, Ust_blokady, &Languages, &String_Lock2};

const _Screen_Ptrs_Masks Screen_Lock = { &String_Lock1, NULL, &Var_Lock1, NULL, NULL, NULL };

/*******************************************************************************
* CAL_RTC settings screen objects definition - SCREEN 18
*******************************************************************************/
const _Variables Var_RTC1 = { 0, 8, 2, &Service_Variables_Set.Serv_Vars.RTC_Cal_Var, TRUE, &Param_RTC, &Languages, NULL };

const _Strings String_RTC1 = { 0, 0, 0, RTC_cal, &Languages, NULL};

const _Screen_Ptrs_Masks Screen_RTC = { &String_RTC1, NULL, &Var_RTC1, NULL, NULL, NULL };


/*******************************************************************************
* Test screen objects definition - SCREEN 19
*******************************************************************************/

const _Variables Var_Test_Prog2 = { 1, 4,  2, &Counters_Variables.Counters_Vars.No_Goout_Test, FALSE, &Counters, &Languages, NULL };
const _Variables Var_Test_Prog1 = { 0, 4,  1, &Counters_Variables.Counters_Vars.No_Goin_Test, FALSE, &Counters, &Languages, &Var_Test_Prog2 };

const _Strings String_Test_Prog6 = { 5, 15, 1, Test_Stop, &Languages, NULL};
const _Strings String_Test_Prog5 = { 4, 15, 1, Test_Start, &Languages, &String_Test_Prog6 };
const _Strings String_Test_Prog4 = { 3, 0, 3, Start_Stop, &Languages, &String_Test_Prog5 };
const _Strings String_Test_Prog3 = { 2, 0, 2, Wy, &Languages, &String_Test_Prog4 };
const _Strings String_Test_Prog2 = { 1, 0, 1, We, &Languages, &String_Test_Prog3 };
const _Strings String_Test_Prog1 = { 0, 0, 0, Prog_testowy, &Languages, &String_Test_Prog2 };

const _Screen_Ptrs_Masks Screen_Prog_Test = { &String_Test_Prog1, NULL, &Var_Test_Prog1, &Test_String_Mask, NULL, NULL };


/*******************************************************************************
* Enkoder screen objects definition - SCREEN 20
*******************************************************************************/
const _Variables Var_Test_Enk2 = { 0, 12, 2, &Controller_Variables.Controller_Vars.Encoder_Var, FALSE, &Param_Enk, &Languages, NULL };
const _Variables Var_Test_Enk1 = { 0,  4, 2, &Controlling_Variables.Ctrl_Vars.Encoder, FALSE, &Param_Enk, &Languages, &Var_Test_Enk2 };

const _Strings String_Test_Enk1 = { 0, 0, 0, Ust_enkodera, &Languages, NULL };

const _Screen_Ptrs_Masks Screen_Ust_Enk = { &String_Test_Enk1, NULL, &Var_Test_Enk1, NULL, NULL, NULL };


/*******************************************************************************
* Total lock screen objects definition - SCREEN 21
*******************************************************************************/
const _Strings String_Blok_Calk1 = { 0, 1, 1, Blokada_calkowita, &Languages, NULL};

const _Screen_Ptrs_Masks Screen_Blok_Calk = { &String_Blok_Calk1, NULL, NULL, NULL, NULL, NULL };


/*******************************************************************************
* Controller lock screen objects definition - SCREEN 22
*******************************************************************************/
const _Strings String_Ster_Blok3 = { 2, 1, 3, Odblokuj, &Languages, NULL };
const _Strings String_Ster_Blok2 = { 1, 1, 1, Zablokowany, &Languages, &String_Ster_Blok3 };
const _Strings String_Ster_Blok1 = { 0, 1, 0, Sterownik, &Languages, &String_Ster_Blok2 };

const _Screen_Ptrs_Masks Screen_Ster_Blok = { &String_Ster_Blok1, NULL, NULL, NULL, NULL, NULL };

/*******************************************************************************
* Lock Password screen objects definition - SCREEN 23
*******************************************************************************/
const _Variables Var_Odblokuj2 = { 0, 18,  3, &Service_Variables_Set.Serv_Vars.Blok_Prog_Pozostalo, FALSE, &Param_2h, &Languages, NULL };
const _Variables Var_Odblokuj1 = { 0, 7,  2, &Password_Set, TRUE, &Passwd, &Languages, &Var_Odblokuj2 };


const _Strings String_Odblokuj2 = { 1, 0, 3, Ilosc_prob, &Languages, NULL};
const _Strings String_Odblokuj1 = { 0, 0, 0, Haslo_blokady, &Languages, &String_Odblokuj2};

const _Screen_Ptrs_Masks Screen_Odblokuj = { &String_Odblokuj1, NULL, &Var_Odblokuj1, NULL, NULL, NULL };

/*******************************************************************************
* Restore factory sets screen objects definition - SCREEN 24
*******************************************************************************/
const _Strings String_Fabryczne3 = { 2, 0, 3, ESC_Nie, &Languages, NULL};
const _Strings String_Fabryczne2 = { 1, 0, 2, OK_Tak, &Languages, &String_Fabryczne3};
const _Strings String_Fabryczne1 = { 0, 0, 0, Menu_8, &Languages, &String_Fabryczne2};

const _Screen_Ptrs_Masks Screen_Fabryczne = { &String_Fabryczne1, NULL, NULL, NULL, NULL, NULL };
/*******************************************************************************
* Reset counters screen objects definition - SCREEN 25
*******************************************************************************/
const _Strings String_Zerowanie3 = { 2, 0, 3, ESC_Nie, &Languages, NULL};
const _Strings String_Zerowanie2 = { 1, 0, 2, OK_Tak, &Languages, &String_Zerowanie3};
const _Strings String_Zerowanie1 = { 0, 0, 0, Zerowanie, &Languages, &String_Zerowanie2};

const _Screen_Ptrs_Masks Screen_Zerowanie = { &String_Zerowanie1, NULL, NULL, NULL, NULL, NULL };

/*******************************************************************************
* Service settings (1/4) screen objects definition - SCREEN 26
*******************************************************************************/
const _Variables Var_Ust_Serwisowe6 = { 5, 14,  3, &Service_Variables_Set.Serv_Vars.Serwis_006, TRUE, &Param_Serwis, &Languages, NULL };
const _Variables Var_Ust_Serwisowe5 = { 4, 14,  2, &Service_Variables_Set.Serv_Vars.Serwis_005, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe6 };
const _Variables Var_Ust_Serwisowe4 = { 3, 14,  1, &Service_Variables_Set.Serv_Vars.Serwis_004, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe5 };
const _Variables Var_Ust_Serwisowe3 = { 2, 4,  3, &Service_Variables_Set.Serv_Vars.Serwis_003, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe4 };
const _Variables Var_Ust_Serwisowe2 = { 1, 4,  2, &Service_Variables_Set.Serv_Vars.Serwis_002, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe3 };
const _Variables Var_Ust_Serwisowe1 = { 0, 4,  1, &Service_Variables_Set.Serv_Vars.Serwis_001, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe2 };

const _Strings String_Ust_Serwisowe6 = { 5, 0, 3, Par3, &Languages, NULL};
const _Strings String_Ust_Serwisowe5 = { 4, 0, 2, Par2, &Languages, &String_Ust_Serwisowe6};
const _Strings String_Ust_Serwisowe4 = { 3, 0, 1, Par1, &Languages, &String_Ust_Serwisowe5};
const _Strings String_Ust_Serwisowe3 = { 2, 18, 0, z4, &Languages, &String_Ust_Serwisowe4};
const _Strings String_Ust_Serwisowe2 = { 1, 16, 0, _1z, &Languages, &String_Ust_Serwisowe3};
const _Strings String_Ust_Serwisowe1 = { 0, 0, 0, Ust_serwis, &Languages, &String_Ust_Serwisowe2};

const _Screen_Ptrs_Masks Screen_Ust_Serwis1 = { &String_Ust_Serwisowe1, NULL, &Var_Ust_Serwisowe1, NULL, NULL, NULL };
/*******************************************************************************
* Service settings (2/4) screen objects definition - SCREEN 27
*******************************************************************************/
const _Variables Var_Ust_Serwisowe12 = { 5, 14,  3, &Service_Variables_Set.Serv_Vars.Serwis_012, TRUE, &Param_Serwis, &Languages, NULL };
const _Variables Var_Ust_Serwisowe11 = { 4, 14,  2, &Service_Variables_Set.Serv_Vars.Serwis_011, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe12 };
const _Variables Var_Ust_Serwisowe10 = { 3, 14,  1, &Service_Variables_Set.Serv_Vars.Serwis_010, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe11 };
const _Variables Var_Ust_Serwisowe9 = { 2, 4,  3, &Service_Variables_Set.Serv_Vars.Serwis_009, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe10 };
const _Variables Var_Ust_Serwisowe8 = { 1, 4,  2, &Service_Variables_Set.Serv_Vars.Serwis_008, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe9 };
const _Variables Var_Ust_Serwisowe7 = { 0, 4,  1, &Service_Variables_Set.Serv_Vars.Serwis_007, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe8 };

const _Strings String_Ust_Serwisowe12 = { 5, 0, 3, Par6, &Languages, NULL};
const _Strings String_Ust_Serwisowe11 = { 4, 0, 2, Par5, &Languages, &String_Ust_Serwisowe12};
const _Strings String_Ust_Serwisowe10 = { 3, 0, 1, Par4, &Languages, &String_Ust_Serwisowe11};
const _Strings String_Ust_Serwisowe9 = { 2, 18, 0, z4, &Languages, &String_Ust_Serwisowe10};
const _Strings String_Ust_Serwisowe8 = { 1, 16, 0, _2z, &Languages, &String_Ust_Serwisowe9};
const _Strings String_Ust_Serwisowe7 = { 0, 0, 0, Ust_serwis, &Languages, &String_Ust_Serwisowe8};

const _Screen_Ptrs_Masks Screen_Ust_Serwis2 = { &String_Ust_Serwisowe7, NULL, &Var_Ust_Serwisowe7, NULL, NULL, NULL };
/*******************************************************************************
* Service settings (3/4) screen objects definition - SCREEN 28
*******************************************************************************/
const _Variables Var_Ust_Serwisowe18 = { 5, 14,  3, &Service_Variables_Set.Serv_Vars.Serwis_018, TRUE, &Param_Serwis, &Languages, NULL };
const _Variables Var_Ust_Serwisowe17 = { 4, 14,  2, &Service_Variables_Set.Serv_Vars.Serwis_017, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe18 };
const _Variables Var_Ust_Serwisowe16 = { 3, 14,  1, &Service_Variables_Set.Serv_Vars.Serwis_016, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe17 };
const _Variables Var_Ust_Serwisowe15 = { 2, 4,  3, &Service_Variables_Set.Serv_Vars.Serwis_015, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe16 };
const _Variables Var_Ust_Serwisowe14 = { 1, 4,  2, &Service_Variables_Set.Serv_Vars.Serwis_014, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe15 };
const _Variables Var_Ust_Serwisowe13 = { 0, 4,  1, &Service_Variables_Set.Serv_Vars.Serwis_013, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe14 };

const _Strings String_Ust_Serwisowe18 = { 5, 0, 3, Par9, &Languages, NULL};
const _Strings String_Ust_Serwisowe17 = { 4, 0, 2, Par8, &Languages, &String_Ust_Serwisowe18};
const _Strings String_Ust_Serwisowe16 = { 3, 0, 1, Par7, &Languages, &String_Ust_Serwisowe17};
const _Strings String_Ust_Serwisowe15 = { 2, 18, 0, z4, &Languages, &String_Ust_Serwisowe16};
const _Strings String_Ust_Serwisowe14 = { 1, 16, 0, _3z, &Languages, &String_Ust_Serwisowe15};
const _Strings String_Ust_Serwisowe13 = { 0, 0, 0, Ust_serwis, &Languages, &String_Ust_Serwisowe14};

const _Screen_Ptrs_Masks Screen_Ust_Serwis3 = { &String_Ust_Serwisowe13, NULL, &Var_Ust_Serwisowe13, NULL, NULL, NULL };

/*******************************************************************************
* Service settings (4/4) screen objects definition - SCREEN 28-1
*******************************************************************************/
const _Variables Var_Ust_Serwisowe24 = { 5, 14,  3, &Service_Variables_Set.Serv_Vars.Serwis_024, TRUE, &Param_Serwis, &Languages, NULL };
const _Variables Var_Ust_Serwisowe23 = { 4, 14,  2, &Service_Variables_Set.Serv_Vars.Serwis_023, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe24 };
const _Variables Var_Ust_Serwisowe22 = { 3, 14,  1, &Service_Variables_Set.Serv_Vars.Serwis_022, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe23 };
const _Variables Var_Ust_Serwisowe21 = { 2, 4,  3, &Service_Variables_Set.Serv_Vars.Serwis_021, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe22 };
const _Variables Var_Ust_Serwisowe20 = { 1, 4,  2, &Service_Variables_Set.Serv_Vars.Serwis_020, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe21 };
const _Variables Var_Ust_Serwisowe19 = { 0, 4,  1, &Service_Variables_Set.Serv_Vars.Serwis_019, TRUE, &Param_Serwis, &Languages, &Var_Ust_Serwisowe20 };

const _Strings String_Ust_Serwisowe24 = { 5, 0, 3, Par12, &Languages, NULL};
const _Strings String_Ust_Serwisowe23 = { 4, 0, 2, Par11, &Languages, &String_Ust_Serwisowe24};
const _Strings String_Ust_Serwisowe22 = { 3, 0, 1, Par10, &Languages, &String_Ust_Serwisowe23};
const _Strings String_Ust_Serwisowe21 = { 2, 18, 0, z4, &Languages, &String_Ust_Serwisowe22};
const _Strings String_Ust_Serwisowe20 = { 1, 16, 0, _4z, &Languages, &String_Ust_Serwisowe21};
const _Strings String_Ust_Serwisowe19 = { 0, 0, 0, Ust_serwis, &Languages, &String_Ust_Serwisowe20};

const _Screen_Ptrs_Masks Screen_Ust_Serwis4 = { &String_Ust_Serwisowe19, NULL, &Var_Ust_Serwisowe19, NULL, NULL, NULL };


/*******************************************************************************
* Service read settings (3/3) screen objects definition - SCREEN 29
*******************************************************************************/
const _Variables Var_Odczyt_Serwis12 = { 11, 15, 3, &Controller_Variables.Controller_Vars.Angle, FALSE, &Param_Angle, &Languages, NULL };
const _Variables Var_Odczyt_Serwis11 = { 10, 16, 2, &Controller_Variables.Controller_Vars.Cosine, FALSE, &Param_Enk, &Languages, &Var_Odczyt_Serwis12 };
const _Variables Var_Odczyt_Serwis10 = {  9, 16, 1, &Controller_Variables.Controller_Vars.Sine, FALSE, &Param_Enk, &Languages, &Var_Odczyt_Serwis11 };


const _Variables Var_Odczyt_Serwis9 = { 8, 11, 3, &Controller_Variables.Controller_Vars.Sensor_Counter, FALSE, &Param_RTC, &Languages, &Var_Odczyt_Serwis10 };
const _Variables Var_Odczyt_Serwis8 = { 7, 5, 3, &Inputs_Level, FALSE, &Param_Enk, &Languages, &Var_Odczyt_Serwis9 };
const _Variables Var_Odczyt_Serwis7 = { 6, 0, 3, &Controller_Variables.Controller_Vars.AD_7, FALSE, &Param_Enk, &Languages, &Var_Odczyt_Serwis8 };
const _Variables Var_Odczyt_Serwis6 = { 5, 11, 2, &Controller_Variables.Controller_Vars.Sensor_Direction_Detect, FALSE, &Param_RTC, &Languages, &Var_Odczyt_Serwis7 };
const _Variables Var_Odczyt_Serwis5 = { 4, 5, 2, &Persons_Counter_Bck, FALSE, &Param_Enk, &Languages, &Var_Odczyt_Serwis6 };
const _Variables Var_Odczyt_Serwis4 = { 3, 0, 2, &Persons_Counter_Fwd, FALSE, &Param_Enk, &Languages, &Var_Odczyt_Serwis5 };
const _Variables Var_Odczyt_Serwis3 = { 2, 11, 1, &Controller_Variables.Controller_Vars.Machine_State_Var, FALSE, &Param_Enk, &Languages, &Var_Odczyt_Serwis4 };
const _Variables Var_Odczyt_Serwis2 = { 1, 5, 1, &Controller_Variables.Controller_Vars.Encoder_Var, FALSE, &Param_Enk, &Languages, &Var_Odczyt_Serwis3 };
const _Variables Var_Odczyt_Serwis1 = { 0, 0, 1, &Controlling_Variables.Ctrl_Vars.Encoder, FALSE, &Param_Enk, &Languages, &Var_Odczyt_Serwis2 };

const _Strings String_Odczyt_Serwis3 = { 2, 18, 0, z1, &Languages, NULL};
const _Strings String_Odczyt_Serwis2 = { 1, 16, 0, _1z, &Languages, &String_Odczyt_Serwis3};
const _Strings String_Odczyt_Serwis1 = { 0, 0, 0, Odczyt_serwis, &Languages, &String_Odczyt_Serwis2};

const _Screen_Ptrs_Masks Screen_Odczyt_Serwis1 = { &String_Odczyt_Serwis1, NULL, &Var_Odczyt_Serwis1, NULL, NULL, NULL };

/*******************************************************************************
* Heat Cool settings (1/1) screen objects definition - SCREEN 30
*******************************************************************************/
const _Variables Var_Heat_Cool6 = { 5, 15, 3, &Controlling_Variables_Set.Ctrl_Vars.Vent_Max, TRUE, &Param_Vent_Max, &Languages, NULL };
const _Variables Var_Heat_Cool5 = { 4, 5, 3, &Controlling_Variables_Set.Ctrl_Vars.Heat_Max, TRUE, &Param_Heat_Max, &Languages, &Var_Heat_Cool6 };
const _Variables Var_Heat_Cool4 = { 3, 15, 2, &Controlling_Variables_Set.Ctrl_Vars.Vent_Min, TRUE, &Param_Vent_Min, &Languages, &Var_Heat_Cool5 };
const _Variables Var_Heat_Cool3 = { 2, 5, 2, &Controlling_Variables_Set.Ctrl_Vars.Heat_Min, TRUE, &Param_Heat_Min, &Languages, &Var_Heat_Cool4 };
const _Variables Var_Heat_Cool2 = { 1, 16, 1, &Controlling_Variables_Set.Ctrl_Vars.Vent_Marker, TRUE, &TN_Param, &Languages, &Var_Heat_Cool3 };
const _Variables Var_Heat_Cool1 = { 0, 6, 1, &Controlling_Variables_Set.Ctrl_Vars.Heat_Marker, TRUE, &TN_Param, &Languages, &Var_Heat_Cool2 };

const _Strings String_Heat_Cool5 = { 4, 11, 1, Cooling, &Languages, NULL};
const _Strings String_Heat_Cool4 = { 3, 0, 3, Max_Val, &Languages, &String_Heat_Cool5};
const _Strings String_Heat_Cool3 = { 2, 0, 2, Min_Val, &Languages, &String_Heat_Cool4};
const _Strings String_Heat_Cool2 = { 1, 0, 1, Heating, &Languages, &String_Heat_Cool3};
const _Strings String_Heat_Cool1 = { 0, 0, 0, Heat_Cool_Param, &Languages, &String_Heat_Cool2};

const _Screen_Ptrs_Masks Screen_Heat_Cool1 = { &String_Heat_Cool1, NULL, &Var_Heat_Cool1, NULL, NULL, NULL };


/*******************************************************************************
* Lights settings (1/1) screen objects definition - SCREEN 31
*******************************************************************************/

const _Variables Var_Lights6 = { 5, 18, 3, &Service_Variables_Set.Serv_Vars.Lights_Minute2, TRUE, &Clock_Minute, &Languages, NULL };
const _Variables Var_Lights5 = { 4, 15, 3, &Service_Variables_Set.Serv_Vars.Lights_Hour2, TRUE, &Clock_Hour, &Languages, &Var_Lights6 };
const _Variables Var_Lights4 = { 3, 10, 3, &Service_Variables_Set.Serv_Vars.Lights_Minute1, TRUE, &Clock_Minute, &Languages, &Var_Lights5 };
const _Variables Var_Lights3 = { 2,  7, 3, &Service_Variables_Set.Serv_Vars.Lights_Hour1, TRUE, &Clock_Hour, &Languages, &Var_Lights4 };
const _Variables Var_Lights2 = { 1, 12, 2, &Service_Variables_Set.Serv_Vars.Lights_Period_Marker, TRUE, &TN_Param, &Languages, &Var_Lights3 };
const _Variables Var_Lights1 = { 0, 12, 1, &Service_Variables_Set.Serv_Vars.Lights_Sensor_Marker, TRUE, &TN_Param, &Languages, &Var_Lights2 };

const _Strings String_Lights4 = { 3, 9, 3, Kropki_1, &Languages, NULL};
const _Strings String_Lights3 = { 2, 0, 2, P_Czasowy, &Languages, &String_Lights4};
const _Strings String_Lights2 = { 1, 0, 1, Sensor, &Languages, &String_Lights3};
const _Strings String_Lights1 = { 0, 0, 0, Swiatla, &Languages, &String_Lights2};

const _Screen_Ptrs_Masks Screen_Lights1 = { &String_Lights1, NULL, &Var_Lights1, NULL, NULL, NULL };



/*******************************************************************************
* Inductive sensors test screen objects definition - SCREEN 32
******************************************************************************/
const _Variables Var_Ind_Test1 = { 0, 17, 3, &Controller_Variables.Controller_Vars.Sensor_Counter, FALSE, &Param_RTC, &Languages, NULL };

const _Strings String_Ind_Test6 = { 5, 9, 2, Ind_false, &Languages, NULL };//&String_Ind_Test2};
const _Strings String_Ind_Test5 = { 4, 9, 2, Ind_ok, &Languages, &String_Ind_Test6};
const _Strings String_Ind_Test4 = { 3, 9, 2, Ind_testing, &Languages, &String_Ind_Test5};
const _Strings String_Ind_Test3 = { 2, 9, 2, Ind_lines, &Languages, &String_Ind_Test4};
const _Strings String_Ind_Test2 = { 1, 0, 2, Ind_status, &Languages, &String_Ind_Test3};
const _Strings String_Ind_Test1 = { 0, 0, 0, Ind_test, &Languages, &String_Ind_Test2};

const _Screen_Ptrs_Masks Screen_Ind_Test = { &String_Ind_Test1, NULL, &Var_Ind_Test1, &Ind_Test_Mask, NULL, NULL };

/*******************************************************************************
* Inputs outputs sensors test screen objects definition - SCREEN 33
*******************************************************************************/
const _Variables Var_WeWy2 = { 1, 16, 3, &Controller_Variables.Controller_Vars.Encoder_Var, FALSE, &Param_Enk, &Languages, NULL };
const _Variables Var_WeWy1 = { 0, 1, 1, &Controller_Variables.Controller_Vars.Test_Status, TRUE, &TestWeWy_Param, &Languages, &Var_WeWy2 };

const _Strings String_WeWy9 = { 8, 0, 0, Test_We_Wy, &Languages, NULL };//&String_weWy10};
const _Strings String_WeWy8 = { 7, 0, 2, In8, &Languages, &String_WeWy9};
const _Strings String_WeWy7 = { 6, 4, 2, In7, &Languages, &String_WeWy8};
const _Strings String_WeWy6 = { 5, 8, 2, In6, &Languages, &String_WeWy7};
const _Strings String_WeWy5 = { 4, 12,2, In5, &Languages, &String_WeWy6};
const _Strings String_WeWy4 = { 3, 16,2, In4, &Languages, &String_WeWy5};
const _Strings String_WeWy3 = { 2, 0, 3, In3, &Languages, &String_WeWy4};
const _Strings String_WeWy2 = { 1, 4, 3, In2, &Languages, &String_WeWy3};
const _Strings String_WeWy1 = { 0, 8, 3, In1, &Languages, &String_WeWy2};


const _Screen_Ptrs_Masks Screen_Test_WeWy = { &String_WeWy1, NULL, &Var_WeWy1, &Manual_Test_Mask, NULL, NULL };

/*******************************************************************************
* Inputs outputs sensors test screen objects definition - SCREEN 34
*******************************************************************************/
const _Variables Var_Info7 = { 6, 5, 3, &Controller_Variables.Controller_Vars.Serial_Number, FALSE, &Param_Enk, &Languages, NULL };
const _Variables Var_Info6 = { 5, 12,2, &Controller_Variables.Controller_Vars.Controller_Date[2], FALSE, &Param_Serial, &Languages, &Var_Info7 };
const _Variables Var_Info5 = { 4, 9, 2, &Controller_Variables.Controller_Vars.Controller_Date[1], FALSE, &Param_Serial, &Languages, &Var_Info6 };
const _Variables Var_Info4 = { 3, 6, 2, &Controller_Variables.Controller_Vars.Controller_Date[0], FALSE, &Param_Serial, &Languages, &Var_Info5 };
const _Variables Var_Info3 = { 2, 15,1, &Controller_Variables.Controller_Vars.Software_Version[2], FALSE, &Param_Serial, &Languages, &Var_Info4 };
const _Variables Var_Info2 = { 1, 12,1, &Controller_Variables.Controller_Vars.Software_Version[1], FALSE, &Param_Serial, &Languages, &Var_Info3 };
const _Variables Var_Info1 = { 0, 9, 1, &Controller_Variables.Controller_Vars.Software_Version[0], FALSE, &Param_Serial, &Languages, &Var_Info2 };

const _Strings String_Info4 = { 3, 0, 3, S_N, &Languages, NULL};
const _Strings String_Info3 = { 2, 0, 2, Manufactured, &Languages, &String_Info4};
const _Strings String_Info2 = { 1, 0, 1, Soft_Version, &Languages, &String_Info3};
const _Strings String_Info1 = { 0, 0, 0, Menu_9, &Languages, &String_Info2};


const _Screen_Ptrs_Masks Screen_Info = { &String_Info1, NULL, &Var_Info1, NULL, NULL, NULL };

/*******************************************************************************
* Screen object main structure definition
*******************************************************************************/
const _Screen_Lcd Screen_Lcd[] = {
/*STANDBY*/    { Keyboard_Standby, &Screen_Standby },
/*ENABLE*/     { Keyboard_Enable, &Screen_Enable },
/*MENU*/       { Keyboard_Menu, &Screen_Menu1 },
/*PARAMETRY*/  { Keyboard_Param, &Screen_Param },
/*USTAWIENIA*/ { Keyboard_Set, &Screen_Set },
/*BRAMKA*/     { Keyboard_Gate, &Screen_Gate },
/*DATA*/       { Keyboard_Date, &Screen_Date },
/*LICENCJA*/   { Keyboard_Lic, &Screen_Lic },
/*TEST*/       { Keyboard_Test, &Screen_Test },
/*HASLO*/      { Keyboard_Haslo, &Screen_Haslo },
/*WYB_JEZ*/    { Keyboard_Language, &Screen_Language },
/*HAS_MENU*/   { Keyboard_Passwd, &Screen_Passwd },
/*HAS_BLOK*/   { Keyboard_Lock, &Screen_Lock },
/*KAL_RTC*/    { Keyboard_RTC, &Screen_RTC },
/*PARAM_MECH1*/{ Keyboard_Param_Mech, &Screen_Param_Mech1 },
/*PARAM_MECH2*/{ Keyboard_Param_Mech, &Screen_Param_Mech2 },
/*PARAM_MECH3*/{ Keyboard_Param_Mech, &Screen_Param_Mech3 },
/*PARAM_MECH4*/{ Keyboard_Param_Mech, &Screen_Param_Mech4 },
/*PARAM_STER1*/{ Keyboard_Param_Ster, &Screen_Param_Ster1 },
/*PARAM_STER2*/{ Keyboard_Param_Ster, &Screen_Param_Ster2 },
/*PARAM_STER2*/{ Keyboard_Param_Ster, &Screen_Param_Ster3 },

/*HEAT_COOL*/  { Keyboard_Heat_Cool, &Screen_Heat_Cool1 },
/*OSWIETLENIE*/{ Keyboard_Lights, &Screen_Lights1 },
/*FUNCTIONS1*/ { Keyboard_Functions1, &Screen_Functions1 },
/*FUNCTIONS2*/ { Keyboard_Functions1, &Screen_Functions2 },
/*FUNCTIONS3*/ { Keyboard_Functions1, &Screen_Functions3 },
/*FUNCTIONS4*/ { Keyboard_Functions1, &Screen_Functions4 },
/*FUNCTIONS4*/ { Keyboard_Functions1, &Screen_Functions5 },

/*PROG_TEST*/  { Keyboard_Prog_Test, &Screen_Prog_Test },
/*UST_ENKOD*/  { Keyboard_Ust_Enk, &Screen_Ust_Enk },

/*BLOK_CALK*/  { NULL, &Screen_Blok_Calk },
/*STER_BLOK*/  { Keyboard_Ster_Blok, &Screen_Ster_Blok },
/*ODBLOKUJ*/   { Keyboard_Odblokuj, &Screen_Odblokuj },
/*FABRYCZNE*/  { Keyboard_Fabryczne, &Screen_Fabryczne },
/*ZEROWANIE*/  { Keyboard_Zerowanie, &Screen_Zerowanie },

/*UST.SERWIS1*/{ Keyboard_Ust_Serwis, &Screen_Ust_Serwis1 },
/*UST.SERWIS2*/{ Keyboard_Ust_Serwis, &Screen_Ust_Serwis2 },
/*UST.SERWIS3*/{ Keyboard_Ust_Serwis, &Screen_Ust_Serwis3 },
/*UST.SERWIS4*/{ Keyboard_Ust_Serwis, &Screen_Ust_Serwis4 },

/*ODCZ.SERWIS*/{ Keyboard_Odczyt_Serwis, &Screen_Odczyt_Serwis1 },
/*IND_TEST*/   { Keyboard_Ind_Test, &Screen_Ind_Test },
/*TEST_WEWY*/  { Keyboard_Test_WeWy, &Screen_Test_WeWy },
/*INFO*/       { Keyboard_Info, &Screen_Info },
};

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
* Function Name  : Display_On_Screen
* Description    : Displaying data on LCD screen - strings/variables/images/Menu list
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Display_On_Screen ( void )
{
    static bool Var_Underline;
    static volatile uint8_t i;
    static int32_t Digit_Conv;
    static uint8_t Variable_Tab[11];
    static uint32_t Mask_Shift_Reg;
    const  uint32_t Mask_Shift_Bit = 0x00000001;

    _String_Number String_Number;

    const _Strings * Str_Ptr;
    const _Variables * Var_Ptr;

    Str_Ptr = Screen_Lcd[Screen_Number].Screen_Objects->String_Ptr;
    Var_Ptr = Screen_Lcd[Screen_Number].Screen_Objects->Variable_Ptr;

/******************************Displaying strings*****************************/
    if ( Reload_Screen == TRUE )
    {
        Lcd_Clear_Buffer ( &Lcd_Buffer );

        if ( Str_Ptr != NULL )
        {
            if ( Screen_Lcd[Screen_Number].Screen_Objects->String_Mask == NULL )
            {
                Mask_Shift_Reg = 0xffffffff;
            }
            else
            {
                Mask_Shift_Reg = *Screen_Lcd[Screen_Number].Screen_Objects->String_Mask;
            }

            while ( Str_Ptr != NULL )
            {
                Lcd_Set_Address (Str_Ptr->String_X_Pos,Str_Ptr->String_Y_Pos);
                String_Number = Str_Ptr->String_Num;
 
                if ( Mask_Shift_Reg & ( Mask_Shift_Bit<<Str_Ptr->String_Index ) )
                {
                    Lcd_Write_Buffer ( &Lcd_Buffer, *(String_Number+Str_Ptr->Languages_Ptr->Languages_Tab[Language_Num]) );
                }

                Str_Ptr = Str_Ptr->Next_String;
            };
        }
/******************************************************************************/

/****************************Displaying menu list******************************/

        if ( Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr != NULL )
        {
            if ( Screen_Lcd[Screen_Number].Screen_Objects->Menu_Mask == NULL )
            {
                Mask_Shift_Reg = 0xffffffff;
            }
            else
            {
                Mask_Shift_Reg = *Screen_Lcd[Screen_Number].Screen_Objects->Menu_Mask;
            }

            for ( i = 0; i < Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Lines_On_Screen; i++ )
            {
                String_Number = Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Menu_String_Num[(i+Menu_Keys.First_On_Screen)];

                Lcd_Set_Address( Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Menu_X_Pos, Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Menu_Y_Pos + i);

                if ( Mask_Shift_Reg & ( Mask_Shift_Bit<<i ) )
                {
                    Lcd_Write_Buffer ( &Lcd_Buffer, *(String_Number+Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Languages_Ptr->Languages_Tab[Language_Num]) );
                }
            }
            String_Number = St_R;
            Lcd_Set_Address ( 0, Menu_Keys.Active_On_Screen+1);
            Lcd_Write_Buffer ( &Lcd_Buffer, *(String_Number+Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Languages_Ptr->Languages_Tab[Language_Num]) );
            String_Number = St_L;
            Lcd_Set_Address ( 19, Menu_Keys.Active_On_Screen+1);
            Lcd_Write_Buffer ( &Lcd_Buffer, *(String_Number+Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Languages_Ptr->Languages_Tab[Language_Num]) );

        }

/******************************************************************************/

/****************************Displaying Variables******************************/
        if ( Var_Ptr != NULL )
        {
            if ( Screen_Lcd[Screen_Number].Screen_Objects->Variable_Mask == NULL )
            {
                Mask_Shift_Reg = 0xffffffff;
            }
            else
            {
                Mask_Shift_Reg = *Screen_Lcd[Screen_Number].Screen_Objects->Variable_Mask;
            }
 
            while ( Var_Ptr != NULL )
            {
                Lcd_Set_Address ( Var_Ptr->Variable_X_Pos, Var_Ptr->Variable_Y_Pos);
           
                if ( Var_Ptr->Var_Param_Ptr->Variable_Type == TEXT_VAR )
                {
                    if ( String_Number > STRINGS_COUNT )
                    {
                        String_Number = STRINGS_COUNT;  
                    }                
                    String_Number = Var_Ptr->Var_Param_Ptr->Variable_String_Num[*(uint8_t *)(Var_Ptr->Var_Ptr)];
                    Lcd_Write_Buffer ( &Lcd_Buffer, *(String_Number+Var_Ptr->Languages_Ptr->Languages_Tab[Language_Num]) );
 
                }
                else
                {
                    if ( Mask_Shift_Reg & ( Mask_Shift_Bit<<Var_Ptr->Variable_Index ) )
                    {
                        if ( Var_Ptr->Var_Param_Ptr->Variable_Type == DIGITS_VAR )
                        {
                            switch ( Var_Ptr->Var_Param_Ptr->Variable_Size )
                            {
                            case VAR_U8:
                                Digit_Conv = *(uint8_t*)Var_Ptr->Var_Ptr;//Bin2String ( *(uint8_t*)Var_Ptr->Var_Ptr, Variable_Tab, Var_Ptr->Var_Param_Ptr->Variable_Digits_Count, Var_Ptr->Var_Param_Ptr->Variable_Resolution, Var_Ptr->Var_Param_Ptr->Variable_Hide_Zeros );
                            break;

                            case VAR_U16:
                                Digit_Conv = *(uint16_t*)Var_Ptr->Var_Ptr;//Bin2String ( *(uint16_t*)Var_Ptr->Var_Ptr, Variable_Tab, Var_Ptr->Var_Param_Ptr->Variable_Digits_Count, Var_Ptr->Var_Param_Ptr->Variable_Resolution, Var_Ptr->Var_Param_Ptr->Variable_Hide_Zeros );
                            break;

                            case VAR_U32:
                                Digit_Conv = *(uint32_t*)Var_Ptr->Var_Ptr;//Bin2String ( *(uint32_t*)Var_Ptr->Var_Ptr, Variable_Tab, Var_Ptr->Var_Param_Ptr->Variable_Digits_Count, Var_Ptr->Var_Param_Ptr->Variable_Resolution, Var_Ptr->Var_Param_Ptr->Variable_Hide_Zeros );
                            break;
  
                            case VAR_S8:
                                Digit_Conv = *(int8_t*)Var_Ptr->Var_Ptr;//Bin2String ( *(int8_t*)Var_Ptr->Var_Ptr, Variable_Tab, Var_Ptr->Var_Param_Ptr->Variable_Digits_Count, Var_Ptr->Var_Param_Ptr->Variable_Resolution, Var_Ptr->Var_Param_Ptr->Variable_Hide_Zeros );
                            break;

                            case VAR_S16:
                                Digit_Conv = *(int16_t*)Var_Ptr->Var_Ptr;//Bin2String ( *(int16_t*)Var_Ptr->Var_Ptr, Variable_Tab, Var_Ptr->Var_Param_Ptr->Variable_Digits_Count, Var_Ptr->Var_Param_Ptr->Variable_Resolution, Var_Ptr->Var_Param_Ptr->Variable_Hide_Zeros );
                            break;

                            case VAR_S32:
                                Digit_Conv = *(int32_t*)Var_Ptr->Var_Ptr;//Bin2String ( *(int32_t*)Var_Ptr->Var_Ptr, Variable_Tab, Var_Ptr->Var_Param_Ptr->Variable_Digits_Count, Var_Ptr->Var_Param_Ptr->Variable_Resolution, Var_Ptr->Var_Param_Ptr->Variable_Hide_Zeros );
                            break;
                            }

                            Bin2String ( Digit_Conv, Variable_Tab, Var_Ptr->Var_Param_Ptr->Variable_Digits_Count, Var_Ptr->Var_Param_Ptr->Variable_Resolution, Var_Ptr->Var_Param_Ptr->Variable_Hide_Zeros );
                        }
                    
                        Lcd_Write_Buffer ( &Lcd_Buffer, Variable_Tab);
                    }
                }       

                if ( Variable_Editing_Ptr != NULL )
                {
                   String_Number = St_R;
                   Lcd_Set_Address ( (Variable_Editing_Ptr->Variable_X_Pos)-1, Variable_Editing_Ptr->Variable_Y_Pos);
                   Lcd_Write_Buffer ( &Lcd_Buffer, *(String_Number+Var_Ptr->Languages_Ptr->Languages_Tab[Language_Num]) );

                   i = Variable_Editing_Ptr->Variable_X_Pos+(Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count-1)-Count_Add;

                   if ( Variable_Editing_Ptr->Var_Param_Ptr->Variable_Resolution != 0 )
                   {
                       if ( Count_Add < Variable_Editing_Ptr->Var_Param_Ptr->Variable_Resolution )
                       {
                           i++;
                       }    
                   }
               
                   Object_Pos_Temp.Object_X_Pos = i;
                   Object_Pos_Temp.Object_Y_Pos = Variable_Editing_Ptr->Variable_Y_Pos;
                   Var_Underline = TRUE;    
                }
                else
                {
                    Var_Underline = FALSE;
                }

                Var_Ptr = Var_Ptr->Next_Variable;
            };
        }
/******************************************************************************/
        Reload_Screen = FALSE;

        Enable_Lcd_Write = TRUE;
        Lcd_Write_Strobe = FALSE;
        Lcd_Write_Ptr = 0;
    }

    if ( Enable_Lcd_Write == TRUE )
    {
        Lcd_Load_Buffer ( &Lcd_Buffer );           
    }

    if ( Var_Underline == TRUE && Enable_Lcd_Write == FALSE )
    {
        Lcd_Cmd(Lcd_Address_Base[Object_Pos_Temp.Object_Y_Pos]+Object_Pos_Temp.Object_X_Pos);    

        Lcd_Cmd ( 0x0E);  
        Var_Underline = FALSE;    
    }
}

/*******************************************************************************
* Function Name  : Bin2String
* Description    : Converting binary count to table of strings
* Input          : Source_Count - Pointer to binary count, Result - pointer to table of result, Resolution - digits after dot, Digit_Count - number of counts to convert, Zero_Blank - dont display zero at front of digit
* Output         : None
* Return         : None
*******************************************************************************/
void Bin2String ( int32_t Source_Count, uint8_t * Result, uint8_t Digits_Count, uint8_t Resolution, uint8_t Zero_Blank )
{
    static uint8_t i;
    static uint8_t Result_Table[9];   
    static uint8_t Zero_Ptr;
    static uint32_t Divider;

    Divider = 100000000;
    Zero_Ptr = 0;
    
    if ( Source_Count < 0 )
    {
        Source_Count *= (-1);
        *Result++ = '-';
    }

    if ( Digits_Count > 9 )
    {
        Digits_Count = 9;
    }

    if ( Resolution > 3 )
    {
        Digits_Count = 3;
    }

    for ( i=0; i<9; i++ )
    {
        Result_Table[i] = (uint8_t)((Source_Count / Divider) | 0x30);
        Source_Count %= Divider;
        Divider /= 10;
    }

    if ( Zero_Blank == 0 )
    {
        for ( i=(9-Digits_Count); i<9; i++ )
        {
            *Result++ = Result_Table[i];
            if ( Resolution != 0 && i == (8-Resolution) )
            {
                *Result++ = '.';
            }          
        }
    }
    else
    {
        for ( i=0; i<(8-Resolution); i++ )
        {
            if ( Result_Table[i] == '0' )
            {
//Dla spacji
                Result_Table[i] = ' ';
//koniec dla spacji
                Zero_Ptr++;
            }
            else
                break;
        }
//Dla spacji
        for ( i=(9-Digits_Count); i<9; i++ )
//Koniec dla spacji
//        for ( i=Zero_Ptr; i<9; i++ )
        {
            *Result++ = Result_Table[i];
            if ( Resolution != 0 && i == (8-Resolution) )
            {
                *Result++ = '.';
            }          
        }
    }

    *Result = 0;
}
/*******************************************************************************
* Function Name  : Menu_Keyboard
* Description    : Handling UP and DOWN keys of keyborad
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Menu_Keyboard ( void )
{

        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_UP:
                if ( (Menu_Keys.Active_String+1) == Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Total_Lines )
                {
                    Menu_Keys.First_On_Screen = 0;
                    Menu_Keys.Active_String = 0;
                    Menu_Keys.Active_On_Screen = 0;
                }
                else
                {
                    Menu_Keys.Active_String++;
                    if ( (Menu_Keys.Active_On_Screen+1) == Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Lines_On_Screen )
                    {
                        Menu_Keys.First_On_Screen++;
                    }
                    else
                    {
                        Menu_Keys.Active_On_Screen++;
                    }
                }
                Reload_Screen = TRUE;

            break;

            case KEY_DOWN:
                if ( Menu_Keys.Active_String == 0 )               {
                    Menu_Keys.First_On_Screen = Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Total_Lines - Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Lines_On_Screen;
                    Menu_Keys.Active_String = Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Total_Lines-1;
                    Menu_Keys.Active_On_Screen = Screen_Lcd[Screen_Number].Screen_Objects->Menu_Ptr->Lines_On_Screen-1;
                }
                else
                {
                    Menu_Keys.Active_String--;
                    if ( Menu_Keys.Active_On_Screen == 0 )
                    {
                        Menu_Keys.First_On_Screen--;
                    }
                    else
                    {
                        Menu_Keys.Active_On_Screen--;
                    }
                }
                Reload_Screen = TRUE;
            break;
        }

}

/*******************************************************************************
* Function Name  : Keyboard_Edit_Variable
* Description    : Keys handling for variables edition
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Keyboard_Edit_Variable ( void )
{
    volatile static int32_t Edit_Variable;
    static int32_t Edit_Variable_Min;
    static int32_t Edit_Variable_Max;

    Edit_Variable_Min = Variable_Editing_Ptr->Var_Param_Ptr->Ranges->Var_min;
    Edit_Variable_Max = Variable_Editing_Ptr->Var_Param_Ptr->Ranges->Var_max;

    switch ( Variable_Editing_Ptr->Var_Param_Ptr->Variable_Size )
    {
        case VAR_U8:
            Edit_Variable = *(uint8_t*)(Variable_Editing_Ptr->Var_Ptr);
        break;

        case VAR_U16:
            Edit_Variable = *(uint16_t*)(Variable_Editing_Ptr->Var_Ptr);
        break;
 
        case VAR_U32:
            Edit_Variable = *(uint32_t*)(Variable_Editing_Ptr->Var_Ptr);
        break;

        case VAR_S8:
            Edit_Variable = *(int8_t*)(Variable_Editing_Ptr->Var_Ptr);
        break;

        case VAR_S16:
            Edit_Variable = *(int16_t*)(Variable_Editing_Ptr->Var_Ptr);
        break;

        case VAR_S32:
            Edit_Variable = *(int32_t*)(Variable_Editing_Ptr->Var_Ptr);
        break;
    }
    switch ( Keyboard.Keyboard_Memory )
    {
        case KEY_RIGHT:
            if ( (Edit_Variable_Max - Edit_Variable) < Add_Tab[Count_Add] )
            {
                Edit_Variable = Edit_Variable_Min;
            }
            else
            {
                Edit_Variable += Add_Tab[Count_Add];
            }
            Reload_Screen = TRUE;
        break;

        case KEY_LEFT:
            if ( (Edit_Variable - Edit_Variable_Min) < Add_Tab[Count_Add] )
            {
                Edit_Variable = Edit_Variable_Max;
            }
            else
            {
                Edit_Variable -= Add_Tab[Count_Add];
            }
            Reload_Screen = TRUE;
        break;
    }

    switch ( Variable_Editing_Ptr->Var_Param_Ptr->Variable_Size )
    {
        case VAR_U8:
            *(uint8_t*)(Variable_Editing_Ptr->Var_Ptr) = Edit_Variable;
        break;

        case VAR_U16:
            *(uint16_t*)(Variable_Editing_Ptr->Var_Ptr) = Edit_Variable;
        break;

        case VAR_U32:
            *(uint32_t*)(Variable_Editing_Ptr->Var_Ptr) = Edit_Variable;
        break;

        case VAR_S8:
            *(int8_t*)(Variable_Editing_Ptr->Var_Ptr) = Edit_Variable;
        break;

        case VAR_S16:
            *(int16_t*)(Variable_Editing_Ptr->Var_Ptr) = Edit_Variable;
        break;

        case VAR_S32:
            *(int32_t*)(Variable_Editing_Ptr->Var_Ptr) = Edit_Variable;
        break;
    }

}

/*******************************************************************************
* Function Name  : Init_Gui
* Description    : General user interface initialization
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Gui_Init ( void )
{
    Read_Settings();
    Update_Lock_Counter ( Lock_Counter );
    Lock_Init();
    Reload_Screen = TRUE;
}



/*******************************************************************************
* Function Name  : First_Editable_Variable
* Description    : Setting a pointer on first editable variable in variables list
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void First_Editable_Variable ( void )
{
    Variable_Editing_Ptr = Screen_Lcd[Screen_Number].Screen_Objects->Variable_Ptr;

    while ( Variable_Editing_Ptr != NULL )
    {

        if ( Variable_Editing_Ptr->Edit_Var == TRUE )
        {
            break;
        }
        else
        {
            Variable_Editing_Ptr = Variable_Editing_Ptr->Next_Variable;
        }
    }
}
/*******************************************************************************
* Function Name  : Next_Editable_Variable
* Description    : Setting a pointer on next editable variable in variables list
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t Next_Editable_Variable ( void )
{
//    Variable_Editing_Ptr = Screen_Lcd[Screen_Number].Screen_Objects->Variable_Ptr->Next_Variable;

    Variable_Editing_Ptr = Variable_Editing_Ptr->Next_Variable;

    while ( Variable_Editing_Ptr != NULL )
    {
        if ( Variable_Editing_Ptr->Edit_Var == TRUE )
        {
            return TRUE;
        }
        else
        {
            Variable_Editing_Ptr = Variable_Editing_Ptr->Next_Variable;
        }
    }
    return FALSE;
}

/*******************************************************************************
* Function Name  : Gui
* Description    : General user interface handling - screen keys etc.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Gui ( void )
{
/**************************************************************/
/**************************************************************/
//Variables handling
    Controller_Variables.Controller_Vars.Machine_State_Var = (uint16_t)Machine_State;

/**************************************************************/
/**************************************************************/
//Encoder setting -> Machine State
//Ind_Test -> MAchine_State
    if ( Screen_Number == UST_ENKODERA )
    {
        Machine_State = HAND_POSITIONING;
    }

/**************************************************************/
/**************************************************************/
//Lock Handling function
    Lock_Handling();

/**************************************************************/
/**************************************************************/
//Keyboard handling function call
    if ( Screen_Lcd[Screen_Number].Keybord_Ptr != NULL )
    {
        (*Screen_Lcd[Screen_Number].Keybord_Ptr)();
    }
/**************************************************************/
/**************************************************************/
//One second pulse timer reload screen in selected screen
	if ( System_Timer_Check ( &Screen_Reload_Timer.Timer, T_10msec ) == TRUE )
	{
        if ( Screen_Number == UST_ENKODERA || Screen_Number == ODCZYT_SERWISOWY || Screen_Number == TEST_WEWY || Screen_Number == IND_TEST )
        {
            Reload_Screen = TRUE;
        }
		System_Timer_Setting ( &Screen_Reload_Timer.Timer, SCREEN_RELOAD_TIME, T_10msec );
	}
/**************************************************************/
/**************************************************************/
//LICENCE STRING MASK HANDLING
    if ( Service_Variables.Serv_Vars.Blokada == 0 )
    {
        String_Lic_Mask = STRING_LICENCE_LOCKED;
        Var_Lic_Mask = VAR_LICENCE_LOCKED;
    }
    else
    {
        String_Lic_Mask = STRING_LICENCE_UNLOCKED;
        Var_Lic_Mask = VAR_LICENCE_UNLOCKED; 
    }
/**************************************************************/
/**************************************************************/
//ENABLE SCREEN STRING MASK HANDLING
    Enable_String_Mask = ENABLE_STRING;

    if ( Service_Variables.Serv_Vars.Sap == 2 )
    {
        Enable_String_Mask &= ~ENABLE_STRING_SAP;    
    }

    if ( Service_Variables.Serv_Vars.W_wyjscie == 2 && Free_Exit_Marker == 2 )
    {
        Enable_String_Mask &= ~ENABLE_STRING_WW;    
    }

    if ( Service_Variables.Serv_Vars.Szybkie_wejscie == 1 && Fast_Pass_Marker == 1 )
    {
        Enable_String_Mask &= ~ENABLE_STRING_SP;    
    }

    if ( Service_Variables.Serv_Vars.Blokada == 1 )
    {
        Enable_String_Mask &= ~ENABLE_STRING_BLO;    
    }

    if ( Service_Variables.Serv_Vars.Free_Marker_1 == 2 &&
         Service_Variables.Serv_Vars.Free_Marker_2 == 2 &&
         Service_Variables.Serv_Vars.Fast_Marker_1 == 1 &&
         Service_Variables.Serv_Vars.Fast_Marker_2 == 1 )
    {
        Enable_String_Mask &= ~ENABLE_STRING_P;    
    }
/**************************************************************/
/**************************************************************/
//Manual test string mask
    Manual_Test_Mask = 0x00000100 | (uint32_t)Inputs_Level_Test;

/**************************************************************/
/**************************************************************/
/**************************************************************/
//Display on screen function

    Display_On_Screen();
/**************************************************************/
/**************************************************************/
/**************************************************************/
}

/*******************************************************************************
* Function Name  : Keyboard_standby
* Description    : Keyboard handling in STANDBY screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Standby ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ENTER:
                Screen_Number = CONTROLLER_ON;
                Reload_Screen = TRUE;         
            break; 
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Enable
* Description    : Keyboard handling in ENABLE screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Enable ( void )
{

    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                   Screen_Number = STANDBY;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                    Menu_Keys.First_On_Screen = 0;
                    Menu_Keys.Active_String = 0;
                    Menu_Keys.Active_On_Screen = 0;
                    Screen_Number = MENU;
                    Reload_Screen = TRUE;
            break;

            default:
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Menu
* Description    : Keyboard handling in MENU screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Menu ( void )
{
    _Screen_Number Next_Screen[] = {
        PARAM_STER,
        UST_STER,
        FUNCTIONS1,
        WYB_BRAM,
        DATA_I_CZAS,
        LICENCJA,
        TEST,
        HASLO,
        INFO,
    };

    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                Variable_Editing_Ptr = NULL;
                Screen_Number = CONTROLLER_ON;
                Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                Menu_Backup_Keys = Menu_Keys;
                Screen_Number = Next_Screen[Menu_Keys.Active_String];
 
                if ( Screen_Number == DATA_I_CZAS )
                {
                    Time_Set = Time;
                    Date_Set = Date;
                    Count_Add = 0;
                    First_Editable_Variable();
                }
    
                if ( Screen_Number == FUNCTIONS1 || Screen_Number == WYB_BRAM )
                {
                    Count_Add = 0;
                    First_Editable_Variable();
                }
            
                if ( Screen_Number == PARAM_STER || Screen_Number == UST_STER || Screen_Number == TEST )
                {
                    Menu_Keys.First_On_Screen = 0;
                    Menu_Keys.Active_String = 0;
                    Menu_Keys.Active_On_Screen = 0;    
                }
            
                if ( Screen_Number == HASLO )
                {
                   Count_Add = 0;
                   First_Editable_Variable();
                   Screen_Before_Password = MENU;
                   Screen_After_Password = UST_FABRYCZNE;
                }
            
                Reload_Screen = TRUE;       
            break;
            
            default:
                Menu_Keyboard();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Date
* Description    : Setting the date and time
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Date ( void )
{

    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Variable_Editing_Ptr = NULL;
                    Write_Date ( &Date_Set );
                    Update_Time ( &Time_Set );
                    Menu_Keys = Menu_Backup_Keys;
                    Screen_Number = MENU;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
/*
                   Count_Add = 0;
                   if ( Next_Editable_Variable() == FALSE )
                   {
                        First_Editable_Variable();
                   }
                   Reload_Screen = TRUE;
*/
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Param
* Description    : Parameters screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Param ( void )
{
    _Screen_Number Next_Param_Screen[] = {
        HASLO,
        PARAM_STER1,
        HASLO,
        OSWIETLENIE,
    };

    _Screen_Number After_Pass_Screen[] = {
        PARAM_MECH1,
        PARAM_STER1,
        HEAT_COOL1,
        OSWIETLENIE,
    };

    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys;
                    Screen_Number = MENU;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                   Menu_Backup_Keys_Set = Menu_Keys;
                   Screen_Number = Next_Param_Screen[Menu_Keys.Active_String];
                   Screen_Before_Password = PARAM_STER;
                   Screen_After_Password = After_Pass_Screen[Menu_Keys.Active_String];
                   Count_Add = 0;
                   First_Editable_Variable();
                   Reload_Screen = TRUE;
            break;

            default:
                   Menu_Keyboard();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Ust
* Description    : Settings screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Set ( void )
{
    _Screen_Number Next_Set_Screen[] = {
        WYB_JEZ,
        HASLO,
        HASLO,
        HASLO, 
        HASLO,    
    };

    _Screen_Number After_Pass_Screen[] = {
        WYB_JEZ,
        HAS_MENU,
        HAS_BLOK,
        KAL_RTC, 
        ZER_LICZNIKI,
    };



    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys;
                    Screen_Number = MENU;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                   Menu_Backup_Keys_Set = Menu_Keys;
                   Screen_Number = Next_Set_Screen[Menu_Keys.Active_String];
                   Screen_Before_Password = UST_STER;
                   Screen_After_Password = After_Pass_Screen[Menu_Keys.Active_String];
                   Reload_Screen = TRUE;
                   Count_Add = 0;
                   First_Editable_Variable();
            
                    if ( Screen_After_Password == HAS_BLOK )
                    {
                        Service_Variables_Set.Serv_Vars.Blokada_rok = Date.Date_Vars.Year;
                        Service_Variables_Set.Serv_Vars.Blokada_miesiac = Date.Date_Vars.Month;
                        Service_Variables_Set.Serv_Vars.Blokada_dzien = Date.Date_Vars.Day;
                    }
            break;

            default:
                   Menu_Keyboard();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}


/*******************************************************************************
* Function Name  : Keyboard_Functions1
* Description    : Parameters screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Functions1 ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Machine_State = STANDBY;
                    Write_Settings();
                    Variable_Editing_Ptr = NULL;
                    Screen_Number = MENU;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                   if ( ++Screen_Number == PROG_TESTOWY )
                   {
                       Screen_Number = FUNCTIONS1;
                   }
                   First_Editable_Variable();
                   Reload_Screen = TRUE;
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}


/*******************************************************************************
* Function Name  : Keyboard_Gate
* Description    : Gate selection screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Gate ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika1 = 30;
                    Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika2 = 20;
                    Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika3 = 30;
                    Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika4 = 20;
                    Service_Variables_Set.Serv_Vars.KOlowrotek = 0;
                
                    if ( Service_Variables_Set.Serv_Vars.Typ_Bramki == 0 )
                    {
                        Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika1 = 40;
                        Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika2 = 33;
                        Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika3 = 45;
                        Controlling_Variables_Set.Ctrl_Vars.Predkosc_Silnika4 = 38;                  
                    }
          
                    if ( Service_Variables_Set.Serv_Vars.Typ_Bramki == 3 )
                    {                   
                        Service_Variables_Set.Serv_Vars.KOlowrotek = 1;
                    }
                
                    Write_Settings();
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys;
                    Stop_Angles_Set();  
                    Machine_State = STANDBY;
                    Screen_Number = MENU;
                    Reload_Screen = TRUE;
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Lic
* Description    : Licence screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Lic ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys;
                    Screen_Number = MENU;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                    if ( Service_Variables.Serv_Vars.Blokada == 0 )
                    {
                        Screen_Number = ODBLOKUJ;
                        Count_Add = 0;
                        Screen_After_Password = CONTROLLER_ON;
                        First_Editable_Variable();
                        Reload_Screen = TRUE;
                    }
            break;

            default:
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Test
* Description    : Test screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Test ( void )
{
    _Screen_Number Next_Param_Screen[] = {
        PROG_TESTOWY,
        HASLO,
        HASLO,
        HASLO,
        ODCZYT_SERWISOWY,
        IND_TEST
    };

    _Screen_Number After_Pass_Screen[] = {
        
        PROG_TESTOWY,
        TEST_WEWY,
        UST_ENKODERA,
        UST_SERWISOWE1,
        ODCZYT_SERWISOWY,
        IND_TEST,
    };

    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys;
                    Screen_Number = MENU;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                    Menu_Backup_Keys_Set = Menu_Keys;
                    Screen_Number = Next_Param_Screen[Menu_Keys.Active_String];
                    Screen_Before_Password = TEST;
                    Screen_After_Password = After_Pass_Screen[Menu_Keys.Active_String];            Reload_Screen = TRUE;
                    Count_Add = 0;
                    First_Editable_Variable();
            
                    if ( Screen_After_Password == PROG_TESTOWY )
                    {
                        Counters_Variables.Counters_Vars.No_Goin_Test = 0;
                        Counters_Variables.Counters_Vars.No_Goout_Test = 0;
                        Test_String_Mask = TEST_STOPPED;
                    }
                
                    if ( Screen_After_Password == TEST_WEWY )
                    {
                        Controller_Variables.Controller_Vars.Test_Status = 0;
                        Machine_State = MANUAL_TEST;
                    }
                
                    if ( Screen_After_Password == IND_TEST )
                    {
                        Ind_Test_Mask = IND_MASK_LINES;
                        Machine_State = IND_STOP;
                    }               
            break;

            default:
                    Menu_Keyboard();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Haslo
* Description    : Password screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Haslo ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = Screen_Before_Password;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                   if ( Service_Variables.Serv_Vars.Haslo_Dostepu == Password_Set || Password_Set == SERVICE_PASSWORD )
                   {
                        Password_Set = 0;
                        Count_Add = 0;
                        Service_Variables_Set.Serv_Vars.Blok_Dostep_Pozostalo = Service_Variables_Set.Serv_Vars.Ilosc_Prob_Dostepu;                  
                        Screen_Number = Screen_After_Password;
                        First_Editable_Variable();                        
                   }
                   else
                   {
                        Password_Set = 0;
                        Count_Add = 0; 
               
                        if ( --Service_Variables_Set.Serv_Vars.Blok_Dostep_Pozostalo == 0 )
                        {
                            Service_Variables_Set.Serv_Vars.Marker_Blokady = STER_BLOKADA_CALKOWITA;
                            Screen_Number = BLOK_CALK;
                        }
                   }
                   Write_Settings;   
                   Reload_Screen = TRUE;
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Language
* Description    : Select Language screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Language ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Write_Settings();
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = UST_STER;
                    Reload_Screen = TRUE;
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Param_Mech
* Description    : Parameters setting
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Param_Mech ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    if ( Service_Variables_Set.Serv_Vars.Sensor_Type == 0 )
                    {
                        Service_Variables_Set.Serv_Vars.Serwis_005 = 15;
                    }
                    else if ( Service_Variables_Set.Serv_Vars.Sensor_Type == 1 )
                    {
                        Service_Variables_Set.Serv_Vars.Serwis_005 = 30;
                    }
    
                    Write_Settings();
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = Screen_Before_Password;;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                   if ( ++Screen_Number == PARAM_STER1 )
                   {
                       Screen_Number = PARAM_MECH1;
                   }
                   First_Editable_Variable();
                   Reload_Screen = TRUE;
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Param_Ster
* Description    : Parameters setting
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Param_Ster ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Write_Settings();
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = Screen_Before_Password;;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                   if ( ++Screen_Number == HEAT_COOL1 )
                   {
                       Screen_Number = PARAM_STER1;
                   }
                   First_Editable_Variable();
                Reload_Screen = TRUE;
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Heat_Cool
* Description    : Heat and cool setting
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Heat_Cool ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Write_Settings();
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = Screen_Before_Password;;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                /*
                    if ( ++Screen_Number == FUNCTIONS1 )
                    {   
                        Screen_Number = PARAM_STER1;
                    }
                    First_Editable_Variable();
                    Reload_Screen = TRUE;
                */
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}


/*******************************************************************************
* Function Name  : Keyboard_Lights
* Description    : Lights setting
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Lights ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Write_Settings();
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = Screen_Before_Password;;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                /*
                    if ( ++Screen_Number == FUNCTIONS1 )
                    {   
                        Screen_Number = PARAM_STER1;
                    }
                    First_Editable_Variable();
                    Reload_Screen = TRUE;
                */
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}



/*******************************************************************************
* Function Name  : Keyboard_Passwd
* Description    : Select Passwd screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Passwd ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Write_Settings();
            
                    Service_Variables.Serv_Vars.Blok_Dostep_Pozostalo = Service_Variables.Serv_Vars.Ilosc_Prob_Dostepu;
                    Service_Variables.Serv_Vars.Blok_Prog_Pozostalo = Service_Variables.Serv_Vars.Ilosc_Prob_Blokady;
            
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = UST_STER;
                    Reload_Screen = TRUE;
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}


/*******************************************************************************
* Function Name  : Keyboard_Lock
* Description    : Select Passwd screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Lock ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Lock_Date_Now_Tmp = Date_2_Days ( Date.Date_Vars.Year, Date.Date_Vars.Month, Date.Date_Vars.Day );
                    Lock_Date_Set_Tmp = Date_2_Days ( Service_Variables_Set.Serv_Vars.Blokada_rok, Service_Variables_Set.Serv_Vars.Blokada_miesiac, Service_Variables_Set.Serv_Vars.Blokada_dzien );
                    Lock_Interval_Tmp = Lock_Date_Set_Tmp - Lock_Date_Now_Tmp;; 

                    if ( Lock_Date_Set_Tmp < Lock_Date_Now_Tmp )
                    {
                        Service_Variables_Set.Serv_Vars.Marker_Blokady = STER_ODBLOKOWANY;
                        Service_Variables_Set.Serv_Vars.Blokada = 1;
                    }
                    else
                    {
                        Service_Variables_Set.Serv_Vars.Lock_Counter = 0;
                        Service_Variables_Set.Serv_Vars.Lock_Interval = Lock_Interval_Tmp;
                        Service_Variables_Set.Serv_Vars.Marker_Blokady = STER_BLOKOWANY;
                    }
                
                    Write_Settings();
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = UST_STER;
                    Reload_Screen = TRUE;
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_RTC
* Description    : Select RTC screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_RTC ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Write_Settings();
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = UST_STER;
                    Reload_Screen = TRUE;
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}





/*******************************************************************************
* Function Name  : Keyboard_Prog_Test
* Description    : Prog_Teast screen habdling
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Prog_Test ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Machine_State = STANDBY;
                    TEST_ENABLE = FALSE;
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = Screen_Before_Password;;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                    if ( TEST_ENABLE == TRUE )
                    {
                        Test_String_Mask = TEST_STOPPED;
                        Machine_State = STANDBY;
                        TEST_ENABLE = FALSE;
                    }
                    else
                    {
                        Test_String_Mask = TEST_STARTED;
                        Test_Cntr = 0;
                        Machine_State = TEST_START;
                        TEST_ENABLE = TRUE;
                    }
                    Reload_Screen = TRUE;
            break;

           default:
           break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}


/*******************************************************************************
* Function Name  : Keyboard_Ust_Enk
* Description    : Prog_Teast screen habdling
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Ust_Enk ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Variable_Editing_Ptr = NULL;
                    Controlling_Variables_Set.Ctrl_Vars.Encoder = Controller_Variables.Controller_Vars.Encoder_Var;
                    Write_Settings();
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = Screen_Before_Password;
                    Reload_Screen = TRUE;
                    Machine_State = STANDBY;
            break;

            default:
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Ster_Blok
* Description    : Handling keys in STER_BLOK screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Ster_Blok ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ENTER:
                   Screen_Number = ODBLOKUJ;
                   Count_Add = 0;
                   First_Editable_Variable();
                   Reload_Screen = TRUE;
            break;

            default:
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : KKeyboard_Odblokuj
* Description    : Handling keys in ODBLOKUJ screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Odblokuj ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    if ( Service_Variables.Serv_Vars.Marker_Blokady == STER_BLOKOWANY || Service_Variables.Serv_Vars.Marker_Blokady == STER_ODBLOKOWANY )
                    {
                        Screen_Number = LICENCJA;
                    }
                    else if ( Service_Variables.Serv_Vars.Marker_Blokady == STER_ZABLOKOWANY )
                    {
                        Screen_Number = STER_BLOK;
                    }
                    Variable_Editing_Ptr = NULL;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                   if ( Service_Variables.Serv_Vars.Haslo_Blokady == Password_Set || Password_Set == SERVICE_PASSWORD )
                   {
                        Password_Set = 0;
                        Machine_State = STANDBY;
                        Service_Variables_Set.Serv_Vars.Blok_Prog_Pozostalo = Service_Variables_Set.Serv_Vars.Ilosc_Prob_Blokady;                  
                        Service_Variables_Set.Serv_Vars.Marker_Blokady = STER_ODBLOKOWANY;
                        Service_Variables_Set.Serv_Vars.Blokada = 1;
                        Variable_Editing_Ptr = NULL;                   
                        Screen_Number = Screen_After_Password;
                   }
                   else
                   {
                        Password_Set = 0;
                        Count_Add = 0; 
               
                        if ( --Service_Variables_Set.Serv_Vars.Blok_Prog_Pozostalo == 0 )
                        {
                            Service_Variables_Set.Serv_Vars.Marker_Blokady = STER_BLOKADA_CALKOWITA;
                            Screen_Number = BLOK_CALK;
                        }
                   }
               
                   Write_Settings();  
                   Reload_Screen = TRUE;
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Fabryczne
* Description    : Handling keys in UST_ FABRYCZNE screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Fabryczne ( void )
{
    uint32_t i;
    uint32_t NoOfWords;

    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys;
                    Screen_Number = MENU;
                    Reload_Screen = TRUE;
            break;

            case KEY_ENTER:
                    NoOfWords = (sizeof ( Service_Variables.Service_Tab )>>2);

                    for ( i = 0; i < NoOfWords; i++ )
                    {
                        Service_Variables.Service_Tab[i] = Settings_Default[i];
                    }
    
                    NoOfWords = (sizeof ( Controlling_Variables.Variables_Tab )>>2);
   
                    for ( i = 0; i < NoOfWords; i++ )
                    {
                        Controlling_Variables.Variables_Tab[i] = Controlling_Default[i];
                    } 
                    Service_Variables_Set = Service_Variables;
                    Controlling_Variables_Set = Controlling_Variables;
                    Write_Settings();

                    if ( Service_Variables.Serv_Vars.Jezyk > NUM_OF_LANG )
                    {
                        Language_Num = 0;
                    }
                    else
                    {
                        Language_Num = Service_Variables.Serv_Vars.Jezyk;
                    }  

                    Stop_Angles_Set();  
                    Screen_Number = CONTROLLER_ON;
                    Machine_State = STANDBY;
                    Reload_Screen = TRUE;
            break;

            default:
                    Menu_Keyboard();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Zerowanie
* Description    : Handling keys in ZEROWANIE_LICZNIKOW screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Zerowanie ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Write_Settings();
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = UST_STER;
                    Reload_Screen = TRUE;
            break;
            
            case KEY_ENTER:
                   Counters_Variables.Counters_Vars.No_Goin = 0;
                   Counters_Variables.Counters_Vars.No_Goout = 0;
                   Update_Counters();
                   Screen_Number = CONTROLLER_ON;
                   Reload_Screen = TRUE;
            break;

            default:
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}




/*******************************************************************************
* Function Name  : Keyboard_Ust_Serwis
* Description    : Parameters setting
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Ust_Serwis ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Variable_Editing_Ptr = NULL;
                    Write_Settings();
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = Screen_Before_Password;
                    Reload_Screen = TRUE;
                    Machine_State = STANDBY;
            break;

            case KEY_ENTER:
                   if ( ++Screen_Number == ODCZYT_SERWISOWY )
                   {
                       Screen_Number = UST_SERWISOWE1;
                   }
                   First_Editable_Variable();
                   Reload_Screen = TRUE;
            break;

            case KEY_DOWN:
              if ( ++Count_Add == Variable_Editing_Ptr->Var_Param_Ptr->Variable_Digits_Count )
              {
                  Count_Add = 0;
              }
              Reload_Screen = TRUE;  
            break;
          
            case KEY_UP:
              Count_Add = 0;
              if ( Next_Editable_Variable() == FALSE )
              {
                  First_Editable_Variable();
              }
              Reload_Screen = TRUE;
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}



/*******************************************************************************
* Function Name  : Keyboard_Odczyt_Serwis
* Description    : Parameters setting
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Odczyt_Serwis ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = Screen_Before_Password;
                    Reload_Screen = TRUE;
            break;

            default:
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Ind_Test
* Description    : Inductive sensor testing
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Ind_Test ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Machine_State = STANDBY;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = Screen_Before_Password;
                    Reload_Screen = TRUE;
            break;
            
            case KEY_ENTER:
                    Controller_Variables.Controller_Vars.Sensor_Counter = IND_OFFSET;
                    Machine_State = IND_START;
                    Ind_Test_Mask = IND_MASK_TEST;
                    Reload_Screen = TRUE;
            break;         

            default:
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}
/*******************************************************************************
* Function Name  : Keyboard_Test_WeWy
* Description    : Inputs outputs testing
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Test_WeWy ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                if ( Controller_Variables.Controller_Vars.Test_Marker != GO_2_TEST_PROGRAM )
                {
                    Machine_State = STANDBY;
                    Menu_Keys = Menu_Backup_Keys_Set;
                    Screen_Number = Screen_Before_Password;
                    Reload_Screen = TRUE;
                }
            break;

            default:
                Keyboard_Edit_Variable();
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}

/*******************************************************************************
* Function Name  : Keyboard_Info
* Description    : Info screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Keyboard_Info ( void )
{
    if ( Keyboard.Keyboard_Memory != 0 && Keyboard.Keyboard_Flag == 1 )
    {
        switch ( Keyboard.Keyboard_Memory )
        {
            case KEY_ESCAPE:
                    Variable_Editing_Ptr = NULL;
                    Menu_Keys = Menu_Backup_Keys;
                    Screen_Number = MENU;
                    Reload_Screen = TRUE;
            break;

            default:
            break;
        }
        Keyboard.Keyboard_Flag = 0;
    }

    if ( Keyboard.Keyboard_State == 0 )
    {
        Keyboard.Keyboard_Memory = 0;
    }
}
/*******************************************************************************
* Function Name  : Write_Settings
* Description    : Load_variables to flash/eeprom memory
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Write_Settings ( void )
{
    uint32_t i;
    uint32_t NoOfWords;

    Flash_Address = EEPROM_BASE_ADDRESS;

    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
    FLASH_ErasePage(Flash_Address);

    NoOfWords = (sizeof ( Service_Variables_Set.Service_Tab )>>2);

    for ( i = 0; i < NoOfWords; i++ )
    {
        FLASH_ProgramWord(Flash_Address, Service_Variables_Set.Service_Tab[i] );
        Flash_Address+=4;    
    }

    Flash_Address = EEPROM_BASE_ADDRESS + sizeof ( Service_Variables.Service_Tab );
    NoOfWords = (sizeof ( Controlling_Variables_Set.Variables_Tab ) >> 2);

    for ( i = 0; i < NoOfWords; i++ )
    {
        FLASH_ProgramWord(Flash_Address, Controlling_Variables_Set.Variables_Tab[i] );
        Flash_Address += 4;
    }

    Controlling_Variables = Controlling_Variables_Set;
    Service_Variables = Service_Variables_Set;

    Language_Num = Service_Variables.Serv_Vars.Jezyk;

    Stop_Angles_Set();
}

/*******************************************************************************
* Function Name  : Read_Settings
* Description    : Read variables from flash/eeprom memory
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Read_Settings ( void )
{
    uint32_t i;
    uint32_t NoOfWords;

    Flash_Address = EEPROM_BASE_ADDRESS;
    NoOfWords = (sizeof ( Service_Variables.Service_Tab )>>2);

    for ( i = 0; i < NoOfWords; i++ )
    {
        Service_Variables.Service_Tab[i] = *(uint32_t*)Flash_Address;
        Flash_Address += 4;
    }

    Flash_Address = EEPROM_BASE_ADDRESS + sizeof ( Service_Variables.Service_Tab );
    NoOfWords = (sizeof ( Controlling_Variables.Variables_Tab ) >> 2);

    for ( i = 0; i < NoOfWords; i++ )
    {
        Controlling_Variables.Variables_Tab[i] = *(uint32_t*)Flash_Address;
        Flash_Address += 4;
    }

//test on/off
    if ( Service_Variables.Serv_Vars.First_On_Marker != CONTROLLER_PROGRAMMED )
    {
        Controller_Variables.Controller_Vars.Test_Marker = 0;//GO_2_TEST_PROGRAM;
        BKP_WriteBackupRegister(BKP_DR13, Controller_Variables.Controller_Vars.Test_Marker);

        NoOfWords = (sizeof ( Service_Variables.Service_Tab )>>2);
   
        for ( i = 0; i < NoOfWords; i++ )
        {
            Service_Variables.Service_Tab[i] = Settings_Default[i];
        }
    
        NoOfWords = (sizeof ( Controlling_Variables.Variables_Tab )>>2);
   
        for ( i = 0; i < NoOfWords; i++ )
        {
            Controlling_Variables.Variables_Tab[i] = Controlling_Default[i];
        } 
        Service_Variables_Set = Service_Variables;
        Controlling_Variables_Set = Controlling_Variables;
        Write_Settings();
        BKP_WriteBackupRegister(BKP_DR14, 0x0000);
    }

    Counters_Variables.Counters_Vars.No_Goin = 0;
    Counters_Variables.Counters_Vars.No_Goout = 0;
    Counters_Variables.Counters_Vars.No_Goin = BKP_ReadBackupRegister(BKP_DR5);
    Counters_Variables.Counters_Vars.No_Goin <<= 16;
    Counters_Variables.Counters_Vars.No_Goin += BKP_ReadBackupRegister(BKP_DR6);
    Counters_Variables.Counters_Vars.No_Goout = BKP_ReadBackupRegister(BKP_DR7);
    Counters_Variables.Counters_Vars.No_Goout <<= 16;
    Counters_Variables.Counters_Vars.No_Goout += BKP_ReadBackupRegister(BKP_DR8);

    Controller_Variables.Controller_Vars.Software_Version[0] = 1;//(BKP_ReadBackupRegister(BKP_DR9) & 0xFF00) >> 8;
    Controller_Variables.Controller_Vars.Software_Version[1] = 8;//BKP_ReadBackupRegister(BKP_DR9) & 0x00FF;
    Controller_Variables.Controller_Vars.Software_Version[2] = 2;//(BKP_ReadBackupRegister(BKP_DR10) & 0xFF00) >> 8;
    Controller_Variables.Controller_Vars.Controller_Date[0] = BKP_ReadBackupRegister(BKP_DR10) & 0x00FF;
    Controller_Variables.Controller_Vars.Controller_Date[1] =(BKP_ReadBackupRegister(BKP_DR11) & 0xFF00) >> 8;
    Controller_Variables.Controller_Vars.Controller_Date[2] = BKP_ReadBackupRegister(BKP_DR11) & 0x00FF;
    Controller_Variables.Controller_Vars.Serial_Number = BKP_ReadBackupRegister(BKP_DR12);
    Controller_Variables.Controller_Vars.Test_Marker = BKP_ReadBackupRegister(BKP_DR13);

    if ( Service_Variables.Serv_Vars.Jezyk > NUM_OF_LANG )
    {
        Language_Num = 0;
    }
    else
    {
        Language_Num = Service_Variables.Serv_Vars.Jezyk;
    }  

    Controlling_Variables_Set = Controlling_Variables;
    Service_Variables_Set = Service_Variables;

    Stop_Angles_Set();
}

/*******************************************************************************
* Function Name  : Update_Counters
* Description    : Update gtate counters in backup registers
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Update_Counters ( void )
{
    uint32_t Bkp_Tmp;

    Bkp_Tmp = (Counters_Variables.Counters_Vars.No_Goin >> 16);
    BKP_WriteBackupRegister(BKP_DR5, Bkp_Tmp);
    Bkp_Tmp = Counters_Variables.Counters_Vars.No_Goin;
    BKP_WriteBackupRegister(BKP_DR6, Bkp_Tmp);
    Bkp_Tmp = (Counters_Variables.Counters_Vars.No_Goout >> 16);
    BKP_WriteBackupRegister(BKP_DR7, Bkp_Tmp);
    Bkp_Tmp = Counters_Variables.Counters_Vars.No_Goout;
    BKP_WriteBackupRegister(BKP_DR8, Bkp_Tmp);
}

/*******************************************************************************
* Function Name  : Update_Lock_Counter
* Description    : Updates lock counters in flash memory during software lock function is enabled
* Input          : Count to update
* Output         : None
* Return         : None
*******************************************************************************/
void Update_Lock_Counter ( uint16_t Update_Counter )
{
    if ( Service_Variables.Serv_Vars.Marker_Blokady == STER_BLOKOWANY )
    {
        Service_Variables_Set.Serv_Vars.Lock_Counter += Update_Counter;
        Write_Settings();
    }
}

/*******************************************************************************
* Function Name  : Lock_Init
* Description    : Lock initialization
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Lock_Init ( void )
{
    if ( Service_Variables.Serv_Vars.Marker_Blokady == STER_ODBLOKOWANY )
    {
        if ( Controller_Variables.Controller_Vars.Test_Marker == GO_2_TEST_PROGRAM )
        {
            Screen_Number = TEST_WEWY;
            Count_Add = 0;
            First_Editable_Variable();        
        }
        else
        {
            Screen_Number = STANDBY_SCR;
        }
    }
    else if ( Service_Variables.Serv_Vars.Marker_Blokady == STER_BLOKOWANY )
    {
        if ( Controller_Variables.Controller_Vars.Test_Marker == GO_2_TEST_PROGRAM )
        {
            Screen_Number = TEST_WEWY; 
            Count_Add = 0;
            First_Editable_Variable();
        }
        else
        {
            Screen_Number = STANDBY_SCR;
        }
    }
    else if ( Service_Variables.Serv_Vars.Marker_Blokady == STER_ZABLOKOWANY )
    {
        Screen_Number = STER_BLOK;  
    }
    else if ( Service_Variables.Serv_Vars.Marker_Blokady == STER_BLOKADA_CALKOWITA )
    {
        Screen_Number = BLOK_CALK;
    }
}

/*******************************************************************************
* Function Name  : Lock_Handling
* Description    : Lock handling
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Lock_Handling ( void )
{
    if ( Service_Variables.Serv_Vars.Marker_Blokady == STER_BLOKOWANY )
    {
        if ( Service_Variables.Serv_Vars.Lock_Counter > Service_Variables.Serv_Vars.Lock_Interval )
        {
            Screen_Number = STER_BLOK;
            Machine_State = SW_LOCKED;
            Screen_After_Password = CONTROLLER_ON;
            Service_Variables_Set.Serv_Vars.Marker_Blokady = STER_ZABLOKOWANY;
            Write_Settings();
        }
    }
}
/*******************************************************************************
* End file main.c
*******************************************************************************/









