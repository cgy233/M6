#ifndef _AC_UART_H_
#define _AC_UART_H_


#include "ac_common.h"

#define UART1_IRQHandler_ENABLE
#define UART2_IRQHandler_ENABLE
#define UART3_IRQHandler_ENABLE
#define UART4_IRQHandler_ENABLE


void UART_Init(UART_TypeDef *UARTx,uint32_t bps, uint8_t stopBits,void (*UARTx_IOInitCallback)(void), void (*UARTx_IRQCallback)(void));

void UART_RxiEnable(UART_TypeDef *UARTx);

void UART_RxiDisable(UART_TypeDef *UARTx);

void UART_SendByte(UART_TypeDef *UARTx, uint8_t ch);

void UART_SendBytes(UART_TypeDef *UARTx, uint8_t *pdata, uint32_t len);

void UART_SendHexBytes(UART_TypeDef *UARTx, uint8_t *pdata, uint32_t len);

//调试串口相关函数

void DEBUG_UartEnable(UART_TypeDef *UARTx,uint8 enable);

void DEBUG_PrintBytes(uint8_t *pdata,int len);
void DEBUG_PrintHexBytes(uint8_t *pdata,int len);

#ifdef UART_DEBUG_ENABLE
    #define dprintf      printf
    #define dprintb      DEBUG_PrintBytes
    #define dprinth      DEBUG_PrintHexBytes
#else
    #define dprintf             ((void)0)  
    #define dprintb             ((void)0)
    #define dprinth             ((void)0) 
#endif

#endif
