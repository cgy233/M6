#include "delay.h"
#include "APP.h"

void delay_us(uint32_t us)
{
	for (int i = 0; i < us * 5; i ++)
	System_Delay(i);
}
void delay_ms(uint16_t ms)
{
	System_Delay_MS(ms);
}