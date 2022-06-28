/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : timer.h
 * Description : timer  header file
 * Author(s)   : bob
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __AC_TIMER_H__
#define __AC_TIMER_H__

#include  "ac_common.h"


//定时器中断处理函数使能

#define TIM2_IRQHandler_ENABLE
#define TIM3_IRQHandler_ENABLE
#define TIM4_IRQHandler_ENABLE
#define TIM6_IRQHandler_ENABLE
#define TIM7_IRQHandler_ENABLE
#define TIM14_IRQHandler_ENABLE
#define TIM15_IRQHandler_ENABLE
#define TIM16_IRQHandler_ENABLE
#define TIM17_IRQHandler_ENABLE


void TIMER_Init(TIM_TypeDef *TIMx, uint32_t usPrescaler,uint32_t usPeriod, void (*TIMx_IRQCallback)(void));

void TIMER_modifyPeriod(TIM_TypeDef *TIMx,uint32_t usPeriod);

void TIMER_EnableIRQ(TIM_TypeDef *TIMx);

void TIMER_DisableIRQ(TIM_TypeDef *TIMx);

void TIMER_Start(TIM_TypeDef *TIMx);

void TIMER_Stop(TIM_TypeDef *TIMx);


#endif

