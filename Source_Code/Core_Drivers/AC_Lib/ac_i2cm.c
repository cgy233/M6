/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@�ļ�����:	ac_i2c.c
@��������:	I2Cģ�麯����
@������Ա:	samliu
@�������:	2021.1.18
@��ǰ�汾:	1.0
@�޸ļ�¼:
�޸�����		�޸���   		�޸�˵��

******************************************************************/
#include "ac_common.h"

#define I2C_TIMEOUT    500            //��λus

#define I2C_PORT_SIZE          2
I2C_TypeDef *I2C_PortList[I2C_PORT_SIZE] = {I2C1,I2C2};

#define I2C_IO_INIT_CALLBACK_SIZE   I2C_PORT_SIZE
static void (*I2C_IOInitCallbackList[I2C_IO_INIT_CALLBACK_SIZE])(void) = { 0 };//I2C IO��ʼ���ص���������

#define  I2C_GET_FLAG(REG, BIT)           ((((REG) & (BIT)) == (BIT) ) ? 1 : 0) 

/*****************************************************************
@����	I2C��ʱ���
@����	timeout - ��ʱʱ��(��λus)
@����	AC_OK-������AC_TIMEOUT-��ʱ
******************************************************************/
static uint8_t I2C_Timeout_Check(I2C_TypeDef *I2Cx, uint32_t Flag, FlagStatus Status, uint32_t timeout)
{
    uint32_t starttick,passtick;
    starttick = get_Systick();
    while (I2C_GET_FLAG(I2Cx->SR, Flag) == Status)
    {
        passtick = get_us_delay(starttick);
        if(passtick > timeout)
        {
            return AC_TIMEOUT;
        }
    }
    return AC_OK;
}

/*****************************************************************
@����	��ȡ��ʼ���ص�������������
@����	I2Cx - I2C�˿�,I2C1,I2C2
@����	������
******************************************************************/
static uint8_t I2C_GetIndex(I2C_TypeDef *I2Cx)
{
    I2C_TypeDef *I2C_PortList[I2C_PORT_SIZE] = {I2C1,I2C2};
    int i;
    for(i=0;i<I2C_PORT_SIZE;i++)
    {
        if(I2C_PortList[i] == I2Cx)
        {
            break;
        }
    }
    if(i==I2C_PORT_SIZE)
    {
        i=0;
    }
    return i;
}
    
/*****************************************************************
@����	����ָ��������ʼ��I2Cģ��
@����	hi2c - I2C1,I2C2
@����	��
******************************************************************/
static void HAL_I2C_MspInit_Ex(I2C_HandleTypeDef *hi2c)
{
    /* I2C1 */
    if (hi2c->Instance == I2C1)
    {
        /* Enable Clock */
        System_Module_Enable(EN_I2C1);

        ((void (*)())(I2C_IOInitCallbackList[0]))();
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(I2C1_IRQn);
        
        /* Enable External Interrupt */
        NVIC_EnableIRQ(I2C1_IRQn);
    }
    /* I2C2 */
    else if (hi2c->Instance == I2C2) 
    {
        /* Enable Clock */
        System_Module_Enable(EN_I2C2);

        ((void (*)())(I2C_IOInitCallbackList[1]))();
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(I2C2_IRQn);
        
        /* Enable External Interrupt */
        NVIC_DisableIRQ(I2C2_IRQn);
    }
}

static HAL_StatusTypeDef I2C_Set_Clock_Speed_Ex(I2C_TypeDef *Instance, uint32_t ClockSpeed)
{
    uint32_t APB_Clock;

    APB_Clock = System_Get_APBClock();

    Instance->CLK_DIV = APB_Clock / (4 * ClockSpeed) - 1;

    return HAL_OK;
}

/************************************************************************
 * function   : HAL_I2C_Init
 * Description: I2c initial with parameters. 
 * input      : hi2c : pointer to a I2C_HandleTypeDef structure that contains
 *                     the configuration information for I2C module
 ************************************************************************/
