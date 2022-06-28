/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@�ļ�����:	ac_norflash.h
@��������:	eFlash header file
@������Ա:	bobzhang 
@�������:	2021.1.18
@��ǰ�汾:	1.0
@�޸ļ�¼:	
�޸�����		�޸���   		�޸�˵��

******************************************************************/


#ifndef _AC_EFLASH_H
#define _AC_EFLASH_H
/************************************************************************/
/*	include files                                                       */
/************************************************************************/

#include  "ac_common.h"

#define EFLASH_PAGE_SIZE_MASK    (EFLASH_PAGE_SIZE-1)

#define EFLASH_START_ADDR 		(0x00000000)
#define EFLASH_CHIP_SIZE 		(0x00080000) //512KB
#define EFLASH_END_ADDR 		(EFLASH_START_ADDR+EFLASH_CHIP_SIZE-1) 

#define EFLASH_NVR_START_ADDR 		(0x00080400)
#define EFLASH_NVR_SIZE 		    (0x00000400)
#define EFLASH_NVR1_START_ADDR 		(EFLASH_NVR_START_ADDR)
#define EFLASH_NVR2_START_ADDR 		(EFLASH_NVR1_START_ADDR+EFLASH_PAGE_SIZE)
#define EFLASH_NVR_END_ADDR 		(EFLASH_NVR2_START_ADDR+EFLASH_PAGE_SIZE-1)


#define EFLASH_UID_ADDR     (0x00080208)   //оƬΨһ���кŵ�ַ��16�ֽ�

//�ֽڶ���
uint8_t EFLASH_Read(uint32_t addr, uint8_t *buff, uint32_t len);

//��ҳ����
uint8_t EFLASH_ErasePages(uint32_t addr,uint32_t num);

//��ҳ����
uint8_t EFLASH_ErasePage(uint32_t addr);

//�������ݣ�֧�ֿ�ҳ
uint8_t EFLASH_Erase(uint32_t addr, uint32_t len);

//������ݣ�֧�ֿ�ҳ
uint8_t EFLASH_Program(uint32_t addr, uint8_t *buff, uint32_t len);

//д�ֽ����ݡ����������Զ�����FLASH����д��
uint8_t EFLASH_Write(uint32_t addr, uint8_t *buff, uint32_t len);


#endif


