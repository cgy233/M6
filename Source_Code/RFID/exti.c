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
#include "exti.h"
#include  "Si14tch.h"
#include  "delay.h"

extern uint8_t PCD_IRQ_flagA;

EXTI_HandleTypeDef RF_EXTI_Handle;

GPIO_InitTypeDef RF_GPIO_Handle;

void RF_EXTI_IRQHandler(void)
{
		PCD_IRQ_flagA = 1;
    printfS("\r\nEXTI_IRQHandler");
		HAL_EXTI_IRQHandler(&RF_EXTI_Handle);
}

void RF_EXTI_Init()
{
		System_Module_Enable(EN_EXTI);

    /* Initialization GPIO */
    RF_GPIO_Handle.Pin       = RF_IRQ_PIN;
    RF_GPIO_Handle.Mode      = GPIO_MODE_INPUT;
    RF_GPIO_Handle.Pull      = GPIO_PULLUP;
    RF_GPIO_Handle.Alternate = GPIO_FUNCTION_0;
		
    HAL_GPIO_Init(RF_IRQ_PORT, &RF_GPIO_Handle);
		HAL_GPIO_WritePin(RF_IRQ_PORT, RF_IRQ_PIN, GPIO_PIN_CLEAR);
    
    /* Config EXTI */
    RF_EXTI_Handle.u32_Line    = RF_IRQ_EXTI_LINE;
    RF_EXTI_Handle.u32_Mode    = EXTI_MODE_INTERRUPT;
    RF_EXTI_Handle.u32_Trigger = EXTI_TRIGGER_FALLING;
		RF_EXTI_Handle.u32_GPIOSel = RF_IRQ_EXTI_PORT;

    HAL_EXTI_SetConfigLine(&RF_EXTI_Handle);
		
		printfS("\n\rEXTI INIT OK.");
    HAL_EXTI_ClearAllPending();
		//printfS("MCU Enter Lowpower, Press User Button To Wakeup MCU!\n");	
    //System_Enter_Stop_Mode(STOPENTRY_WFI);
}