/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_lpuart.c
@功能描述:	低功耗串口模块函数。
@开发人员:	samLiu 
@完成日期:	2021.2.22
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/

#include "ac_lpuart.h"
#include "ac_gpio.h"





LPUART_HandleTypeDef LPUART_handler;   
static void (*LPUART_IOInitCbf[1])(void) = { 0 };//低功耗串口IO初始化回调函数数组


 
/*****************************************************************
@功能	根据指定参数初始化低功耗UART模块
@参数	hlpuart - 低功耗串口句柄结构体
@返回	0: 失败; 1: 成功 
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
@功能 lpuart初始化
@参数 UARTx - UART端口，UART1,UART2,UART3,UART4
@参数 bps-波特率,比如 115200
@参数 clocksrc-时钟源:如LPUART_CLOCK_SOURCE_XTAL
@参数 buff-接收缓冲区
@参数 buffsize-接收缓冲区大小
@参数 lpuartIOInitCbf - 端口IO初始化回调函数
@返回 无
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
@功能	lpuart发送单字节
@参数	ch - 单字节数据
@返回	无
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
@功能	uart发送多字节数据
@参数	len - 数据长度
@参数	pdata - 数据地址
@返回	无
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
@功能	uart把多字节数据转换为HEX发送
@参数	len - 数据长度
@参数	pdata - 数据地址
@返回	无
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







