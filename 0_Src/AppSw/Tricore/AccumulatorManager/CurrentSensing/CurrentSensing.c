/*
 * CurrensSensing.c
 * 
 * Created on 2019. 10. 21
 * Author: Dua
 * 
 */

/* Includes */
#include "CurrentSensing.h"
#include "AccumulatorManager.h"
#include "AdcSensor.h"

/* Macros */
#define C0_CONST_A  1
#define C0_CONST_B  0
#define C1_CONST_A  1
#define C1_CONST_B  0
//FIXME: Arbitrary values

/* Data Structures */


typedef struct
{
    AdcSensor CurrentSensor[2];
    float32 current;
}CurrentSensing_t;



/* Global variables */
CurrentSensing_t CurrentSensing;


/* Private function Prototypes */

/* Function Impementations */
void CurrentSensing_init(void)
{
    AdcSensor_Config config;
    HLD_Vadc_initChannelConfig(&config.adcConfig);

    config.adcConfig.lpf.config.cutOffFrequency = 1/(2.0*IFX_PI*0.005);     //FIXME: Adjust time constant
    config.adcConfig.lpf.config.gain = 1;
    config.adcConfig.lpf.config.samplingTime = 0.001;
    config.adcConfig.lpf.activated = TRUE;
    config.isOvervoltageProtected = TRUE;

    {
        config.adcConfig.channelIn = &AMS_C0_IN;

        config.tfConfig.a = C0_CONST_A;        
        config.tfConfig.b = C0_CONST_B;
        
        AdcSensor_initSensor(&CurrentSensing.CurrentSensor[0], &config);
    }
    {   
        config.adcConfig.channelIn = &AMS_C1_IN;

        config.tfConfig.a = C1_CONST_A;
        config.tfConfig.b = C1_CONST_B;

        AdcSensor_initSensor(&CurrentSensing.CurrentSensor[1], &config);
    }    
}


void CurrentSensing_run(void)
{
    for (uint32 i = 0; i < 2; i++)
    {
        AdcSensor_getData(&CurrentSensing.CurrentSensor[i]);
        //TODO: Transfer function
        //TODO: Error(Hi/Lo) handling
    }
}
