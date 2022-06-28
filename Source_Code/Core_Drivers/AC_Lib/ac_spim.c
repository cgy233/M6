/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_spim.c
@功能描述:	SPI模块函数。
@开发人员:	bobzhang
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:
修改日期		修改人   		修改说明

******************************************************************/

#include "ac_common.h"

#define SPI_PORT_SIZE          4
SPI_TypeDef *SPI_PortList[SPI_PORT_SIZE] = {SPI1, SPI2, SPI3, SPI4};

#define SPI_IO_INIT_CALLBACK_SIZE   SPI_PORT_SIZE
static void (*SPI_IOInitCallbackList[SPI_IO_INIT_CALLBACK_SIZE])(void) = { 0 };//SPI IO初始化回调函数数组

#define  SPI_TIMEOUT    10000            //单位while循环次数

/*****************************************************************
@功能	获取初始化回调函数数组索引
@参数	SPIx - SPI端口, SPI1,SPI2,SPI3,SPI4
@返回	索引号
******************************************************************/
static uint8_t SPI_GetIndex(SPI_TypeDef *SPIx)
{
    int i;
    for(i = 0; i < SPI_PORT_SIZE; i++)
    {
        if(SPI_PortList[i] == SPIx)
        {
            break;
        }
    }
    if(i == SPI_PORT_SIZE)
    {
        i = 0;
    }
    return i;
}

/*****************************************************************
@功能	根据指定参数初始化SPI模块
@参数	hspi - SPI1,SPI2,SPI3,SPI4
@返回	无
******************************************************************/
static void HAL_SPI_MspInit_Ex(SPI_HandleTypeDef *hspi)
{
    /* SPI1 */
    if (hspi->Instance == SPI1)
    {
        /* Enable Clock */
        System_Module_Enable(EN_SPI1);

        ((void (*)())(SPI_IOInitCallbackList[0]))();


        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(SPI1_IRQn);

        /* Enable External Interrupt */
        NVIC_EnableIRQ(SPI1_IRQn);
    }
    /* SPI2 */
    else if (hspi->Instance == SPI2)
    {
        /* Enable Clock */
        System_Module_Enable(EN_SPI2);

        ((void (*)())(SPI_IOInitCallbackList[1]))();


        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(SPI2_IRQn);

        /* Enable External Interrupt */
        NVIC_EnableIRQ(SPI2_IRQn);
    }
    /* SPI3 */
    else if (hspi->Instance == SPI3)
    {
        /* Enable Clock */
        System_Module_Enable(EN_SPI3);

        ((void (*)())(SPI_IOInitCallbackList[2]))();

        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(SPI3_IRQn);

        /* Enable External Interrupt */
        NVIC_EnableIRQ(SPI3_IRQn);
    }
    /* SPI4 */
    else if (hspi->Instance == SPI4)
    {
        /* Enable Clock */
        System_Module_Enable(EN_SPI4);

        ((void (*)())(SPI_IOInitCallbackList[3]))();

        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(SPI4_IRQn);

        /* Enable External Interrupt */
        NVIC_EnableIRQ(SPI4_IRQn);
    }

}

