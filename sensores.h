#ifndef __SENSORES_H
#define __SENSORES_H

#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include <stdint.h>
#include "Driver_I2C.h"

// Registro del LM75
#define LM75_TEMP_REG      0x00
#define LM75_CONF_REG 		 0x01
#define LM75_THYST_REG 		 0x02
#define LM75_TOS_REG			 0x03

// Direcciones I2C
#define LM75_EXT_ADDR      0x49   // Sensor exterior
#define LM75_INT_ADDR      0x48   // Sensor interior

#define LM75_OS_GPIO_PORT   GPIOC
#define LM75_OS_PIN         GPIO_PIN_9
#define LM75_OS_IRQn        EXTI1_IRQn

#define MSGQUEUE_SENS_OBJECTS  1

typedef struct {
  float Te;   // Temperatura exterior
  float Ti;   // Temperatura interior
} MSGQUEUE_SENS_t;

typedef struct{
	float Ta;
	}MSGQUEUE_SENS_TA_t;

extern osMessageQueueId_t sens_TaQueue;

extern osMessageQueueId_t sens_Queue;

int Init_Thsensor (void);

#endif