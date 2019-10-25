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
#include "Gpio_Debounce.h"


/* Macros */
#define DEBOUNCE_BUFLEN 10


/* Global Variables */
Accumulator_Status_t Accumulator_Status =
{
    .bms[0]     = Accumulator_Bms_Status_ok,
    .bms[1]     = Accumulator_Bms_Status_ok,
    .tsal       = Accumulator_Tsal_off,
    .indicator  = Accumulator_Indicator_off,
};

boolean canReceiveState = FALSE;
uint32 testSuccess = 0;
uint32 testFail = 0;


CanCommunication_Message AmsCanMsg0;
CanCommunication_Message AmsCanMsg1;

Gpio_Debounce_input     AmsB0;
Gpio_Debounce_input     AmsB1;
Gpio_Debounce_input     AmsTsal;
Gpio_Debounce_input     AmsInd;

/* Private Function Implementation */


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
        // config.frameType		=	IfxMultican_Frame_transmit;
        config.frameType		=	IfxMultican_Frame_receive;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&AmsCanMsg0, &config);
    }
    {
        CanCommunication_Message_Config config;
        config.messageId		=	AMS_CAN_MSG_1;
        // config.frameType		=	IfxMultican_Frame_receive;
        config.frameType		=	IfxMultican_Frame_transmit;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&AmsCanMsg1, &config);
    }
/* 
    IfxPort_setPinModeInput(AMS_B0_IN.port,AMS_B0_IN.pinIndex,IfxPort_InputMode_noPullDevice);
    IfxPort_setPinModeInput(AMS_B1_IN.port,AMS_B1_IN.pinIndex,IfxPort_InputMode_noPullDevice);
    IfxPort_setPinModeInput(AMS_TSAL_IN.port,AMS_TSAL_IN.pinIndex,IfxPort_InputMode_noPullDevice);
    IfxPort_setPinModeInput(AMS_IND_IN.port,AMS_IND_IN.pinIndex,IfxPort_InputMode_noPullDevice);
 */
    Gpio_Debounce_inputConfig inputConfig;
    Gpio_Debounce_initInputConfig(&inputConfig);
    inputConfig.port = &AMS_B0_IN;
    inputConfig.bufferLen = Gpio_Debounce_BufferLength_10;
    Gpio_Debounce_initInput(&AmsB0, &inputConfig);

    inputConfig.port = &AMS_B1_IN;
    Gpio_Debounce_initInput(&AmsB1, &inputConfig);

    inputConfig.port = &AMS_TSAL_IN;
    Gpio_Debounce_initInput(&AmsTsal, &inputConfig);

    inputConfig.port = &AMS_IND_IN;
    Gpio_Debounce_initInput(&AmsInd, &inputConfig);

    

}

void AccumulatorManager_run_1ms(void)
{
    VoltageSensing_run();
    CurrentSensing_run();
    TemperatureSensing_run();

    /* GPIO polling */
    {
        boolean bms0Bar = Gpio_Debounce_pollInput(&AmsB0);
        if(bms0Bar)
        {
            Accumulator_Status.bms[0] = Accumulator_Bms_Status_cutOff;
        }
        else
        {
            Accumulator_Status.bms[0] = Accumulator_Bms_Status_ok;
        }

        boolean bms1Bar = Gpio_Debounce_pollInput(&AmsB1);
        if(bms1Bar)
        {
            Accumulator_Status.bms[1] = Accumulator_Bms_Status_cutOff;
        }
        else
        {
            Accumulator_Status.bms[1] = Accumulator_Bms_Status_ok;
        }
        

        boolean tsalGreen = Gpio_Debounce_pollInput(&AmsTsal);
        switch(Accumulator_Status.tsal)
        {
            case Accumulator_Tsal_off:
                if(tsalGreen)
                { 
                    Accumulator_Status.tsal = Accumulator_Tsal_ready; break;
                }
            case Accumulator_Tsal_ready:
                if(tsalGreen==FALSE)
                {
                     Accumulator_Status.tsal = Accumulator_Tsal_run; break;
                }
            case Accumulator_Tsal_run:
                if(tsalGreen) 
                {
                    Accumulator_Status.tsal = Accumulator_Tsal_run; break;
                }
        }

        boolean ind = Gpio_Debounce_pollInput(&AmsInd);
        if(ind)
        {
            Accumulator_Status.indicator = Accumulator_Indicator_on;
        }
        else
        {
            Accumulator_Status.indicator = Accumulator_Indicator_off;
        }
        
    }

    /* CanTest_receive */
    canReceiveState = CanCommunication_receiveMessage(&AmsCanMsg0);
    
    /* CanTest_transmit */
    CanCommunication_setMessageData(0x87654321,0x20191025,&AmsCanMsg1);
    CanCommunication_transmitMessage(&AmsCanMsg1);
   
}