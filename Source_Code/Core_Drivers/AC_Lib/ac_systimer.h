/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : systimer.h
 * Description : systimer  header file
 * Author(s)   : bob
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __AC_SYSTIMER_H__
#define __AC_SYSTIMER_H__

#include  "ac_common.h"

// #define TIMERX_US

#define EN_TIMERX         EN_TIM2
#define TIMERX            TIM2
#define TIMERX_COUNTER   ((TIMERX)->CNT)  //Timer 计数器的值

extern volatile uint16_t gu16_timerx_1usCount;

#define SYSTIMER_MS_TIMER_CALLBACK_SIZE    	5   //系统定时器中断处理函数最大允许个数
#define SYSTIMER_CYCLE_MS   		1   //系统定时器周期，毫秒单位
#define SYSTIMER_CYCLE_US   		(1000*SYSTIMER_CYCLE_MS)

extern void (*SysTimer_msTimerCallbackList[SYSTIMER_MS_TIMER_CALLBACK_SIZE])(void);//系统定时器回调函数数组
extern uint8_t SysTimer_msTimerCallbackNum;//回调函数数量

void Timerx_LowSpeed_Init(void);
void Timerx_HighSpeed_Init(void);
void SysTimer_Init(void); 
void SysTimer_Irq_Enable(void);
void SysTimer_Irq_Disable(void); 							
uint8_t SysTimer_AddMsTimerCallback(void (*msTimerHandler)(void));		
uint32_t get_ms_tick(void);							
uint32_t get_ms_delay(uint32_t start); 
uint32_t get_Systick(void); 
uint32_t get_us_delay(uint32_t startTick);			
void delay_us(uint32_t us);							
void delay_ms(uint32_t ms);							
void delay(uint32_t count);

#endif

