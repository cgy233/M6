/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_norflash.c
@功能描述:	NorFlash模块函数。
@开发人员:	samliu 
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/

/*****************************************************************
注意：使用本模块要求堆栈设置在6KB以上。
因为NorFlash_Write函数内部使用4KB堆栈空间，缓存扇区备份数据。
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
@功能	NorFlash IO 初始化
@参数	无
@返回	无
******************************************************************/
static void NorFlash_SPI_IOInitCallback(void)
{
    //用户需要根据实际电路定义
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
    //四线传输时，请配置SPI IO2和SPI IO3
    //SPI IO2
    GPIO_InitAFIO(NORFLASH_SPI_IO2_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, NORFLASH_SPI_IO2_AF);
    //SPI IO3 
    GPIO_InitAFIO(NORFLASH_SPI_IO3_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, NORFLASH_SPI_IO3_AF);
#endif
    //默认为单线模式
    SPI_SetXMode(NORFLASH_SPI, SPI_1X_MODE);

}

/*****************************************************************
@功能	NorFlash初始化。
@参数	无
@返回	AC_OK,AC_FAIL
******************************************************************/
uint8_t NorFlash_Init(void)
{
    uint8_t ret = AC_OK;
    uint8_t uniqueID[16],deviceID[2];
    uint8_t tempID[16];
    uint16_t id;
    
   //用户需要根据实际电路定义
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
@功能	使能写操作
@参数	无
@返回	无
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
@功能	不使能写操作
@参数	无
@返回	无
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
@功能	获取厂商ID/设备ID
@参数	无
@返回	厂商ID/设备ID
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
@功能	获取厂商ID/存储类型/存储容量。在深度休眠或写操作期间不允许发起该命令。
@参数	buff - 读取的数据的缓冲区
@返回	厂商ID/存储类型/存储容量
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
@功能	获取设备唯一ID
@参数	buff - 读取的数据的缓冲区
@返回	无
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
@功能	读取状态寄存器1
@参数	无
@返回	状态寄存器1值
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
@功能	读取状态寄存器2
@参数	无
@返回	状态寄存器2的值
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
@功能	将数据写入状态寄存器
@参数	reg1 - 状态寄存器1
@参数	reg2 - 状态寄存器2
@返回	AC_OK,AC_FAIL
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
@功能	等待Norflash退出忙状态
@参数	timeout - 超时时间
@返回	AC_OK, AC_FAIL
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
@功能	四线模式使能
@参数	quadEnable - 1使能，0-不使能 
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	进入掉电模式。编程或擦除操作期间，设备不响应该指令。
@参数	无
@返回	无
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
@功能	唤醒
@参数	无
@返回	无
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
@功能	复位
@参数	无
@返回	无
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
@功能	单线模式读取数据	
@参数	addr - 起始地址
@参数	buff - 读取的数据的缓冲区
@参数	len  - 要读取的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	单线模式快速读取数据	
@参数	addr - 起始地址
@参数	buff - 读取的数据的缓冲区
@参数	len  - 要读取的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	两线Output模式读取数据	
@参数	addr - 起始地址
@参数	buff - 读取的数据的缓冲区
@参数	len  - 要读取的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	两线I/O模式读取数据	
@参数	addr - 起始地址
@参数	buff - 读取的数据的缓冲区
@参数	len  - 要读取的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	四线Output模式读取数据		
@参数	addr - 起始地址
@参数	buff - 读取的数据的缓冲区
@参数	len  - 要读取的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	四线I/O模式读取数据	
@参数	addr - 起始地址
@参数	buff - 读取的数据的缓冲区
@参数	len  - 要读取的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	读取数据。根据配置自动选择1线，2线或4线模式	
@参数	addr - 起始地址
@参数	buff - 读取的数据的缓冲区
@参数	len  - 要读取的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	往NorFlash写入数据
@参数	addr - 起始地址
@参数	buff - 写入的数据的缓冲区
@参数	len  - 要写入的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	2线快速往NorFlash写入数据
@参数	addr - 起始地址
@参数	buff - 写入的数据的缓冲区
@参数	len  - 要写入的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	4线快速往NorFlash写入数据
@参数	addr - 起始地址
@参数	buff - 写入的数据的缓冲区
@参数	len  - 要写入的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	往NorFlash编程数据。支持跨页。根据配置自动选择1线，2线或4线模式
@参数	addr - 起始地址
@参数	buff - 写入的数据的缓冲区
@参数	len  - 要写入的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	整个扇区(4096Bytes)擦除
@参数	addr - 扇区起始地址
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	多个扇区擦除
@参数	addr - 扇区起始地址
@参数	num  - 扇区数
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	整块(32k Bytes)擦除
@参数	addr - 块起始地址
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	整块(64K Bytes)擦除
@参数	addr - 块起始地址
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	整片擦除
@参数	无
@返回	AC_OK-成功 ,AC_FAIL-失败
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
@功能	擦除数据，支持跨扇区擦除
@参数	addr - 擦除起始地址
@参数	len - 擦除长度
@返回	AC_OK-成功 ,AC_FAIL-失败
******************************************************************/
uint8_t NorFlash_Erase(uint32_t addr,uint32_t len)
{
    uint8_t ret;
    uint32_t eraseAddr,sectorNum;

    eraseAddr = addr &  (~NORFLASH_SECTOR_SIZE_MASK);//扇区首地址
    len += addr &  NORFLASH_SECTOR_SIZE_MASK;    
    sectorNum = (len+ NORFLASH_SECTOR_SIZE_MASK)/NORFLASH_SECTOR_SIZE;
  
    ret = NorFlash_EraseSectors(eraseAddr,sectorNum);
    return ret;
}

/*****************************************************************
@功能	往NorFlash写入数据。自动擦除扇区再写入，支持跨页写。本函数内部使用4KB堆栈空间，缓存扇区备份数据。
@参数	addr - 起始地址
@参数	buff - 写入的数据的缓冲区
@参数	len  - 要写入的字节数
@返回	AC_OK-成功 ,AC_FAIL-失败
******************************************************************/
uint8_t NorFlash_Write(uint32_t addr,uint8_t *buff, uint32_t len)
{
    uint16_t offset; 
    uint16_t writeLen;
    uint32_t writeAddr;
    uint8_t *writePtr;    
    uint8_t  flash_buff[NORFLASH_SECTOR_SIZE];

    offset = (addr & NORFLASH_SECTOR_SIZE_MASK);	//在扇区内偏移
    writeAddr = addr - offset;                      //扇区起始地址

    while(len>0)
    {
        //本次写入长度
        if(len <= (NORFLASH_SECTOR_SIZE - offset))//写入数据不大于扇区内剩余空间，没有超过扇区范围
        {
            writeLen = len;
        }
        else
        {
            writeLen = NORFLASH_SECTOR_SIZE - offset;
        } 
        
        if(writeLen != NORFLASH_SECTOR_SIZE)//不是整扇区写入
        {
            NorFlash_Read(writeAddr, flash_buff, NORFLASH_SECTOR_SIZE);//读出整扇区的内容
            memcpy(flash_buff+offset,buff,writeLen);
            
            writePtr = flash_buff;
        }
        else//整页写入
        {
            writePtr = buff;
        }
        
        //先擦除后写入1个扇区
        
        if(NorFlash_EraseSector(writeAddr)!=AC_OK)
        {
            return AC_FAIL;
        }
        
        if(NorFlash_Program(writeAddr,writePtr,NORFLASH_SECTOR_SIZE)!= AC_OK)  
        {        
            return  AC_FAIL;
        }
		     
        offset = 0;				                   //偏移位置为0 
        writeAddr += NORFLASH_SECTOR_SIZE;	       //写地址偏移        
        buff += writeLen;  	           //指针偏移
        len -= writeLen;	           //字节数递减
    }
	return AC_OK;
}

