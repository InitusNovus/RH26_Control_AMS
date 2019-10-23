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

    if(config->isOvervoltageProtected)
        sensor->isOvervoltageProtected = config->isOvervoltageProtected;

    sensor->status = AdcSensor_Status_ok;
}


IFX_STATIC float32 AdcSensor_getValueFromAdc(AdcSensor* sensor)
{
    float32 a = sensor->tf.a;
    float32 b = sensor->tf.b;
    return sensor->value = a * sensor->data.voltage + b;
}

float32 AdcSensor_getData(AdcSensor* sensor)
{
    float32 value;
    HLD_Vadc_getData(&sensor->data, &sensor->adcChannel);
    value = AdcSensor_getValueFromAdc(sensor);

    if(sensor->isOvervoltageProtected)
    {
        if(value > ADCSENSOR_OVP_HI)
            sensor->status |= AdcSensor_Status_errorTooHigh;
        else if(value < ADCSENSOR_OVP_LO)
            sensor->status |= AdcSensor_Status_errorTooLow;
        else
            sensor->status = AdcSensor_Status_ok;
    }

    return value;
}

boolean AdcSensor_isTooHigh(AdcSensor* sensor)
{
    if(sensor->status&AdcSensor_Status_errorTooHigh)
        return TRUE;
    else
        return FALSE;
}

boolean AdcSensor_isTooLow(AdcSensor* sensor)
{
    if(sensor->status&AdcSensor_Status_errorTooLow)
        return TRUE;
    else
        return FALSE;
}

boolean AdnSensor_isError(AdcSensor* sensor)
{
    if(sensor->status != AdcSensor_Status_ok)
        return TRUE;
    else 
        return FALSE;
}