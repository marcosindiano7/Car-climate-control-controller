#ifndef __COM_PC_H
#define __COM_PC_H

#include "cmsis_os2.h"

extern osMessageQueueId_t m_MQID_USART_RECEIVE;
extern osMessageQueueId_t m_MQID_USART_SEND;

#define SOH 0x01
#define EOT 0xFE

#define USART_SEND_SUCCESS 0x01
#define USART_RECEIVE_SUCCESS 0x02

#define MAXIMUN_MSGQUEUE_OBJECTS 1
#define MAXIMUN_MSGQUEUE_OBJECTS_FOR_SEND 10

typedef struct{
	unsigned char data[64];
	uint8_t length;
} G_DATA_FROM_USART;

typedef struct{
	uint8_t length;
	unsigned char data[64];
} G_DATA_TO_PC;

int Init_COM_PC(void);
int Init_COM_PC_Test(void);
#endif
