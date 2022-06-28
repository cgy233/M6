
#ifndef __AC_LPUART_H__
#define __AC_LPUART_H__

#include  "ac_common.h"





void LPUART_Init(uint32_t bps, LPUART_CLOCK_SOURCE clocksrc, uint8_t *buff, uint16_t buffsize,void (*lpuartIOInitCbf)(void));

void LPUART_SendByte(uint8_t ch);

void LPUART_SendBytes( uint8_t *pdata, uint32_t len);

void LPUART_SendHexBytes(uint8_t *pdata, uint32_t len);


/* APP_LPUart_Test */
void LPUART_Test(void)    ;
#endif
