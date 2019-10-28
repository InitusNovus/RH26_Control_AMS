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
#define TEMP_CUT_DEGC   60.0f
#define TEMP_CUT_STK    10


/* Data Structures */
#define AMS_TX0_DLEN    8
typedef union 
{
    struct 
    {
        sint16 d0;
        sint16 d1;
        sint16 d2;
        sint16 d3;
    }S;
    uint32 B[AMS_TX0_DLEN/4];
}AmsCanMsg0_data_t;




/* Global Variables */
Accumulator_Status_t Accumulator_Status =
{
    .tsal       = Accumulator_Tsal_off,
    .indicator  = Accumulator_Indicator_off,

    .bms[0]     = Accumulator_Bms_Status_ok,
    .bms[1]     = Accumulator_Bms_Status_ok,
    .temp       = Accumulator_Temp_Status_ok,
    
    .ams        = Accumulator_Ams_Status_ok,
};

boolean canReceiveState = FALSE;
uint32 testSuccess = 0;
uint32 testFail = 0;

CanCommunication_Message    AmsCanMsg0;
AmsCanMsg0_data_t           AmsCanMsg0_data = 
{
    .S.d0 = 0,
    .S.d1 = 0,
    .S.d2 = 0,
    .S.d3 = 0,
};
CanCommunication_Message    AmsCanMsg1;

Gpio_Debounce_input     AmsB0;
Gpio_Debounce_input     AmsB1;
Gpio_Debounce_input     AmsTsal;
Gpio_Debounce_input     AmsInd;

boolean bmsCheck = FALSE;

/* Private Function Implementation */


/* Function Implementation */
void AccumualatorManager_init(void)
{
    VoltageSensing_init();
    CurrentSensing_init();
    TemperatureSensing_init();
    CanCommunication_init();

/* CAN message init */
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
        // config.frameType		=	IfxMultican_Frame_transmit;
        config.frameType		=	IfxMultican_Frame_receive;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&AmsCanMsg1, &config);
    }

/* GPIO init */
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

    IfxPort_setPinModeOutput(AMS_BMSF_OUT.port, AMS_BMSF_OUT.pinIndex, IfxPort_OutputMode_pushPull,IfxPort_OutputIdx_general);
    IfxPort_setPinLow(AMS_BMSF_OUT.port, AMS_BMSF_OUT.pinIndex);

}

void AccumulatorManager_run_1ms(void)
{
    VoltageSensing_run();
    CurrentSensing_run();
    TemperatureSensing_run();

/* GPI polling */
    
    /* Bms cutoff check */
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
    
    /* TSAL check */
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

    /* Battery indicator check */
    boolean ind = Gpio_Debounce_pollInput(&AmsInd);
    if(ind)
    {
        Accumulator_Status.indicator = Accumulator_Indicator_on;
    }
    else
    {
        Accumulator_Status.indicator = Accumulator_Indicator_off;
    }
    

/* Temperature status check */
    boolean overTemp = FALSE;
    for(uint32 index = 0; index < TEMP_SENSOR_NUM; index++)
    {
        if(TemperatureSensing.temperature[index] > TEMP_CUT_DEGC)
        {
            overTemp = TRUE;
            break;
        }
    }

    static uint32 overTempStack = 0;
    if(Accumulator_Status.temp == Accumulator_Temp_Status_ok)
    {
        if(overTemp)
        {
            overTempStack++;
        }
        else
        {
            overTempStack = 0;
        }
        if(overTempStack > TEMP_CUT_STK)
        {
            Accumulator_Status.temp = Accumulator_Temp_Status_tempHigh;
        }
    }
    else
    {
        if(overTemp)
        {
            overTempStack = 0;
        }
        else
        {
            overTempStack ++;
        }
        if(overTempStack > TEMP_CUT_STK)
        {
            Accumulator_Status.temp = Accumulator_Temp_Status_ok;
        }
    }

/* Update BMS fault output*/
    boolean bmsFaultCondition = (Accumulator_Status.temp == Accumulator_Temp_Status_tempHigh)
                                ||( bmsCheck && 
                                    (Accumulator_Status.bms[0] == Accumulator_Bms_Status_cutOff 
                                        || Accumulator_Status.bms[1] == Accumulator_Bms_Status_cutOff) );
    if(bmsFaultCondition)
    {
        IfxPort_setPinHigh(AMS_BMSF_OUT.port, AMS_BMSF_OUT.pinIndex);
    }
    else
    {
        IfxPort_setPinLow(AMS_BMSF_OUT.port, AMS_BMSF_OUT.pinIndex);
    }

/* CAN tx message data parsing */
    

/* CAN message encoding */


/* Transmit CAN message */

/*
    CanCommunication_setMessageData(AmsCanMsg0_data.B[0],AmsCanMsg0_data.B[1],&AmsCanMsg0);
    CanCommunication_transmitMessage(&AmsCanMsg0);

    CanCommunication_setMessageData(0x87654321,0x20191025,&AmsCanMsg1);
    CanCommunication_transmitMessage(&AmsCanMsg1);
 */

/* Receive CAN message */
    canReceiveState = CanCommunication_receiveMessage(&AmsCanMsg0);
    CanCommunication_receiveMessage(&AmsCanMsg1);

/* CAN rx message data parsing */
    AmsCanMsg0_data.B[0] = AmsCanMsg0.msg.data[0];
    AmsCanMsg0_data.B[1] = AmsCanMsg0.msg.data[1];

}