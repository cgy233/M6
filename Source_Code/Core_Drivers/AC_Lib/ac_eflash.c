/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_eflash.c
@功能描述:	eflash模块函数。
@开发人员:	bobzhang 
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/
#include "ac_common.h"


/*****************************************************************
@原型	uint8_t EFLASH_ReadBytes(uint32_t addr, uint8_t *buff, uint32_t len)
@功能	flash字节读。如果地址、buff、长度都是4字节对准，则内部自动按WORD读取，提高效率。
@参数	addr - 起始地址
@参数	buf-数据缓存，字节地址 
@参数	len-读取的字节数 
@返回	AC_OK-成功,AC_FAIL-失败
******************************************************************/
uint8_t EFLASH_Read(uint32_t addr, uint8_t *buff, uint32_t len)
{
	uint32_t i;
	uint8_t *bptrFlash;
	uint32_t *wptrFlash,*wptrBuff;

    if((addr<EFLASH_START_ADDR)||(addr+len > EFLASH_START_ADDR+EFLASH_CHIP_SIZE))
	{
        if((addr<EFLASH_NVR_START_ADDR) || (addr+len > EFLASH_NVR_START_ADDR+EFLASH_NVR_SIZE))
        {
            return AC_FAIL;
        }
	}
    
    if((addr&0x03) || (len&0x03) || ((uint32_t)buff & 0x03))   //按字节读
    {   
        bptrFlash = (uint8_t *)addr;

        for (i = 0; i < len; i++)
        {
            *buff++ = *bptrFlash++;
        }
    }
    else   //按WORD读
    {
        wptrFlash = (uint32_t *)addr;
        wptrBuff =  (uint32_t *)buff;
        
        len = len >> 2;

        for (i = 0; i < len; i++)
        {
            *wptrBuff++ = *wptrFlash++;
        }         
    }
    
	return AC_OK; 
}


/*****************************************************************
@功能	eflash 多页擦除
@参数	addr - 页起始地址 
@参数	num - 要擦除的页数 
@返回	AC_OK-成功,AC_FAIL-失败
******************************************************************/
uint8_t EFLASH_ErasePages(uint32_t addr,uint32_t num)
{
    addr = addr & (~(EFLASH_PAGE_SIZE-1)); //地址按页对齐
    if(addr<EFLASH_START_ADDR)
    {
        return AC_FAIL;
    }
        
    if((addr<EFLASH_START_ADDR)||(addr + EFLASH_PAGE_SIZE*num > EFLASH_START_ADDR+EFLASH_CHIP_SIZE))
	{
        if((addr<EFLASH_NVR_START_ADDR) || (addr+EFLASH_PAGE_SIZE*num > EFLASH_NVR_START_ADDR+EFLASH_NVR_SIZE))
        {
            return AC_FAIL;
        }
	}
    
    while(num--)
    {
		HAL_EFlash_ErasePage(addr);
		addr += EFLASH_PAGE_SIZE;
    }
    
	return AC_OK;
}

/*****************************************************************
@功能	eflash 单页擦除
@参数	addr - 页起始地址
@返回	AC_OK-成功, AC_FAIL-失败
******************************************************************/
uint8_t EFLASH_ErasePage(uint32_t addr)
{	
    return EFLASH_ErasePages(addr,1);		
	return AC_OK;
}


/*****************************************************************
@功能	擦除数据，支持跨页擦除
@参数	addr - 擦除起始地址
@参数	len - 擦除长度
@返回	AC_OK-成功 ,AC_FAIL-失败
******************************************************************/
uint8_t EFLASH_Erase(uint32_t addr, uint32_t len)
{
    uint8_t ret;
    uint32_t eraseAddr,pageNum;

    eraseAddr = addr &  (~EFLASH_PAGE_SIZE_MASK);//页首地址
    len += addr &  EFLASH_PAGE_SIZE_MASK;    
    pageNum = (len+ EFLASH_PAGE_SIZE_MASK)/EFLASH_PAGE_SIZE;
  
    ret = EFLASH_ErasePages(eraseAddr,pageNum);
    return ret;	
}



