/*
 ******************************************************************************
 * @file    HAL_TK.h
 * @version V1.0.0
 * @date    2020
 * @brief   Header file of TouchKey HAL module.
 ******************************************************************************
 */
#ifndef __HAL_TK_H__
#define __HAL_TK_H__

#include "ACM32Fxx_HAL.h"

typedef enum {
    TK_ID_REG,             	 // (0x00)
    TK_SENS_0_REG,           // (0x01)
    TK_SENS_1_REG,           // (0x02)
    TK_SENS_2_REG,           // (0x03)
    TK_SENS_3_REG,           // (0x04)
    TK_SENS_4_REG,           // (0x05)
    TK_SENS_5_REG,           // (0x06)
    TK_SENS_6_REG,           // (0x07)
    TK_SENS_7_REG,           // (0x08)
    TK_SENS_8_REG,           // (0x09)
    TK_SENS_9_REG,           // (0x0A)
    TK_SENS_10_REG,          // (0x0B)
    TK_SENS_11_REG,          // (0x0C)
    TK_SENS_12_REG,          // (0x0D)
    TK_SENS_13_REG,          // (0x0E)
    TK_CH_EN_0_REG,          // (0x0F)
    TK_CH_EN_1_REG,          // (0x10)
    EINT_CFG_0_REG,          // (0x11)
    EINT_CFG_1_REG,          // (0x12)
    EINT_CFG_2_REG,          // (0x13)
    EINT_CFG_3_REG,          // (0x14)
    TK_CRTL_REG,             	 // (0x15)
    TK_STATUS_0_REG,             // (0x16)
    TK_STATUS_1_REG,             // (0x17)
    TK_STATUS_2_REG,             // (0x18)
    TK_VALUE_L_REG,          // (0x19)
    TK_VALUE_H_REG,          // (0x1A)
    EINT_LEVEL_REG,          // (0x1B)
    TK_REG_MAX,
} TK_RegTypeDef;



typedef enum {
	TK_CTRL_TK_EN=0x01, 
	TK_CTRL_SLEEP=0x02,    
	TK_CTRL_DBG_EN=0x40,
	TK_CTRL_RST=0x80
} TK_CtrlEnum;


typedef enum {
	EINT_NO_PULL,	//no pull resister
	EINT_PULL_UP,	//pull up resister
	EINT_PULL_DOWN	//pull down resister
} EINT_PullEnum;

typedef enum {
	EINT_IT_DIS,	//disable int
	EINT_IT_RISING, //rising edge int
	EINT_IT_FALLING,//falling edge int
	EINT_IT_DOUBLE,	//double edge int
} EINT_EdgeEnum;

typedef struct {
    EINT_EdgeEnum  edge;
	EINT_PullEnum  pull;
} EINT_ConfigTypeDef;


typedef struct {   
    uint8_t  chSens[14];
    uint16_t chEnable; 
    EINT_ConfigTypeDef eintConfig[8];    
    uint8_t  ctrl;
} TK_ConfigParaTypeDef;




/*****************************************************************
@功能	TK初始化
@参数	无
@返回	HAL_OK,HAL_ERROR
******************************************************************/
HAL_StatusTypeDef HAL_TK_Init(void);

/*****************************************************************
@功能	写TK寄存器
@参数	addr: 寄存器地址
@参数	len: 长度
@参数	pData: 要写入的数据
@返回	HAL_OK,HAL_ERROR
******************************************************************/
HAL_StatusTypeDef HAL_TK_WriteReg(uint8_t addr, uint16_t len, uint8_t *pData);

/*****************************************************************
@功能	读TK寄存器
@参数	addr: 寄存器地址
@参数	len: 长度
@参数	pData: 读取的数据
@返回	HAL_OK,HAL_ERROR
******************************************************************/
HAL_StatusTypeDef HAL_TK_ReadReg(uint8_t addr, uint16_t len, uint8_t *pData);


#endif