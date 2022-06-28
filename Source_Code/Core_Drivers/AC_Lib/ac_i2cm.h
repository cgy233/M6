#ifndef _AC_I2CM_H_
#define _AC_I2CM_H_


#include "ac_common.h"


void I2CM_Init(I2C_TypeDef *I2Cx,uint32_t clock_Speed,void (*I2Cx_IOInitCallback)(void));

uint8_t I2CM_TxBytes(I2C_TypeDef *I2Cx, uint16_t devAddress,uint8_t *txData, uint32_t txLen);

uint8_t I2CM_RxBytes(I2C_TypeDef *I2Cx, uint16_t devAddress, uint8_t *rxData, uint32_t rxLen);

uint8_t I2CM_TxBytes_TxBytes(I2C_TypeDef *I2Cx, uint16_t devAddress,uint8_t *txData1, uint32_t txLen1,uint8_t *txData2, uint32_t txLen2);
















#endif


