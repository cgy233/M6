#include "led.h"
#include "delay.h"
#include  "nfc_iic.h"
#include "ACM32Fxx_HAL.h"
/**************************************************************************************
函数名称：LED_Init
功    能：LED初始化函数，初始化引脚
参    数：无
返 回 值：无
**************************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;		
	
	// GORT A
	GPIO_InitStructure.Pin = GPIO_LED_0 | GPIO_LED_POUND;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Alternate = GPIO_FUNCTION_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// GORT C
	GPIO_InitStructure.Pin = GPIO_LED_1 | GPIO_LED_4 | GPIO_LED_5 | GPIO_LED_8 | GPIO_LED_START;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// GORT D
	GPIO_InitStructure.Pin = GPIO_LED_2 | GPIO_LED_6 | GPIO_LED_7 | GPIO_LED_9;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	// GORT E
	GPIO_InitStructure.Pin = GPIO_LED_3;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	HAL_GPIO_WritePin(GPIOA, GPIO_LED_0 | GPIO_LED_POUND, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_LED_1 | GPIO_LED_4 | GPIO_LED_5 | GPIO_LED_8 | GPIO_LED_START , GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_LED_START | GPIO_LED_2 | GPIO_LED_6 | GPIO_LED_7 | GPIO_LED_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_LED_3, GPIO_PIN_SET);
	delay_ms(1);
  
}
/**************************************************************************************
函数名称：LED_On_Off
功    能：LED开关
参    数：led_num 灯位 on_off 0 or 1
返 回 值：无
**************************************************************************************/
void LED_On_Off(uint8_t led_num, uint8_t on_off)
{
	if (!on_off)
	{
		switch(led_num)
		{
		case 0:{HAL_GPIO_WritePin(GPIOA, GPIO_LED_0, GPIO_PIN_CLEAR);break;};
		case 1:{HAL_GPIO_WritePin(GPIOC, GPIO_LED_1, GPIO_PIN_CLEAR);break;};
		case 2:{HAL_GPIO_WritePin(GPIOD, GPIO_LED_2, GPIO_PIN_CLEAR);break;};
		case 3:{HAL_GPIO_WritePin(GPIOE, GPIO_LED_3, GPIO_PIN_CLEAR);break;};
		case 4:{HAL_GPIO_WritePin(GPIOC, GPIO_LED_4, GPIO_PIN_CLEAR);break;};
		case 5:{HAL_GPIO_WritePin(GPIOC, GPIO_LED_5, GPIO_PIN_CLEAR);break;};
		case 6:{HAL_GPIO_WritePin(GPIOD, GPIO_LED_6, GPIO_PIN_CLEAR);break;};
		case 7:{HAL_GPIO_WritePin(GPIOD, GPIO_LED_7, GPIO_PIN_CLEAR);break;};
		case 8:{HAL_GPIO_WritePin(GPIOC, GPIO_LED_8, GPIO_PIN_CLEAR);break;};
		case 9:{HAL_GPIO_WritePin(GPIOD, GPIO_LED_9, GPIO_PIN_CLEAR);break;};
		case 10:{HAL_GPIO_WritePin(GPIOC, GPIO_LED_START, GPIO_PIN_CLEAR);break;};
		case 11:{HAL_GPIO_WritePin(GPIOA, GPIO_LED_POUND, GPIO_PIN_CLEAR);break;};
		}
	}
	else
	{
		switch(led_num)
		{
		case 0:{HAL_GPIO_WritePin(GPIOA, GPIO_LED_0, GPIO_PIN_SET);break;};
		case 1:{HAL_GPIO_WritePin(GPIOC, GPIO_LED_1, GPIO_PIN_SET);break;};
		case 2:{HAL_GPIO_WritePin(GPIOD, GPIO_LED_2, GPIO_PIN_SET);break;};
		case 3:{HAL_GPIO_WritePin(GPIOE, GPIO_LED_3, GPIO_PIN_SET);break;};
		case 4:{HAL_GPIO_WritePin(GPIOC, GPIO_LED_4, GPIO_PIN_SET);break;};
		case 5:{HAL_GPIO_WritePin(GPIOC, GPIO_LED_5, GPIO_PIN_SET);break;};
		case 6:{HAL_GPIO_WritePin(GPIOD, GPIO_LED_6, GPIO_PIN_SET);break;};
		case 7:{HAL_GPIO_WritePin(GPIOD, GPIO_LED_7, GPIO_PIN_SET);break;};
		case 8:{HAL_GPIO_WritePin(GPIOC, GPIO_LED_8, GPIO_PIN_SET);break;};
		case 9:{HAL_GPIO_WritePin(GPIOD, GPIO_LED_9, GPIO_PIN_SET);break;};
		case 10:{HAL_GPIO_WritePin(GPIOC, GPIO_LED_START, GPIO_PIN_SET);break;};
		case 11:{HAL_GPIO_WritePin(GPIOA, GPIO_LED_POUND, GPIO_PIN_SET);break;};
		}
	}
	
}

