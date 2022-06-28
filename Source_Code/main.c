/*
 * @Author: cgy233 1781387847@qq.com
 * @Date: 2022-06-14 11:51:35
 * @LastEditors: cgy233 1781387847@qq.com
 * @LastEditTime: 2022-06-27 10:55:07
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
			//LED_OperaFail();
			if(times > 5)
			{
				printfS("\r\nNO CHECK SI14TCH!");
				times = 0;
			}
			else times++;
		}
	
  // SI14TCH_hard_Reset();

	delay_ms(200);
	SI14TCH_Init(); 
	
	EXTI->IENR |=  IRQ_EXTI_LINE;
  delay_ms(500);
		
		//LED_OperaFail();
		//GPIO_SetBits(GPIOA, GPIO_Pin_0); 
			
  while(1)
		{
			
				//if (PCD_IRQ_flagA)
				if (1)
				{
					EXTI->IENR &=  ~IRQ_EXTI_LINE;
					
					SI12_ReadData();
			 
					if( (data_buf[0] & 0x03)  != 0 )   flag = 1;
					if( ((data_buf[0] >>2) & 0x03) != 0 ) flag = 2;
					if( ((data_buf[0] >>4) & 0x03) != 0 ) flag = 3;
					if( ((data_buf[0] >>6) & 0x03) != 0 ) flag = 4;
							 
					if( (data_buf[1] & 0x03) != 0 )      flag = 5;
					if( ((data_buf[1]>>2) & 0x03) != 0 ) flag = 6;  
					if( ((data_buf[1]>>4) & 0x03) != 0 ) flag = 7; 
					if( ((data_buf[1]>>6) & 0x03) != 0 ) flag = 8;  
			 
					if( (data_buf[2] & 0x03) != 0 )      flag = 9;  
					if( ((data_buf[2]>>2) & 0x03) != 0 ) flag = 10; 
					if( ((data_buf[2]>>4) & 0x03) != 0 ) flag = 11; 
					if( ((data_buf[2]>>6) & 0x03) != 0 ) flag = 12;
					if(flag !=0)
					{
						switch (flag)
						{
							case 1 :{ flag = 0; printfS("\r\n T1 TOUCH  4"); break; }
							case 2 :{ flag = 0; printfS("\r\n T2 TOUCH  5"); break; }
							case 3 :{ flag = 0; printfS("\r\n T3 TOUCH  7"); break; }
							case 4 :{ flag = 0; printfS("\r\n T4 TOUCH  8"); break; }
							case 5 :{ flag = 0; printfS("\r\n T5 TOUCH  *"); break; }
							case 6 :{ flag = 0; printfS("\r\n T6 TOUCH  0"); break; }
							case 7 :{ flag = 0; printfS("\r\n T7 TOUCH  #"); break; }
							case 8 :{ flag = 0; printfS("\r\n T8 TOUCH  9"); break; }
							case 9 :{ flag = 0; printfS("\r\n T9 TOUCH  6"); break; }
							case 10:{ flag = 0; printfS("\r\n T10 TOUCH 3"); break; }
							case 11:{ flag = 0; printfS("\r\n T11 TOUCH 2"); break; }
							case 12:{ flag = 0; printfS("\r\n T12 TOUCH 1"); break; }	
						}
					do
					{
						// delay_ms (25);
						SI12_ReadData();   //¶ÁÈ¡¼Ä´æÆ÷output1,2,3Êý¾Ý
					}while(data_buf [0]!=0 ||data_buf [1]!=0||data_buf [2]!=0||data_buf [3]!=0);

					//GPIO_SetBits(GPIOA,GPIO_Pin_0);
					EXTI->IENR |=  IRQ_EXTI_LINE;
					// printfS("\r\nTOUCH END!");
			}		
			else
			{	
				//GPIO_SetBits(GPIOA,GPIO_Pin_0);
				//printfS("\r\nNO TOUCH !");
				// delay_ms(100);
			}	
		}

			// delay_ms(500);
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
    UART2_Handle.Instance        = UART2;    
    UART2_Handle.Init.BaudRate   = 115200; 
    UART2_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    UART2_Handle.Init.StopBits   = UART_STOPBITS_1;
    UART2_Handle.Init.Parity     = UART_PARITY_NONE;
    UART2_Handle.Init.Mode       = UART_MODE_TX_RX_DEBUG;
    UART2_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    
    HAL_UART_Init(&UART2_Handle);
    
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
//	LED_Init();
//	LED_On_Off(5, 1);
//	LED_On_Off(11, 1);
//	IIC_Init();
//	delay_ms(200);	
//		EXTIX_Init();
//	delay_ms(200);
//	touch_read();
	
	/* UARTx Tx */
	//delay_ms(1000);
	//uint8_t test_buff[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x08, 0x31, 0x00, 0x01, 0x03, 0x00, 0x0F, 0x00, 0x4D};
	//HAL_UART_Transmit(&UART1_Handle, test_buff, sizeof(test_buff),0);	
	//HAL_UART_Transmit(&UART1_Handle, guint8_t_UART2Test, strlen((char *)guint8_t_UART2Test),0);
	//HAL_UART_Transmit(&UART3_Handle, guint8_t_UART3Test, strlen((char *)guint8_t_UART3Test),0);
		
	/* Finger TEST */
  /* Select Mode: TEST_LOOP、TEST_UART1_IT、TEST_DMA、TEST_UART_ABORT、TEST_UART2,TEST_UART3*/
	APP_Uart_Test(TEST_UART1_IT);

	/* Si523 ACD TEST */
	EXTIX_Init();
	ACD_init_Fun();
	ACD_Fun();
	//PCD_SI523_TypeA_Init();
	//PCD_SI523_TypeA();
	
	//IIC_Init();
	while(1)
	{
		uint8_t temp;
		//SI14TCH_WriteOneByte(Output1,0xBB);
  
		a=SI14TCH_ReadOneByte(Output1);
		temp=SI14TCH_ReadOneByte(Output1);
		printfS("\r\nTemp:0x%02X",temp);
		delay_us(200);

	}
}

