/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@�ļ�����:	ac_wdt.c
@��������:	���Ź�ģ�麯����
@������Ա:	samliu
@�������:	2021.1.18
@��ǰ�汾:	1.0
@�޸ļ�¼:	
�޸�����		�޸���   		�޸�˵��

******************************************************************/
#include "ac_common.h"


/*****************************************************************
@����	ι������
@����	��
@����	��
******************************************************************/
void WDT_Feed(void)
{
#ifdef ENABLE_WDT
    WDT->FEED = 0xAA55A55A;
#endif
}

/*****************************************************************
@����	�������Ź�
@����	��
@����	��
******************************************************************/
void WDT_Start(void)
{
#ifdef ENABLE_WDT
    WDT->CTRL |= WDT_ENABLE;
#endif
}

/*****************************************************************
@����	ֹͣ���Ź�
@����	��
@����	��
******************************************************************/
void WDT_Stop(void)
{
#ifdef ENABLE_WDT
    WDT->CTRL &= WDT_DISABLE;
#endif
}

/*****************************************************************
@����	�������Ź��ж�
@����	��
@����	��
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
@����	�رտ��Ź��ж�
@����	��
@����	��
******************************************************************/
void WDT_Int_Disable(void)
{
#ifdef ENABLE_WDT
    WDT->CTRL &= WDT_INT_DISABLE;
#endif
}

/*****************************************************************
@����	���Ź���ʼ������
@����	mode - ģʽ��WDT_MODE_RST-��λģʽ��WDT_MODE_INT-�ж�ģʽ
@����	wdt_time - ʱ�䣨��λ��(S)��
@����	��
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



