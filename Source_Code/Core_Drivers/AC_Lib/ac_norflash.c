/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@�ļ�����:	ac_norflash.c
@��������:	NorFlashģ�麯����
@������Ա:	samliu 
@�������:	2021.1.18
@��ǰ�汾:	1.0
@�޸ļ�¼:	
�޸�����		�޸���   		�޸�˵��

******************************************************************/

/*****************************************************************
ע�⣺ʹ�ñ�ģ��Ҫ���ջ������6KB���ϡ�
��ΪNorFlash_Write�����ڲ�ʹ��4KB��ջ�ռ䣬���������������ݡ�
******************************************************************/

#include "ac_common.h"

#ifdef NORFLASH_HARDWARE_CS_ENABLE 
	#define NORFLASH_CS_RELEASE()                      
	#define NORFLASH_CS_SELECT()  
#else
	#define NORFLASH_CS_RELEASE()                        GPIO_SetPin(NORFLASH_SPI_CS_PIN)
	#define NORFLASH_CS_SELECT()                         GPIO_ClrPin(NORFLASH_SPI_CS_PIN)
#endif

/*****************************************************************
@����	NorFlash IO ��ʼ��
@����	��
@����	��
******************************************************************/
static void NorFlash_SPI_IOInitCallback(void)
{
    //�û���Ҫ����ʵ�ʵ�·����
    //SPI CLK 
    GPIO_InitAFIO(NORFLASH_SPI_SCLK_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, NORFLASH_SPI_SCLK_AF);
    //SPI MOSI
    GPIO_InitAFIO(NORFLASH_SPI_MISO_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, NORFLASH_SPI_MISO_AF);
    //SPI MISO
    GPIO_InitAFIO(NORFLASH_SPI_MOSI_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, NORFLASH_SPI_MOSI_AF);
#ifdef   NORFLASH_HARDWARE_CS_ENABLE 
    /* SPI CS   */
    GPIO_InitAFIO(NORFLASH_SPI_CS_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, NORFLASH_SPI_CS_AF);
#else
    GPIO_InitOutput(NORFLASH_SPI_CS_PIN, GPIO_PIN_SET);
#endif 

#if ((defined NORFLASH_QUAD_READ_ENABLE ) | (defined NORFLASH_QUAD_PROGRAM_ENABLE ))
    //���ߴ���ʱ��������SPI IO2��SPI IO3
    //SPI IO2
    GPIO_InitAFIO(NORFLASH_SPI_IO2_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, NORFLASH_SPI_IO2_AF);
    //SPI IO3 
    GPIO_InitAFIO(NORFLASH_SPI_IO3_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, NORFLASH_SPI_IO3_AF);
#endif
    //Ĭ��Ϊ����ģʽ
    SPI_SetXMode(NORFLASH_SPI, SPI_1X_MODE);

}

/*****************************************************************
@����	NorFlash��ʼ����
@����	��
@����	AC_OK,AC_FAIL
******************************************************************/
uint8_t NorFlash_Init(void)
{
    uint8_t ret = AC_OK;
    uint8_t uniqueID[16],deviceID[2];
    uint8_t tempID[16];
    uint16_t id;
    
   //�û���Ҫ����ʵ�ʵ�·����
   SPIM_Init(NORFLASH_SPI,SPI_WORK_MODE_0,SPI_1X_MODE,45000000,NorFlash_SPI_IOInitCallback);
   NorFlash_Wakeup();
#if ((defined NORFLASH_QUAD_READ_ENABLE ) | (defined NORFLASH_QUAD_PROGRAM_ENABLE ))
        ret = NorFlash_QuadEnable(1);
#endif
 
    // NorFlash_Read_Manu_Device_ID(deviceID);
    // id = (deviceID[0]<<8) | deviceID[1];
    // if((id!=0) && (id!=0xffff))
    //     return AC_OK;
    
    // NorFlash_Read_Unique_ID(uniqueID);
    // memset(tempID,0,16);
    // if(memcmp(uniqueID,tempID,16)==0)
    //     return AC_FAIL;
    
    // memset(tempID,0xff,16);
    // if(memcmp(uniqueID,tempID,16)==0)
    //     return AC_FAIL;
    
    return ret;
}