static HAL_StatusTypeDef HAL_I2C_Init_Ex(I2C_HandleTypeDef *hi2c)
{
    /* Check I2C Parameter */
    if (!IS_I2C_ALL_INSTANCE(hi2c->Instance))              return HAL_ERROR;
    if (!IS_I2C_ALL_MODE(hi2c->Init.I2C_Mode))             return HAL_ERROR;
    if (!IS_I2C_CLOCK_SPEED(hi2c->Init.Clock_Speed))       return HAL_ERROR;
    if (!IS_I2C_TX_AUTO_EN(hi2c->Init.Tx_Auto_En))         return HAL_ERROR;
    if (!IS_I2C_STRETCH_EN(hi2c->Init.No_Stretch_Mode))    return HAL_ERROR;

    /* Disable the selected I2C peripheral */
    CLEAR_BIT(hi2c->Instance->CR, I2C_CR_MEN); 

    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    HAL_I2C_MspInit_Ex(hi2c);

    switch (hi2c->Init.I2C_Mode)
    {
        /* Master Mode */
        case I2C_MODE_MASTER:
        {
            /* Set Master Mode */
            SET_BIT(hi2c->Instance->CR, I2C_CR_MASTER);
            
            /* Set Clock Speed */
            I2C_Set_Clock_Speed_Ex(hi2c->Instance, hi2c->Init.Clock_Speed);
            
            /* Set SDA auto change the direction */
            if (hi2c->Init.Tx_Auto_En == TX_AUTO_EN_ENABLE)
                SET_BIT(hi2c->Instance->CR, I2C_CR_TX_AUTO_EN);
            else
                CLEAR_BIT(hi2c->Instance->CR, I2C_CR_TX_AUTO_EN);
            
            /* Enable the selected I2C peripheral */
            SET_BIT(hi2c->Instance->CR, I2C_CR_MEN);
        }break;
        
        /* Slave Mode */
        case I2C_MODE_SLAVE: 
        {
            SET_BIT(hi2c->Instance->CR, I2C_CR_TXE_SEL);

            /* Set SDA auto change the direction */
            if (hi2c->Init.Tx_Auto_En == TX_AUTO_EN_ENABLE)
                SET_BIT(hi2c->Instance->CR, I2C_CR_TX_AUTO_EN);
            else
                CLEAR_BIT(hi2c->Instance->CR, I2C_CR_TX_AUTO_EN);
            
            /* Set Clock Stretch Mode */
            if (hi2c->Init.No_Stretch_Mode == NO_STRETCH_MODE_NOSTRETCH)
                SET_BIT(hi2c->Instance->CR, I2C_CR_NOSTRETCH);
            else
                CLEAR_BIT(hi2c->Instance->CR, I2C_CR_NOSTRETCH);

            /* Set Address 1 */
            hi2c->Instance->SLAVE_ADDR1 = hi2c->Init.Own_Address;
            
            /* Enable the selected I2C peripheral */
            SET_BIT(hi2c->Instance->CR, I2C_CR_MEN);
        }break;
        
        default: break; 
    }

    return HAL_OK;
}

/*****************************************************************
@����	I2C��ʼ��
@����	I2Cx - I2C�˿�: I2C1,I2C2
@����	clock_Speed-��������: CLOCK_SPEED_STANDARD,CLOCK_SPEED_FAST,CLOCK_SPEED_FAST_PLUS
@����	I2Cx_IOInitCallback-I2C IO��ʼ���ص�����
@����	��
******************************************************************/
void I2CM_Init(I2C_TypeDef *I2Cx,uint32_t clock_Speed,void (*I2Cx_IOInitCallback)(void))
{
    I2C_HandleTypeDef I2Cx_Handle;
    
    I2Cx_Handle.Instance         = I2Cx;
    I2Cx_Handle.Init.I2C_Mode    = I2C_MODE_MASTER;
    I2Cx_Handle.Init.Tx_Auto_En  = TX_AUTO_EN_ENABLE;
    I2Cx_Handle.Init.Clock_Speed = clock_Speed;
    I2Cx_Handle.Init.No_Stretch_Mode = NO_STRETCH_MODE_STRETCH;

    I2C_IOInitCallbackList[I2C_GetIndex(I2Cx)] = I2Cx_IOInitCallback;

    HAL_I2C_Init_Ex(&I2Cx_Handle);
}

