/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : hrng.h
 * Description : hrng header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#ifndef __HRNG_H__
#define __HRNG_H__


//#define hrng_delay_32() __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
#define hrng_delay_32()__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();  
	 

/*********************************************************************************
* Function Name  : hrng_initial
* Description    : config hrng module
* Input          : - ctrl   : input ctrl reg data;
				 : - cmpres : input cmpres reg data;
* Output         : None
* Return         : None
*********************************************************************************/
void hrng_initial(void);
/*********************************************************************************
* Function Name  : hrng_source_disable
* Description    : disable hrng source 
* Input          : - ctrl   : input ctrl reg data;
				 : - cmpres : input cmpres reg data;
* Output         : None
* Return         : None
*********************************************************************************/
void hrng_source_disable(void);
/*********************************************************************************
* Function Name  : get_hrng8
* Description    : get 8bit random number
* Input          : None
* Output         : None
* Return         : 8 bit random number
*********************************************************************************/
uint8_t get_hrng8(void);
/*********************************************************************************
* Function Name  : get_hrng32
* Description    : get 32bit random number
* Input          : None
* Output         : None
* Return         : 32 bit random number
*********************************************************************************/
UINT32 get_hrng32(void);

/*********************************************************************************
* Function Name  : get_hrng
* Description    : get random number
* Input          : byte_len :  the byte length of random number
* Output         : *hdata   :  the start address of random number the size must be 16bytes
* Return         : 0: hrng data is ok; 1: hrng data is bad
*********************************************************************************/
uint8_t get_hrng(uint8_t *hdata, UINT32 byte_len);

#endif



