/*
 * @Author: cgy233 1781387847@qq.com
 * @Date: 2022-06-21 09:38:07
 * @LastEditors: cgy233 1781387847@qq.com
 * @LastEditTime: 2022-06-22 13:58:48
 * @FilePath: \MDK_Project\finger.h
 * @Description: 
 * 
 * Copyright (c) 2022 by cgy233 1781387847@qq.com, All Rights Reserved. 
 */
#ifndef __FINGER_H__
#define __FINGER_H__

#include "ACM32Fxx_HAL.h"

/* Fingerprints */
uint8_t PS_AutoEnroll(uint8_t *send_buff, uint16_t id, uint8_t count, uint16_t param);
uint8_t PS_AutoIdentify(uint8_t *send_buff, uint8_t safety_level, uint16_t id, uint16_t param);
uint8_t PS_DeletChar(uint8_t *send_buff, uint16_t page_id, uint16_t n);
uint8_t PS_Empty(uint8_t *send_buff);
uint8_t PS_Cancel(uint8_t *send_buff);
uint8_t PS_Sleep(uint8_t *send_buff);
uint8_t PS_ValidTempleteNum(uint8_t *send_buff);
uint8_t PS_SetPwd(uint8_t *send_buff, uint32_t passwd);
uint8_t PS_VfyPwd(uint8_t *send_buff, uint32_t passwd);
uint8_t PS_AuraLedConfig(uint8_t *send_buff, uint8_t cmd, uint8_t speed, uint8_t color, uint8_t count);
uint8_t PS_SetChipAddr(uint8_t *send_buff,  uint8_t chip_addr);

#endif