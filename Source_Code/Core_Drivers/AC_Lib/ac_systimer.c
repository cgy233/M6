/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_systimer.c
@功能描述:	系统定时器模块函数。
@开发人员:	bobzhang 
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/
#include "ac_common.h"

#define REG_SYST_CVR  SysTick->VAL

extern uint32_t gu32_SystemClock;
extern volatile uint32_t gu32_SystemCount;

static uint32_t SysTimer_reload;   //systimer 重载值
static uint32_t SysTick_1usCount; //1us计数
static volatile uint32_t *SysTick_VAL;

void (*SysTimer_msTimerCallbackList[SYSTIMER_MS_TIMER_CALLBACK_SIZE])(void) = { 0 };//系统定时器回调函数数组
uint8_t SysTimer_msTimerCallbackNum;//回调函数数量


/*****************************************************************
@功能	Timerx低速初始化
@参数	无
@返回	无
******************************************************************/
void Timerx_LowSpeed_Init(void)
{
    System_Module_Enable(EN_TIMERX);
    TIMERX->PSC = 31;
    TIMERX->ARR = 0xFFFFFFFF;
    TIMERX->EGR = BIT0;
    TIMERX->CR1 = BIT0;  
}


/*****************************************************************
@功能	Timerx高速初始化
@参数	无
@返回	无
******************************************************************/
void Timerx_HighSpeed_Init(void)
{
    uint32_t timer_clock;
    System_Module_Enable(EN_TIMERX);
    timer_clock = System_Get_APBClock(); 
    if (System_Get_SystemClock() != System_Get_APBClock())  // if hclk/pclk != 1, then timer clk = pclk * 2  
    {
       timer_clock =  System_Get_APBClock() << 1;    
    }
    TIMERX->PSC = (timer_clock / 1000000) - 1;
    TIMERX->ARR = 0xFFFFFFFF;
    TIMERX->EGR = BIT0;
    TIMERX->CR1 = BIT0;  
}


/*****************************************************************
@功能	系统定时器初始化
@参数	无
@返回	无
******************************************************************/
void SysTimer_Init(void)
{
	gu32_SystemCount = 0;
	SysTimer_msTimerCallbackNum = 0;
    SysTick_VAL = &(SysTick->VAL) ;
	
	SysTimer_reload = gu32_SystemClock / 1000 - 1;
	SysTick_1usCount = gu32_SystemClock / 1000000;
 
	NVIC_SetPriority(SysTick_IRQn, 0);
}

/*****************************************************************
@功能	关闭系统定时器中断
@参数	无
@返回	无
******************************************************************/
void SysTimer_Irq_Enable(void)
{
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}


/*****************************************************************
@功能	开启系统定时器中断
@参数	无
@返回	无
******************************************************************/
void SysTimer_Irq_Disable(void)
{
    SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);
}

/*****************************************************************
@功能	添加系统定时器中断处理回调函数
@参数	无
@返回	AC_OK-成功，AC_FAIL-失败
******************************************************************/
uint8_t SysTimer_AddMsTimerCallback(void (*msTimerHandler)(void))
{
	if(SysTimer_msTimerCallbackNum < SYSTIMER_MS_TIMER_CALLBACK_SIZE)
	{
		SysTimer_msTimerCallbackList[SysTimer_msTimerCallbackNum++] = msTimerHandler;
		return AC_OK;
	}
	else
	{
		return AC_FAIL;
	}
}


/*****************************************************************
@功能	得到当前毫秒时钟
@参数	无
@返回	当前毫秒值
******************************************************************/
uint32_t get_ms_tick(void)
{
	return gu32_SystemCount;	
}


/*****************************************************************
@功能	计算毫秒延迟时间
@参数	start[in]:起始毫秒
@返回	计算出来的毫秒延迟时间
******************************************************************/
uint32_t get_ms_delay(uint32_t start)
{
	uint32_t curr;
	curr = gu32_SystemCount;
    return (curr-start);
}

/*****************************************************************
@功能	得到当前systick
@参数	无
@返回	当前systick
******************************************************************/
uint32_t get_Systick(void)
{
	return SysTick->VAL;	
}

/*****************************************************************
@功能	计算微秒延时时间，延时时间必须小于1ms（由SysTick_Config定义决定，最大为93ms）。
@参数	startTick[in]:起始systick
@返回	延时时间us
******************************************************************/
uint32_t get_us_delay(uint32_t startTick)
{
    uint32_t currTick;
    uint32_t count;

    currTick = *SysTick_VAL;//REG_SYST_CVR;

    if(currTick >= startTick)  //计数器向下计数，过零处理
    {
        count = SysTimer_reload - currTick + startTick;
    }
    else
    {
        count = startTick - currTick;
    }
    return count / SysTick_1usCount;
       
}

/*****************************************************************
@功能	微秒延时
@参数	us[in]:延时时间
@返回	无
******************************************************************/
void delay_us(uint32_t us)
{
    uint32_t startCounter,currCounter;
    uint32_t intervalCounter;
    uint32_t count;


    startCounter = *SysTick_VAL;//REG_SYST_CVR;
	intervalCounter = SysTick_1usCount * us;
    count = 0;

    while(1)
    {
        currCounter = *SysTick_VAL;//REG_SYST_CVR;

        if(currCounter >= startCounter)  //计数器向下计数，过零处理
        {
            count += SysTimer_reload - currCounter + startCounter;
        }
        else
        {
            count += startCounter - currCounter;
        }
        startCounter = currCounter;
        if(count >= intervalCounter)
        {
            break;
        }
    }
}

/*****************************************************************
@功能	毫秒延时
@参数	us[in]:延时时间
@返回	无
******************************************************************/
void delay_ms(uint32_t ms)
{
	volatile uint32_t start;
	if(ms<=100)  //100ms内，调用delay_us，提高延时精度
	{
		delay_us(ms * 1000);
		return;
	}
	start = gu32_SystemCount;
    while(get_ms_delay(start) < ms);

}

/*****************************************************************
@功能	简单延时
@参数	count[in]:延时时间
@返回	无
******************************************************************/
void delay(uint32_t count)
{
    volatile uint32_t delay_count;

    delay_count = count;

    while(delay_count--);
}


