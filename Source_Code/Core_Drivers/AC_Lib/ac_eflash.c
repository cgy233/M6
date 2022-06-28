/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@�ļ�����:	ac_eflash.c
@��������:	eflashģ�麯����
@������Ա:	bobzhang 
@�������:	2021.1.18
@��ǰ�汾:	1.0
@�޸ļ�¼:	
�޸�����		�޸���   		�޸�˵��

******************************************************************/
#include "ac_common.h"


/*****************************************************************
@ԭ��	uint8_t EFLASH_ReadBytes(uint32_t addr, uint8_t *buff, uint32_t len)
@����	flash�ֽڶ��������ַ��buff�����ȶ���4�ֽڶ�׼�����ڲ��Զ���WORD��ȡ�����Ч�ʡ�
@����	addr - ��ʼ��ַ
@����	buf-���ݻ��棬�ֽڵ�ַ 
@����	len-��ȡ���ֽ��� 
@����	AC_OK-�ɹ�,AC_FAIL-ʧ��
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
    
    if((addr&0x03) || (len&0x03) || ((uint32_t)buff & 0x03))   //���ֽڶ�
    {   
        bptrFlash = (uint8_t *)addr;

        for (i = 0; i < len; i++)
        {
            *buff++ = *bptrFlash++;
        }
    }
    else   //��WORD��
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
@����	eflash ��ҳ����
@����	addr - ҳ��ʼ��ַ 
@����	num - Ҫ������ҳ�� 
@����	AC_OK-�ɹ�,AC_FAIL-ʧ��
******************************************************************/
uint8_t EFLASH_ErasePages(uint32_t addr,uint32_t num)
{
    addr = addr & (~(EFLASH_PAGE_SIZE-1)); //��ַ��ҳ����
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
@����	eflash ��ҳ����
@����	addr - ҳ��ʼ��ַ
@����	AC_OK-�ɹ�, AC_FAIL-ʧ��
******************************************************************/
uint8_t EFLASH_ErasePage(uint32_t addr)
{	
    return EFLASH_ErasePages(addr,1);		
	return AC_OK;
}


/*****************************************************************
@����	�������ݣ�֧�ֿ�ҳ����
@����	addr - ������ʼ��ַ
@����	len - ��������
@����	AC_OK-�ɹ� ,AC_FAIL-ʧ��
******************************************************************/
uint8_t EFLASH_Erase(uint32_t addr, uint32_t len)
{
    uint8_t ret;
    uint32_t eraseAddr,pageNum;

    eraseAddr = addr &  (~EFLASH_PAGE_SIZE_MASK);//ҳ�׵�ַ
    len += addr &  EFLASH_PAGE_SIZE_MASK;    
    pageNum = (len+ EFLASH_PAGE_SIZE_MASK)/EFLASH_PAGE_SIZE;
  
    ret = EFLASH_ErasePages(eraseAddr,pageNum);
    return ret;	
}



/*****************************************************************
@����	eflash������ݡ����ñ�����ǰ�����Ȳ���FLASH���ݣ�
        ��ʼ���ַδ4�ֽڶ�׼ʱ�����ȡFLASH WORD�������д������ƴ�ӳ�WORD��д�룻
@����	addr - ��ʼ��ַ
@����	buff-���ݻ��棬�ֽڵ�ַ 
@����	len-д����ֽ���
@����	AC_OK-�ɹ�,AC_FAIL-ʧ��
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
		    
        offset = 0;			    //ƫ��λ��Ϊ0 
        writeAddr += 4;	        //д��ַƫ��        
        buff += writeLen;  	    //ָ��ƫ��
        len -= writeLen;	    //�ֽ����ݼ�
    }    
    
	return AC_OK; 
}

/*****************************************************************
@����	eflashд�ֽ����ݡ����������Զ�����FLASH����д��
@����	addr - ��ʼ��ַ
@����	buff-���ݻ��棬�ֽڵ�ַ 
@����	len-д����ֽ���
@����	AC_OK-�ɹ�,AC_FAIL-ʧ��
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
    
    offset = (addr & EFLASH_PAGE_SIZE_MASK);	//��ҳ��ƫ��
    writeAddr = addr - offset;                      //ҳ��ʼ��ַ

    while(len>0)
    {
        //����д�볤��
        if(len <= (EFLASH_PAGE_SIZE - offset))//д�����ݲ�����ҳ��ʣ��ռ䣬û�г���ҳ��Χ
        {
            writeLen = len;
        }
        else
        {
            writeLen = EFLASH_PAGE_SIZE - offset;
        } 
        
        if(writeLen != EFLASH_PAGE_SIZE)//������ҳд��
        {
            EFLASH_Read(writeAddr, flash_buff, EFLASH_PAGE_SIZE);//������ҳ������
            memcpy(flash_buff+offset,buff,writeLen);
            
            writePtr = flash_buff;
        }
        else//��ҳд��
        {
            writePtr = buff;
        }
        
        //�Ȳ�����д��1������
        
        if(EFLASH_ErasePage(writeAddr)!=AC_OK)
        {
            return AC_FAIL;
        }
        
        if(EFLASH_Program(writeAddr,writePtr,EFLASH_PAGE_SIZE)!= AC_OK)  
        {        
            return  AC_FAIL;
        }
		     
        offset = 0;				            //ƫ��λ��Ϊ0 
        writeAddr += EFLASH_PAGE_SIZE;	    //д��ַƫ��        
        buff += writeLen;  	                //ָ��ƫ��
        len -= writeLen;	                //�ֽ����ݼ�
    }
	return AC_OK; 
}