/*****************************************************************
@功能	SPI设置波特率
@参数	SPIx - SPI端口，SPI1,SPI2,SPI3,SPI4
@参数	bps-波特率
@返回	无
******************************************************************/
void SPI_SetBps(SPI_TypeDef *SPIx, uint32_t bps)
{
    uint32_t m_sysClock, div1, div2_add1;
    m_sysClock = System_Get_SystemClock() / 1000000;
    bps = bps / 1000000;

    // baud = Fhclk /  (div1*(div2+1)

    if(m_sysClock == 64)
    {
        if(bps >= 32)
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 1;
        }
        else if(bps >= 16)
        {
            div1 = 4;   //2-254之间的偶数
            div2_add1 = 1;
        }
        else if(bps >= 11)
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 3;
        }
        else if(bps >= 8)
        {
            div1 = 8;   //2-254之间的偶数
            div2_add1 = 1;
        }
        else if(bps >= 4)
        {
            div1 = 16;   //2-254之间的偶数
            div2_add1 = 1;
        }
        else // if(bps>=1)
        {
            div1 = 64;   //2-254之间的偶数
            div2_add1 = 1;
        }
    }
    else if(m_sysClock == 32)
    {
        if(bps >= 16)
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 1;
        }
        else if(bps >= 11)
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 3;
        }
        else if(bps >= 8)
        {
            div1 = 4;   //2-254之间的偶数
            div2_add1 = 1;
        }
        else if(bps >= 4)
        {
            div1 = 8;   //2-254之间的偶数
            div2_add1 = 1;
        }
        else // if(bps>=1)
        {
            div1 = 32;   //2-254之间的偶数
            div2_add1 = 1;
        }
    }
    else if(m_sysClock == 16)
    {
        if(bps >= 8)
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 1;
        }
        else if(bps >= 4)
        {
            div1 = 4;   //2-254之间的偶数
            div2_add1 = 1;
        }
        else // if(bps>=1)
        {
            div1 = 16;   //2-254之间的偶数
            div2_add1 = 1;
        }
    }
    else if(m_sysClock == 8)
    {
        if(bps >= 4)
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 1;
        }
        else // if(bps>=1)
        {
            div1 = 8;   //2-254之间的偶数
            div2_add1 = 1;
        }
    }
    else //if(sysClock == 180000000)
    {
        if(bps >= 45)
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 2;
        }
        else if(bps >= 30)
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 3;
        }
        else if(bps >= 22) //22.5
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 4;
        }
        else if(bps >= 18)
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 5;
        }
        else if(bps >= 15)
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 6;
        }
        else if(bps >= 12) //11.25
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 8;
        }
        else if(bps >= 10)
        {
            div1 = 6;   //2-254之间的偶数
            div2_add1 = 3;
        }
        else if(bps >= 8) //8.18M
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 11;
        }
        else if(bps >= 4)
        {
            div1 = 2;   //2-254之间的偶数
            div2_add1 = 23;
        }
        else // if(bps>=1)
        {
            div1 = 180;   //2-254之间的偶数
            div2_add1 = 1;
        }
    }

    SPIx->BAUD = div1 | ((div2_add1 - 1) << 8);

}

/*****************************************************************
@功能	SPI设置线模式
@参数	SPIx - SPI端口: SPI1,SPI2,SPI3,SPI4
@参数	xMode-几线: SPI_1X_MODE,SPI_2X_MODE,SPI_4X_MODE
@返回	无
******************************************************************/
void SPI_SetXMode(SPI_TypeDef *SPIx, uint32_t xMode)
{
    /* Set SPI X_Mode */
    SPIx->CTL = ((SPIx->CTL) & (~SPI_CTL_X_MODE)) | xMode;
}

/************************************************************************
 * function   : HAL_SPI_Init
 * Description: SPI initial with parameters.
 * input      : hspi : pointer to a SPI_HandleTypeDef structure that contains
 *                     the configuration information for SPI module
 ************************************************************************/
