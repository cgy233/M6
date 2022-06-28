/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_uart.c
@功能描述:	串口模块函数。
@开发人员:	bobzhang 
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/

#include "ac_common.h"


#define UART_PORT_SIZE          4
UART_TypeDef *UART_PortList[UART_PORT_SIZE] = {UART1,UART2,UART3,UART4};

#define UART_IRQ_CALLBACK_SIZE    UART_PORT_SIZE
static void (*UART_IRQCallbackList[UART_IRQ_CALLBACK_SIZE])(void) = { 0 };//UART中断回调函数数组

#define UART_IO_INIT_CALLBACK_SIZE   UART_IRQ_CALLBACK_SIZE
static void (*UART_IOInitCallbackList[UART_IO_INIT_CALLBACK_SIZE])(void) = { 0 };//串口IO初始化回调函数数组

#ifdef UART1_IRQHandler_ENABLE
void UART1_IRQHandler(void)
{
    UART_IRQCallbackList[0]();    
}
#endif

#ifdef UART2_IRQHandler_ENABLE
void UART2_IRQHandler(void)
{
    UART_IRQCallbackList[1]();    
}
#endif

#ifdef UART3_IRQHandler_ENABLE
void UART3_IRQHandler(void)
{
    UART_IRQCallbackList[2]();    
}
#endif

#ifdef UART4_IRQHandler_ENABLE
void UART4_IRQHandler(void)
{
    UART_IRQCallbackList[3]();    
}
#endif

/*****************************************************************
@功能	获取初始化回调函数数组索引
@参数	UARTx - UART端口，UART1,UART2,UART3,UART4
@返回	索引号
******************************************************************/
static uint8_t UART_GetIndex(UART_TypeDef *UARTx)
{
    int i;
    for(i=0;i<UART_PORT_SIZE;i++)
    {
        if(UART_PortList[i] == UARTx)
        {
            break;
        }
    }
    if(i==UART_PORT_SIZE)
    {
        i=0;
    }
    return i;
}
    
/*****************************************************************
@功能	根据指定参数初始化UART模块
@参数	UARTx - UART端口，UART1,UART2,UART3,UART4
@返回	无
******************************************************************/
static void HAL_UART_MspInit_Ex(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART1) 
    {
        /* Enable Clock */
        System_Module_Enable(EN_UART1);

        
        UART_IOInitCallbackList[0](); 

        /* NVIC Config */
        NVIC_ClearPendingIRQ(UART1_IRQn);
//        NVIC_SetPriority(UART1_IRQn, 5);
        NVIC_EnableIRQ(UART1_IRQn);
    }
    else if (huart->Instance == UART2) 
    {
        /* Enable Clock */
        System_Module_Enable(EN_UART2);

        
        UART_IOInitCallbackList[1](); 

        /* NVIC Config */
        NVIC_ClearPendingIRQ(UART2_IRQn);
//        NVIC_SetPriority(UART2_IRQn, 5);
        NVIC_EnableIRQ(UART2_IRQn);
    }
    else if (huart->Instance == UART3) 
    {
        /* Enable Clock */
        System_Module_Enable(EN_UART3);

        
        UART_IOInitCallbackList[2](); 

        /* NVIC Config */
        NVIC_ClearPendingIRQ(UART3_IRQn);
//        NVIC_SetPriority(UART3_IRQn, 5);
        NVIC_EnableIRQ(UART3_IRQn);
    }
    else if (huart->Instance == UART4) 
    {
        /* Enable Clock */
        System_Module_Enable(EN_UART4);

        
        UART_IOInitCallbackList[3](); 

        /* NVIC Config */
        NVIC_ClearPendingIRQ(UART4_IRQn);
//        NVIC_SetPriority(UART4_IRQn, 5);
        NVIC_EnableIRQ(UART4_IRQn);
    }
    
}

