/* 
 * AdcSensor.h
 * 
 * Created on 2019. 10. 21
 * Author: Dua
 * 
 */

#ifndef ADCSENSOR_H_
#define ADCSENSOR_H_

/* Includes */
#include "AccumulatorManager.h"

/* Data Structures */
typedef struct
{
	HLD_Vadc_Channel adcChannel;
	HLD_Vadc_Data data;
    struct
    {
        float32 a;
        float32 b;
        //O = a*X + b
    }tf;
	float32 value;
}AdcSensor;

typedef struct
{
    HLD_Vadc_Channel_Config adcConfig;
    struct
    {
        float32 a;
        float32 b;
    }tfConfig;
}AdcSensor_Config;

/* Function Prototypes */

IFX_EXTERN void AdcSensor_initSensor(AdcSensor* sensor, AdcSensor_Config* config);
IFX_EXTERN float32 AdcSensor_getData(AdcSensor* sensor);




#endif //ADCSENSOR_H_