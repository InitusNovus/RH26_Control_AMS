/*
 * VoltageSensing.c
 *
 *  Created on: 2019. 10. 18.
 *      Author: Dua
 */



/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "VoltageSensing.h"
#include "AccumulatorManager.h"
#include "HLD.h"
#include "AdcSensor.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define V0_CONST_A 		-9.2980636458f
#define V0_CONST_B		100.7315829998f
// #define V0_CONST_A 		-9.5065117681f
// #define V0_CONST_B		102.5833640229f
//FIXME: Real-value test
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
AdcSensor VoltageSensor0;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
void VoltageSensing_init(void)
{
	AdcSensor_Config config;
    HLD_Vadc_initChannelConfig(&config.adcConfig);

    config.adcConfig.lpf.config.cutOffFrequency = 1/(2.0*IFX_PI*0.05
	);		//FIXME: Adjust time constant
    config.adcConfig.lpf.config.gain = 1;
    config.adcConfig.lpf.config.samplingTime = 0.001;
    config.adcConfig.lpf.activated = TRUE;

	config.adcConfig.channelIn = &AMS_V0_IN;
	config.tfConfig.a = V0_CONST_A;
	config.tfConfig.b = V0_CONST_B;

	config.isOvervoltageProtected = TRUE;

	AdcSensor_initSensor(&VoltageSensor0, &config);
}


void VoltageSensing_run(void)
{
	AdcSensor_getData(&VoltageSensor0);
	//TODO: Transfer function
	//TODO: Error(Hi/Lo) handling
	
}
