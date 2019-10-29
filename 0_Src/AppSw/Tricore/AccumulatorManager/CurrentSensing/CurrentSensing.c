/*
 * CurrensSensing.c
 * 
 * Created on 2019. 10. 21
 * Author: Dua
 * 
 */
//TODO: Remote configuration via CAN
/* Includes */
#include "CurrentSensing.h"
#include "AccumulatorManager.h"


/* Macros */
#define C0_CONST_A  -357.52865414f
#define C0_CONST_B  894.85768521f
//FIXME: To reflect C1 circuit test
#define C1_CONST_A  -357.52865414f
#define C1_CONST_B  894.85768521f
//FIXME: Zero adjust




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
        //TODO: Error(Hi/Lo) handling
    }
    CurrentSensing.current = (CurrentSensing.CurrentSensor[0].value + CurrentSensing.CurrentSensor[1].value)/2;
}