static HAL_StatusTypeDef HAL_SPI_Init_Ex(SPI_HandleTypeDef *hspi)
{
    /* Check SPI Parameter */
    if (!IS_SPI_ALL_INSTANCE(hspi->Instance))                         return HAL_ERROR;
    if (!IS_SPI_ALL_MODE(hspi->Init.SPI_Mode))                        return HAL_ERROR;
    if (!IS_SPI_WORK_MODE(hspi->Init.SPI_Work_Mode))                  return HAL_ERROR;
    if (!IS_SPI_X_MODE(hspi->Init.X_Mode))                            return HAL_ERROR;
    if (!IS_SPI_FIRST_BIT(hspi->Init.First_Bit))                      return HAL_ERROR;
    if (!IS_SPI_BAUDRATE_PRESCALER(hspi->Init.BaudRate_Prescaler))    return HAL_ERROR;

    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    HAL_SPI_MspInit_Ex(hspi);

    /* Automatic change direction */
    hspi->Instance->CTL |= (SPI_CTL_IO_MODE);

    /* Set SPI Work mode */
    if (hspi->Init.SPI_Mode == SPI_MODE_MASTER)
    {
        SET_BIT(hspi->Instance->CTL, SPI_CTL_MST_MODE);
    }
    else
    {
        CLEAR_BIT(hspi->Instance->CTL, SPI_CTL_MST_MODE);

        hspi->Instance->BATCH = (hspi->Instance->BATCH & (~0x000FFFFFU)) | (1 << 0);

        hspi->Instance->TX_CTL |= SPI_TX_CTL_MODE;

        if (hspi->Init.X_Mode != SPI_1X_MODE)
        {
            hspi->Instance->CTL |= SPI_CTL_SFILTER;
        }

        /* Slave Alternate Enable */
        hspi->Instance->CTL |= SPI_CTL_SLAVE_EN;

        /* Slave Mode Enable Rx By Default */
        hspi->Instance->RX_CTL |= SPI_RX_CTL_EN;
    }

    /* Set SPI First Bit */
    if (hspi->Init.First_Bit == SPI_FIRSTBIT_LSB)
        SET_BIT(hspi->Instance->CTL, SPI_CTL_LSB_FIRST);
    else
        CLEAR_BIT(hspi->Instance->CTL, SPI_CTL_LSB_FIRST);

    /* Set SPI Wprk Mode */
    hspi->Instance->CTL = ((hspi->Instance->CTL) & (~(SPI_CTL_CPHA | SPI_CTL_CPOL))) | (hspi->Init.SPI_Work_Mode);

    /* Set SPI X_Mode */
    hspi->Instance->CTL = ((hspi->Instance->CTL) & (~SPI_CTL_X_MODE)) | (hspi->Init.X_Mode);

    /* Set SPI BaudRate Prescaler */
    hspi->Instance->BAUD = ((hspi->Instance->BAUD) & (~0x0000FFFF)) | (hspi->Init.BaudRate_Prescaler);

    /* Disable All Interrupt */
    hspi->Instance->IE = 0x00000000;

    return HAL_OK;
}

/*****************************************************************
@功能	SPI初始化
@参数	SPIx - SPI端口: SPI1,SPI2,SPI3,SPI4
@参数	workMode-工作模式: SPI_WORK_MODE_0,SPI_WORK_MODE_1,SPI_WORK_MODE_2，SPI_WORK_MODE_3
@参数	xMode-几线: SPI_1X_MODE,SPI_2X_MODE,SPI_4X_MODE
@参数	bps-波特率
@返回	无
******************************************************************/
void SPIM_Init(SPI_TypeDef *SPIx, uint32_t workMode, uint32_t xMode, uint32_t bps, void (*SPIx_IOInitCallback)(void))
{
    SPI_HandleTypeDef SPIx_Handle;

    SPIx_Handle.Instance                 = SPIx;
    SPIx_Handle.Init.SPI_Mode            = SPI_MODE_MASTER;
    SPIx_Handle.Init.SPI_Work_Mode       = workMode;
    SPIx_Handle.Init.X_Mode              = xMode;
    SPIx_Handle.Init.First_Bit           = SPI_FIRSTBIT_MSB;
    SPIx_Handle.Init.BaudRate_Prescaler  = SPI_BAUDRATE_PRESCALER_64;

    SPI_IOInitCallbackList[SPI_GetIndex(SPIx)] = SPIx_IOInitCallback;
    HAL_SPI_Init_Ex(&SPIx_Handle);

    SPI_SetBps(SPIx, bps);
}

/*****************************************************************
@功能	SPI释放CS
@参数	SPIx - SPI端口，SPI1,SPI2,SPI3,SPI4
@参数	无
@返回	无
******************************************************************/
void SPIM_ReleaseCS(SPI_TypeDef *SPIx)
{
    /* Transmit End */
    SPIx->CS &= (~SPI_CS_CS0);
}

