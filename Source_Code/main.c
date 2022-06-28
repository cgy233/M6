/*
 * @Author: cgy233 1781387847@qq.com
 * @Date: 2022-06-14 11:51:35
 * @LastEditors: cgy233 1781387847@qq.com
 * @LastEditTime: 2022-06-28 15:38:58
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

#include "Si14tch.h"
#include "led.h"

uint8_t  PCD_IRQ_flagA = 0;

// Touch
#define SIZE sizeof(TEXT_Buffer)	
extern uint8_t data_buf[4];
extern uint8_t flag ;
uint16_t times=0 ; 
uint8_t a;

void touch_read()
{
		//LED_Init();					// Initializes LED/Gpio
	 	while(SI14TCH_Check())
		{
			delay_ms(500);
			if(times > 5)
			{
				printfS("\r\nNO CHECK SI14TCH!");
				times = 0;
			}
			else times++;
		}

	delay_ms(200);
	SI14TCH_Init(); 
	
	EXTI->IENR |=  IRQ_EXTI_LINE;
  delay_ms(500);
			
  while(1)
		{
					if(flag !=0)
					{
						EXTI->IENR &=  ~IRQ_EXTI_LINE;
						switch (flag)
						{
							case 1 :{ flag = 0; printfS("\r\n T1 TOUCH  9"); break; }
							case 2 :{ flag = 0; printfS("\r\n T2 TOUCH  8"); break; }
							case 3 :{ flag = 0; printfS("\r\n T3 TOUCH  5"); break; }
							case 4 :{ flag = 0; printfS("\r\n T4 TOUCH  6"); break; }
							case 5 :{ flag = 0; printfS("\r\n T5 TOUCH  3"); break; }
							case 6 :{ flag = 0; printfS("\r\n T6 TOUCH  2"); break; }
							case 7 :{ flag = 0; printfS("\r\n T7 TOUCH  1"); break; }
							case 8 :{ flag = 0; printfS("\r\n T8 TOUCH  4"); break; }
							case 9 :{ flag = 0; printfS("\r\n T9 TOUCH  7"); break; }
							case 10:{ flag = 0; printfS("\r\n T10 TOUCH *"); break; }
							case 11:{ flag = 0; printfS("\r\n T11 TOUCH 0"); break; }
							case 12:{ flag = 0; printfS("\r\n T12 TOUCH #"); break; }	
						}
						do
						{
							delay_ms (25);
							SI12_ReadData();   //¶ÁÈ¡¼Ä´æÆ÷output1,2,3Êý¾Ý
						}while(data_buf [0]!=0 ||data_buf [1]!=0||data_buf [2]!=0||data_buf [3]!=0);
						EXTI->IENR |=  IRQ_EXTI_LINE;
					// printfS("\r\nTOUCH END!");
					}		
		}
	
}
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
	LED_Init();
//	LED_On_Off(5, 1);
//	LED_On_Off(11, 1);
	 IIC_Init();
	 delay_ms(200);
	 EXTIX_Init();
	 delay_ms(200);
	
	 touch_read();
	
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
	//EXTIX_Init();
	//ACD_init_Fun();
	//ACD_Fun();
	//PCD_SI523_TypeA_Init();
	//PCD_SI523_TypeA();
	
	//IIC_Init();
	while(1)
	{
//		uint8_t temp;
//		SI14TCH_WriteOneByte(Output1,0xBB);
//  
//		a=SI14TCH_ReadOneByte(Output1);
//		temp=SI14TCH_ReadOneByte(Output1);
//		printfS("\r\nTemp:0x%02X",temp);
		delay_ms(200);

	}
}

