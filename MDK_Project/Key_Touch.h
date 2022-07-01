/*
 * @Author: cgy233 1781387847@qq.com
 * @Date: 2022-06-30 13:52:44
 * @LastEditors: cgy233 1781387847@qq.com
 * @LastEditTime: 2022-06-30 16:00:32
 * @FilePath: \MDK_Projectd:\Ethan\Downloads\touchkey\Key_Touch.h
 * @Description: 
 * 
 * Copyright (c) 2022 by cgy233 1781387847@qq.com, All Rights Reserved. 
 */

#ifndef __KEY_TOUCH_H__
#define __KEY_TOUCH_H__

#include "ACM32Fxx_HAL.h"
#include "string.h"
#include "stdint.h"
#include "drv_touch.h"

#define NO_INPUT                0
#define INPUT_PROCESS           1
#define INPUT_END               2
#define INPUT_PASSWD_TIMEOUT    10000

void Reset_InputPassword(void);
void key_mask(uint32_t uwKeyMaskTemporary);
void Reset_InputPassword(void);
uint8_t touch_key_scan(uint8_t *pucKey);
uint16_t get_touch_key(uint8_t *pucKey, uint32_t time_out);
void TK_Irq_Callback(void);
void TouchKey_GPIO_Init(void);
void TouchKey_Init(void);
uint32_t App_KeyReg(uint8_t ucAttr);
uint32_t App_KeyMatch(uint8_t ucRecFlag, uint8_t ucAttr);
void Tkeyirq_disable(void);
void Tkeyirq_enable(void);
void key_suspend(void);



#endif