/*****************************************************************
@功能	SPI发送数据。硬件自动选择CS，发送结束后CS继续有效。所有收发操作结束后，需要调用SPIM_ReleaseCS()函数释放CS。
@参数	SPIx - SPI端口，SPI1,SPI2,SPI3,SPI4
@参数	txData - 数据地址
@参数	txLen - 数据长度
@返回	AC_OK-成功，AC_FAIL-失败
******************************************************************/
uint8_t SPIM_TxBytes_KeepCS(SPI_TypeDef *SPIx, uint8_t *txData, uint32_t txLen)
{
    int timeout;
    uint8_t Status = AC_OK;

    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Clear TX FIFO */
    SET_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);

    /* Set Data Size */
    SPIx->BATCH = txLen;

    /* Tx Enable */
    SPIx->TX_CTL |= SPI_TX_CTL_EN;

    /* Transmit Start */
    SPIx->CS |= SPI_CS_CS0;

    //为提高效率，先发一字节
    SPIx->DAT = *txData++;
    txLen--;

    while(txLen--)
    {
        /* Wait Tx FIFO Not Full */
        timeout = SPI_TIMEOUT;
        while (SPIx->STATUS & SPI_STATUS_TX_FIFO_FULL)
        {
            if((timeout--) <= 0)
            {
                Status = AC_TIMEOUT;
                goto TxBytes_KeepCSEnd;
            }

        }
        SPIx->DAT = *txData++;
    }

    /* Wait Transmit Done */
    timeout = SPI_TIMEOUT;
    while (!(SPIx->STATUS & SPI_STATUS_TX_BATCH_DONE))
    {
        if((timeout--) <= 0)
        {
            Status = AC_TIMEOUT;
            goto TxBytes_KeepCSEnd;
        }
    }

TxBytes_KeepCSEnd:
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Tx Disable */
    SPIx->TX_CTL &= (~SPI_TX_CTL_EN);

    /* Transmit End */
    if(Status == AC_TIMEOUT)
    {
        SPIx->CS &= (~SPI_CS_CS0);
    }

    return Status;
}

/*****************************************************************
@功能	SPI接收数据。硬件自动选择CS，接收结束后CS继续有效。所有收发操作结束后，需要调用SPIM_ReleaseCS()函数释放CS。
@参数	SPIx - SPI端口，SPI1,SPI2,SPI3,SPI4
@参数	rxData - 接收数据地址
@参数	rxLen - 发送数据长度
@返回	AC_OK-成功，AC_FAIL-失败
******************************************************************/
uint8_t SPIM_RxBytes_KeepCS(SPI_TypeDef *SPIx, uint8_t *rxData, uint32_t rxLen)
{
    int timeout;
    uint8_t Status = AC_OK;
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Clear RX FIFO */
    SET_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET); 
    
    /* Set Data Size */
    SPIx->BATCH = rxLen;

    /* Rx Enable */
    SPIx->RX_CTL |= SPI_RX_CTL_EN;

    /* Transmit Start */
    SPIx->CS |= SPI_CS_CS0;

    while(rxLen--)
    {
        /* Wait Rx FIFO Not Empty */
        timeout = SPI_TIMEOUT;
        while (SPIx->STATUS & SPI_STATUS_RX_FIFO_EMPTY)
        {
            if((timeout--) <= 0)
            {
                Status = AC_TIMEOUT;
                goto RxBytes_KeepCSEnd;
            }
        }
        *rxData++ = SPIx->DAT;
    }

    /* Wait Transmit Done */
    timeout = SPI_TIMEOUT;
    while (!(SPIx->STATUS & SPI_STATUS_BATCH_DONE))
    {
        if((timeout--) <= 0)
        {
            Status = AC_TIMEOUT;
            goto RxBytes_KeepCSEnd;
        }
    }
RxBytes_KeepCSEnd:
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Rx Disable */
    SPIx->RX_CTL &= (~SPI_RX_CTL_EN);

    /* Transmit End */
    if(Status == AC_TIMEOUT)
    {
        SPIx->CS &= (~SPI_CS_CS0);
    }

    return Status;
}

