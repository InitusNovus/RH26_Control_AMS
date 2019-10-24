/* 
 * CanCommunication.c
 * Created on 2019. 10. 24.
 * Author: Dua
 */


/* Includes */
#include <stdio.h>
#include "CanCommunication.h"

/* GlobalVariables */
IfxMultican_Can			CanCommunication_canModule;

IfxMultican_Can_Node	CanCommunication_canNode0;

CanCommunication_Message CanCommunication_message0;


/* Function Implementation */
void CanCommunication_init(void)
{
	/* Can Module initialiazation */
	IfxMultican_Can_Config canConfig;
	IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);
	IfxMultican_Can_initModule(&CanCommunication_canModule, &canConfig);


	/* Can node0 initialization */
	IfxMultican_Can_NodeConfig canNodeConfig;
	IfxMultican_Can_Node_initConfig(&canNodeConfig, &CanCommunication_canModule);

	canNodeConfig.baudrate 	= 1000000;		//1Mbps
	canNodeConfig.nodeId	= IfxMultican_NodeId_0;
	canNodeConfig.rxPin		= &CAN0NODE0IN;
	canNodeConfig.rxPinMode	= IfxPort_InputMode_pullUp;
	canNodeConfig.txPin		= &CAN0NODE0OUT;
	canNodeConfig.txPinMode	= IfxPort_OutputMode_pushPull;
	IfxMultican_Can_Node_init(&CanCommunication_canNode0, &canNodeConfig);


	/* Message0 configuration */
	CanCommunication_Message* ccMsg = &CanCommunication_message0;

	IfxMultican_Can_MsgObj* obj 	= &ccMsg->obj;
	IfxMultican_Can_Node* node		= (ccMsg->node = &CanCommunication_canNode0);
	
	IfxMultican_Can_MsgObjConfig canMsgObjConfig;
	IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, node);

	canMsgObjConfig.msgObjId		= 0;
	canMsgObjConfig.messageId		= CANCOMM_MSGID0;
	canMsgObjConfig.acceptanceMask	= 0x7FFFFFFFUL;
	canMsgObjConfig.frame			= IfxMultican_Frame_receive;
	canMsgObjConfig.control.messageLen		= IfxMultican_DataLengthCode_8;
	canMsgObjConfig.control.extendedFrame	= TRUE;
	canMsgObjConfig.control.messageLen		= TRUE;
	IfxMultican_Can_MsgObj_init(obj, &canMsgObjConfig);

	ccMsg->isUpdated = FALSE;
}



boolean CanCommunication_receiveMessage(CanCommunication_Message* msg)
{
	boolean isReceived;
	IfxMultican_Status  readStatus;
	if(IfxMultican_Can_MsgObj_isRxPending(&msg->obj))
	{
		//IfxMultican_Message_init(&msg->message, 0xdead, 0xdeadbeef, 0xdeadbeef, IfxMultican_DataLengthCode_8);	//Dummy msg.

		readStatus = IfxMultican_Can_MsgObj_readMessage(&msg->obj, &msg->msg);

		/* if no new data is been received report an error */
		if (!(readStatus & IfxMultican_Status_newData))
		{
			printf("ERROR: IfxMultican_Can_MsgObj_readMessage for canMsgObj00 returned 0x%04x\n", readStatus);
		}

		/* if a new data is been received but one lost, report the status */
		if ((readStatus & IfxMultican_Status_newDataButOneLost) == IfxMultican_Status_newDataButOneLost)
		{
			printf(" IfxMultican_Can_MsgObj_readMessage for canMsgObj00 returned 0x%04x\n", readStatus);
		}

		if (readStatus & IfxMultican_Status_newData)
		{
			msg->isUpdated = TRUE;
		}
		isReceived = TRUE;
	}
	else 
	{
		isReceived = FALSE;
	}

	return isReceived;
}


void CanCommunication_resetUpdateState(CanCommunication_Message* msg)
{
	msg->isUpdated = FALSE;
}


void CanCommunication_transmitMessage(CanCommunication_Message *msg)
{
/*
	if((IfxMultican_Can_MsgObj_sendMessage(&msg->msgObj, &msg->message)) == IfxMultican_Status_ok)
	{
		msg->isUpdated = TRUE;
	}
*/
	while(IfxMultican_Can_MsgObj_sendMessage(&msg->obj, &msg->msg) == IfxMultican_Status_notSentBusy );
	msg->isUpdated = TRUE;
}


void CanCommunication_setMessageData(uint32 data0, uint32 data1, CanCommunication_Message *msg)
{
	msg->msg.data[0] = data0;
	msg->msg.data[1] = data1;
}