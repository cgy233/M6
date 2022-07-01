/*
 * @Author: cgy233 1781387847@qq.com
 * @Date: 2022-06-28 18:49:36
 * @LastEditors: cgy233 1781387847@qq.com
 * @LastEditTime: 2022-06-29 16:24:48
 * @FilePath: \MDK_Projectd:\Ethan\Project\ACM32\ModulesDemo\UART\demo\Source_Code\delay.c
 * @Description: 
 * 
 * Copyright (c) 2022 by cgy233 1781387847@qq.com, All Rights Reserved. 
 */
#include "delay.h"
#include "APP.h"

void delay_us(uint32_t us)
{
	for (int i = 0; i < us * 5; i ++)
	System_Delay(i);
}
void delay_ms(uint16_t ms)
{
	System_Delay_MS(ms);
}