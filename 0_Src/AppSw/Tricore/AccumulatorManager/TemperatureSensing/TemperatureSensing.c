/* 
 * TemperatureSensing.c
 * 
 * Created on 2019. 10. 22
 * Author: Dua
 */

/* Includes */
#include "TemperatureSensing.h"


/* Macros */
#define TEMP_SENSOR_NUM     8
#define TN_INIT(n)          (inputArr[##n] = &AMS_T##n_IN)


/* Data Structures */
typedef struct
{
    AdcSensor TemperatureSensor[TEMP_SENSOR_NUM];
    float32 temperature[TEMP_SENSOR_NUM];
}TemperatureSensing_t;

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

    HLD_Vadc_Channel_In* inputArr[TEMP_SENSOR_NUM];
    for(int index = 0; index < TEMP_SENSOR_NUM; i++)
        TN_INIT(i);


}

void TemperatureSensing_run(void)
{

}