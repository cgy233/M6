/*
 * @Author: cgy233 1781387847@qq.com
 * @Date: 2022-06-14 11:51:35
 * @LastEditors: cgy233 1781387847@qq.com
 * @LastEditTime: 2022-06-29 09:16:15
 * @FilePath: \MDK_Projectd:\Ethan\Project\ACM32\ModulesDemo\UART\demo\Source_Code\main.c
 * @Description: 
 * 
 * Copyright (c) 2022 by cgy233 1781387847@qq.com, All Rights Reserved. 
 */
/*
  ******************************************************************************
  * @file    main.c
  * @brief   main source File.
  ******************************************************************************
*/
#include "APP.h" 
#include "nfc_iic.h"
#include "delay.h"
#include "exti.h"
#include "SI523_App.h"
#include "finger.h"
#include "touch.h"


uint8_t  PCD_IRQ_flagA = 0;

UART_HandleTypeDef UART1_Handle;

static uint8_t guint8_t_UART2Test[] = {"This is UART2 Test Data"};
static uint8_t guint8_t_UART3Test[] = {"This is UART3 Test Data"};


/************************************************************************
 * function   : Uart_Init
 * Description: Uart Initiation. 
 ************************************************************************/ 
void Uart_Init(void) 
{
    UART1_Handle.Instance        = UART1;    
    UART1_Handle.Init.BaudRate   = 115200; 
    UART1_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    UART1_Handle.Init.StopBits   = UART_STOPBITS_1;
    UART1_Handle.Init.Parity     = UART_PARITY_NONE;
    UART1_Handle.Init.Mode       = UART_MODE_TX_RX_DEBUG;
    UART1_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    
    HAL_UART_Init(&UART1_Handle);
    
    /* UART_DEBUG_ENABLE control printfS */     
    //printfS("MCU is running, HCLK=%dHz, PCLK=%dHz\n", System_Get_SystemClock(), System_Get_APBClock());       
}


/*********************************************************************************
* Function    : main
* Description : 
* Input       : 
* Outpu       : 
* Author      : PJ                        Date : 2021  
**********************************************************************************/
int main(void)
{
	System_Init();
  
	Uart_Init();
	printfS("\n\rAPP Start.");
	UART2_Init();
	UART3_Init();
	delay_ms(200);

	/* Touck TEST */
	touch();
	
	/* UARTx Tx */
	//delay_ms(1000);
	//uint8_t test_buff[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x08, 0x31, 0x00, 0x01, 0x03, 0x00, 0x0F, 0x00, 0x4D};
	//HAL_UART_Transmit(&UART1_Handle, test_buff, sizeof(test_buff),0);	
	//HAL_UART_Transmit(&UART1_Handle, guint8_t_UART2Test, strlen((char *)guint8_t_UART2Test),0);
	//HAL_UART_Transmit(&UART3_Handle, guint8_t_UART3Test, strlen((char *)guint8_t_UART3Test),0);
		
	/* Finger TEST */
  /* Select Mode: TEST_LOOP、TEST_UART1_IT、TEST_DMA、TEST_UART_ABORT、TEST_UART2,TEST_UART3*/
	//APP_Uart_Test(TEST_UART1_IT);

	/* Si523 ACD TEST */
	//RF_EXTI_Init();
	//ACD_init_Fun();
	//ACD_Fun();
	//PCD_SI523_TypeA_Init();
	//PCD_SI523_TypeA();
	
	while(1)
	{
//    Si12T
//		uint8_t temp;
//		SI14TCH_WriteOneByte(Output1,0xBB);
//  
//		a=SI14TCH_ReadOneByte(Output1);
//		temp=SI14TCH_ReadOneByte(Output1);
//		printfS("\r\nTemp:0x%02X",temp);
		
// 		Si523
//		uint8_t temp;
//		I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigK << 2) | 0x40);		//手动设置一个K值
//		I_SI523_IO_Write(ACDConfigReg, 0x0F);
//		temp = I_SI523_IO_Read(ACDConfigK);
//		printfS("\r\nK:0x%02X",temp);
		delay_ms(200);

	}
}

