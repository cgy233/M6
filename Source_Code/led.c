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
		
	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1;	 //Config GPIOA.0 GPIOA.1
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 			 //Set GPIO Direction : Out
	GPIO_InitStructure.Pull = GPIO_PULLUP;			 //Set output type 
	GPIO_InitStructure.Alternate = GPIO_FUNCTION_0;		 //Set GPIO NOPULL
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);					 //Initialize GPIOA by the configuration above
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_SET);
	delay_ms(1);
  
  GPIO_InitStructure.Pin = GPIO_PIN_7;	//(IIC_EN--RF_MOSI)
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitStructure.Pull = GPIO_PULLUP;  			
	GPIO_InitStructure.Alternate = GPIO_FUNCTION_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = GPIO_PIN_2;	//(RST--RF_CE)
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitStructure.Pull = GPIO_PULLUP;  			
	GPIO_InitStructure.Alternate = GPIO_FUNCTION_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = GPIO_PIN_5;	//(OUT1-RF_SCK)
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT; 
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  			
	GPIO_InitStructure.Alternate = GPIO_FUNCTION_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
//	IIC_EN=0;
//	GPIO_ResetBits(GPIOA,GPIO_Pin_7); 
}

void LED_PowerOn(void)
{
//	LED_RED = !LED_RED;
//	delay_ms(100);
//	LED_RED = !LED_RED;
//	delay_ms(100);
//	LED_RED = !LED_RED;
//	delay_ms(100);
//	LED_RED = 1;	//关闭LED_RED
}

void LED_OperaSuccess(void)
{
//	LED_GREEN = !LED_GREEN;
//	delay_ms(100);
//	LED_GREEN = !LED_GREEN;
//	delay_ms(100);
//	LED_GREEN = !LED_GREEN;
//	delay_ms(100);
//	LED_GREEN = 1;	//关闭LED_GREEN
//	LED_RED = 1;	//关闭LED_RED
}

void LED_OperaFail(void)
{
//	LED_RED = !LED_RED;
//	delay_ms(100);
//	LED_RED = !LED_RED;
//	delay_ms(100);
//	LED_RED = !LED_RED;
//	delay_ms(100);
//	LED_GREEN = 1;	//关闭LED_GREEN
//	LED_RED = 0;	//开启LED_RED
}