/*****************************************************************
@����	ʹ��д����
@����	��
@����	��
******************************************************************/
static void NorFlash_WriteEnable(void)
{
    uint8_t writeBuffer[1];
    
    writeBuffer[0] = NORFLASH_WRITE_ENABLE;
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes(NORFLASH_SPI, writeBuffer, 1);

    /* CS Realse */
    NORFLASH_CS_RELEASE();

}

/*****************************************************************
@����	��ʹ��д����
@����	��
@����	��
******************************************************************/
static void NorFlash_WriteDisable(void)
{
    uint8_t writeBuffer[1];
    
    writeBuffer[0] = NORFLASH_WRITE_DISABLE;
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes(NORFLASH_SPI, writeBuffer, 1);
    /* CS Realse */
    NORFLASH_CS_RELEASE();
}


/*****************************************************************
@����	��ȡ����ID/�豸ID
@����	��
@����	����ID/�豸ID
******************************************************************/
void NorFlash_Read_Manu_Device_ID(uint8_t buff[2])
{
    uint8_t writeBuffer[4];
    uint8_t readBuffer[2]={0};
    
    writeBuffer[0] = NORFLASH_READ_MANU_DEVICE_ID;
    writeBuffer[1] = 0;
    writeBuffer[2] = 0;
    writeBuffer[3] = 0;
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* 1-Send command,2-Recieve Data */
    SPIM_TxBytes_RxBytes(NORFLASH_SPI, writeBuffer, 4, buff, 2);
    /* CS Realse */
    NORFLASH_CS_RELEASE();
}

/*****************************************************************
@����	��ȡ����ID/�洢����/�洢��������������߻�д�����ڼ䲻����������
@����	buff - ��ȡ�����ݵĻ�����
@����	����ID/�洢����/�洢����
******************************************************************/
void NorFlash_Read_ID(uint8_t buff[3])
{
    uint8_t writeBuffer[4];
    
    writeBuffer[0] = NORFLASH_READ_ID;
    writeBuffer[1] = 0;
    writeBuffer[2] = 0;
    writeBuffer[3] = 0;
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* 1-Send command,2-Recieve Data */
    SPIM_TxBytes_RxBytes(NORFLASH_SPI, writeBuffer, 4, buff, 3);
    /* CS Realse */
    NORFLASH_CS_RELEASE();
}

/*****************************************************************
@����	��ȡ�豸ΨһID
@����	buff - ��ȡ�����ݵĻ�����
@����	��
******************************************************************/
void NorFlash_Read_Unique_ID(uint8_t buff[16])
{
    uint8_t writeBuffer[5];
    
    writeBuffer[0] = NORFLASH_READ_UNIQUE_ID;
    writeBuffer[1] = 0;
    writeBuffer[2] = 0;
    writeBuffer[3] = 0;
    writeBuffer[4] = 0;    
    
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* 1-Send command,2-Recieve Data */
    SPIM_TxBytes_RxBytes(NORFLASH_SPI, writeBuffer, 5, buff, 16);
    /* CS Realse */
    NORFLASH_CS_RELEASE();
    
}

/*****************************************************************
@����	��ȡ״̬�Ĵ���1
@����	��
@����	״̬�Ĵ���1ֵ
******************************************************************/
uint8_t NorFlash_ReadStatusReg1(void)
{
    uint8_t writeBuffer[1];
    uint8_t readBuffer[1] ={0};
    
    writeBuffer[0] = NORFLASH_READ_STATUS_REGISTER1;
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* 1-Send command,2-Recieve Data */
    SPIM_TxBytes_RxBytes(NORFLASH_SPI, writeBuffer, 1, readBuffer, 1);
    /* CS Realse */
    NORFLASH_CS_RELEASE();

    return readBuffer[0];
}

