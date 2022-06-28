/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@�ļ�����:	ac_lpuart.c
@��������:	�͹��Ĵ���ģ�麯����
@������Ա:	samLiu 
@�������:	2021.2.22
@��ǰ�汾:	1.0
@�޸ļ�¼:	
�޸�����		�޸���   		�޸�˵��

******************************************************************/

#include "ac_lpuart.h"
#include "ac_gpio.h"





LPUART_HandleTypeDef LPUART_handler;   
static void (*LPUART_IOInitCbf[1])(void) = { 0 };//�͹��Ĵ���IO��ʼ���ص���������


 
/*****************************************************************
@����	����ָ��������ʼ���͹���UARTģ��
@����	hlpuart - �͹��Ĵ��ھ���ṹ��
@����	0: ʧ��; 1: �ɹ� 
******************************************************************/
void HAL_LPUART_MSPInit(LPUART_HandleTypeDef *hlpuart) 
{ 
    GPIO_InitTypeDef gpio_init; 
    if(0x00 == IS_LPUART_INSTANCE(hlpuart->Instance))   
    {
        return;  
    }
    System_Module_Reset(RST_LPUART);
    
    ((void (*)())(LPUART_IOInitCbf[0]))(); 
    
    System_Module_Enable(EN_LPUART);   
}

/*****************************************************************
@���� lpuart��ʼ��
@���� UARTx - UART�˿ڣ�UART1,UART2,UART3,UART4
@���� bps-������,���� 115200
@���� clocksrc-ʱ��Դ:��LPUART_CLOCK_SOURCE_XTAL
@���� buff-���ջ�����
@���� buffsize-���ջ�������С
@���� lpuartIOInitCbf - �˿�IO��ʼ���ص�����
@���� ��
******************************************************************/
void LPUART_Init(uint32_t bps, LPUART_CLOCK_SOURCE clocksrc, uint8_t *buff, uint16_t buffsize,void (*lpuartIOInitCbf)(void))
{
    LPUART_handler.Instance = LPUART;
    LPUART_handler.ConfigParam.BaudRate = bps;
    LPUART_handler.ConfigParam.ClockSrc = clocksrc;     
    LPUART_handler.ConfigParam.Parity = LPUART_PARITY_NONE;  
    LPUART_handler.ConfigParam.StopBits = LPUART_STOPBITS_ONE;
    LPUART_handler.ConfigParam.WordLength = LPUART_DATABITS_8; 
    LPUART_handler.rx_buffer_size = buffsize;
    LPUART_handler.rx_buffer = buff;   

    LPUART_IOInitCbf[0] = lpuartIOInitCbf;

    HAL_LPUART_Init(&LPUART_handler); 

}

/*****************************************************************
@����	lpuart���͵��ֽ�
@����	ch - ���ֽ�����
@����	��
******************************************************************/
void LPUART_SendByte(uint8_t ch)
{
    
    if ((LPUART->SR) & (1U << LPUART_SR_TX_EMPTY_INDEX) )  
    {
        LPUART->TXDR = ch;   
    }    
    //wait uart not busy 
    while (0 == (LPUART->SR & (1 << LPUART_SR_TX_FINISH_INDEX) ) );  
    LPUART->SR = (1 << LPUART_SR_TX_FINISH_INDEX);       
}



/*****************************************************************
@����	uart���Ͷ��ֽ�����
@����	len - ���ݳ���
@����	pdata - ���ݵ�ַ
@����	��
******************************************************************/
void LPUART_SendBytes( uint8_t *pdata, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; i++)
    {
        LPUART_SendByte(*pdata++);     
    }
}

/*****************************************************************
@����	uart�Ѷ��ֽ�����ת��ΪHEX����
@����	len - ���ݳ���
@����	pdata - ���ݵ�ַ
@����	��
******************************************************************/
void LPUART_SendHexBytes(uint8_t *pdata, uint32_t len)
{
    char tempstr[8];
    while(len--)
    {
        sprintf(tempstr,"%02x ",*pdata++);
        LPUART_SendBytes((uint8_t *)tempstr,3);
    }
}







