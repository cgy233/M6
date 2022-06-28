 
#ifndef _AC_COMMON_H_
#define _AC_COMMON_H_


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "ac_typedef.h"

#include "ACM32Fxx_HAL.h"


#pragma clang diagnostic ignored "-Winvalid-source-encoding"

//大小端模式
typedef enum {
    endian_Big,                  
    endian_Little,              
}Endian_BigLittleType;


typedef enum
{
    AC_OK       = 0x00,
    AC_FAIL,
    AC_BUSY,
    AC_TIMEOUT,
    AC_TIMEOUT_ADDR,
    AC_TIMEOUT_RX,
    AC_TIMEOUT_BUSY,
    AC_TIMEOUT_DATA,
    AC_TIMEOUT_DATA_LAST,
    AC_NACK_ERR,
    AC_TX_ERR,
}AC_StatusTypeDef;


//模块使能

#define AC_GPIO_ENABLE
#define AC_SYSTIMER_ENABLE
//#define AC_TIMER_ENABLE
#define AC_CONVERT_ENABLE

#define AC_UART_ENABLE
#define AC_I2CM_ENABLE
#define AC_SPIM_ENABLE

#define AC_EFLASH_ENABLE
#define AC_NORFLASH_ENABLE

#define AC_WDT_ENABLE
#define AC_RTC_ENABLE

#define AC_FIFO_ENABLE


#ifdef AC_GPIO_ENABLE
    #include "ac_gpio.h"
    #include "ac_gpio_af.h"    
#endif

#ifdef AC_SYSTIMER_ENABLE
    #include "ac_systimer.h"
#endif


#ifdef AC_TIMER_ENABLE
    #include "ac_timer.h"
#endif

#ifdef AC_CONVERT_ENABLE
    #include "ac_convert.h"
#endif

#ifdef AC_UART_ENABLE
    #include "ac_uart.h"
#endif

#ifdef AC_I2CM_ENABLE
    #include "ac_i2cm.h"
#endif

#ifdef AC_SPIM_ENABLE
    #include "ac_spim.h"
#endif

#ifdef AC_EFLASH_ENABLE
    #include "ac_eflash.h"
#endif

#ifdef AC_NORFLASH_ENABLE
    #include "ac_norflash.h"
#endif


#ifdef AC_WDT_ENABLE
    #include "ac_wdt.h"
#endif

#ifdef AC_RTC_ENABLE
    #include "ac_rtc.h"
#endif

#ifdef AC_FIFO_ENABLE
    #include "ac_fifo.h"
#endif



/*****************************************************************
@功能	将2字节的uint8_t类型数据转换成uint16_t类型数据（大端模式）
@参数	ptr[in]: 要转换的uint8_t类型数据缓冲区
@返回	uint16_t类型数据
******************************************************************/
#define TRANSFORM_BYTES_TO_UINT16(ptr) \
    ((uint16_t)(((*(ptr+1)) & 0x00FF) + (((*(ptr)) & 0x00FF) << 8)))

/*****************************************************************
@功能	将4字节的uint8_t类型数据转换成uint32_t类型数据（大端模式）
@参数	ptr[in]: 要转换的uint8_t类型数据缓冲区
@返回	uint32_t类型数据
******************************************************************/
#define TRANSFORM_BYTES_TO_UINT32(ptr) \
    ((uint32_t)((uint32_t)((*(ptr+3)) & 0x00FF) \
    + ((uint32_t)((*(ptr+2)) & 0x00FF) << 8) \
	+ ((uint32_t)((*(ptr+1)) & 0x00FF) << 16) \
    + ((uint32_t)((*(ptr+0)) & 0x00FF) << 24)))

/*****************************************************************
@功能	将uint32_t类型数据转换成4字节uint8_t类型数据（大端模式）
@参数	val[in]: 要转换的uint32_t类型数据
@参数	ptr[out]: 转换后的uint8_t类型数据缓冲区
******************************************************************/
#define TRANSFORM_UINT32_TO_BYTES(val,pBuf) \
    do { \
        *(pBuf+0) = ((((uint32_t)(val)) >>  24) & 0xFF); \
        *(pBuf+1) = ((((uint32_t)(val)) >>  16) & 0xFF); \
        *(pBuf+2) = ((((uint32_t)(val)) >> 8) & 0xFF); \
        *(pBuf+3) = ((((uint32_t)(val)) >> 0) & 0xFF); \
	} while (0)

/*****************************************************************
@功能	将uint16_t类型数据转换成2字节uint8_t类型数据（大端模式）
@参数	val[in]: 要转换的uint16_t类型数据
@参数	ptr[out]: 转换后的uint8_t类型数据缓冲区
******************************************************************/
#define TRANSFORM_UINT16_TO_BYTES(val,pBuf) \
    do { \
        *(pBuf+0) = ((((uint16_t)(val)) >>  8) & 0xFF); \
        *(pBuf+1) = ((((uint16_t)(val)) >>  0) & 0xFF); \
	} while (0)




uint16_t GetCheckSum(const uint8_t *ptr,uint32_t len);

void Transform_UcharToString(uint8_t *string, const uint8_t *data, uint8_t len, Endian_BigLittleType endian);

void Transform_StringToUchar(uint8_t *data, uint8_t *data_len,const uint8_t *string,  Endian_BigLittleType endian);

uint8_t Transform_BcdToDec(uint8_t bcd_data);

uint8_t Transform_DecToBcd(uint8_t dec_data);



#endif
