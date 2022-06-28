/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_gpio.c
@功能描述:	GPIO模块函数。
@开发人员:	bobzhang 
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/

#include "ac_common.h"


#define GPIO_IRQ_CALLBACK_SIZE     96

static void (*GPIO_IRQCallbackList[GPIO_IRQ_CALLBACK_SIZE])(void) = { 0 };//GPIO中断回调函数数组

/*****************************************************************
@功能	根据GPIO编号获取PORT号
@参数	无
@返回	无
******************************************************************/
__STATIC_INLINE GPIO_TypeDef *GPIO_GetPinRegGroup(GPIO_Type gpioNum)
{
	return (GPIO_TypeDef *)(GPIOAB_BASE + ((gpioNum>>5)<<10));
}

/*****************************************************************
@功能	根据GPIO编号获取PIN BIT号
@参数	无
@返回	无
******************************************************************/
__STATIC_INLINE uint32_t GPIO_GetPinRegBit(GPIO_Type gpioNum)
{
	return (1<<(gpioNum&0x1f));
}

/*****************************************************************
@功能	GPIO 中断处理程序入口
@参数	无
@返回	无
******************************************************************/
void GPIO_IRQ_Handler(GPIO_Type gpioNum)
{
	GPIO_TypeDef *GPIOx;
	uint32_t pinRegBit; 

	GPIOx = GPIO_GetPinRegGroup(gpioNum);
	pinRegBit = GPIO_GetPinRegBit(gpioNum);

    if (GPIOx->MIS & pinRegBit) 
    {   
        GPIOx->IC = pinRegBit;

        //中断回调函数
        if (GPIO_IRQCallbackList[gpioNum] != 0)
            (GPIO_IRQCallbackList[gpioNum])();
    }
}

/*****************************************************************
@功能	GPIOAB 中断处理程序入口
@参数	无
@返回	无
******************************************************************/
void GPIOAB_IRQHandler(void)
{
	int i;
	uint32_t mis;

	//查询并清除中断
	mis = GPIOAB->MIS;
	if(mis==0)
		return;
	GPIOAB->IC = mis;	

	//回调中断处理函数
	for(i=0;i<32;i++)
	{
		if ((mis & 0x01) && (GPIO_IRQCallbackList[i] != 0))
		{
            (GPIO_IRQCallbackList[i])();
		}
		mis >>= 1;
		if(mis==0)
			break;
	}

}

/*****************************************************************
@功能	GPIOCD 中断处理程序入口
@参数	无
@返回	无
******************************************************************/
void GPIOCD_IRQHandler(void)
{
	int i;
	uint32_t mis;

	//查询并清除中断
	mis = GPIOCD->MIS;
	if(mis==0)
		return;
	GPIOCD->IC = mis;

	//回调中断处理函数
	for(i=32;i<64;i++)
	{
		if ((mis & 0x01) && (GPIO_IRQCallbackList[i] != 0))
		{
            (GPIO_IRQCallbackList[i])();
		}
		mis >>= 1;
		if(mis==0)
			break;
	}
}

/*****************************************************************
@功能	GPIOEF 中断处理程序入口
@参数	无
@返回	无
******************************************************************/
void GPIOEF_IRQHandler(void)
{
	int i;
	uint32_t mis;
	
	//查询并清除中断
	mis = GPIOEF->MIS;
	if(mis==0)
		return;
	GPIOEF->IC = mis;

	//回调中断处理函数
	for(i=64;i<96;i++)
	{
		if ((mis & 0x01) && (GPIO_IRQCallbackList[i] != 0))
		{
            (GPIO_IRQCallbackList[i])();
		}
		mis >>= 1;
		if(mis==0)
			break;
	}
}

/*****************************************************************
@功能	GPIO模块全局初始化
@参数	无
@返回	无
******************************************************************/
void GPIO_GlobalInit(void)
{
	System_Module_Enable(EN_GPIOAB);
	NVIC_ClearPendingIRQ(GPIOAB_IRQn);
	NVIC_EnableIRQ(GPIOAB_IRQn);

	System_Module_Enable(EN_GPIOCD);
	NVIC_ClearPendingIRQ(GPIOCD_IRQn);
	NVIC_EnableIRQ(GPIOCD_IRQn);

	System_Module_Enable(EN_GPIOEF);
	NVIC_ClearPendingIRQ(GPIOEF_IRQn);
	NVIC_EnableIRQ(GPIOEF_IRQn);

}