/*****************************************************************
@����	I2C��������
@����	I2Cx -I2C�˿ڣ�I2C1,I2C2
@����	devAddress -�ӻ���ַ
@����	txData - �������ݻ�����
@����	txLen - Ҫ���͵����ݳ���
@����	AC_OK-�ɹ���AC_FAIL-ʧ��
******************************************************************/
uint8_t I2CM_TxBytes(I2C_TypeDef *I2Cx, uint16_t devAddress,uint8_t *txData, uint32_t txLen)
{
    uint32_t i;
    /* Generate Start */
    SET_BIT(I2Cx->CR, I2C_CR_START);

    /* Clear MTF, To Prevent Errors */
    I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);

    /* Send Device Address */
    I2Cx->DR = devAddress & 0xFE;

    /* Wait for transmission End*/
    if(I2C_Timeout_Check(I2Cx, I2C_SR_MTF, RESET, I2C_TIMEOUT))
    {
        return AC_TIMEOUT_ADDR;
    }
    /* Clear MTF */
    I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);

    /* Get NACK */
    if (READ_BIT(I2Cx->SR, I2C_SR_RACK))
    {
        /* Generate Stop */
        SET_BIT(I2Cx->CR, I2C_CR_STOP);

        /* Wait for the bus to idle */
        if(I2C_Timeout_Check(I2Cx, I2C_SR_BUS_BUSY, SET, I2C_TIMEOUT))
        {
            return AC_TIMEOUT_BUSY;
        }

        return AC_NACK_ERR;
    }

    for (i = 0; i < txLen; i++)
    {
        /* Wait TXE Flag */
        if(I2C_Timeout_Check(I2Cx, I2C_SR_TXE, RESET, I2C_TIMEOUT))
        {
            return AC_TX_ERR;
        }
        /* Send Data */
        I2Cx->DR = txData[i];

        /* Wait for transmission End*/
        if(I2C_Timeout_Check(I2Cx, I2C_SR_MTF, RESET, I2C_TIMEOUT))
        {
            return AC_TIMEOUT_DATA;
        }
        /* Clear MTF */
        I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);

        /* Get NACK */
        if (READ_BIT(I2Cx->SR, I2C_SR_RACK))
        {
            /* Generate Stop */
            SET_BIT(I2Cx->CR, I2C_CR_STOP);

            /* Wait for the bus to idle */
            if(I2C_Timeout_Check(I2Cx, I2C_SR_BUS_BUSY, SET, I2C_TIMEOUT))
            {
                return AC_TIMEOUT_BUSY;
            }
            return AC_NACK_ERR;
        }
    }

    /* Generate Stop */
    SET_BIT(I2Cx->CR, I2C_CR_STOP);

    /* Wait for the bus to idle */
    if(I2C_Timeout_Check(I2Cx, I2C_SR_BUS_BUSY, SET, I2C_TIMEOUT))
    {
        return AC_TIMEOUT_BUSY;
    }

    return AC_OK;

}

