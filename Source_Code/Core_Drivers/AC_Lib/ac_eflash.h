/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_norflash.h
@功能描述:	eFlash header file
@开发人员:	bobzhang 
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

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


#define EFLASH_UID_ADDR     (0x00080208)   //芯片唯一序列号地址，16字节

//字节读。
uint8_t EFLASH_Read(uint32_t addr, uint8_t *buff, uint32_t len);

//多页擦除
uint8_t EFLASH_ErasePages(uint32_t addr,uint32_t num);

//单页擦除
uint8_t EFLASH_ErasePage(uint32_t addr);

//擦除数据，支持跨页
uint8_t EFLASH_Erase(uint32_t addr, uint32_t len);

//编程数据，支持跨页
uint8_t EFLASH_Program(uint32_t addr, uint8_t *buff, uint32_t len);

//写字节数据。本函数会自动擦除FLASH后再写。
uint8_t EFLASH_Write(uint32_t addr, uint8_t *buff, uint32_t len);


#endif


