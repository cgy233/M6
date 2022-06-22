/****************************************************************************
                            LED驱动头文件
                       (C) Copyright 2000-2020, ***
                            All Rights Reserved
*****************************************************************************
                        Update
V1.1 / 2017-10-19 	
	1.增加位带操作支持，通过结构体控制单独引脚

*****************************************************************************/
#ifndef _LED_H
#define _LED_H

#include "ACM32Fxx_HAL.h"


#define LED1_On()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)	//PA0 = 0
#define LED1_Off()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_CLEAR)		//PA0 = 1

#define LED2_On()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)	//PA1 = 0
#define LED2_Off()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_CLEAR)		//PA1 = 1

#define LED_RED 	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)
#define LED_GREEN 	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)

void Gpio_Init(void);
void LED_PowerOn(void);
void LED_OperaSuccess(void);
void LED_OperaFail(void);


#endif