/*****************************************************************
@����	��ȡ״̬�Ĵ���2
@����	��
@����	״̬�Ĵ���2��ֵ
******************************************************************/
uint8_t NorFlash_ReadStatusReg2(void)
{
    uint8_t writeBuffer[1];
    uint8_t readBuffer[1] ={0};
    
    writeBuffer[0] = NORFLASH_READ_STATUS_REGISTER2;
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* 1-Send command,2-Recieve Data */
    SPIM_TxBytes_RxBytes(NORFLASH_SPI, writeBuffer, 1, readBuffer, 1);
    /* CS Realse */
    NORFLASH_CS_RELEASE();

    return readBuffer[0];
}

/*****************************************************************
@����	������д��״̬�Ĵ���
@����	reg1 - ״̬�Ĵ���1
@����	reg2 - ״̬�Ĵ���2
@����	AC_OK,AC_FAIL
******************************************************************/
uint8_t NorFlash_WriteStatusReg(uint8_t reg1,uint8_t reg2)
{
    uint8_t ret, writeBuffer[3];

    writeBuffer[0] = NORFLASH_WRITE_STATUS_REGISTER;
    writeBuffer[1] = reg1;
    writeBuffer[2] = reg2;

    /* Write Enable */
    NorFlash_WriteEnable();

    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes(NORFLASH_SPI, writeBuffer, 3);
    /* CS Realse */
    NORFLASH_CS_RELEASE();

    /* Wait Write register End */
    ret = NorFlash_WaitBusy(NORFLASH_NORMAL_OP_TIME);

    /* Write Disable */
    NorFlash_WriteDisable();
    
    return ret;
}


/*****************************************************************
@����	�ȴ�Norflash�˳�æ״̬
@����	timeout - ��ʱʱ��
@����	AC_OK, AC_FAIL
******************************************************************/
uint8_t NorFlash_WaitBusy(uint32_t timeout)
{  
    uint32_t start_tick;
    uint32_t pass_tick; 
    /* Wait IC Not Busy */
    start_tick = get_ms_tick();
    while(1)
    {
        if((NorFlash_ReadStatusReg1() & NORFLASH_STATUS_REG1_WIP) == 0)
        {
            //DBG("wait busy:%d\r\n",get_ms_delay(start_tick));
            return AC_OK;
        }
        pass_tick = get_ms_delay(start_tick);
        if(pass_tick > timeout)
        {
            return AC_FAIL;
        }
    }
}


/*****************************************************************
@����	����ģʽʹ��
@����	quadEnable - 1ʹ�ܣ�0-��ʹ�� 
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_QuadEnable(uint8_t quadEnable)
{
    uint8_t ret, reg2;
    if((NorFlash_ReadStatusReg2() & NORFLASH_STATUS_REG2_QE) == 0)
    {
        if (quadEnable) 
        {
            reg2 = NORFLASH_STATUS_REG2_QE;
            ret = NorFlash_WriteStatusReg(NORFLASH_REG_NULL, reg2);
            if(ret != AC_FAIL)
                return ret;
            
            if((NorFlash_ReadStatusReg2() & NORFLASH_STATUS_REG2_QE) == 0)
            {
                return AC_FAIL;
            }
        }
    }
    else
    {
        if (quadEnable==0) 
        {
            ret = NorFlash_WriteStatusReg(NORFLASH_REG_NULL,NORFLASH_REG_NULL);  
            if(ret != AC_FAIL)
                return ret;
            
            if((NorFlash_ReadStatusReg2() & NORFLASH_STATUS_REG2_QE) != 0)
            {
                return AC_FAIL;
            }
            
        }        
    }
    return AC_OK;
}


/*****************************************************************
@����	�������ģʽ����̻���������ڼ䣬�豸����Ӧ��ָ�
@����	��
@����	��
******************************************************************/
void NorFlash_PowerDown(void)
{ 
    uint8_t writeBuffer[1];
    
    writeBuffer[0] = NORFLASH_DEEP_POWER_DOWN;
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes(NORFLASH_SPI, writeBuffer, 1);
    /* CS Realse */
    NORFLASH_CS_RELEASE();
}

