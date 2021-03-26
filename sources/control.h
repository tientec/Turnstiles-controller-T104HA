#ifndef _CONTROL_H
#define _CONTROL_H

/*******************************************************************************
********************************************************************************
* File Name          : control.h
* Author             : Tomasz Nowak / GASTOP
* Version            : V1.00a
* Date               : 04/08/2011
* Description        : control.c header file
********************************************************************************
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/


/*******************************************************************************
* Exported Preprocessor #define Constants
*******************************************************************************/
#define	ENC_OFFSET			1000
#define ENC_MAX             1023

#define	FORWARD_SHIFT	20  //S001ok
#define BACKWORDS_SHIFT	20  //S002ok

#define NO_HYSTERESIS   0   //S003ok
#define HYSTERESIS      20  //S004ok


#define ARM_SAMPLE_TIME 15  //S005ok
#define WINDOW_OFFSET	5   //S006ok

#define TEST_CNT_FWD	10  //S007ok
#define TEST_CNT_BCK	20  //S008ok

/*Slowing the motor speed during moving*/
#define	SLOW_SHIFT		130 //S009ok

#define POS_RANGE		10  //S010ok

/* BR2 8 / bez zwory 30 */
/* BR3/4 10 */

/*Count (in msec) defines how long the controoler is in the STANDBY mode after which it goes to STANDBY_PWM mode*/
#define STANDBY_TIME	10  //S011ok
#define ZWORA_TIME      75  //S012ok

#define FORCING_10_SEC	1000//S013ok

/*******************************************************************************
* Exported Preprocessor #define MACROS
*******************************************************************************/


/*******************************************************************************
* Exported Type Declarations
*******************************************************************************/
typedef enum {
/*0*/	STANDBY = 0,
/*1*/	STANDBY_PWM,
/*2*/	RESET_COUNTERS,

/*3*/	ZWORA_FWD,
/*4*/	ZWORA_BCK,

/*5*/   STANDBY_ZWORA_FWD,
/*6*/	STANDBY_ZWORA_BCK,

/*7*/   FREE_ENTER_FWD,
/*8*/	FREE_ENTER_BCK,

/*9*/   WAIT_FORWARD,
/*10*/	MOVE_FORWARD,
/*11*/	WAIT_BACK,
/*12*/	MOVE_BACK,
    
/*13*/  ARM_STOP_FWD,
/*14*/  ARM_STOP_BCK,

/*15*/  TEST_START,
/*16*/	TEST_FWD,
/*17*/	TEST_LOOP_F,
/*18*/	TEST_BCK,
/*19*/	TEST_LOOP_B,
/*20*/	TEST_RES,
/*21*/  TEST_BOLTS,

/*22*/  FWD_SIGNAL,
/*23*/	BCK_SIGNAL,

/*24*/  HAND_POSITIONING,

/*25*/  SELF_TEST,

/*26*/  FORCING,
/*27*/	WAIT_FOR_FWD,//
/*28*/	MOVE_FOR_FWD,
/*29*/	WAIT_FOR_BCK,
/*30*/	MOVE_FOR_BCK,

/*31*/  SAP_ALERT,
/*32*/  SAP_ALERT_FWD,
/*33*/  SAP_ALERT_BCK,

/*34*/  SW_LOCKED,

/*35*/ STANDBY_NOBRAKE,
/*36*/ FWD_SIG_NOBRAKE,
/*37*/ BCK_SIG_NOBRAKE,
/*38*/ STANDBY_WAIT_NB,
/*39*/ GO_FORWARD,
/*40*/ GO_BACK,
/*41*/ MANUAL_TEST,

/*42*/ IND_STOP,
/*43*/ IND_START,
/*44*/ IND_FWD,
/*45*/ IND_STOP_FWD,
/*46*/ IND_BCK,
/*47*/ IND_STOP_BCK,

/*48*/ FREE_EXIT_BOTH,
} _Machine_State;

typedef enum {
	Angle_1 = 0,
	Angle_2,
	Angle_3,
	Angle_4,
	Unknown,
} _Encoder_Angle;

typedef struct {
	uint16_t Min_Pos1;
	uint16_t Max_Pos1;
	uint16_t Min_Pos2;
	uint16_t Max_Pos2;
	uint16_t Min_Pos3;
	uint16_t Max_Pos3;
	uint16_t Min_Pos4;
	uint16_t Max_Pos4;
} _Arms_Stop_Pos;
/*******************************************************************************
* Exported Const Object Declarations
*******************************************************************************/


/*******************************************************************************
* Exported Object Declarations
*******************************************************************************/
extern volatile  _Machine_State Machine_State;
extern volatile _Encoder_Angle Encoder_Angle;
extern volatile _Encoder_Angle Actual_Angle;
extern volatile _Arms_Stop_Pos Arms_Stop_Pos;

extern volatile uint16_t Arm_Window_Pos;

extern uint8_t Test_Cntr;
extern uint8_t Ind_Test_Marker;

extern uint16_t Persons_Counter_Fwd;
extern uint16_t Persons_Counter_Bck;

extern bool STANDBY_PWM_WAIT;
/*******************************************************************************
* Exported Function Prototypes
*******************************************************************************/
void Control ( void );
uint8_t Position_Check ( uint16_t Histeresis );

/*******************************************************************************
 * Exported Inline Function Definitions and #define Function-Like Macros
*******************************************************************************/


/*******************************************************************************
* End fo file control.h
*******************************************************************************/
#endif /*_CONTROL_H*/



