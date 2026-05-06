#include "com.h"
#include "Driver_USART.h"
#include <stdlib.h>
#include <string.h>

extern ARM_DRIVER_USART Driver_USART3;
ARM_DRIVER_USART* USARTdrv = &Driver_USART3;


osMessageQueueId_t m_MQID_USART_RECEIVE;
osMessageQueueId_t m_MQID_USART_SEND;

osThreadId_t tid_com_pc_receive_thread;
void Thread_COM_PC_Receive_Call(void *arguments);
void Process_Data(G_DATA_FROM_USART *data, uint8_t index);

osThreadId_t tid_com_pc_send_thread;
void Thread_COM_PC_Send_Call(void *arguments);

void Init_USART_Connection(void);
void USART_SignalEvent(unsigned int event);

osThreadId_t tid_com_pc_receive_test_thread;
void Thread_COM_PC_Receive_Test_Call(void *arguments);

osThreadId_t tid_com_pc_send_test_thread;
void Thread_COM_PC_Send_Test_Call(void *arguments);

G_DATA_FROM_USART struct_data;
G_DATA_TO_PC data_send;


const osThreadAttr_t thread1_attr_com = {
  .stack_size = 256                            // Create the thread stack with a size of 512 bytes
};

int Init_COM_PC(void){

	tid_com_pc_receive_thread = osThreadNew(Thread_COM_PC_Receive_Call, NULL, &thread1_attr_com);
	
	if(tid_com_pc_receive_thread == NULL){
		return -1;
	}

	tid_com_pc_send_thread = osThreadNew(Thread_COM_PC_Send_Call, NULL, &thread1_attr_com);
	
	if(tid_com_pc_send_thread == NULL){
		return -1;
	}
	
	Init_USART_Connection();
	
	return 0;
}

void Thread_COM_PC_Send_Call(void *arguments){
	
	int i;
	m_MQID_USART_SEND = osMessageQueueNew(MAXIMUN_MSGQUEUE_OBJECTS_FOR_SEND, sizeof(G_DATA_TO_PC), NULL);
	while (1) {
		osMessageQueueGet(m_MQID_USART_SEND, &data_send, 0U, osWaitForever);
		for (i = 0; i < data_send.length; i++){
			USARTdrv->Send(&data_send.data[i], 1);
			osThreadFlagsWait(USART_SEND_SUCCESS, osFlagsWaitAll, osWaitForever);
			if (data_send.data[i] == EOT){
				break;
			}
			
		}
	}
}

void Thread_COM_PC_Receive_Call(void *arguments){

	uint8_t data_receive = 0;
	uint8_t index = 0;
	m_MQID_USART_RECEIVE = osMessageQueueNew(MAXIMUN_MSGQUEUE_OBJECTS, sizeof(G_DATA_FROM_USART), NULL);
	
	while (1) {
		USARTdrv->Receive(&data_receive, 1);
		osThreadFlagsWait(USART_RECEIVE_SUCCESS, osFlagsWaitAll, osWaitForever);
		struct_data.data[index++] = data_receive;
		if (data_receive == EOT){
			Process_Data(&struct_data, index);
			index = 0;
		}
	}
}
void Process_Data(G_DATA_FROM_USART *data_usart, uint8_t index){
	
	if (SOH != data_usart->data[0]){
		return;
	}
	
	if (index != data_usart->data[2]){
		return;
	}
	
	data_usart->length = index;
	
	osMessageQueuePut(m_MQID_USART_RECEIVE, data_usart, 0U, 0U);
}


void Init_USART_Connection(void){

	USARTdrv->Initialize(USART_SignalEvent);
	USARTdrv->PowerControl(ARM_POWER_FULL);
	
	USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |
										ARM_USART_DATA_BITS_8 |
										ARM_USART_PARITY_NONE |
										ARM_USART_STOP_BITS_1 |
										ARM_USART_FLOW_CONTROL_NONE,
										9600
	);
	
	USARTdrv->Control(ARM_USART_CONTROL_TX, 1);
	USARTdrv->Control(ARM_USART_CONTROL_RX, 1);
}

void USART_SignalEvent(unsigned int event){
	
	static uint16_t send_state = 	ARM_USART_EVENT_SEND_COMPLETE |
																ARM_USART_EVENT_TX_COMPLETE;
	
	static uint16_t rec_state = ARM_USART_EVENT_RECEIVE_COMPLETE |
															ARM_USART_EVENT_TRANSFER_COMPLETE;

	if (event & send_state){
		osThreadFlagsSet(tid_com_pc_send_thread, USART_SEND_SUCCESS);
	}
	
	if (event & rec_state){
		osThreadFlagsSet(tid_com_pc_receive_thread, USART_RECEIVE_SUCCESS);
	}
	return;
}

int Init_COM_PC_Test(void){

	tid_com_pc_receive_test_thread = osThreadNew(Thread_COM_PC_Receive_Test_Call, NULL, &thread1_attr_com);
	
	if(tid_com_pc_receive_test_thread == NULL){
		return -1;
	}
	
	tid_com_pc_send_test_thread = osThreadNew(Thread_COM_PC_Send_Test_Call, NULL, &thread1_attr_com);
	
	if(tid_com_pc_send_test_thread == NULL){
		return -1;
	}
	return 0;
}

void Thread_COM_PC_Receive_Test_Call(void *arguments){
	G_DATA_FROM_USART data_receive_test;
	
	while (1){
		osMessageQueueGet(m_MQID_USART_RECEIVE, &data_receive_test, 0U, 0U);
		osDelay(500U);
	}

}
void Thread_COM_PC_Send_Test_Call(void *arguments){
	
	G_DATA_TO_PC data_send_test;
	unsigned char test[] = "Hello World!";
	
	memcpy(data_send_test.data, test, sizeof(test));
	data_send_test.length = 13;

	while (1) {
		osMessageQueuePut(m_MQID_USART_SEND, &data_send_test, 0U, 0U);
		osDelay(5000U);
	}
}
