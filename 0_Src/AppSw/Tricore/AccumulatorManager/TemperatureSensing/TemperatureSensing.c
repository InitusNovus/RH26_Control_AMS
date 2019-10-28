/* 
 * TemperatureSensing.c
 * 
 * Created on 2019. 10. 22
 * Author: Dua
 */

/* Includes */
#include "TemperatureSensing.h"
#include "Vadc.h"


/* Macros */
#define TEMP_SENSOR_NUM         8
#define AMS_TN_IN(n)            (AMS_T##n##_IN)
#define AMS_TN_IN_INIT(n)       inputArr[n] = &AMS_TN_IN(n)



/* Global Variables */
TemperatureSensing_t TemperatureSensing;


/* Function Implementation */
void TemperatureSensing_init(void)
{
    AdcSensor_Config config;
    HLD_Vadc_initChannelConfig(&config.adcConfig);

    config.adcConfig.lpf.config.cutOffFrequency = 1/(2.0*IFX_PI*0.01);
    config.adcConfig.lpf.config.gain = 1;
    config.adcConfig.lpf.config.samplingTime = 0.001;
    config.adcConfig.lpf.activated = TRUE;
    config.isOvervoltageProtected = TRUE;

    /* Temperature sensor is not linear, 
     * so we let sensor tf be y = 1*x + 0 = x. It is a unity function.
     * and use external non-linear transfer function.(Look-up table)
     */
    config.tfConfig.a = 1;
    config.tfConfig.b = 0;

    HLD_Vadc_Channel_In* inputArr[TEMP_SENSOR_NUM];
    AMS_TN_IN_INIT(0);
    AMS_TN_IN_INIT(1);
    AMS_TN_IN_INIT(2);
    AMS_TN_IN_INIT(3);
    AMS_TN_IN_INIT(4);
    AMS_TN_IN_INIT(5);
    AMS_TN_IN_INIT(6);
    AMS_TN_IN_INIT(7);

    for(uint32 index = 0; index < TEMP_SENSOR_NUM; index++)
    {
        config.adcConfig.channelIn = inputArr[index];
        AdcSensor_initSensor(&TemperatureSensing.TemperatureSensor[index], &config);

        TemperatureSensing.lut.lut.segmentCount = TemperatureSensorLUTCount;
        TemperatureSensing.lut.lut.segments = TemperatureSensorLUT_segments;
    }
}

void TemperatureSensing_run(void)
{
    for (uint32 index = 0; index < TEMP_SENSOR_NUM; index++)
    {
        AdcSensor_getData(&TemperatureSensing.TemperatureSensor[index]);
        TemperatureSensing.temperature[index] = Ifx_LutLinearF32_searchBin(
            &TemperatureSensing.lut.lut, TemperatureSensing.TemperatureSensor[index].value);
    }
}