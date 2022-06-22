/*
 * @Author: cgy233 1781387847@qq.com
 * @Date: 2022-06-14 11:51:35
 * @LastEditors: cgy233 1781387847@qq.com
 * @LastEditTime: 2022-06-22 15:59:11
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

#define UART_BAUD_RATE  115200


void touch_test()
{
	//Uart_Init(115200);	// Initializes the uart 115200 bps
	//Gpio_Init();					// Initializes LED/Gpio
	//EXTIX_Init();				// Initializes external interrupt,do not enable

	//EXTI->IMR |=0x00000008;     //Enable external interrupt
  //IIC_Init();

	//IIC_EN = 0 ; 	
    
	 	while(SI14TCH_Check())
		{
			delay_ms(500);
			LED_OperaFail();
			if(times > 5)
			{
				printf("\r\nNO CHECK SI14TCH!\r\n");
				times = 0;
			}
			else times++;
		}
		
	//??SI14TCH????
   SI14TCH_hard_Reset();    //???

	//	delay_ms(200);
   SI14TCH_Init();
//Si12TouchKey_Init();
//Si12T_EnterSleep();
	 //IIC_EN = 1 ;   //??IIC
 // while(1); 
		
   //EXTI->IMR |=0x00000008;
   delay_ms(500);
		
		//LED_OperaFail();
		//GPIO_SetBits(GPIOA, GPIO_Pin_0); 
			
  while(1)
		{
//			u8 i=0,Keyvalue;
//			

//			
//			Keyvalue = flag;
////			printf ("\r\n keyvalue = %d",Keyvalue );
//			delay_ms (50);
//			
//		EXTI2_3_IRQHandler();
//		
//	printf ("\r\n flag = %d \r\n",flag  );
//					
//		if(Keyvalue !=0)
//		{
//			if(Keyvalue ==flag )
//			{
//				if(i<=1)
//					{
//								
//						return 0;
//						i++;
//					}
//					else
//					{
//										
//			
//						return flag ;
//						i=0;
//					}
//			}
//			else
//			{
//				return flag ;
//				i=0;
//			}
//			
//		}
//		else
//			{
//				flag =0;
//				i++;
//			}
//		
		
//		if(flag !=0)
//			{
//			switch (flag)
//				{
//				case 1 :{ flag = 0; printf(" T1 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); break; }
//				case 2 :{ flag = 0; printf(" T2 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); break; }
//				case 3 :{ flag = 0; printf(" T3 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); break; }
//				case 4 :{ flag = 0; printf(" T4 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); break; }
//				
//				case 5 :{ flag = 0; printf(" T5 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); printf("   output2=0x%2x",data_buf[1]); break; }
//				case 6 :{ flag = 0; printf(" T6 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); printf("   output2=0x%2x",data_buf[1]); break; }
//				case 7 :{ flag = 0; printf(" T7 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); printf("   output2=0x%2x",data_buf[1]); break; }
//				case 8 :{ flag = 0; printf(" T8 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); printf("   output2=0x%2x",data_buf[1]); break; }
//				
//				case 9 :{ flag = 0; printf(" T9 TOUCH  \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); printf("   output3=0x%2x",data_buf[2]); break; }
//				case 10:{ flag = 0; printf(" T10 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); printf("   output3=0x%2x",data_buf[2]); break; }
//				case 11:{ flag = 0; printf(" T11 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); printf("   output3=0x%2x",data_buf[2]); break; }
//				case 12:{ flag = 0; printf(" T12 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); printf("   output3=0x%2x",data_buf[2]); break; }
//			
//				case 13:{ flag = 0; printf(" T13 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); printf("   output4=0x%2x",data_buf[3]); break; }
//				case 14:{ flag = 0; printf(" T14 TOUCH \r\n"); GPIO_ResetBits(GPIOA, GPIO_Pin_0); printf("   output4=0x%2x",data_buf[3]); break; }		
//			}
//		}
//			else
//			{	
//				GPIO_SetBits(GPIOA,GPIO_Pin_0);
//				printf("\r\nNO TOUCH !\r\n");
//				delay_ms(100);
//			
//			}
//		}


//	   SI12_ReadData();   //¶ÁÈ¡¼Ä´æÆ÷output1,2,3Êý¾Ý
//	
//		if(data_buf [0]!=0 ||data_buf [1]!=0||data_buf [2]!=0||data_buf [3]!=0)
//		{
//			delay_ms (50);

//			EXTI2_3_IRQHandler();  //É¨¼üÖµ

			if(flag !=0)
			{
				switch (flag)
					{
					case 1 :{ flag = 0; printf(" T1 TOUCH \r\n"); break; }
					case 2 :{ flag = 0; printf(" T2 TOUCH \r\n"); break; }
					case 3 :{ flag = 0; printf(" T3 TOUCH \r\n"); break; }
					case 4 :{ flag = 0; printf(" T4 TOUCH \r\n"); break; }
					
					case 5 :{ flag = 0; printf(" T5 TOUCH \r\n"); break; }
					case 6 :{ flag = 0; printf(" T6 TOUCH \r\n"); break; }
					case 7 :{ flag = 0; printf(" T7 TOUCH \r\n"); break; }
					case 8 :{ flag = 0; printf(" T8 TOUCH \r\n"); break; }
					
					case 9 :{ flag = 0; printf(" T9 TOUCH  \r\n"); break; }
					case 10:{ flag = 0; printf(" T10 TOUCH \r\n"); break; }
					case 11:{ flag = 0; printf(" T11 TOUCH \r\n"); break; }
					case 12:{ flag = 0; printf(" T12 TOUCH \r\n"); break; }
				
					case 13:{ flag = 0; printf(" T13 TOUCH \r\n"); break; }
					case 14:{ flag = 0; printf(" T14 TOUCH \r\n"); break; }		
					}
					do
				{
					delay_ms (25);
					SI12_ReadData();   //¶ÁÈ¡¼Ä´æÆ÷output1,2,3Êý¾Ý
			}while(data_buf [0]!=0 ||data_buf [1]!=0||data_buf [2]!=0||data_buf [3]!=0);

			//GPIO_SetBits(GPIOA,GPIO_Pin_0);
				printf("  TOUCH END!\r\n");
		}		
			else
			{	
				//GPIO_SetBits(GPIOA,GPIO_Pin_0);
				printf("   NO TOUCH !\r\n");
				delay_ms(100);
			}	
			delay_ms(500);
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
    UART1_Handle.Init.BaudRate   = UART_BAUD_RATE; 
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
	UART2_Init();
	UART3_Init();
	
  printfS("\n\rAPP Version 1.");
	/* UARTx Tx */
	//HAL_UART_Transmit(&UART2_Handle, guint8_t_UART2Test, strlen((char *)guint8_t_UART2Test),0);
	//HAL_UART_Transmit(&UART3_Handle, guint8_t_UART3Test, strlen((char *)guint8_t_UART3Test),0);
	
    /* Select Mode: TEST_LOOP、TEST_UART1_IT、TEST_DMA、TEST_UART_ABORT、TEST_UART2,TEST_UART3*/
	//APP_Uart_Test(TEST_UART1_IT);

	EXTIX_Init();
	ACD_init_Fun();
	ACD_Fun();
	// PCD_SI523_TypeA_Init();
	// PCD_SI523_TypeA();
	
	//SI523_I2C_LL_Init();
	while(1)
	{
		uint8_t k = 0x6c;
		printfS("\n\r********wirte:%02x*******", k);
		I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigK << 2) | 0x40);
		I_SI523_IO_Write(ACDConfigReg, k );
		printfS("\n\r********read********");
		char version = I_SI523_IO_Read(ACDConfigReg);
		printfS("\n\rACDConfigReg: %02x", version);
		//IIC_Send_Byte(0xff);
		delay_ms(500);
	};
}