/*****************************************************************
@功能	SPI同时收发数据。硬件自动选择CS，结束后CS继续有效。所有收发操作结束后，需要调用SPIM_ReleaseCS()函数释放CS。
@参数	SPIx - SPI端口，SPI1,SPI2,SPI3,SPI4
@参数	txData - 发送数据地址
@参数	rxData - 接收数据地址
@参数	len - 发送数据长度
@返回	AC_OK-成功，AC_FAIL-失败
******************************************************************/
uint8_t SPIM_TxRxBytes_KeepCS(SPI_TypeDef *SPIx, uint8_t *txData, uint8_t *rxData, uint32_t len)
{
    int timeout;
    uint32_t txLen, rxLen;
    uint8_t Status = AC_OK;

    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Clear TX FIFO */
    SET_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);
    
    /* Clear RX FIFO */
    SET_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET); 
    
    /* Set Data Size */
    SPIx->BATCH = len;
    txLen = len;
    rxLen = len;
 
    /* Tx Enable */
    SPIx->TX_CTL |= SPI_TX_CTL_EN;

    /* Rx Enable */
    SPIx->RX_CTL |= SPI_RX_CTL_EN;

    /* Transmit Start */
    SPIx->CS |= SPI_CS_CS0;

    //为提高效率，先发一字节
    SPIx->DAT = *txData++;
    txLen--;
    
    timeout = SPI_TIMEOUT;
    while(1)
    {
        /* Wait Tx FIFO Not Full */
        if ((SPIx->STATUS & (SPI_STATUS_TX_FIFO_FULL | SPI_STATUS_RX_FIFO_FULL))==0)
        {
            if(txLen>0)
            {
                SPIx->DAT = *txData++; 
                txLen--;
                timeout = SPI_TIMEOUT;
            }
        } 
        
        /* Wait Rx FIFO Not Empty */
        if( (SPIx->STATUS & SPI_STATUS_RX_FIFO_EMPTY) == 0)
        {
            if(rxLen>0)
            {
                *rxData++ = SPIx->DAT; 
                rxLen--;
                timeout = SPI_TIMEOUT;
            }         
        }
        
        if((txLen==0) && (rxLen==0))
            break;
        
        if((timeout--) <= 0)
        {
            Status = AC_TIMEOUT;
            goto TxRxBytes_END;
        }
    }
    
    /* Wait Transmit Done */
    timeout = SPI_TIMEOUT;
    while (!(SPIx->STATUS & SPI_STATUS_BATCH_DONE))
    {
    	if((timeout--) <= 0)
    	{
            Status = AC_TIMEOUT;
            goto TxRxBytes_END;
    	}
    }
TxRxBytes_END:
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);
    /* Tx Disable */
    SPIx->TX_CTL &= (~SPI_TX_CTL_EN);

    /* Rx Disable */
    SPIx->RX_CTL &= (~SPI_RX_CTL_EN);

    /* Transmit End */
    if(Status == AC_TIMEOUT)
    {
        SPIx->CS &= (~SPI_CS_CS0);
    }
    return Status;
}

/*****************************************************************
@功能	SPI接收数据。支持硬件自动选择与释放CS。
@参数	SPIx - SPI端口，SPI1,SPI2,SPI3,SPI4
@参数	rxData - 接收数据地址
@参数	rxLen - 发送数据长度
@返回	AC_OK-成功，AC_FAIL-失败
******************************************************************/
uint8_t SPIM_RxBytes(SPI_TypeDef *SPIx, uint8_t *rxData, uint32_t rxLen)
{
    int timeout;
    uint8_t Status = AC_OK;
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Clear RX FIFO */
    SET_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET); 
    
    /* Set Data Size */
    SPIx->BATCH = rxLen;

    /* Rx Enable */
    SPIx->RX_CTL |= SPI_RX_CTL_EN;

    /* Transmit Start */
    SPIx->CS |= SPI_CS_CS0;

    while(rxLen--)
    {
        /* Wait Rx FIFO Not Empty */
        timeout = SPI_TIMEOUT;
        while (SPIx->STATUS & SPI_STATUS_RX_FIFO_EMPTY)
        {
            if((timeout--) <= 0)
            {
                Status = AC_TIMEOUT;
                goto RxBytes_End;
            }
        }
        *rxData++ = SPIx->DAT;
    }

    /* Wait Transmit Done */
    timeout = SPI_TIMEOUT;
    while (!(SPIx->STATUS & SPI_STATUS_BATCH_DONE))
    {
        if((timeout--) <= 0)
        {
            Status = AC_TIMEOUT;
            goto RxBytes_End;
        }
    }