void GPIO_C13_C14_C15_Pull(GPIO_Type gpioNum, uint32_t pull)
{
    if(gpioNum == GPIOC13)
    {
        SCU->STOPCFG |= SCU_STOPCFG_RTC_WE;        
        PMU->IOCR &= ~0x43;//配置为数字接口，禁止上下拉电阻
        PMU->IOCR |= pull; 
        SCU->STOPCFG &= (~SCU_STOPCFG_RTC_WE);
    }
    else if(gpioNum == GPIOC14)
    {
        SCU->STOPCFG |= SCU_STOPCFG_RTC_WE;   
        PMU->IOCR &= ~0x4300;//配置为数字接口，禁止上下拉电阻
        PMU->IOCR |= (pull<<8); 
        SCU->STOPCFG &= (~SCU_STOPCFG_RTC_WE);
    }    
    else if(gpioNum == GPIOC15)
    {
        SCU->STOPCFG |= SCU_STOPCFG_RTC_WE;   
        PMU->IOCR &= ~0x430000;//配置为数字接口，禁止上下拉电阻
        PMU->IOCR |= (pull<<16); 
        SCU->STOPCFG &= (~SCU_STOPCFG_RTC_WE);
    }    
}

/*****************************************************************
@功能	初始化输出引脚   (默认为Output Push Pull Mode)
@参数	gpioNum-引脚号
@参数	PinState-引脚输出状态
@返回	无
******************************************************************/
void GPIO_InitOutput(GPIO_Type gpioNum, enum_PinState_t PinState)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    uint16_t PINx;
    enum_GPIOx_t GPIOx;

    PINx = 1<<(gpioNum & 0x0f);
	GPIOx = (gpioNum >> 4);

    GPIO_InitStruct.Pin = PINx;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;

    GPIO_C13_C14_C15_Pull(gpioNum,GPIO_NOPULL);
    
    HAL_GPIO_Init(GPIOx,&GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOx, PINx, PinState);
}


/*****************************************************************
@功能	初始化输入引脚 
@参数	gpioNum-引脚号
@参数	pull-上下拉设置: GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
@返回	无
******************************************************************/
void GPIO_InitInput(GPIO_Type gpioNum, uint32_t pull)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    uint16_t PINx;
    enum_GPIOx_t GPIOx;

    PINx = 1<<(gpioNum & 0x0f);
	GPIOx = (gpioNum >> 4);

    GPIO_InitStruct.Pin = PINx;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = pull;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
    
    GPIO_C13_C14_C15_Pull(gpioNum,pull);

    HAL_GPIO_Init(GPIOx,&GPIO_InitStruct);
}

/*****************************************************************
@功能	初始化复用功能引脚 
@参数	gpioNum-引脚号
@参数	gpioMode-GPIO模式: GPIO_MODE_AF_PP,GPIO_MODE_AF_OD...
@参数	gpioPull-上下拉设置: GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
@参数   gpioAF-复用功能设置: GPIO_FUNCTION_0,GPIO_FUNCTION_1, GPIO_FUNCTION_2, GPIO_FUNCTION_3, GPIO_FUNCTION_4...GPIO_FUNCTION_9
							 或GPIOA0_AF_SPI2_SCK,GPIOA0_AF_UART2_CTS...(详情请见ac_gpio_af.h)
@返回	无
******************************************************************/
void GPIO_InitAFIO(GPIO_Type gpioNum, uint32_t gpioMode,uint32_t gpioPull, uint32_t gpioAF)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    uint16_t PINx;
    enum_GPIOx_t GPIOx;

    PINx = 1<<(gpioNum & 0x0f);
	GPIOx = (gpioNum >> 4);

    GPIO_InitStruct.Pin = PINx;
    GPIO_InitStruct.Mode = gpioMode;
    GPIO_InitStruct.Pull = gpioPull;
    GPIO_InitStruct.Alternate = gpioAF;
    
    GPIO_C13_C14_C15_Pull(gpioNum,gpioPull);

    HAL_GPIO_Init(GPIOx,&GPIO_InitStruct);
}

/*****************************************************************
@功能	初始化GPIO中断 
@参数	gpioNum-引脚号
@参数	gpioPull-上下拉设置: GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
@参数	mode-中断模式: GPIO_MODE_IT_RISING, GPIO_MODE_IT_FALLING,GPIO_MODE_IT_RISING_FALLING,GPIO_MODE_IT_HIGH_LEVEL,GPIO_MODE_IT_LOW_LEVEL
@参数	cb-中断回调函数
@返回	无
******************************************************************/
void GPIO_InitGpiIRQ(GPIO_Type gpioNum, uint32_t pull,uint32_t mode, void (*GPIOxx_IRQCallback)(void))
{
	GPIO_InitTypeDef GPIO_InitStruct;
	uint16_t PINx;
	enum_GPIOx_t GPIOx;

	PINx = 1<<(gpioNum & 0x0f);
	GPIOx = (gpioNum >> 4);

	GPIO_InitStruct.Pin       = PINx;
	GPIO_InitStruct.Mode      = mode;
	GPIO_InitStruct.Pull      = pull;
	GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;

    GPIO_C13_C14_C15_Pull(gpioNum,pull);
    
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

    GPIO_IRQCallbackList[gpioNum] = GPIOxx_IRQCallback;
}