static void UART_Config_BaudRate_Ex(UART_HandleTypeDef *huart)
{
    uint32_t lu32_PCLK;
    uint32_t lu32_IBAUD, lu32_FBAUD;
    uint64_t lu64_TempValue;

    lu32_PCLK = System_Get_APBClock();

    /* Integral part */
    lu32_IBAUD = lu32_PCLK / (huart->Init.BaudRate * 16);

    /* Fractional part */
    lu64_TempValue = lu32_PCLK % (huart->Init.BaudRate * 16);
    lu64_TempValue = (lu64_TempValue * 1000000) / (huart->Init.BaudRate * 16);
    lu32_FBAUD     = (lu64_TempValue * 64 + 500000) / 1000000;

    if (lu32_FBAUD >= 64) 
    {
        huart->Instance->IBRD = lu32_IBAUD + 1;
        huart->Instance->FBRD = 0;
    }
    else 
    {
        huart->Instance->IBRD = lu32_IBAUD;
        huart->Instance->FBRD = lu32_FBAUD;
    }
}

static void HAL_UART_Init_Ex(UART_HandleTypeDef *huart)
{

#if (USE_FULL_ASSERT == 1)
    if (!IS_UART_ALL_INSTANCE(huart->Instance))                   return;
    if (!IS_UART_WORDLENGTH(huart->Init.WordLength))              return;
    if (!IS_UART_STOPBITS(huart->Init.StopBits))                  return;
    if (!IS_UART_PARITY(huart->Init.Parity))                      return;
    if (!IS_UART_MODE(huart->Init.Mode))                          return;
    if (!IS_UART_HARDWARE_FLOW_CONTROL(huart->Init.HwFlowCtl))    return;
#endif

    /* Init the low level hardware : GPIO, CLOCK, NVIC */

    HAL_UART_MspInit_Ex(huart);
    
    /* Config BaudRate */   
    UART_Config_BaudRate_Ex(huart);
    
    /* Set the UART Communication parameters */
    huart->Instance->LCRH = huart->Init.WordLength | UART_LCRH_FEN | huart->Init.StopBits | huart->Init.Parity;
    huart->Instance->CR = huart->Init.HwFlowCtl | huart->Init.Mode | UART_CR_UARTEN;
    
    extern UART_TypeDef    *Uart_Debug;
    if (huart->Init.Mode == UART_MODE_TX_RX_DEBUG) 
    {
        Uart_Debug = huart->Instance;
    }
    else if (huart->Init.Mode == UART_MODE_HALF_DUPLEX) 
    {
        huart->Instance->CR2 = UART_CR2_TXOE_SEL;
    }
}

/*****************************************************************
@功能 uart初始化
@参数 UARTx - UART端口，UART1,UART2,UART3,UART4
@参数 bps-波特率,比如 115200
@参数 stopBits-停止位:1/2
@参数 UARTx_IOInitCallback - 端口IO初始化回调函数
@返回 无
******************************************************************/
void UART_Init(UART_TypeDef *UARTx,uint32_t bps, uint8_t stopBits,void (*UARTx_IOInitCallback)(void), void (*UARTx_IRQCallback)(void))
{
    UART_HandleTypeDef uartHandle;
 
    uartHandle.Instance = UARTx;
    uartHandle.Init.BaudRate   = bps;
    uartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    if(stopBits==2){
        uartHandle.Init.StopBits = UART_STOPBITS_2;        
    }
    else{
        uartHandle.Init.StopBits = UART_STOPBITS_1;  
    } 
    
    uartHandle.Init.Parity     = UART_PARITY_NONE;
    uartHandle.Init.Mode       = UART_MODE_TX_RX;  
    uartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    
    UART_IOInitCallbackList[UART_GetIndex(UARTx)] = UARTx_IOInitCallback;
    UART_IRQCallbackList[UART_GetIndex(UARTx)] = UARTx_IRQCallback;    

    HAL_UART_Init_Ex(&uartHandle);

}

