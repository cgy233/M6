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
#define TIMERX_COUNTER   ((TIMERX)->CNT)  //Timer ��������ֵ

extern volatile uint16_t gu16_timerx_1usCount;

#define SYSTIMER_MS_TIMER_CALLBACK_SIZE    	5   //ϵͳ��ʱ���жϴ���������������
#define SYSTIMER_CYCLE_MS   		1   //ϵͳ��ʱ�����ڣ����뵥λ
#define SYSTIMER_CYCLE_US   		(1000*SYSTIMER_CYCLE_MS)

extern void (*SysTimer_msTimerCallbackList[SYSTIMER_MS_TIMER_CALLBACK_SIZE])(void);//ϵͳ��ʱ���ص���������
extern uint8_t SysTimer_msTimerCallbackNum;//�ص���������

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