/*****************************************************************
@����	I2C����������������
@����	I2Cx -I2C�˿ڣ�I2C1,I2C2
@����	devAddress -�ӻ���ַ
@����	txData1 - �������ݻ�����
@����	txLen1 - Ҫ���͵����ݳ���
@����	txData2 - �������ݻ�����
@����	txLen2 - Ҫ���͵����ݳ���
@����	HAL_OK,HAL_ERROR
******************************************************************/
uint8_t I2CM_TxBytes_TxBytes(I2C_TypeDef *I2Cx, uint16_t devAddress,uint8_t *txData1, uint32_t txLen1,uint8_t *txData2, uint32_t txLen2)
{
    uint32_t i;

    /* Generate Start */
    SET_BIT(I2Cx->CR, I2C_CR_START);

    /* Clear MTF, To Prevent Errors */
    I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);

    /* Send Device Address */
    I2Cx->DR = devAddress & 0xFE;

    /* Wait for transmission End*/
    if(I2C_Timeout_Check(I2Cx, I2C_SR_MTF, RESET, I2C_TIMEOUT))
    {
        return AC_TIMEOUT_ADDR;
    }
    /* Clear MTF */
    I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);

    /* Get NACK */
    if (READ_BIT(I2Cx->SR, I2C_SR_RACK))
    {
        /* Generate Stop */
        SET_BIT(I2Cx->CR, I2C_CR_STOP);
        
        /* Wait for the bus to idle */
        if(I2C_Timeout_Check(I2Cx, I2C_SR_BUS_BUSY, SET, I2C_TIMEOUT))
        {
            return AC_TIMEOUT_BUSY;
        }

        return AC_NACK_ERR;
    }
    
    for (i = 0; i < txLen1; i++)
    {
        /* Wait TXE Flag */
        if(I2C_Timeout_Check(I2Cx, I2C_SR_TXE, RESET, I2C_TIMEOUT))
        {
            return AC_TX_ERR;
        }

        /* Send Data */
        I2Cx->DR = txData1[i];

        /* Wait for transmission End*/
        if(I2C_Timeout_Check(I2Cx, I2C_SR_MTF, RESET, I2C_TIMEOUT))
        {
            return AC_TIMEOUT_DATA;
        }
        /* Clear MTF */
        I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);

        /* Get NACK */
        if (READ_BIT(I2Cx->SR, I2C_SR_RACK))
        {
            /* Generate Stop */
            SET_BIT(I2Cx->CR, I2C_CR_STOP);

            /* Wait for the bus to idle */
            if(I2C_Timeout_Check(I2Cx, I2C_SR_BUS_BUSY, SET, I2C_TIMEOUT))
            {
                return AC_TIMEOUT_BUSY;
            }
            return AC_NACK_ERR;
        }
    }
    for (i = 0; i < txLen2; i++)
    {
        /* Wait TXE Flag */
        if(I2C_Timeout_Check(I2Cx, I2C_SR_TXE, RESET, I2C_TIMEOUT))
        {
            return AC_TX_ERR;
        }

        /* Send Data */
        I2Cx->DR = txData2[i];

        /* Wait for transmission End*/
        if(I2C_Timeout_Check(I2Cx, I2C_SR_MTF, RESET, I2C_TIMEOUT))
        {
            return AC_TIMEOUT_DATA;
        }
        /* Clear MTF */
        I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);

        /* Get NACK */
        if (READ_BIT(I2Cx->SR, I2C_SR_RACK))
        {
            /* Generate Stop */
            SET_BIT(I2Cx->CR, I2C_CR_STOP);

            /* Wait for the bus to idle */
            if(I2C_Timeout_Check(I2Cx, I2C_SR_BUS_BUSY, SET, I2C_TIMEOUT))
            {
                return AC_TIMEOUT_BUSY;
            }
            return AC_NACK_ERR;
        }
    }
    
    /* Generate Stop */
    SET_BIT(I2Cx->CR, I2C_CR_STOP);

    /* Wait for the bus to idle */
    if(I2C_Timeout_Check(I2Cx, I2C_SR_BUS_BUSY, SET, I2C_TIMEOUT))
    {
        return AC_TIMEOUT_BUSY;
    }
    
    return HAL_OK;

}


