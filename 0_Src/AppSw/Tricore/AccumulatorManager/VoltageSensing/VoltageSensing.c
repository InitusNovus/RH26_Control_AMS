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
#define V0_CONST_A 		1
#define V0_CONST_B		0
//FIXME: Arbitraty values
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
	HLD_Vadc_Channel adcChannel;
	HLD_Vadc_Data data;
	float32 voltage;
}VoltageSensor;



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
/* 	HLD_Vadc_Channel_Config adcConfig;
	HLD_Vadc_initChannelConfig(&adcConfig);
	adcConfig.channelIn = &AMS_V0_IN;
	HLD_Vadc_initChannel(&VoltageSensor0.adcChannel, &adcConfig); //TODO: LPF adc input
 */
	AdcSensor_Config config;
    HLD_Vadc_initChannelConfig(&config.adcConfig);

    config.adcConfig.lpf.config.cutOffFrequency = 1/(2.0*IFX_PI*0.005);
    config.adcConfig.lpf.config.gain = 1;
    config.adcConfig.lpf.config.samplingTime = 0.001;
    config.adcConfig.lpf.activated = TRUE;

	config.adcConfig.channelIn = &AMS_V0_IN;
	config.tfConfig.a = V0_CONST_A;
	config.tfConfig.b = V0_CONST_B;

	AdcSensor_initSensor(&VoltageSensor0, &config);
}


void VoltageSensing_run(void)
{
	AdcSensor_getData(&VoltageSensor0);
}