RxBytes_End:
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Rx Disable */
    SPIx->RX_CTL &= (~SPI_RX_CTL_EN);

    /* Transmit End */
    SPIx->CS &= (~SPI_CS_CS0);

    return  Status;
}
/*****************************************************************
@功能	SPI发送数据，不限长度。支持硬件自动选择与释放CS。
@参数	SPIx - SPI端口，SPI1,SPI2,SPI3,SPI4
@参数	txData - 数据地址
@参数	txLen - 数据长度
@返回	AC_OK-成功，AC_FAIL-失败
******************************************************************/
uint8_t SPIM_TxBytes(SPI_TypeDef *SPIx, uint8_t *txData, uint32_t txLen)
{
    int timeout;
    uint8_t Status = AC_OK;

    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Clear TX FIFO */
    SET_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);

    /* Set Data Size */
    SPIx->BATCH = txLen;

    /* Tx Enable */
    SPIx->TX_CTL |= SPI_TX_CTL_EN;

    /* Transmit Start */
    SPIx->CS |= SPI_CS_CS0;

    //为提高效率，先发一字节
    SPIx->DAT = *txData++;
    txLen--;

    while(txLen--)
    {
        /* Wait Tx FIFO Not Full */
        timeout = SPI_TIMEOUT;
        while (SPIx->STATUS & SPI_STATUS_TX_FIFO_FULL)
        {
            if((timeout--) <= 0)
            {
                Status = AC_TIMEOUT;
                goto TxBytes_End;
            }
        }
        SPIx->DAT = *txData++;
    }

    /* Wait Transmit Done */
    timeout = SPI_TIMEOUT;
    while (!(SPIx->STATUS & SPI_STATUS_BATCH_DONE))
    {
        if((timeout--) <= 0)
        {
            Status = AC_TIMEOUT;
            goto TxBytes_End;
        }
    }
TxBytes_End:
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Tx Disable */
    SPIx->TX_CTL &= (~SPI_TX_CTL_EN);

    /* Transmit End */
    SPIx->CS &= (~SPI_CS_CS0);

    return Status;
}

/*****************************************************************
@功能	SPI发送两段数据。支持硬件自动选择与释放CS。
@参数	SPIx - SPI端口，SPI1,SPI2,SPI3,SPI4
@参数	txData1 - 数据地址
@参数	txLen1 - 数据长度
@参数	txData2 - 数据地址
@参数	txLen2 - 数据长度，不限长度
@返回	AC_OK-成功，AC_FAIL-失败
******************************************************************/
uint8_t SPIM_TxBytes_TxBytes(SPI_TypeDef *SPIx, uint8_t *txData1, uint32_t txLen1, uint8_t *txData2, uint32_t txLen2)
{
    int timeout;
    uint8_t Status = AC_OK;
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Clear TX FIFO */
    SET_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);

    /* Set Data Size */
    SPIx->BATCH = txLen1 + txLen2;

    /* Tx Enable */
    SPIx->TX_CTL |= SPI_TX_CTL_EN;

    /* Transmit Start */
    SPIx->CS |= SPI_CS_CS0;

    //为提高效率，先发一字节
    SPIx->DAT = *txData1++;
    txLen1--;

    while(txLen1--)
    {
        /* Wait Tx FIFO Not Full */
        timeout = SPI_TIMEOUT;
        while (SPIx->STATUS & SPI_STATUS_TX_FIFO_FULL)
        {
            if((timeout--) <= 0)
            {
                Status = AC_TIMEOUT;
                goto TxBytesLE16_TxBytes_End;
            }
        }
        SPIx->DAT = *txData1++;
    }

    while(txLen2--)
    {
        /* Wait Tx FIFO Not Full */
        timeout = SPI_TIMEOUT;
        while (SPIx->STATUS & SPI_STATUS_TX_FIFO_FULL)
        {
            if((timeout--) <= 0)
            {
                Status = AC_TIMEOUT;
                goto TxBytesLE16_TxBytes_End;
            }
        }
        SPIx->DAT = *txData2++;
    }

    /* Wait Transmit Done */
    timeout = SPI_TIMEOUT;
    while (!(SPIx->STATUS & SPI_STATUS_BATCH_DONE))
    {
        if((timeout--) <= 0)
        {
            Status = AC_TIMEOUT;
            goto TxBytesLE16_TxBytes_End;
        }
    }