/*****************************************************************
@功能	GPIO使能中断 
@参数	gpioNum-引脚号  
@返回	无
******************************************************************/
void GPIO_EnableGpiIRQ(GPIO_Type gpioNum)
{
	GPIO_TypeDef *GPIOx;
	uint32_t pinRegBit;

	GPIOx = GPIO_GetPinRegGroup(gpioNum);
	pinRegBit = GPIO_GetPinRegBit(gpioNum);
    GPIOx->IC   = pinRegBit; 
	GPIOx->IEN |= pinRegBit;

}

/*****************************************************************
@功能	GPIO禁止中断 
@参数	gpioNum-引脚号  
@返回	无
******************************************************************/
void GPIO_DisableGpiIRQ(GPIO_Type gpioNum)
{
	GPIO_TypeDef *GPIOx;
	uint32_t pinRegBit;

	GPIOx = GPIO_GetPinRegGroup(gpioNum);
	pinRegBit = GPIO_GetPinRegBit(gpioNum);

	GPIOx->IEN &= (~pinRegBit);
}

/*****************************************************************
@功能	引脚输出高/低电平 
@参数	gpioNum-引脚号
@参数	PinState-电平
@返回	无
******************************************************************/
void GPIO_WritePin(GPIO_Type gpioNum, enum_PinState_t PinState)
{
	GPIO_TypeDef *GPIOx;
	uint32_t pinRegBit;

	GPIOx = GPIO_GetPinRegGroup(gpioNum);
	pinRegBit = GPIO_GetPinRegBit(gpioNum);

	if (GPIO_PIN_SET == PinState) 
	{
			GPIOx->ODATA |= pinRegBit;
	}
	else 
	{
			GPIOx->ODATA &= ~pinRegBit;
	}
}


/*****************************************************************
@功能	引脚输出高 
@参数	gpioNum-引脚号
@返回	无
******************************************************************/
void GPIO_SetPin(GPIO_Type gpioNum)
{
	GPIO_TypeDef *GPIOx;
	uint32_t pinRegBit;

	GPIOx = GPIO_GetPinRegGroup(gpioNum);
	pinRegBit = GPIO_GetPinRegBit(gpioNum);

    GPIOx->SET = pinRegBit;
}

/*****************************************************************
@功能	引脚输出低 
@参数	gpioNum-引脚号
@返回	无
******************************************************************/
void GPIO_ClrPin(GPIO_Type gpioNum)
{
	GPIO_TypeDef *GPIOx;
	uint32_t pinRegBit;

	GPIOx = GPIO_GetPinRegGroup(gpioNum);
	pinRegBit = GPIO_GetPinRegBit(gpioNum);

	GPIOx->CLR = pinRegBit;
}


/*****************************************************************
@功能	引脚取反 
@参数	gpioNum-引脚号
@返回	无
******************************************************************/
void GPIO_InvertPin(GPIO_Type gpioNum)
{
	GPIO_TypeDef *GPIOx;
	uint32_t pinRegBit;

	GPIOx = GPIO_GetPinRegGroup(gpioNum);
	pinRegBit = GPIO_GetPinRegBit(gpioNum);

	if (GPIOx->IDATA & pinRegBit) 
	{
		GPIOx->CLR = pinRegBit;
	}
	else 
	{
		GPIOx->SET = pinRegBit;
	}
}

/*****************************************************************
@功能	读取引脚状态 
@参数	gpioNum-引脚号
@返回	GPIO_PIN_CLEAR, GPIO_PIN_SET
******************************************************************/
enum_PinState_t GPIO_ReadPin(GPIO_Type gpioNum)
{
	GPIO_TypeDef *GPIOx;
	uint32_t pinRegBit;

	GPIOx = GPIO_GetPinRegGroup(gpioNum);
	pinRegBit = GPIO_GetPinRegBit(gpioNum);

	if (GPIOx->IDATA & pinRegBit) 
	{
		return GPIO_PIN_SET;
	}
	else 
	{
		return GPIO_PIN_CLEAR;
	}
}