/*****************************************************************
@����	����
@����	��
@����	��
******************************************************************/
void NorFlash_Wakeup(void)
{  
    uint8_t writeBuffer[1];
    
    writeBuffer[0] = NORFLASH_RELEASE_DEEP;
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes(NORFLASH_SPI, writeBuffer, 1);
    /* CS Realse */
    NORFLASH_CS_RELEASE();
}

/*****************************************************************
@����	��λ
@����	��
@����	��
******************************************************************/
void NorFlash_Reset(void)
{
    uint8_t enableBuffer[1];
    uint8_t resetBuffer[1]; 
    
    enableBuffer[0] = NORFLASH_ENABLE_RESET;
    resetBuffer[0] = NORFLASH_RESET;
    
    /* 1-Enable Resrt,2-Reset */ 
    
    /* CS Select */
    NORFLASH_CS_SELECT();  
    SPIM_TxBytes(NORFLASH_SPI,enableBuffer,1); 
    /* CS Realse */
    NORFLASH_CS_RELEASE();
    
    delay_ms(1);
    
    /* CS Select */
    NORFLASH_CS_SELECT();     
    SPIM_TxBytes(NORFLASH_SPI,resetBuffer,1);
    /* CS Realse */
    NORFLASH_CS_RELEASE();
    delay_ms(1);
}

/*****************************************************************
@����	����ģʽ��ȡ����	
@����	addr - ��ʼ��ַ
@����	buff - ��ȡ�����ݵĻ�����
@����	len  - Ҫ��ȡ���ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Read_Data_1x(uint32_t addr, uint8_t *buff, uint32_t len)
{
    uint8_t writeBuffer[4];

    if((buff == NULL) || (len == 0))
    {
        return AC_FAIL;
    }
    //DBG("read data 1x\r\n");
    writeBuffer[0] = NORFLASH_READ_DATA;
    writeBuffer[1] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[3] = (uint8_t)(addr >> 0  & 0xFF);
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* 1-Send command,2-Recieve Data */
    SPIM_TxBytes_RxBytes(NORFLASH_SPI, writeBuffer, 4, buff, len);
    /* CS Realse */
    NORFLASH_CS_RELEASE();
    return AC_OK;
}

/*****************************************************************
@����	����ģʽ���ٶ�ȡ����	
@����	addr - ��ʼ��ַ
@����	buff - ��ȡ�����ݵĻ�����
@����	len  - Ҫ��ȡ���ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Read_Data_Fast(uint32_t addr, uint8_t *buff, uint32_t len)
{
    uint8_t writeBuffer[5];

    if((buff == NULL) || (len == 0))
    {
        return AC_FAIL;
    }
    
    writeBuffer[0] = NORFLASH_READ_DATA_FAST;
    writeBuffer[1] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[3] = (uint8_t)(addr >> 0  & 0xFF);
    writeBuffer[4] = 0;    
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* 1-Send command,2-Recieve Data */
    SPIM_TxBytes_RxBytes(NORFLASH_SPI, writeBuffer, 5, buff, len);
    /* CS Realse */
    NORFLASH_CS_RELEASE();
    return AC_OK;
}


