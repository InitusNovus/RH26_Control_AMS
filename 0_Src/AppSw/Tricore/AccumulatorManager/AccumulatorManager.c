/* 
 * AccumulatorManager.c
 * 
 *  Created on 2019. 10. 21
 *  Author: Dua
 * 
 */

/* Includes */
#include "AccumulatorManager.h"
#include "VoltageSensing.h"
#include "CurrentSensing.h"
#include "TemperatureSensing.h"

/* Global Variables */
boolean canReceiveState = FALSE;
uint32 testSuccess = 0;
uint32 testFail = 0;


/* Function Implementation */
void AccumualatorManager_init(void)
{
    VoltageSensing_init();
    CurrentSensing_init();
    TemperatureSensing_init();
    CanCommunication_init();
}

void AccumulatorManager_run_1ms(void)
{
    VoltageSensing_run();
    CurrentSensing_run();
    TemperatureSensing_run();
    /* CanTest_receive */
    canReceiveState = CanCommunication_receiveMessage(&CanCommunication_message0);
    canReceiveState ? testSuccess++ : testFail++;
}