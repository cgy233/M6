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

// PORT A
#define GPIO_LED_0 GPIO_PIN_1
#define GPIO_LED_POUND GPIO_PIN_8

// PORT C
#define GPIO_LED_1 GPIO_PIN_3
#define GPIO_LED_4 GPIO_PIN_2
#define GPIO_LED_5 GPIO_PIN_6
#define GPIO_LED_8 GPIO_PIN_7
#define GPIO_LED_START GPIO_PIN_8

// PORT D
#define GPIO_LED_2 GPIO_PIN_9
#define GPIO_LED_6 GPIO_PIN_12
#define GPIO_LED_7 GPIO_PIN_10
#define GPIO_LED_9 GPIO_PIN_11

// PORT E
#define GPIO_LED_3 GPIO_PIN_7

void LED_Init(void);
void LED_On_Off(uint8_t led_num, uint8_t on_off);
void led_flicker(uint8_t led_num);

#endif