/*****************************************************************
@����	����Outputģʽ��ȡ����	
@����	addr - ��ʼ��ַ
@����	buff - ��ȡ�����ݵĻ�����
@����	len  - Ҫ��ȡ���ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Read_Dual_Output(uint32_t addr, uint8_t *buff, uint32_t len)
{
    uint8_t writeBuffer[5];

    if((buff == NULL) || (len == 0))
    {
        return AC_FAIL;
    }

    writeBuffer[0] = NORFLASH_DUAL_OUTPUT_FAST_READ;
    writeBuffer[1] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[3] = (uint8_t)(addr >> 0  & 0xFF);
    writeBuffer[4] = (0x00);
    
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes_KeepCS(NORFLASH_SPI, writeBuffer, 5);

    /* Set SPI Work In 2 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_2X_MODE);

    /* Recieve Data */
    SPIM_RxBytes(NORFLASH_SPI, buff, len);
    /* CS Realse */
    NORFLASH_CS_RELEASE();

    /* Set SPI Work In 1 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_1X_MODE);
    return AC_OK;
}

/*****************************************************************
@����	����I/Oģʽ��ȡ����	
@����	addr - ��ʼ��ַ
@����	buff - ��ȡ�����ݵĻ�����
@����	len  - Ҫ��ȡ���ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Read_Dual_IO(uint32_t addr, uint8_t *buff, uint32_t len)
{
    uint8_t writeBuffer[4];

    if((buff == NULL) || (len == 0))
    {
        return AC_FAIL;
    }

    writeBuffer[0] = NORFLASH_DUAL_IO_FAST_READ;

    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes_KeepCS(NORFLASH_SPI, writeBuffer, 1);

    writeBuffer[0] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[1] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 0  & 0xFF);
    writeBuffer[3] = (0xC8);

    /* Set SPI Work In 2 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_2X_MODE);

    /* 1-Send Address,2-Recieve Data */
    SPIM_TxBytes_RxBytes(NORFLASH_SPI, writeBuffer, 4, buff, len);
    /* CS Realse */
    NORFLASH_CS_RELEASE();

    /* Set SPI Work In 1 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_1X_MODE);
    return AC_OK;
}



/*****************************************************************
@����	����Outputģʽ��ȡ����		
@����	addr - ��ʼ��ַ
@����	buff - ��ȡ�����ݵĻ�����
@����	len  - Ҫ��ȡ���ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Read_Quad_Output(uint32_t addr, uint8_t *buff, uint32_t len)
{
    uint8_t writeBuffer[5];

    if((buff == NULL) || (len == 0))
    {
        return AC_FAIL;
    }

    writeBuffer[0] = NORFLASH_QUAD_OUTPUT_FAST_READ;
    writeBuffer[1] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[3] = (uint8_t)(addr >> 0  & 0xFF);
    writeBuffer[4] = (0x00);
    
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes_KeepCS(NORFLASH_SPI, writeBuffer, 5);

    /* Set SPI Work In 4 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_4X_MODE);

    /* Recieve Data */
    SPIM_RxBytes(NORFLASH_SPI, buff, len);
    /* CS Realse */
    NORFLASH_CS_RELEASE();

    /* Set SPI Work In 1 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_1X_MODE);
    return AC_OK;
}

/*****************************************************************
@����	����I/Oģʽ��ȡ����	
@����	addr - ��ʼ��ַ
@����	buff - ��ȡ�����ݵĻ�����
@����	len  - Ҫ��ȡ���ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Read_Quad_IO(uint32_t addr, uint8_t *buff, uint32_t len)
{
    uint8_t writeBuffer[6];

    if((buff == NULL) || (len == 0))
    {
        return AC_FAIL;
    }

    writeBuffer[0] = NORFLASH_QUAD_IO_FAST_READ;

    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes_KeepCS(NORFLASH_SPI, writeBuffer, 1);

    writeBuffer[0] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[1] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 0  & 0xFF);
    writeBuffer[3] = (0x00);

    writeBuffer[4] = (0x00);
    writeBuffer[5] = (0x00);

    /* Set SPI Work In 4 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_4X_MODE);

    /* 1-Send Address,2-Recieve Data */
    SPIM_TxBytes_RxBytes(NORFLASH_SPI, writeBuffer, 6, buff, len);
    /* CS Realse */
    NORFLASH_CS_RELEASE();

    /* Set SPI Work In 1 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_1X_MODE);
    return AC_OK;
}


/*****************************************************************
@����	��ȡ���ݡ����������Զ�ѡ��1�ߣ�2�߻�4��ģʽ	
@����	addr - ��ʼ��ַ
@����	buff - ��ȡ�����ݵĻ�����
@����	len  - Ҫ��ȡ���ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Read(uint32_t addr,uint8_t *buff, uint32_t len)
{
    uint16_t result;
#ifdef NORFLASH_QUAD_READ_ENABLE
    result = NorFlash_Read_Quad_Output(addr, buff, len);    
#elif defined NORFLASH_DUAL_READ_ENABLE
    result = NorFlash_Read_Dual_Output(addr, buff, len);    
#else
    result = NorFlash_Read_Data_1x(addr, buff, len);        
#endif
	return result;
}


/*****************************************************************
@����	��NorFlashд������
@����	addr - ��ʼ��ַ
@����	buff - д������ݵĻ�����
@����	len  - Ҫд����ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_PageProgram_1x(uint32_t addr, uint8_t *buff, uint32_t len)
{
    uint8_t ret, writeBuffer[4];

    if((buff == NULL) || (len == 0))
    {
        return AC_FAIL;
    }
    
    writeBuffer[0] = NORFLASH_PAGE_PROGARM;
    writeBuffer[1] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[3] = (uint8_t)(addr >> 0  & 0xFF);

    /* Write Enable */
    NorFlash_WriteEnable();

    /* CS Select */
    NORFLASH_CS_SELECT();
    
    /* 1-Send command,2-Send Data */
    SPIM_TxBytes_TxBytes(NORFLASH_SPI,writeBuffer,4,buff,len);

    /* CS Realse */
    NORFLASH_CS_RELEASE();

    /* Wait write End */
    ret = NorFlash_WaitBusy(NORFLASH_PAGE_PROGRAM_TIME);

    /* Write Disable */
    NorFlash_WriteDisable();
    return ret;
}

