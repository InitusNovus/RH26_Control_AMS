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


CanCommunication_Message AmsCanMsg0;
CanCommunication_Message AmsCanMsg1;


/* Function Implementation */
void AccumualatorManager_init(void)
{
    VoltageSensing_init();
    CurrentSensing_init();
    TemperatureSensing_init();
    CanCommunication_init();
    {
        CanCommunication_Message_Config config;
        config.messageId		=	AMS_CAN_MSG_0;
        config.frameType		=	IfxMultican_Frame_transmit;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&AmsCanMsg0, &config);
    }
    {
        CanCommunication_Message_Config config;
        config.messageId		=	AMS_CAN_MSG_1;
        config.frameType		=	IfxMultican_Frame_receive;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&AmsCanMsg1, &config);
    }

}

void AccumulatorManager_run_1ms(void)
{
    VoltageSensing_run();
    CurrentSensing_run();
    TemperatureSensing_run();
    /* CanTest_receive */
    
    canReceiveState = CanCommunication_receiveMessage(&AmsCanMsg1);
    
    /* CanTest_transmit */
    CanCommunication_setMessageData(0x12345678,0x20191024,&AmsCanMsg0);
    CanCommunication_transmitMessage(&AmsCanMsg0);
   
}