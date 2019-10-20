/*
 * AdcSensor.c
 * Created on 2019. 10. 21
 * Author: Dua
 * 
 */

/* Includes */
#include "AdcSensor.h"
#include "Vadc.h"


/* Private Function Prototypes */
IFX_STATIC float32 AdcSensor_getValueFromAdc(AdcSensor* sensor);


/* Function Inplementation */

void AdcSensor_initSensor(AdcSensor* sensor, AdcSensor_Config* config)
{
    HLD_Vadc_initChannel(&sensor->adcChannel, &config->adcConfig);
    sensor->tf.a = config->tfConfig.a;
    sensor->tf.b = config->tfConfig.b;
}


IFX_STATIC float32 AdcSensor_getValueFromAdc(AdcSensor* sensor)
{
    float32 a = sensor->tf.a;
    float32 b = sensor->tf.b;
    return sensor->value = a * sensor->data.voltage + b;
}

float32 AdcSensor_getData(AdcSensor* sensor)
{
    HLD_Vadc_getData(&sensor->data, &sensor->adcChannel);
    return AdcSensor_getValueFromAdc(sensor);
}