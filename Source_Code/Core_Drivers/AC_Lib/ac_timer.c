/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_timer.c
@功能描述:	基本定时器模块函数。
@开发人员:	bobzhang 
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/
#include "ac_common.h"


//TIMER资源： 2,3/4,6/7,14,15/16/17

#define TIMER_IRQ_CALLBACK_SIZE  18
static void (*TIMER_IRQCallbackList[TIMER_IRQ_CALLBACK_SIZE])(void) = { 0 };//TIMER中断回调函数数组

#ifdef TIM2_IRQHandler_ENABLE
void TIM2_IRQHandler(void)
{
	if (TIM2->SR & TIMER_SR_UIF)
	{
        TIMER_IRQCallbackList[2]();
	}
	TIM2->SR = 0;   
}
#endif

#ifdef TIM3_IRQHandler_ENABLE
void TIM3_IRQHandler(void)
{
	if (TIM3->SR & TIMER_SR_UIF)
	{
        TIMER_IRQCallbackList[3]();
	}
	TIM3->SR = 0;   
}
#endif

#ifdef TIM4_IRQHandler_ENABLE
void TIM4_IRQHandler(void)
{
	if (TIM4->SR & TIMER_SR_UIF)
	{
        TIMER_IRQCallbackList[4]();
	}
	TIM4->SR = 0;   
}
#endif

#ifdef TIM6_IRQHandler_ENABLE
void TIM6_IRQHandler(void)
{
	if (TIM6->SR & TIMER_SR_UIF)
	{
        TIMER_IRQCallbackList[6]();
	}
	TIM6->SR = 0;   
}
#endif

#ifdef TIM7_IRQHandler_ENABLE
void TIM7_IRQHandler(void)
{
	if (TIM7->SR & TIMER_SR_UIF)
	{
        TIMER_IRQCallbackList[7]();
	}
	TIM7->SR = 0;   
}
#endif

#ifdef TIM14_IRQHandler_ENABLE
void TIM14_IRQHandler(void)
{
	if (TIM14->SR & TIMER_SR_UIF)
	{
        TIMER_IRQCallbackList[14]();
	}
	TIM14->SR = 0;   
}
#endif

#ifdef TIM15_IRQHandler_ENABLE
void TIM15_IRQHandler(void)
{
	if (TIM15->SR & TIMER_SR_UIF)
	{
        TIMER_IRQCallbackList[15]();
	}
	TIM15->SR = 0;   
}  
#endif

#ifdef TIM16_IRQHandler_ENABLE
void TIM16_IRQHandler(void)
{
	if (TIM16->SR & TIMER_SR_UIF)
	{
        TIMER_IRQCallbackList[16]();
	}
	TIM16->SR = 0;   
}  
#endif

#ifdef TIM17_IRQHandler_ENABLE
void TIM17_IRQHandler(void)
{
	if (TIM17->SR & TIMER_SR_UIF)
	{
        TIMER_IRQCallbackList[17]();
	}
	TIM17->SR = 0;   
}   
#endif

static uint32_t HAL_TIMER_MSP_Init_Ex(TIM_HandleTypeDef * htim, void (*TIMx_IRQCallback)(void))  
{
	uint32_t Timer_Instance;

	Timer_Instance = (uint32_t)(htim->Instance); 
	
	switch(Timer_Instance) 
	{
		case TIM2_BASE: 
		System_Module_Reset(RST_TIM2);   
		System_Module_Enable(EN_TIM2); 
		NVIC_ClearPendingIRQ(TIM2_IRQn);   
		NVIC_EnableIRQ(TIM2_IRQn);
		TIMER_IRQCallbackList[2] = TIMx_IRQCallback;		    
		break;		 

		case TIM3_BASE: 
		System_Module_Reset(RST_TIM3); 
		System_Module_Enable(EN_TIM3);
		NVIC_ClearPendingIRQ(TIM3_IRQn);   
		NVIC_EnableIRQ(TIM3_IRQn);
		TIMER_IRQCallbackList[3] = TIMx_IRQCallback;		 
		break;
		
		case TIM4_BASE: 
		System_Module_Reset(RST_TIM4); 
		System_Module_Enable(EN_TIM4);
		NVIC_ClearPendingIRQ(TIM4_IRQn);   
		NVIC_EnableIRQ(TIM4_IRQn);
		TIMER_IRQCallbackList[4] = TIMx_IRQCallback;		 
		break;		
		
		case TIM6_BASE:  
		System_Module_Reset(RST_TIM6); 			
		System_Module_Enable(EN_TIM6);    
		NVIC_ClearPendingIRQ(TIM6_IRQn);   
		NVIC_EnableIRQ(TIM6_IRQn);
		TIMER_IRQCallbackList[6] = TIMx_IRQCallback;
		break;
		
		case TIM7_BASE:  
		System_Module_Reset(RST_TIM7); 			
		System_Module_Enable(EN_TIM7);    
		NVIC_ClearPendingIRQ(TIM7_IRQn);   
		NVIC_EnableIRQ(TIM7_IRQn);
		TIMER_IRQCallbackList[7] = TIMx_IRQCallback;
		break;		 
		
		case TIM14_BASE:   
		System_Module_Reset(RST_TIM14); 				
		System_Module_Enable(EN_TIM14); 
		NVIC_ClearPendingIRQ(TIM14_IRQn);   
		NVIC_EnableIRQ(TIM14_IRQn);
		TIMER_IRQCallbackList[14] = TIMx_IRQCallback;  
		break;
		
		case TIM15_BASE:   
		System_Module_Reset(RST_TIM15);   	  		
		System_Module_Enable(EN_TIM15); 
		NVIC_ClearPendingIRQ(TIM15_IRQn);   
		NVIC_EnableIRQ(TIM15_IRQn);
		TIMER_IRQCallbackList[15] = TIMx_IRQCallback;  
		break;
		
		case TIM16_BASE: 
		System_Module_Reset(RST_TIM16); 	  	
		System_Module_Enable(EN_TIM16);   
		NVIC_ClearPendingIRQ(TIM16_IRQn);   
		NVIC_EnableIRQ(TIM16_IRQn);
		TIMER_IRQCallbackList[16] = TIMx_IRQCallback; 
		break;
		
		case TIM17_BASE: 
		System_Module_Reset(RST_TIM17); 	 		
		System_Module_Enable(EN_TIM17); 
		NVIC_ClearPendingIRQ(TIM17_IRQn);   
		NVIC_EnableIRQ(TIM17_IRQn);
		TIMER_IRQCallbackList[17] = TIMx_IRQCallback; 
		break;  
		
		default:
		return 1;  
	}
	
	return 0;   
}

