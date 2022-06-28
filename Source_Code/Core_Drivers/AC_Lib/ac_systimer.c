/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@�ļ�����:	ac_systimer.c
@��������:	ϵͳ��ʱ��ģ�麯����
@������Ա:	bobzhang 
@�������:	2021.1.18
@��ǰ�汾:	1.0
@�޸ļ�¼:	
�޸�����		�޸���   		�޸�˵��

******************************************************************/
#include "ac_common.h"

#define REG_SYST_CVR  SysTick->VAL

extern uint32_t gu32_SystemClock;
extern volatile uint32_t gu32_SystemCount;

static uint32_t SysTimer_reload;   //systimer ����ֵ
static uint32_t SysTick_1usCount; //1us����
static volatile uint32_t *SysTick_VAL;

void (*SysTimer_msTimerCallbackList[SYSTIMER_MS_TIMER_CALLBACK_SIZE])(void) = { 0 };//ϵͳ��ʱ���ص���������
uint8_t SysTimer_msTimerCallbackNum;//�ص���������


/*****************************************************************
@����	Timerx���ٳ�ʼ��
@����	��
@����	��
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
@����	Timerx���ٳ�ʼ��
@����	��
@����	��
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
@����	ϵͳ��ʱ����ʼ��
@����	��
@����	��
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
@����	�ر�ϵͳ��ʱ���ж�
@����	��
@����	��
******************************************************************/
void SysTimer_Irq_Enable(void)
{
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}


/*****************************************************************
@����	����ϵͳ��ʱ���ж�
@����	��
@����	��
******************************************************************/
void SysTimer_Irq_Disable(void)
{
    SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);
}

/*****************************************************************
@����	���ϵͳ��ʱ���жϴ���ص�����
@����	��
@����	AC_OK-�ɹ���AC_FAIL-ʧ��
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
@����	�õ���ǰ����ʱ��
@����	��
@����	��ǰ����ֵ
******************************************************************/
uint32_t get_ms_tick(void)
{
	return gu32_SystemCount;	
}


/*****************************************************************
@����	��������ӳ�ʱ��
@����	start[in]:��ʼ����
@����	��������ĺ����ӳ�ʱ��
******************************************************************/
uint32_t get_ms_delay(uint32_t start)
{
	uint32_t curr;
	curr = gu32_SystemCount;
    return (curr-start);
}

/*****************************************************************
@����	�õ���ǰsystick
@����	��
@����	��ǰsystick
******************************************************************/
uint32_t get_Systick(void)
{
	return SysTick->VAL;	
}

/*****************************************************************
@����	����΢����ʱʱ�䣬��ʱʱ�����С��1ms����SysTick_Config������������Ϊ93ms����
@����	startTick[in]:��ʼsystick
@����	��ʱʱ��us
******************************************************************/
uint32_t get_us_delay(uint32_t startTick)
{
    uint32_t currTick;
    uint32_t count;

    currTick = *SysTick_VAL;//REG_SYST_CVR;

    if(currTick >= startTick)  //���������¼��������㴦��
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
@����	΢����ʱ
@����	us[in]:��ʱʱ��
@����	��
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

        if(currCounter >= startCounter)  //���������¼��������㴦��
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
@����	������ʱ
@����	us[in]:��ʱʱ��
@����	��
******************************************************************/
void delay_ms(uint32_t ms)
{
	volatile uint32_t start;
	if(ms<=100)  //100ms�ڣ�����delay_us�������ʱ����
	{
		delay_us(ms * 1000);
		return;
	}
	start = gu32_SystemCount;
    while(get_ms_delay(start) < ms);

}

/*****************************************************************
@����	����ʱ
@����	count[in]:��ʱʱ��
@����	��
******************************************************************/
void delay(uint32_t count)
{
    volatile uint32_t delay_count;

    delay_count = count;

    while(delay_count--);
}


