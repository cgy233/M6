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
#ifndef __TK_EXTI_H__
#define __TK_EXTI_H__

#include "ACM32Fxx_HAL.h"

// Si14T
 #define TK_IRQ_PORT GPIOC
 #define TK_IRQ_PIN GPIO_PIN_12

 #define TK_IRQ_EXTI_PORT EXTI_GPIOC
 #define TK_IRQ_EXTI_LINE EXTI_LINE_12

void TK_EXTI_Init();
#endif /* __MAIN_H__ */