/*****************************************************************
@����	2�߿�����NorFlashд������
@����	addr - ��ʼ��ַ
@����	buff - д������ݵĻ�����
@����	len  - Ҫд����ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_PageProgram_Dual(uint32_t addr, uint8_t *buff, uint32_t len)
{
    uint8_t ret, writeBuffer[4];

    if((buff == NULL) || (len == 0))
    {
        return AC_FAIL;
    }
    
    writeBuffer[0] = NORFLASH_DUAL_PAGE_PROGRAM;
    writeBuffer[1] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[3] = (uint8_t)(addr >> 0  & 0xFF);

    /* Write Enable */
    NorFlash_WriteEnable();
    
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes_KeepCS(NORFLASH_SPI, writeBuffer, 4);

    /* Set SPI Work In 4 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_2X_MODE);

    /* Send Data */
    SPIM_TxBytes(NORFLASH_SPI, buff, len);
    /* CS Realse */
    NORFLASH_CS_RELEASE();

    /* Set SPI Work In 1 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_1X_MODE);

    /* Wait write End */
    ret = NorFlash_WaitBusy(NORFLASH_PAGE_PROGRAM_TIME);

    /* Write Disable */
    NorFlash_WriteDisable();
    return ret;
}

/*****************************************************************
@����	4�߿�����NorFlashд������
@����	addr - ��ʼ��ַ
@����	buff - д������ݵĻ�����
@����	len  - Ҫд����ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_PageProgram_Quad(uint32_t addr, uint8_t *buff, uint32_t len)
{
    uint8_t ret, writeBuffer[4];

    if((buff == NULL) || (len == 0))
    {
        return AC_FAIL;
    }
    
    writeBuffer[0] = NORFLASH_QUAD_PAGE_PROGRAM;
    writeBuffer[1] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[3] = (uint8_t)(addr >> 0  & 0xFF);

    /* Write Enable */
    NorFlash_WriteEnable();
    
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes_KeepCS(NORFLASH_SPI, writeBuffer, 4);

    /* Set SPI Work In 4 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_4X_MODE);

    /* Send Data */
    SPIM_TxBytes(NORFLASH_SPI, buff, len);
    /* CS Realse */
    NORFLASH_CS_RELEASE();

    /* Set SPI Work In 1 Wire Mode */
    SPI_SetXMode(NORFLASH_SPI, SPI_1X_MODE);

    /* Wait write End */
    ret = NorFlash_WaitBusy(NORFLASH_PAGE_PROGRAM_TIME);

    /* Write Disable */
    NorFlash_WriteDisable();
    return ret;
}

