/*
 * @Author: cgy233 1781387847@qq.com
 * @Date: 2022-06-20 15:33:34
 * @LastEditors: cgy233 1781387847@qq.com
 * @LastEditTime: 2022-06-28 15:39:32
 * @FilePath: \MDK_Projectd:\Ethan\Project\ACM32\ModulesDemo\UART\demo\Source_Code\exti.h
 * @Description: 
 * 
 * Copyright (c) 2022 by cgy233 1781387847@qq.com, All Rights Reserved. 
 */
#ifndef __EXTI_H__
#define __EXTI_H__

#include "ACM32Fxx_HAL.h"

// Si523
#define RF_IRQ_PORT GPIOA
#define RF_IRQ_PIN GPIO_PIN_2

#define RF_IRQ_EXTI_PORT EXTI_GPIOA
#define RF_IRQ_EXTI_LINE EXTI_LINE_2

void RF_EXTI_Init();
#endif /* __MAIN_H__ */