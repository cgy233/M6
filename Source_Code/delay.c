#include "delay.h"

void delay_us(uint32_t nus)
{
	for (int i = 0; i < nus*5; i++)
	{
		System_Delay(nus);
	}
	
}
void delay_ms(uint16_t nms)
{
	System_Delay_MS(nms);
}