/*****************************************************************
@功能	基本定时器初始化 
@参数	TIMx-定时器号: TIM2,TIM3,TIM4,TIM6,TIM7,TIM14,TIM15,TIM16,TIM17
@参数	usPeriod-定时器周期
@参数	TIMx_IRQCallback-中断回调函数
@返回	无
******************************************************************/
void TIMER_Init(TIM_TypeDef *TIMx, uint32_t usPrescaler,uint32_t usPeriod, void (*TIMx_IRQCallback)(void))
{
    uint32_t timer_clock;
	uint32_t divider2;
    TIM_HandleTypeDef TIMx_Handler;  
    
    timer_clock = System_Get_APBClock(); 
    
    //注意，if hclk/pclk != 1, then timer clk = pclk * 2
    if (System_Get_SystemClock() != System_Get_APBClock())    
    {
       timer_clock =  System_Get_APBClock() << 1;    
    }
    
	TIMx_Handler.Instance = TIMx;
	TIMx_Handler.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;    
	TIMx_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	TIMx_Handler.Init.CounterMode = TIM_COUNTERMODE_UP; 
	TIMx_Handler.Init.RepetitionCounter = 0; 
	
//	divider2 = timer_clock/1000*usPeriod/1000;
//	if(divider2<=0x10000)
//	{	
//		TIMx_Handler.Init.Prescaler = 0; 
//		TIMx_Handler.Init.Period = divider2-1;
//	}
//	else
//	{
//		TIMx_Handler.Init.Prescaler = (timer_clock/10000) - 1; 
//		TIMx_Handler.Init.Period = usPeriod/100 - 1; 		
//	}

    TIMx_Handler.Init.Prescaler = usPrescaler-1; 
    TIMx_Handler.Init.Period = usPeriod-1;
    
	HAL_TIMER_MSP_Init_Ex(&TIMx_Handler,TIMx_IRQCallback);        
	HAL_TIMER_Base_Init(&TIMx_Handler);
	  
}

/*****************************************************************
@功能	修改定时器周期 
@参数	TIMx-定时器号: TIM2,TIM3,TIM4,TIM6,TIM7,TIM14,TIM15,TIM16,TIM17
@参数	usPeriod-定时器周期
@返回	无
******************************************************************/
void TIMER_modifyPeriod(TIM_TypeDef *TIMx,uint32_t usPeriod)
{
    uint32_t timer_clock;
	uint32_t divider2;
    
	divider2 = timer_clock/1000*usPeriod/1000;
	if(divider2<=0x10000)
	{	
    	TIMx->ARR = 0; 
		TIMx->PSC = divider2-1;  	
	}
	else
	{
    	TIMx->ARR = (timer_clock/10000) - 1; 
		TIMx->PSC = usPeriod/100 - 1;   				
	}
}

/*****************************************************************
@功能	使能定时器中断
@参数	TIMx-定时器号: TIM2,TIM3,TIM4,TIM6,TIM7,TIM14,TIM15,TIM16,TIM17
@返回	无
******************************************************************/
void TIMER_EnableIRQ(TIM_TypeDef *TIMx)
{
    TIM_HandleTypeDef TIMx_Handler; 
    TIMx_Handler.Instance = TIMx;  	
	HAL_TIM_ENABLE_IT(&TIMx_Handler, TIMER_INT_EN_UPD);	
}

/*****************************************************************
@功能	禁止定时器中断
@参数	TIMx-定时器号: TIM2,TIM3,TIM4,TIM6,TIM7,TIM14,TIM15,TIM16,TIM17
@返回	无
******************************************************************/
void TIMER_DisableIRQ(TIM_TypeDef *TIMx)
{
    TIM_HandleTypeDef TIMx_Handler; 
    TIMx_Handler.Instance = TIMx;  	
	HAL_TIM_DISABLE_IT(&TIMx_Handler, TIMER_INT_EN_UPD);	
}

/*****************************************************************
@功能	启动定时器
@参数	TIMx-定时器号: TIM2,TIM3,TIM4,TIM6,TIM7,TIM14,TIM15,TIM16,TIM17
@返回	无
******************************************************************/
void TIMER_Start(TIM_TypeDef *TIMx)
{
	HAL_TIMER_Base_Start(TIMx);		
}

/*****************************************************************
@功能	停止定时器
@参数	TIMx-定时器号: TIM2,TIM3,TIM4,TIM6,TIM7,TIM14,TIM15,TIM16,TIM17
@返回	无
******************************************************************/
void TIMER_Stop(TIM_TypeDef *TIMx)
{
    TIMx->CR1 &= (~BIT0);  	
}

