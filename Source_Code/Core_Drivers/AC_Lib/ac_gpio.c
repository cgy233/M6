/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@�ļ�����:	ac_gpio.c
@��������:	GPIOģ�麯����
@������Ա:	bobzhang 
@�������:	2021.1.18
@��ǰ�汾:	1.0
@�޸ļ�¼:	
�޸�����		�޸���   		�޸�˵��

******************************************************************/

#include "ac_common.h"


#define GPIO_IRQ_CALLBACK_SIZE     96

static void (*GPIO_IRQCallbackList[GPIO_IRQ_CALLBACK_SIZE])(void) = { 0 };//GPIO�жϻص���������

/*****************************************************************
@����	����GPIO��Ż�ȡPORT��
@����	��
@����	��
******************************************************************/
__STATIC_INLINE GPIO_TypeDef *GPIO_GetPinRegGroup(GPIO_Type gpioNum)
{
	return (GPIO_TypeDef *)(GPIOAB_BASE + ((gpioNum>>5)<<10));
}

/*****************************************************************
@����	����GPIO��Ż�ȡPIN BIT��
@����	��
@����	��
******************************************************************/
__STATIC_INLINE uint32_t GPIO_GetPinRegBit(GPIO_Type gpioNum)
{
	return (1<<(gpioNum&0x1f));
}

/*****************************************************************
@����	GPIO �жϴ���������
@����	��
@����	��
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

        //�жϻص�����
        if (GPIO_IRQCallbackList[gpioNum] != 0)
            (GPIO_IRQCallbackList[gpioNum])();
    }
}

/*****************************************************************
@����	GPIOAB �жϴ���������
@����	��
@����	��
******************************************************************/
void GPIOAB_IRQHandler(void)
{
	int i;
	uint32_t mis;

	//��ѯ������ж�
	mis = GPIOAB->MIS;
	if(mis==0)
		return;
	GPIOAB->IC = mis;	

	//�ص��жϴ�����
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
@����	GPIOCD �жϴ���������
@����	��
@����	��
******************************************************************/
void GPIOCD_IRQHandler(void)
{
	int i;
	uint32_t mis;

	//��ѯ������ж�
	mis = GPIOCD->MIS;
	if(mis==0)
		return;
	GPIOCD->IC = mis;

	//�ص��жϴ�����
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
@����	GPIOEF �жϴ���������
@����	��
@����	��
******************************************************************/
void GPIOEF_IRQHandler(void)
{
	int i;
	uint32_t mis;
	
	//��ѯ������ж�
	mis = GPIOEF->MIS;
	if(mis==0)
		return;
	GPIOEF->IC = mis;

	//�ص��жϴ�����
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
@����	GPIOģ��ȫ�ֳ�ʼ��
@����	��
@����	��
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
        PMU->IOCR &= ~0x43;//����Ϊ���ֽӿڣ���ֹ����������
        PMU->IOCR |= pull; 
        SCU->STOPCFG &= (~SCU_STOPCFG_RTC_WE);
    }
    else if(gpioNum == GPIOC14)
    {
        SCU->STOPCFG |= SCU_STOPCFG_RTC_WE;   
        PMU->IOCR &= ~0x4300;//����Ϊ���ֽӿڣ���ֹ����������
        PMU->IOCR |= (pull<<8); 
        SCU->STOPCFG &= (~SCU_STOPCFG_RTC_WE);
    }    
    else if(gpioNum == GPIOC15)
    {
        SCU->STOPCFG |= SCU_STOPCFG_RTC_WE;   
        PMU->IOCR &= ~0x430000;//����Ϊ���ֽӿڣ���ֹ����������
        PMU->IOCR |= (pull<<16); 
        SCU->STOPCFG &= (~SCU_STOPCFG_RTC_WE);
    }    
}

/*****************************************************************
@����	��ʼ���������   (Ĭ��ΪOutput Push Pull Mode)
@����	gpioNum-���ź�
@����	PinState-�������״̬
@����	��
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
@����	��ʼ���������� 
@����	gpioNum-���ź�
@����	pull-����������: GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
@����	��
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
@����	��ʼ�����ù������� 
@����	gpioNum-���ź�
@����	gpioMode-GPIOģʽ: GPIO_MODE_AF_PP,GPIO_MODE_AF_OD...
@����	gpioPull-����������: GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
@����   gpioAF-���ù�������: GPIO_FUNCTION_0,GPIO_FUNCTION_1, GPIO_FUNCTION_2, GPIO_FUNCTION_3, GPIO_FUNCTION_4...GPIO_FUNCTION_9
							 ��GPIOA0_AF_SPI2_SCK,GPIOA0_AF_UART2_CTS...(�������ac_gpio_af.h)
@����	��
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
@����	��ʼ��GPIO�ж� 
@����	gpioNum-���ź�
@����	gpioPull-����������: GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
@����	mode-�ж�ģʽ: GPIO_MODE_IT_RISING, GPIO_MODE_IT_FALLING,GPIO_MODE_IT_RISING_FALLING,GPIO_MODE_IT_HIGH_LEVEL,GPIO_MODE_IT_LOW_LEVEL
@����	cb-�жϻص�����
@����	��
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
@����	GPIOʹ���ж� 
@����	gpioNum-���ź�  
@����	��
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
@����	GPIO��ֹ�ж� 
@����	gpioNum-���ź�  
@����	��
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
@����	���������/�͵�ƽ 
@����	gpioNum-���ź�
@����	PinState-��ƽ
@����	��
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
@����	��������� 
@����	gpioNum-���ź�
@����	��
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
@����	��������� 
@����	gpioNum-���ź�
@����	��
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
@����	����ȡ�� 
@����	gpioNum-���ź�
@����	��
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
@����	��ȡ����״̬ 
@����	gpioNum-���ź�
@����	GPIO_PIN_CLEAR, GPIO_PIN_SET
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


