/*
 * CurrentSensing.h
 * 
 * Created on 2019. 10. 21 
 * Author: Dua
 *  
 */

#ifndef CURRENTSENSING_H_
#define CURRENTSENSING_H_

/* Includes */
#include "AccumulatorManager.h"
#include "AdcSensor.h"
// #include "HLD.h"



/* Data Structures */
typedef struct
{
    AdcSensor CurrentSensor[2];
    float32 current;
}CurrentSensing_t;


/* Global Variables */
IFX_EXTERN CurrentSensing_t CurrentSensing;

/* Function Prototypes */
IFX_EXTERN void CurrentSensing_init(void);
IFX_EXTERN void CurrentSensing_run(void);


#endif //CURRENTSENSING_H_