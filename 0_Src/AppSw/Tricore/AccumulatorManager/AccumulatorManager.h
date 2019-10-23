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
#include "VoltageSensing.h"
#include "CurrentSensing.h"
#include "TemperatureSensing.h"
#include "CanCommunication.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

#define AMS_V0_IN   HLD_Vadc_AN11_G0CH11_X103_39

#define AMS_C0_IN   HLD_Vadc_AN17_G1CH5_X103_37
#define AMS_C1_IN   HLD_Vadc_AN21_G1CH9_X103_35

#define AMS_T0_IN	HLD_Vadc_AN10_G0CH10_X102_5
#define AMS_T1_IN	HLD_Vadc_AN9_G0CH9_X102_6
#define AMS_T2_IN	HLD_Vadc_AN16_G1CH4_X102_7
#define AMS_T3_IN	HLD_Vadc_AN15_G1CH3_X102_8
#define AMS_T4_IN	HLD_Vadc_AN3_G0CH3_X102_9
#define AMS_T5_IN	HLD_Vadc_AN2_G0CH2_X102_10
#define AMS_T6_IN	HLD_Vadc_AN1_G0CH1_X102_11
#define AMS_T7_IN	HLD_Vadc_AN0_G0CH0_X102_12

/******************************************************************************/
/*------------------------------Type Definitions------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void AccumualatorManager_init(void);
void AccumulatorManager_run_1ms(void);
/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/



#endif
