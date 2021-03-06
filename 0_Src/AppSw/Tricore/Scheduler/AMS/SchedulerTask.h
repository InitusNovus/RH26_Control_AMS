/*
 * SchedulerTask.h
 *
 *  Created on: 2018. 5. 17.
 *      Author: bigbi_000
 */

#ifndef SCHEDULERTASK_H
#define SCHEDULERTASK_H

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "HLD.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Type Definitions------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void Task_init(void);

//void Task_1ms (void);
void Task_10ms (void);
void Task_10ms_slot1 (void);

void Task_100ms (void);
void Task_100ms_slot4 (void);
void Task_100ms_slot5 (void);
void Task_100ms_slot14 (void);

void Task_1000ms (void);
void Task_1000ms_slot3 (void);

void Task_IsrCb_1ms (void);
void Task_idle (void);

#endif /* SCHEDULERTASK_H */