TxBytesLE16_TxBytes_End:
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Tx Disable */
    SPIx->TX_CTL &= (~SPI_TX_CTL_EN);

    /* Transmit End */
    SPIx->CS &= (~SPI_CS_CS0);

    return Status;
}

/*****************************************************************
@功能	SPI发送先发送再接收数据。支持硬件自动选择与释放CS。
@参数	SPIx - SPI端口，SPI1,SPI2,SPI3,SPI4
@参数	txData - 发送数据地址
@参数	txLen - 发送数据长度
@参数	rxData - 接收数据地址
@参数	rxLen - 接收数据长度，不限长度
@返回	AC_OK-成功，AC_FAIL-失败
******************************************************************/
uint8_t SPIM_TxBytes_RxBytes(SPI_TypeDef *SPIx, uint8_t *txData, uint32_t txLen, uint8_t *rxData, uint32_t rxLen)
{
    int timeout;
    uint8_t Status = AC_OK;
    
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_TX_BATCH_DONE);
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Clear TX FIFO */
    SET_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);
    
    /* Clear RX FIFO */
    SET_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET); 
    
    /* Set Data Size */
    SPIx->BATCH = txLen;

    /* Tx Enable */
    SPIx->TX_CTL |= SPI_TX_CTL_EN;


    /* Transmit Start */
    SPIx->CS |= SPI_CS_CS0;

    //为提高效率，先发一字节
    SPIx->DAT = *txData++;
    txLen--;

    while(txLen--)
    {
        /* Wait Tx FIFO Not Full */
        timeout = SPI_TIMEOUT;        
        while (SPIx->STATUS & SPI_STATUS_TX_FIFO_FULL)
        {
            if((timeout--) <= 0)
            {
                /* Clear Batch Done Flag  */
                SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

                /* Tx Disable */
                SPIx->TX_CTL &= (~SPI_TX_CTL_EN);

                /* Transmit End */
                SPIx->CS &= (~SPI_CS_CS0);

                return AC_TIMEOUT;
            }  
        }        
        SPIx->DAT = *txData++;
    }

    /* Wait Transmit Done */
    timeout = SPI_TIMEOUT;
    while (!(SPIx->STATUS & SPI_STATUS_BATCH_DONE))
    {
        if((timeout--) <= 0)
        {
            /* Clear Batch Done Flag  */
            SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

            /* Tx Disable */
            SPIx->TX_CTL &= (~SPI_TX_CTL_EN);

            /* Transmit End */
            SPIx->CS &= (~SPI_CS_CS0);

            return AC_TIMEOUT;
        }
    }

    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Tx Disable */
    SPIx->TX_CTL &= (~SPI_TX_CTL_EN);

    /* Set Data Size */
    SPIx->BATCH = rxLen;

    /* Rx Enable */
    SPIx->RX_CTL |= SPI_RX_CTL_EN;

    /* recv reStart */
    SPIx->CS |= SPI_CS_CS0;

    while(rxLen--)
    {
        /* Wait Rx FIFO Not Empty */
        timeout = SPI_TIMEOUT;
        while (SPIx->STATUS & SPI_STATUS_RX_FIFO_EMPTY)
        {
            if((timeout--) <= 0)
            {
                Status = AC_TIMEOUT;
                goto TxBytesLE16_RxBytes_End;
            }
        }
        *rxData++ = SPIx->DAT;
    }

    /* Wait Transmit Done */
    timeout = SPI_TIMEOUT;
    while (!(SPIx->STATUS & SPI_STATUS_BATCH_DONE))
    {
        if((timeout--) <= 0)
        {
            Status = AC_TIMEOUT;
            goto TxBytesLE16_RxBytes_End;
        }
    }
