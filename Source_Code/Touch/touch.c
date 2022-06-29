/*
 * @Author: cgy233 1781387847@qq.com
 * @Date: 2022-06-29 11:08:01
 * @LastEditors: cgy233 1781387847@qq.com
 * @LastEditTime: 2022-06-29 15:39:26
 * @FilePath: \MDK_Projectd:\Ethan\Project\ACM32\ModulesDemo\UART\demo\Source_Code\Touch\touch.c
 * @Description: 
 * 
 * Copyright (c) 2022 by cgy233 1781387847@qq.com, All Rights Reserved. 
 */
#include "delay.h"
#include "Si14tch.h"
#include "tk_exti.h"
#include "led.h"

// Touch
#define SIZE sizeof(TEXT_Buffer)	
extern uint8_t data_buf[4];
extern uint8_t flag ;
uint16_t times=0 ; 
uint8_t a;

void touch()
{
		LED_Init();
		delay_ms(200);
		led_flicker(0);
		IIC_Init();
		delay_ms(200);
		TK_EXTI_Init();
		delay_ms(200);
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
	
	EXTI->IENR |=  TK_IRQ_EXTI_LINE;
  delay_ms(500);
	uint8_t led_flag = 13;
			
  while(1)
		{
					if(flag !=0)
					{
						EXTI->IENR &=  ~TK_IRQ_EXTI_LINE;
						switch (flag)
						{
							case 1 :{ flag = 0;led_flag = 8; printfS("\r\n T1 TOUCH  9"); break; }
							case 2 :{ flag = 0;led_flag = 7; printfS("\r\n T2 TOUCH  8"); break; }
							case 3 :{ flag = 0;led_flag = 4; printfS("\r\n T3 TOUCH  5"); break; }
							case 4 :{ flag = 0;led_flag = 5; printfS("\r\n T4 TOUCH  6"); break; }
							case 5 :{ flag = 0;led_flag = 2; printfS("\r\n T5 TOUCH  3"); break; }
							case 6 :{ flag = 0;led_flag = 1; printfS("\r\n T6 TOUCH  2"); break; }
							case 7 :{ flag = 0;led_flag = 0; printfS("\r\n T7 TOUCH  1"); break; }
							case 8 :{ flag = 0;led_flag = 3; printfS("\r\n T8 TOUCH  4"); break; }
							case 9 :{ flag = 0;led_flag = 6; printfS("\r\n T9 TOUCH  7"); break; }
							case 10:{ flag = 0;led_flag = 9; printfS("\r\n T10 TOUCH *"); break; }
							case 11:{ flag = 0;led_flag = 11; printfS("\r\n T11 TOUCH 0"); break; }
							case 12:{ flag = 0;led_flag = 10; printfS("\r\n T12 TOUCH #"); break; }	
						}
						// led_flicker(led_flag);
						// led_flag = 13;
						EXTI->IENR |=  TK_IRQ_EXTI_LINE;
						// do
						// {
						// 	led_flicker(led_flag);
						// 	led_flag = 13;
						// 	// delay_us(20);
						// 	SI12_ReadData();   //?¨¢¨¨???¡ä??¡Âoutput1,2,3¨ºy?Y
						// }while(data_buf [0]!=0 ||data_buf [1]!=0||data_buf [2]!=0||data_buf [3]!=0);
						// printfS("\r\nTOUCH END!");
					}		
		}
	
}