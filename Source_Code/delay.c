#include "delay.h"
#include  "ac_common.h"

void delay_us(uint32_t nus)
{
	System_Delay(nus);
}
void delay_ms(uint16_t nms)
{
	System_Delay_MS(nms);
}