TxBytesLE16_RxBytes_End:
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Rx Disable */
    SPIx->RX_CTL &= (~SPI_RX_CTL_EN);

    /* Transmit End */
    SPIx->CS &= (~SPI_CS_CS0);

    return Status;
}

/*****************************************************************
@功能	SPI同时收发数据。支持硬件自动选择与释放CS。
@参数	SPIx - SPI端口，SPI1,SPI2,SPI3,SPI4
@参数	txData - 发送数据地址
@参数	rxData - 接收数据地址
@参数	len - 发送数据长度
@返回	AC_OK-成功，AC_FAIL-失败
******************************************************************/
uint8_t SPIM_TxRxBytes(SPI_TypeDef *SPIx, uint8_t *txData, uint8_t *rxData, uint32_t len)
{
    int timeout;
    uint32_t txLen, rxLen;
    uint8_t Status = AC_OK;
    uint32_t starttick, passtick;

    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);

    /* Clear TX FIFO */
    SET_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);
    
    /* Clear RX FIFO */
    SET_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET);  
    
    /* Set Data Size */
    SPIx->BATCH = len;
    txLen = len;
    rxLen = len;
   
    /* Tx Enable */
    SPIx->TX_CTL |= SPI_TX_CTL_EN;

    /* Rx Enable */
    SPIx->RX_CTL |= SPI_RX_CTL_EN;

    /* Transmit Start */
    SPIx->CS |= SPI_CS_CS0;

    //为提高效率，先发一字节
    SPIx->DAT = *txData++;
    txLen--;
    
    timeout = SPI_TIMEOUT;
    while(1)
    {
        /* Wait Tx FIFO Not Full */
        if ((SPIx->STATUS & (SPI_STATUS_TX_FIFO_FULL | SPI_STATUS_RX_FIFO_FULL))==0)
        {
            if(txLen>0)
            {
                SPIx->DAT = *txData++; 
                txLen--;
                timeout = SPI_TIMEOUT;
            }
        } 
        
        /* Wait Rx FIFO Not Empty */
        if( (SPIx->STATUS & SPI_STATUS_RX_FIFO_EMPTY) == 0)
        {
            if(rxLen>0)
            {
                *rxData++ = SPIx->DAT; 
                rxLen--;
                timeout = SPI_TIMEOUT;
            }         
        }
        
        if((txLen==0) && (rxLen==0))
            break;
        
        if((timeout--) <= 0)
        {
            Status = AC_TIMEOUT;
            goto TxRxBytes_END;
        }
    }
    
    /* Wait Transmit Done */
    timeout = SPI_TIMEOUT;
    while (!(SPIx->STATUS & SPI_STATUS_BATCH_DONE))
    {
    	if((timeout--) <= 0)
    	{
            Status = AC_TIMEOUT;
            goto TxRxBytes_END;
    	}
    }
TxRxBytes_END:
    /* Clear Batch Done Flag  */
    SET_BIT(SPIx->STATUS, SPI_STATUS_BATCH_DONE);
    /* Tx Disable */
    SPIx->TX_CTL &= (~SPI_TX_CTL_EN);

    /* Rx Disable */
    SPIx->RX_CTL &= (~SPI_RX_CTL_EN);

    /* Transmit End */
    SPIx->CS &= (~SPI_CS_CS0);

    return Status;
}


