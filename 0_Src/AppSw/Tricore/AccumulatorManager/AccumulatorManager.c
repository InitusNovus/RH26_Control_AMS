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
#include "AccumulatorManager_CanMessage.h"


/* Macros */
#define DEBOUNCE_BUFLEN 10
#define TEMP_CUT_DEGC   60.0f
#define TEMP_CUT_STK    10


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
    .S.voltage = 0,
    .S.current = 0,
    .S.status.U = 0,
    .S.err0.U = 0,
    .S.err1.U = 0
};
CanCommunication_Message    AmsCanMsg1;
AmsCanMsg1_data_t           AmsCanMsg1_data;


Gpio_Debounce_input     AmsB0;
Gpio_Debounce_input     AmsB1;
Gpio_Debounce_input     AmsTsal;
Gpio_Debounce_input     AmsInd;

boolean bmsCheck = FALSE;
boolean bmsFaultCondition = FALSE;

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
        config.frameType		=	IfxMultican_Frame_transmit;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&AmsCanMsg0, &config);
    }
    {
        CanCommunication_Message_Config config;
        config.messageId		=	AMS_CAN_MSG_1;
        config.frameType		=	IfxMultican_Frame_transmit;
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

/* CAN tx message data encoding */

    /* CanMsg0 encoding */
    AmsCanMsg0_data.S.voltage = AmsCanMsg_encodeData(VoltageSensor0.value, &AmsCanMsg_voltageConst);
    AmsCanMsg0_data.S.current= AmsCanMsg_encodeData(CurrentSensing.current, &AmsCanMsg_currentConst);
    
    AmsCanMsg0_data.S.status.B.bms0 = Accumulator_Status.bms[0];
    AmsCanMsg0_data.S.status.B.bms1 = Accumulator_Status.bms[1];
    AmsCanMsg0_data.S.status.B.tempStatus = Accumulator_Status.temp;
    AmsCanMsg0_data.S.status.B.tsal = Accumulator_Status.tsal;
    AmsCanMsg0_data.S.status.B.amsCut = Accumulator_Status.ams;

    if(VoltageSensor0.status&AdcSensor_Status_errorTooHigh)
        AmsCanMsg0_data.S.err0.B.V_Hi = TRUE;
    else
        AmsCanMsg0_data.S.err0.B.V_Hi = FALSE;
    
    if(VoltageSensor0.status&AdcSensor_Status_errorTooLow)
        AmsCanMsg0_data.S.err0.B.V_Lo = TRUE;
    else 
        AmsCanMsg0_data.S.err0.B.V_Lo = FALSE;

    if( (CurrentSensing.CurrentSensor[0].status&AdcSensor_Status_errorTooHigh) || (CurrentSensing.CurrentSensor[1].status&AdcSensor_Status_errorTooHigh) )
        AmsCanMsg0_data.S.err0.B.I_Hi = TRUE;
    else
        AmsCanMsg0_data.S.err0.B.I_Hi = FALSE;

    if( (CurrentSensing.CurrentSensor[0].status&AdcSensor_Status_errorTooLow) || (CurrentSensing.CurrentSensor[1].status&AdcSensor_Status_errorTooLow) )
        AmsCanMsg0_data.S.err0.B.I_Lo = TRUE;
    else 
        AmsCanMsg0_data.S.err0.B.I_Lo = FALSE;

    /* CanMsg1 encoding */
    for(uint32 index = 0; index < TEMP_SENSOR_NUM; index++)
    {
        AmsCanMsg1_data.S.temperature[index] = TemperatureSensing.temperature[index];
    }
    
    


/* Transmit CAN message */
    CanCommunication_setMessageData(AmsCanMsg0_data.U[0],AmsCanMsg0_data.U[1],&AmsCanMsg0);
    CanCommunication_transmitMessage(&AmsCanMsg0);

    CanCommunication_setMessageData(AmsCanMsg1_data.U[0],AmsCanMsg1_data.U[1],&AmsCanMsg1);
    CanCommunication_transmitMessage(&AmsCanMsg1);
}