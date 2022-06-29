/*
 * @Author: cgy233 1781387847@qq.com
 * @Date: 2022-06-20 15:33:34
 * @LastEditors: cgy233 1781387847@qq.com
 * @LastEditTime: 2022-06-29 09:55:15
 * @FilePath: \MDK_Projectd:\Ethan\Project\ACM32\ModulesDemo\UART\demo\Source_Code\exti.c
 * @Description: 
 * 
 * Copyright (c) 2022 by cgy233 1781387847@qq.com, All Rights Reserved. 
 */
#include "tk_exti.h"
#include  "Si14tch.h"
#include  "delay.h"

extern uint8_t data_buf[4];

uint8_t flag ;

EXTI_HandleTypeDef TK_EXTI_Handle;

GPIO_InitTypeDef TK_GPIO_Handle;

void EXTI_IRQHandler(void)
{
    //printfS("\r\nEXTI_IRQHandler");
		HAL_EXTI_IRQHandler(&TK_EXTI_Handle);
		EXTI->IENR &=  ~TK_IRQ_EXTI_LINE;
		SI12_ReadData();   // ??Output??
		 
		 if( (data_buf[0] & 0x03)  != 0 )   flag = 1;
		 if( ((data_buf[0] >>2) & 0x03) != 0 ) flag = 2;
		 if( ((data_buf[0] >>4) & 0x03) != 0 ) flag = 3;
		 if( ((data_buf[0] >>6) & 0x03) != 0 ) flag = 4;
		 
		 
		 if( (data_buf[1] & 0x03) != 0 )      flag = 5;
		 if( ((data_buf[1]>>2) & 0x03) != 0 ) flag = 6;  
		 if( ((data_buf[1]>>4) & 0x03) != 0 ) flag = 7; 
		 if( ((data_buf[1]>>6) & 0x03) != 0 ) flag = 8;  
		 
		 if( (data_buf[2] & 0x03) != 0 )      flag = 9;  
		 if( ((data_buf[2]>>2) & 0x03) != 0 ) flag = 10; 
		 if( ((data_buf[2]>>4) & 0x03) != 0 ) flag = 11; 
		 if( ((data_buf[2]>>6) & 0x03) != 0 ) flag = 12;
		 EXTI->IENR |=  TK_IRQ_EXTI_LINE;
}

void TK_EXTI_Init()
{
		System_Module_Enable(EN_EXTI);

    /* Initialization GPIO */
    TK_GPIO_Handle.Pin       = TK_IRQ_PIN;
    TK_GPIO_Handle.Mode      = GPIO_MODE_INPUT;
    TK_GPIO_Handle.Pull      = GPIO_PULLUP;
    TK_GPIO_Handle.Alternate = GPIO_FUNCTION_0;
		
    HAL_GPIO_Init(TK_IRQ_PORT, &TK_GPIO_Handle);
		HAL_GPIO_WritePin(TK_IRQ_PORT, TK_IRQ_PIN, GPIO_PIN_CLEAR);
    
    /* Config EXTI */
    TK_EXTI_Handle.u32_Line    = TK_IRQ_EXTI_LINE;
    TK_EXTI_Handle.u32_Mode    = EXTI_MODE_INTERRUPT;
    TK_EXTI_Handle.u32_Trigger = EXTI_TRIGGER_FALLING;
    TK_EXTI_Handle.u32_GPIOSel = TK_IRQ_EXTI_PORT;

    HAL_EXTI_SetConfigLine(&TK_EXTI_Handle);
		
		printfS("\n\rEXTI INIT OK.");
    HAL_EXTI_ClearAllPending();
		//printfS("MCU Enter Lowpower, Press User Button To Wakeup MCU!\n");	
    //System_Enter_Stop_Mode(STOPENTRY_WFI);
}