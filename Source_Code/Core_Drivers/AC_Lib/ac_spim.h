#ifndef _AC_SPIM_H_
#define _AC_SPIM_H_

#include  "ac_common.h"


void SPI_SetBps(SPI_TypeDef *SPIx,uint32_t bps);

void SPI_SetXMode(SPI_TypeDef *SPIx, uint32_t xMode);

void SPIM_ReleaseCS(SPI_TypeDef *SPIx);

void SPIM_Init(SPI_TypeDef *SPIx,uint32_t workMode, uint32_t xMode,uint32_t bps,void (*SPIx_IOInitCallback)(void));

uint8_t SPIM_TxBytes_KeepCS(SPI_TypeDef *SPIx, uint8_t *txData, uint32_t txLen);

uint8_t SPIM_RxBytes_KeepCS(SPI_TypeDef *SPIx, uint8_t *rxData, uint32_t rxLen);

uint8_t SPIM_TxRxBytes_KeepCS(SPI_TypeDef *SPIx, uint8_t *txData, uint8_t *rxData, uint32_t len);

uint8_t SPIM_RxBytes(SPI_TypeDef *SPIx, uint8_t *rxData, uint32_t rxLen);

uint8_t SPIM_TxBytes(SPI_TypeDef *SPIx, uint8_t *txData, uint32_t txLen);

uint8_t SPIM_TxBytes_TxBytes(SPI_TypeDef *SPIx,uint8_t *txData1, uint32_t txLen1, uint8_t *txData2, uint32_t txLen2);

uint8_t SPIM_TxBytes_RxBytes(SPI_TypeDef *SPIx, uint8_t *txData, uint32_t txLen, uint8_t *rxData, uint32_t rxLen);

uint8_t SPIM_TxRxBytes(SPI_TypeDef *SPIx, uint8_t *txData, uint8_t *rxData, uint32_t len);


#endif
