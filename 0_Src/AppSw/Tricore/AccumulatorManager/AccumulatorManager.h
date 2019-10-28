/*
 * AccumulatorManager.h
 *
 *  Created on: 2019. 10. 18.
 *      Author: Dua
 */

#ifndef SRC_APPSW_TRICORE_ACCUMULATORMANAGER_ACCUMULATORMANAGER_H_
#define SRC_APPSW_TRICORE_ACCUMULATORMANAGER_ACCUMULATORMANAGER_H_

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "HLD.h"
#include "Configuration.h"
#include "AdcSensor.h"
#include "VoltageSensing.h"
#include "CurrentSensing.h"
#include "TemperatureSensing.h"
#include "CanCommunication.h"


/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define AMS_CAN_MSG_0	0x10010001UL
#define AMS_CAN_MSG_1	0x10010010UL


#define AMS_V0_IN   HLD_Vadc_AN11_G0CH11_X103_39

// #define AMS_C0_IN   HLD_Vadc_AN17_G1CH5_X103_37
//FIXME: AN17 input does not operate.
#define AMS_C0_IN   HLD_Vadc_AN20_G1CH8_X103_36
#define AMS_C1_IN   HLD_Vadc_AN21_G1CH9_X103_35

#define AMS_T0_IN	HLD_Vadc_AN10_G0CH10_X102_5
#define AMS_T1_IN	HLD_Vadc_AN9_G0CH9_X102_6
#define AMS_T2_IN	HLD_Vadc_AN16_G1CH4_X102_7
#define AMS_T3_IN	HLD_Vadc_AN15_G1CH3_X102_8
#define AMS_T4_IN	HLD_Vadc_AN3_G0CH3_X102_9
#define AMS_T5_IN	HLD_Vadc_AN2_G0CH2_X102_10
#define AMS_T6_IN	HLD_Vadc_AN1_G0CH1_X102_11
#define AMS_T7_IN	HLD_Vadc_AN0_G0CH0_X102_12

#define AMS_B0_IN		ACCUMULATORMANAGER_GPIO0
#define AMS_B1_IN		ACCUMULATORMANAGER_GPIO1
#define	AMS_TSAL_IN		ACCUMULATORMANAGER_GPIO2
#define	AMS_IND_IN		ACCUMULATORMANAGER_GPIO3
#define AMS_BMSF_OUT	ACCUMULATORMANAGER_GPIO4


/******************************************************************************/
/*------------------------------Type Definitions------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/
typedef enum
{
	Accumulator_Bms_Status_cutOff	= 0,
	Accumulator_Bms_Status_ok,
}Accumulator_Bms_Status;

typedef enum
{
	Accumulator_Tsal_off		= 0,
	Accumulator_Tsal_ready		= 1,	//Green right
	Accumulator_Tsal_run		= 2,	//Red right
}Accumulator_Tsal_Status;

typedef enum 
{
	Accumulator_Indicator_off	= 0,
	Accumulator_Indicator_on,
}Accumulator_Indicator_Status;

typedef enum 
{
	Accumulator_Temp_Status_ok			= 0,
	Accumulator_Temp_Status_tempHigh,
}Accumulator_Temp_Status;

typedef enum
{
	Accumulator_Ams_Status_ok		= 0,
	Accumulator_Ams_Status_cutOff,
}Accumulator_Ams_Status;

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct 
{
	Accumulator_Bms_Status 			bms[2];
	Accumulator_Tsal_Status			tsal;
	Accumulator_Indicator_Status	indicator;
	Accumulator_Temp_Status			temp;
	Accumulator_Ams_Status			ams;
	
}Accumulator_Status_t;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void AccumualatorManager_init(void);
IFX_EXTERN void AccumulatorManager_run_1ms(void);
/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/



#endif