/*****************************************************************
@功能	uart接收中断使能
@参数	UARTx - UART端口，UART1,UART2,UART3,UART4
@返回	无
******************************************************************/
void UART_RxiEnable(UART_TypeDef *UARTx)
{
    /* Clear RXI Status */
    UARTx->ICR = UART_ICR_RXI;
    /* FIFO Disable */
    UARTx->LCRH &= ~UART_LCRH_FEN;
    /* Enable RX interrupt */
    UARTx->IE |= UART_IE_RXI;
}

/*****************************************************************
@功能	uart接收中断禁止
@参数	UARTx - UART端口，UART1,UART2,UART3,UART4
@返回	无
******************************************************************/
void UART_RxiDisable(UART_TypeDef *UARTx)
{
    UARTx->IE &= ~UART_IE_RXI;	
}


/*****************************************************************
@功能	uart发送单字节
@参数	UARTx - UART端口，UART1,UART2,UART3,UART4
@参数	c - 单字节数据
@返回	无
******************************************************************/
void UART_SendByte(UART_TypeDef *UARTx, uint8_t ch)
{
    UARTx->DR = ch;
    //while (UARTx->FR & UART_FR_TXFF);    
    while (!(UARTx->FR & UART_FR_TXFE));
}



/*****************************************************************
@功能	uart发送多字节数据
@参数	UARTx - UART端口，UART1,UART2,UART3,UART4
@参数	len - 数据长度
@参数	pdata - 数据地址
@返回	无
******************************************************************/
void UART_SendBytes(UART_TypeDef *UARTx, uint8_t *pdata, uint32_t len)
{
    while(len--)
    {
        UARTx->DR = *pdata++; 
        //while (UARTx->FR & UART_FR_TXFF);          
        while (!(UARTx->FR & UART_FR_TXFE));
    }
}

/*****************************************************************
@功能	uart把多字节数据转换为HEX发送
@参数	UARTx - UART端口，UART1,UART2,UART3,UART4
@参数	len - 数据长度
@参数	pdata - 数据地址
@返回	无
******************************************************************/
void UART_SendHexBytes(UART_TypeDef *UARTx, uint8_t *pdata, uint32_t len)
{
    char tempstr[8];
    while(len--)
    {
        sprintf(tempstr,"%02x ",*pdata++);
        UART_SendBytes(UARTx,(uint8_t *)tempstr,3);
    }
}


/*****************************************************************
@功能	Debug调试功能串口使能函数
@参数	UARTx - UART端口，UART1,UART2,UART3,UART4
@参数   enable - 1调试端口使能,0禁止
@返回	无
******************************************************************/
void DEBUG_UartEnable(UART_TypeDef *UARTx,uint8 enable)
{
    extern UART_TypeDef *Uart_Debug;
    if(enable)
    {
        Uart_Debug = UARTx;
    }
    else
    {
        Uart_Debug = NULL;
    }
    
}

/*****************************************************************
@功能	调试串口发送字节数据
@参数	pdata - 数据地址
@参数	len - 数据长度
@返回	无
******************************************************************/
void DEBUG_PrintBytes(uint8_t *pdata,int len)
{
#ifdef UART_DEBUG_ENABLE
    extern UART_TypeDef    *Uart_Debug;  
    if(Uart_Debug!=NULL)
    {
        UART_SendBytes(Uart_Debug,pdata,len);
        
    }
#endif

}

/*****************************************************************
@功能	调试串口把二进制数据转换为十六进字符串发送出去
@参数	pdata - 数据地址
@参数	len - 数据长度
@返回	无
******************************************************************/
void DEBUG_PrintHexBytes(uint8_t *pdata,int len)
{
#ifdef UART_DEBUG_ENABLE    
    extern UART_TypeDef    *Uart_Debug;  
    if(Uart_Debug!=NULL)
    {
        UART_SendHexBytes(Uart_Debug,pdata,len);
        
    }    
#endif    
    
}