/*****************************************************************
@����	��NorFlash������ݡ�֧�ֿ�ҳ�����������Զ�ѡ��1�ߣ�2�߻�4��ģʽ
@����	addr - ��ʼ��ַ
@����	buff - д������ݵĻ�����
@����	len  - Ҫд����ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Program(uint32_t addr,uint8_t *buff, uint32_t len)
{
    uint8_t ret;
    uint32_t offset,writeLen,writeAddr;
    
    offset = addr & NORFLASH_PAGE_SIZE_MASK;
    writeAddr = addr;
    
    while(len)
    {
        if((offset+len) < NORFLASH_PAGE_SIZE)
        {
            writeLen = len;  
        }
        else
        {
            writeLen = NORFLASH_PAGE_SIZE-offset;              
        }
        
#ifdef NORFLASH_QUAD_PROGRAM_ENABLE
        ret = NorFlash_PageProgram_Quad(writeAddr, buff, writeLen);    
#elif defined NORFLASH_DUAL_PROGRAM_ENABLE
        ret = NorFlash_PageProgram_Dual(writeAddr, buff, writeLen);  
#else
        ret = NorFlash_PageProgram_1x(writeAddr, buff, writeLen);      
#endif 
        if(ret != AC_OK)
            return  AC_FAIL;
        writeAddr += writeLen;
        buff += writeLen;
        len -= writeLen;
        offset = 0;
    }
	return AC_OK;
}

/*****************************************************************
@����	��������(4096Bytes)����
@����	addr - ������ʼ��ַ
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_EraseSector(uint32_t addr)
{
    uint8_t ret=AC_OK, writeBuffer[4];

    writeBuffer[0] = NORFLASH_SECTOR_ERASE;
    writeBuffer[1] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[3] = (uint8_t)(addr >> 0  & 0xFF);

    /* Write Enable */
    NorFlash_WriteEnable();
    /* CS Select */
    NORFLASH_CS_SELECT();
    /* Send command */
    SPIM_TxBytes(NORFLASH_SPI, writeBuffer, 4);
     /* CS Realse */
    NORFLASH_CS_RELEASE();   
    /* Wait Erase End */
    ret = NorFlash_WaitBusy(NORFLASH_SECTOR_ERASE_TIME);
    /* Write Disable */
    NorFlash_WriteDisable();
    return ret;
}

/*****************************************************************
@����	�����������
@����	addr - ������ʼ��ַ
@����	num  - ������
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_EraseSectors(uint32_t addr,uint16_t num)
{
    uint8_t ret=AC_OK, writeBuffer[4];
    uint16_t i;

    addr = addr & (~NORFLASH_SECTOR_SIZE_MASK);

    for(i = 0; i < num; i++)
    {
        ret = NorFlash_EraseSector(addr + i*NORFLASH_SECTOR_SIZE);
        if(ret!=AC_OK)
            break;
    }
    return ret;
}

/*****************************************************************
@����	����(32k Bytes)����
@����	addr - ����ʼ��ַ
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Erase32KBlock(uint32_t addr)
{
    uint8_t ret=AC_OK,  writeBuffer[4];

    writeBuffer[0] = NORFLASH_BLOCK_ERASE_32K;
    writeBuffer[1] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[3] = (uint8_t)(addr >> 0  & 0xFF);

    /* Write Enable */
    NorFlash_WriteEnable();
    /* CS Select */
    NORFLASH_CS_SELECT();    
    /* Send command */
    SPIM_TxBytes(NORFLASH_SPI, writeBuffer, 4);
    /* CS Realse */
    NORFLASH_CS_RELEASE();    
    /* Wait Erase End */
    ret = NorFlash_WaitBusy(NORFLASH_BLOCK32K_ERASE_TIME);

    /* Write Disable */
    NorFlash_WriteDisable();
    return ret;
}

/*****************************************************************
@����	����(64K Bytes)����
@����	addr - ����ʼ��ַ
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Erase64KBlock(uint32_t addr)
{
    uint8_t ret, writeBuffer[4];

    writeBuffer[0] = NORFLASH_BLOCK_ERASE_64K;
    writeBuffer[1] = (uint8_t)(addr >> 16 & 0xFF);
    writeBuffer[2] = (uint8_t)(addr >> 8  & 0xFF);
    writeBuffer[3] = (uint8_t)(addr >> 0  & 0xFF);

    /* Write Enable */
    NorFlash_WriteEnable();
    /* CS Select */
    NORFLASH_CS_SELECT();    
    /* Send command */
    SPIM_TxBytes(NORFLASH_SPI, writeBuffer, 4);
    /* CS Realse */
    NORFLASH_CS_RELEASE();    
    /* Wait Erase End */
    ret = NorFlash_WaitBusy(NORFLASH_BLOCK32K_ERASE_TIME);

    /* Write Disable */
    NorFlash_WriteDisable();
    return ret;
}