/*****************************************************************
@����	I2C�������ݡ�
@����	I2Cx - I2C�˿ڣ�I2C1,I2C2
@����	devAddress -�ӻ���ַ
@����	rxData - �������ݻ�����
@����	rxLen - Ҫ���յ����ݳ���
@����	AC_OK-�ɹ���AC_FAIL-ʧ��
******************************************************************/
uint8_t I2CM_RxBytes(I2C_TypeDef *I2Cx, uint16_t devAddress, uint8_t *rxData, uint32_t rxLen)
{
    uint32_t i;

    /* Generate Start */
    SET_BIT(I2Cx->CR, I2C_CR_START);

    /* Clear MTF, To Prevent Errors */
    I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);

    if(rxLen == 1)//����1���ֽ�����ʱ
    {
        __set_PRIMASK(1);         // disable interrupt 
        SysTimer_Irq_Disable();   // disable systick interrupt  
    }

    /* Send Device Address */
    I2Cx->DR = devAddress | 0x01;

    /* Wait for transmission End */
    if(I2C_Timeout_Check(I2Cx, I2C_SR_MTF, RESET, I2C_TIMEOUT))
    {
        return AC_TIMEOUT_ADDR;
    }

    /* Clear MTF */
    I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);

    /* Get NACK */
    if (READ_BIT(I2Cx->SR, I2C_SR_RACK))
    {
        /* Generate Stop */
        SET_BIT(I2Cx->CR, I2C_CR_STOP);

        /* Wait for the bus to idle */
        if(I2C_Timeout_Check(I2Cx, I2C_SR_BUS_BUSY, SET, I2C_TIMEOUT))
        {
            return AC_TIMEOUT_BUSY;
        }
        return AC_NACK_ERR;
    }

    /* Wait Master Transition receiving state */
    if(I2C_Timeout_Check(I2Cx, I2C_SR_TX_RX_FLAG, RESET, I2C_TIMEOUT))
    {
        return AC_TIMEOUT_RX;
    }
    /* Clear TX_RX_FLAG */
    I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_TX_RX_FLAG);

    /* Generate ACK */
    CLEAR_BIT(I2Cx->CR, I2C_CR_TACK);

    for (i = 0; i < rxLen - 1; i++)
    {
        /* Wait RXNE Flag */
        if(I2C_Timeout_Check(I2Cx, I2C_SR_RXNE, RESET, I2C_TIMEOUT))
        {
            return AC_TIMEOUT_RX;
        }
        /* Read Data */
        rxData[i] = I2Cx->DR;
        /* Wait for transmission End*/
        if(I2C_Timeout_Check(I2Cx, I2C_SR_MTF, RESET, I2C_TIMEOUT))
        {
            return AC_TIMEOUT_DATA;
        }
        
        /* Clear MTF */
        I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);
    }

    if(rxLen > 1)//���ճ���1���ֽ�����ʱ
    {
        __set_PRIMASK(1);         // disable interrupt 
        SysTimer_Irq_Disable();   // disable systick interrupt  
    }

    /* Prepare for Generate NACK */
    SET_BIT(I2Cx->CR, I2C_CR_TACK);
    /* Prepare for Generate STOP */
    SET_BIT(I2Cx->CR, I2C_CR_STOP);

    /* Wait RXNE Flag */
    if(I2C_Timeout_Check(I2Cx, I2C_SR_RXNE, RESET, I2C_TIMEOUT))
    {
        return AC_TIMEOUT_RX;
    }
    /* Read Data */
    rxData[i] = I2Cx->DR;
    /* Wait for transmission End*/
    if(I2C_Timeout_Check(I2Cx, I2C_SR_MTF, RESET, I2C_TIMEOUT))
    {
        return AC_TIMEOUT_DATA_LAST;
    }

    __set_PRIMASK(0);        // enable interrupt 
    SysTimer_Irq_Enable();   // enable systick interrupt  

    /* Clear MTF */
    I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);

    /* Wait for the bus to idle */
    if(I2C_Timeout_Check(I2Cx, I2C_SR_BUS_BUSY, SET, I2C_TIMEOUT))
    {
        return AC_TIMEOUT_BUSY;
    }

    /* Generate ACK */
    CLEAR_BIT(I2Cx->CR, I2C_CR_TACK);

    return AC_OK;

}
