/*
  ******************************************************************************
  * @file    APP.h
  * @author  Chris_Kyle
  * @version V1.0.0
  * @date    2020
  * @brief   UART demo Header file.
  ******************************************************************************
*/
#ifndef __APP_H__
#define __APP_H__

#include "ACM32Fxx_HAL.h"

#define m_MAXUserBuffLen        512
#define m_UART_data_len         10
#define UART2_Frame_Start_Byte  0xA2
#define UART2_Frame_End_Byte    0x0D
#define UART3_Frame_Start_Byte  0xA3
#define UART3_Frame_End_Byte    0x0A
#define UARTx_Recv_End_Flag     0x01

typedef enum
{
    TEST_LOOP,
    TEST_UART1_IT,
    TEST_DMA,
	TEST_UART_ABORT,
	TEST_UART2,
	TEST_UART3
}enum_TEST_MODE_t;

typedef struct{
	
  uint8_t     DataBuff[m_MAXUserBuffLen];
  uint32_t    DataLength;
  uint8_t     DataRecvEndFlag;

}UARTx_Frame_t;

/*--------------------extern vars-----------------*/

extern UART_HandleTypeDef  UART2_Handle;
extern UART_HandleTypeDef  UART3_Handle;

/*---------------------functions------------------*/
/*UART2 Init*/
void UART2_Init(void);

/*UART3 Init*/
void UART3_Init(void);

void UARTxReceiveITEnable(void);

void UART2RecvDataHandle(void);

void UART3RecvDataHandle(void);

void send_cmdToZw(uint8_t *send_buf,uint8_t bs,uint16_t len,uint8_t cmd,uint8_t *ply);

/* APP_Uart_Test */
void APP_Uart_Test(enum_TEST_MODE_t fe_Mode);

#endif
