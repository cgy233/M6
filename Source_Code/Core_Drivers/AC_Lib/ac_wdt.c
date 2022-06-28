/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_wdt.c
@功能描述:	看门狗模块函数。
@开发人员:	samliu
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/
#include "ac_common.h"


/*****************************************************************
@功能	喂狗函数
@参数	无
@返回	无
******************************************************************/
void WDT_Feed(void)
{
#ifdef ENABLE_WDT
    WDT->FEED = 0xAA55A55A;
#endif
}

/*****************************************************************
@功能	开启开门狗
@参数	无
@返回	无
******************************************************************/
void WDT_Start(void)
{
#ifdef ENABLE_WDT
    WDT->CTRL |= WDT_ENABLE;
#endif
}

/*****************************************************************
@功能	停止开门狗
@参数	无
@返回	无
******************************************************************/
void WDT_Stop(void)
{
#ifdef ENABLE_WDT
    WDT->CTRL &= WDT_DISABLE;
#endif
}

/*****************************************************************
@功能	开启开门狗中断
@参数	无
@返回	无
******************************************************************/
void WDT_Int_Enable(void)
{
#ifdef ENABLE_WDT
    WDT->CTRL |= WDT_INT_ENABLE; 
    NVIC_ClearPendingIRQ(WDT_IRQn);  
    NVIC_EnableIRQ(WDT_IRQn);
#endif
}

/*****************************************************************
@功能	关闭开门狗中断
@参数	无
@返回	无
******************************************************************/
void WDT_Int_Disable(void)
{
#ifdef ENABLE_WDT
    WDT->CTRL &= WDT_INT_DISABLE;
#endif
}

/*****************************************************************
@功能	看门狗初始化函数
@参数	mode - 模式，WDT_MODE_RST-复位模式，WDT_MODE_INT-中断模式
@参数	wdt_time - 时间（单位秒(S)）
@返回	无
******************************************************************/
void WDT_Init(WDT_MODE mode,uint16_t wdt_time)
{
#ifdef ENABLE_WDT
    WDT_HandleTypeDef WDT_Handle;
    uint32_t timer_clk_hz;

    WDT_Handle.Instance=WDT;
    WDT_Handle.Init.WDTMode = mode;    
    WDT_Handle.Init.WDTDivisor = WDT_DIVISOR_128;  //PCLK=180M/2=90M  WDTCLK=90M/128=90M/128
    
    timer_clk_hz = System_Get_APBClock();//timer_clk_hz=90M
    WDT_Handle.Init.WDTLoad = (timer_clk_hz / 128) * wdt_time;
    WDT_Handle.Init.WDTINTCLRTIME = 0xffff;//max time

    HAL_WDT_Init(&WDT_Handle);  
    WDT_Start(); 
#endif    
} 



