/* 
 * TemperatureSensing.h
 * 
 * Created on 2019. 10. 22
 * Author: Dua
 * 
 */ 
#ifndef TEMPERATURESENSING_H_
#define TEMPERATURESENSING_H_

/* Includes */
#include "AccumulatorManager.h"
#include "AdcSensor.h"
#include "TemperatureSensorLUT.h"

/* Macros */
#define TEMP_SENSOR_NUM         8

/* Data Structures */
typedef struct
{
    Ifx_LutLinearF32    lut;
}TemperatureSensing_SensorLut;

typedef struct
{
    AdcSensor TemperatureSensor[TEMP_SENSOR_NUM];
    float32 temperature[TEMP_SENSOR_NUM];
    TemperatureSensing_SensorLut lut;
}TemperatureSensing_t;


/* GlobalVariables */
IFX_EXTERN TemperatureSensing_t TemperatureSensing;


/* Function Prototypes */
IFX_EXTERN void TemperatureSensing_init(void);
IFX_EXTERN void TemperatureSensing_run(void);

#endif