/*****************************************************************
@����	��Ƭ����
@����	��
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_EraseChip(void)
{
    uint8_t ret, writeBuffer[1];

    writeBuffer[0] = NORFLASH_CHIP_ERASE;

    /* Write Enable */
    NorFlash_WriteEnable();
     /* CS Select */
    NORFLASH_CS_SELECT();   
    /* Send command */
    SPIM_TxBytes(NORFLASH_SPI, writeBuffer, 1);
     /* CS Realse */
    NORFLASH_CS_RELEASE();   
    /* Wait Erase End */
    ret = NorFlash_WaitBusy(NORFLASH_CHIP_ERASE_TIME);

    /* Write Disable */
    NorFlash_WriteDisable();
    return ret;
}

/*****************************************************************
@����	�������ݣ�֧�ֿ���������
@����	addr - ������ʼ��ַ
@����	len - ��������
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Erase(uint32_t addr,uint32_t len)
{
    uint8_t ret;
    uint32_t eraseAddr,sectorNum;

    eraseAddr = addr &  (~NORFLASH_SECTOR_SIZE_MASK);//�����׵�ַ
    len += addr &  NORFLASH_SECTOR_SIZE_MASK;    
    sectorNum = (len+ NORFLASH_SECTOR_SIZE_MASK)/NORFLASH_SECTOR_SIZE;
  
    ret = NorFlash_EraseSectors(eraseAddr,sectorNum);
    return ret;
}

/*****************************************************************
@����	��NorFlashд�����ݡ��Զ�����������д�룬֧�ֿ�ҳд���������ڲ�ʹ��4KB��ջ�ռ䣬���������������ݡ�
@����	addr - ��ʼ��ַ
@����	buff - д������ݵĻ�����
@����	len  - Ҫд����ֽ���
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t NorFlash_Write(uint32_t addr,uint8_t *buff, uint32_t len)
{
    uint16_t offset; 
    uint16_t writeLen;
    uint32_t writeAddr;
    uint8_t *writePtr;    
    uint8_t  flash_buff[NORFLASH_SECTOR_SIZE];

    offset = (addr & NORFLASH_SECTOR_SIZE_MASK);	//��������ƫ��
    writeAddr = addr - offset;                      //������ʼ��ַ

    while(len>0)
    {
        //����д�볤��
        if(len <= (NORFLASH_SECTOR_SIZE - offset))//д�����ݲ�����������ʣ��ռ䣬û�г���������Χ
        {
            writeLen = len;
        }
        else
        {
            writeLen = NORFLASH_SECTOR_SIZE - offset;
        } 
        
        if(writeLen != NORFLASH_SECTOR_SIZE)//����������д��
        {
            NorFlash_Read(writeAddr, flash_buff, NORFLASH_SECTOR_SIZE);//����������������
            memcpy(flash_buff+offset,buff,writeLen);
            
            writePtr = flash_buff;
        }
        else//��ҳд��
        {
            writePtr = buff;
        }
        
        //�Ȳ�����д��1������
        
        if(NorFlash_EraseSector(writeAddr)!=AC_OK)
        {
            return AC_FAIL;
        }
        
        if(NorFlash_Program(writeAddr,writePtr,NORFLASH_SECTOR_SIZE)!= AC_OK)  
        {        
            return  AC_FAIL;
        }
		     
        offset = 0;				                   //ƫ��λ��Ϊ0 
        writeAddr += NORFLASH_SECTOR_SIZE;	       //д��ַƫ��        
        buff += writeLen;  	           //ָ��ƫ��
        len -= writeLen;	           //�ֽ����ݼ�
    }
	return AC_OK;
}

