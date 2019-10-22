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


/* Function Implementation */
void AccumualatorManager_init(void)
{
    VoltageSensing_init();
    CurrentSensing_init();
}

void AccumulatorManager_run_1ms(void)
{
    VoltageSensing_run();
    CurrentSensing_run();
}