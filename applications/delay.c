#include "delay.h"
#include <rthw.h>
#include <rtthread.h>
#include "board.h"
#include <drivers/pin.h>

void delay_us(uint32_t us)
{
	uint16_t i=0;  
   while(us--)
   {
      i=10;
      while(i--) ;    
   }
}
void delay_ms(uint16_t ms)
{
	rt_thread_mdelay(ms);
}