/*****************************************************************
@功能	eflash编程数据。调用本函数前，请先擦除FLASH数据；
        起始或地址未4字节对准时，会读取FLASH WORD数据与待写入数据拼接成WORD再写入；
@参数	addr - 起始地址
@参数	buff-数据缓存，字节地址 
@参数	len-写入的字节数
@返回	AC_OK-成功,AC_FAIL-失败
******************************************************************/
uint8_t EFLASH_Program(uint32_t addr, uint8_t *buff, uint32_t len)
{
	uint32_t wData,offset,writeAddr,writeLen;
    
    if((addr<EFLASH_START_ADDR)||(addr + len > EFLASH_START_ADDR+EFLASH_CHIP_SIZE))
	{
        if((addr<EFLASH_NVR_START_ADDR) || (addr+len > EFLASH_NVR_START_ADDR+EFLASH_NVR_SIZE))
        {
            return AC_FAIL;
        }
	}
    
    offset = addr&0x03;
    writeAddr = addr - offset; 
    
    while(len>0)
    {
        if(len <= (4 - offset))
        {
            writeLen = len;
        }
        else
        {
            writeLen = 4 - offset;
        } 
        
        if(writeLen != 4)
        {
            wData = *(uint32_t *)writeAddr;
        }
        memcpy((uint8_t *)(&wData)+offset,buff,writeLen);

        HAL_EFlash_Program_Word(writeAddr,wData);
		    
        offset = 0;			    //偏移位置为0 
        writeAddr += 4;	        //写地址偏移        
        buff += writeLen;  	    //指针偏移
        len -= writeLen;	    //字节数递减
    }    
    
	return AC_OK; 
}

/*****************************************************************
@功能	eflash写字节数据。本函数会自动擦除FLASH后再写。
@参数	addr - 起始地址
@参数	buff-数据缓存，字节地址 
@参数	len-写入的字节数
@返回	AC_OK-成功,AC_FAIL-失败
******************************************************************/
uint8_t EFLASH_Write(uint32_t addr, uint8_t *buff, uint32_t len)
{
    uint16_t offset; 
    uint16_t writeLen;
    uint32_t writeAddr;
    uint8_t *writePtr;    
    uint8_t  flash_buff[EFLASH_PAGE_SIZE];

    if((addr<EFLASH_START_ADDR)||(addr+len > EFLASH_START_ADDR+EFLASH_CHIP_SIZE))
	{
        if((addr<EFLASH_NVR_START_ADDR) || (addr+len > EFLASH_NVR_START_ADDR+EFLASH_NVR_SIZE))
        {
            return AC_FAIL;
        }
	}
    
    offset = (addr & EFLASH_PAGE_SIZE_MASK);	//在页内偏移
    writeAddr = addr - offset;                      //页起始地址

    while(len>0)
    {
        //本次写入长度
        if(len <= (EFLASH_PAGE_SIZE - offset))//写入数据不大于页内剩余空间，没有超过页范围
        {
            writeLen = len;
        }
        else
        {
            writeLen = EFLASH_PAGE_SIZE - offset;
        } 
        
        if(writeLen != EFLASH_PAGE_SIZE)//不是整页写入
        {
            EFLASH_Read(writeAddr, flash_buff, EFLASH_PAGE_SIZE);//读出整页的内容
            memcpy(flash_buff+offset,buff,writeLen);
            
            writePtr = flash_buff;
        }
        else//整页写入
        {
            writePtr = buff;
        }
        
        //先擦除后写入1个扇区
        
        if(EFLASH_ErasePage(writeAddr)!=AC_OK)
        {
            return AC_FAIL;
        }
        
        if(EFLASH_Program(writeAddr,writePtr,EFLASH_PAGE_SIZE)!= AC_OK)  
        {        
            return  AC_FAIL;
        }
		     
        offset = 0;				            //偏移位置为0 
        writeAddr += EFLASH_PAGE_SIZE;	    //写地址偏移        
        buff += writeLen;  	                //指针偏移
        len -= writeLen;	                //字节数递减
    }
	return AC_OK; 
}
