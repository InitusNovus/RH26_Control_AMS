/* 
 * CanCommunication.h
 * Created on 2019. 10. 24.
 * Author: Dua
 */

#ifndef CANCOMMUNICATION_H_
#define CANCOMMUNICATION_H_


/* Includes */
#include <Ifx_Types.h>
#include <IfxMultican_Can.h>
#include "Configuration.h"


/* Macros */
#define CANCOMM_MSGID0		0x00100001UL



/* DataStructures */
typedef struct
{
	IfxMultican_Can_Node* 	node;
	IfxMultican_Can_MsgObj	obj;
	IfxMultican_Message		msg;
	boolean					isUpdated;
}CanCommunication_Message;


/* Global Variables */
IFX_EXTERN CanCommunication_Message CanCommunication_message0;


/* Function Prototypes */
IFX_EXTERN void CanCommunication_init(void);

IFX_EXTERN void CanCommunication_receiveMessage(CanCommunication_Message* msg);
IFX_EXTERN void CanCommunication_resetUpdateState(CanCommunication_Message* msg);

IFX_EXTERN void CanCommunication_transmitMessage(CanCommunication_Message *msg);
IFX_EXTERN void CanCommunication_setMessageData(uint32 data0, uint32 data1, CanCommunication_Message *msg);



#endif