/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_norflash.h
@功能描述:	NorFlash header file
@开发人员:	samliu 
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/



#ifndef __AC_NORFLASH_H__
#define __AC_NORFLASH_H__

#include  "ac_common.h"



//************ 用户需要根据实际电路定义  Start ************//

//FLASH 硬件PCB接口配置**************************
#define NORFLASH_QFN32_INN      1
#define NORFLASH_QFN32_EXT      2
#define NORFLASH_QFN64_EXT      3
#define NORFLASH_QFN88_EXT      4
#define NORFLASH_QFN64_INN      5


#define NORFLASH_PCB_TYPE       NORFLASH_QFN64_INN


#if ((NORFLASH_PCB_TYPE == NORFLASH_QFN32_INN) || (NORFLASH_PCB_TYPE == NORFLASH_QFN64_INN))

    //NorFlash使用的SPI端口，SPI1,SPI2,SPI3,SPI4
    #define NORFLASH_SPI     SPI3

    //NorFlash  GPIO定义
    #define NORFLASH_SPI_CS_PIN           GPIOD13
    #define NORFLASH_SPI_CS_AF            GPIOD13_AF_SPI3_CS

    #define NORFLASH_SPI_SCLK_PIN         GPIOE3
    #define NORFLASH_SPI_SCLK_AF          GPIOE3_AF_SPI3_SCK

    #define NORFLASH_SPI_MISO_PIN         GPIOD14
    #define NORFLASH_SPI_MISO_AF          GPIOD14_AF_SPI3_MISO

    #define NORFLASH_SPI_MOSI_PIN         GPIOE2
    #define NORFLASH_SPI_MOSI_AF          GPIOE2_AF_SPI3_MOSI

    #define NORFLASH_SPI_IO2_PIN          GPIOC9
    #define NORFLASH_SPI_IO2_AF           GPIOC9_AF_SPI3_IO2

    #define NORFLASH_SPI_IO3_PIN          GPIOE4
    #define NORFLASH_SPI_IO3_AF           GPIOE4_AF_SPI3_IO3
    
    //硬件SPI_CS(是否启用硬件CS)
    #define NORFLASH_HARDWARE_CS_ENABLE
    
#elif (NORFLASH_PCB_TYPE == NORFLASH_QFN32_EXT)

    //NorFlash使用的SPI端口，SPI1,SPI2,SPI3,SPI4
    #define NORFLASH_SPI     SPI1

    //NorFlash  GPIO定义
    #define NORFLASH_SPI_CS_PIN           GPIOB0
    #define NORFLASH_SPI_CS_AF            GPIOB0_AF_SPI1_CS

    #define NORFLASH_SPI_SCLK_PIN         GPIOA5
    #define NORFLASH_SPI_SCLK_AF          GPIOA5_AF_SPI1_SCK

    #define NORFLASH_SPI_MISO_PIN         GPIOB4
    #define NORFLASH_SPI_MISO_AF          GPIOB4_AF_SPI1_MISO

    #define NORFLASH_SPI_MOSI_PIN         GPIOB5
    #define NORFLASH_SPI_MOSI_AF          GPIOB5_AF_SPI1_MOSI

    #define NORFLASH_SPI_IO2_PIN          GPIOA7
    #define NORFLASH_SPI_IO2_AF           GPIOA7_AF_SPI1_IO2
    
    #define NORFLASH_SPI_IO3_PIN          GPIOA6
    #define NORFLASH_SPI_IO3_AF           GPIOA6_AF_SPI1_IO3

    //硬件SPI_CS(是否启用硬件CS)
    #define  NORFLASH_HARDWARE_CS_ENABLE

#elif (NORFLASH_PCB_TYPE == NORFLASH_QFN64_EXT)

    //NorFlash使用的SPI端口，SPI1,SPI2,SPI3,SPI4
    #define NORFLASH_SPI     SPI3

    //NorFlash  GPIO定义
    #define NORFLASH_SPI_CS_PIN           GPIOA15
    #define NORFLASH_SPI_CS_AF            GPIOA15_AF_SPI3_CS

    #define NORFLASH_SPI_SCLK_PIN         GPIOC10
    #define NORFLASH_SPI_SCLK_AF          GPIOC10_AF_SPI3_SCK

    #define NORFLASH_SPI_MISO_PIN         GPIOC11
    #define NORFLASH_SPI_MISO_AF          GPIOC11_AF_SPI3_MISO

    #define NORFLASH_SPI_MOSI_PIN         GPIOC12
    #define NORFLASH_SPI_MOSI_AF          GPIOC12_AF_SPI3_MOSI

    #define NORFLASH_SPI_IO2_PIN          GPIOC9
    #define NORFLASH_SPI_IO2_AF           GPIOC9_AF_SPI3_IO2
    
    #define NORFLASH_SPI_IO3_PIN          GPIOC8
    #define NORFLASH_SPI_IO3_AF           GPIOC8_AF_SPI3_IO3

    //硬件SPI_CS(是否启用硬件CS)
    #define  NORFLASH_HARDWARE_CS_ENABLE

#elif (NORFLASH_PCB_TYPE == NORFLASH_QFN88_EXT) 

    //NorFlash使用的SPI端口，SPI1,SPI2,SPI3,SPI4
    #define NORFLASH_SPI     SPI3

    //NorFlash  GPIO定义
    #define NORFLASH_SPI_CS_PIN           GPIOA15
    #define NORFLASH_SPI_CS_AF            GPIOA15_AF_SPI3_CS

    #define NORFLASH_SPI_SCLK_PIN         GPIOC10
    #define NORFLASH_SPI_SCLK_AF          GPIOC10_AF_SPI3_SCK

    #define NORFLASH_SPI_MISO_PIN         GPIOC11
    #define NORFLASH_SPI_MISO_AF          GPIOC11_AF_SPI3_MISO

    #define NORFLASH_SPI_MOSI_PIN         GPIOC12
    #define NORFLASH_SPI_MOSI_AF          GPIOC12_AF_SPI3_MOSI

    #define NORFLASH_SPI_IO2_PIN          GPIOE10
    #define NORFLASH_SPI_IO2_AF           GPIOE10_AF_SPI3_IO2
    
    #define NORFLASH_SPI_IO3_PIN          GPIOE9
    #define NORFLASH_SPI_IO3_AF           GPIOE9_AF_SPI3_IO3

    //硬件SPI_CS(是否启用硬件CS)
    #define  NORFLASH_HARDWARE_CS_ENABLE

#endif

    
//是否使能四线快速读写
#define NORFLASH_QUAD_READ_ENABLE
#define NORFLASH_QUAD_PROGRAM_ENABLE 

//是否使能二线快速读写
//#define NORFLASH_DUAL_READ_ENABLE
//#define NORFLASH_DUAL_PROGRAM_ENABLE

#define NORFLASH_PAGE_SIZE        (256)
#define NORFLASH_PAGE_SIZE_MASK   (255)
#define NORFLASH_SECTOR_SIZE      (4096)
#define NORFLASH_SECTOR_SIZE_MASK (4095) 
#define NORFLASH_BLOCK32K_SIZE      (32768) 
#define NORFLASH_BLOCK64K_SIZE      (65536) 

// NorFlash 操作时间
#define NORFLASH_NORMAL_OP_TIME         20   //单位ms   
#define NORFLASH_PAGE_PROGRAM_TIME      20   //单位ms 
#define NORFLASH_SECTOR_ERASE_TIME      100   //单位ms 
#define NORFLASH_BLOCK32K_ERASE_TIME    1000   //单位ms 
#define NORFLASH_BLOCK64K_ERASE_TIME    1000   //单位ms 
#define NORFLASH_CHIP_ERASE_TIME        10000   //单位ms 

//************ 用户需要根据实际电路定义  End ************//


//  NorFlash Command Descriptions
#define NORFLASH_WRITE_ENABLE                             (0x06)
#define NORFLASH_WRITE_DISABLE                            (0x04)
#define NORFLASH_READ_STATUS_REGISTER1                    (0x05)
#define NORFLASH_READ_STATUS_REGISTER2                    (0x35)
#define NORFLASH_WRITE_STATUS_REGISTER                    (0x01)
#define NORFLASH_READ_MANU_DEVICE_ID                      (0x90) 
#define NORFLASH_READ_ID                                  (0x9F) 
#define NORFLASH_READ_UNIQUE_ID                           (0x4B) 
#define NORFLASH_ENABLE_RESET                             (0x66)
#define NORFLASH_RESET                                    (0x99)
#define NORFLASH_DEEP_POWER_DOWN                          (0xB9)
#define NORFLASH_RELEASE_DEEP                             (0xAB)

#define NORFLASH_READ_DATA                                (0x03)
#define NORFLASH_READ_DATA_FAST                           (0x0B)
#define NORFLASH_DUAL_OUTPUT_FAST_READ                    (0x3B)
#define NORFLASH_QUAD_OUTPUT_FAST_READ                    (0x6B)
#define NORFLASH_DUAL_IO_FAST_READ                        (0xBB)
#define NORFLASH_QUAD_IO_FAST_READ                        (0xEB)

#define NORFLASH_PAGE_PROGARM                             (0x02)
#define NORFLASH_DUAL_PAGE_PROGRAM                        (0xA2)
#define NORFLASH_QUAD_PAGE_PROGRAM                        (0x32)

#define NORFLASH_PAGE_ERASE                               (0x81)
#define NORFLASH_SECTOR_ERASE                             (0x20)
#define NORFLASH_BLOCK_ERASE_32K                          (0x52)
#define NORFLASH_BLOCK_ERASE_64K                          (0xD8)
#define NORFLASH_CHIP_ERASE                               (0xC7)


//  NorFlash Stauts Register

#define NORFLASH_REG_NULL            (0)

#define NORFLASH_STATUS_REG1_SRP0    (1 << 7)
#define NORFLASH_STATUS_REG1_BP4     (1 << 6)
#define NORFLASH_STATUS_REG1_BP3     (1 << 5)
#define NORFLASH_STATUS_REG1_BP2     (1 << 4)
#define NORFLASH_STATUS_REG1_BP1     (1 << 3)
#define NORFLASH_STATUS_REG1_BP0     (1 << 2)
#define NORFLASH_STATUS_REG1_WEL     (1 << 1)
#define NORFLASH_STATUS_REG1_WIP     (1 << 0)   //write in progress

#define NORFLASH_STATUS_REG2_SUS     (1 << 7)
#define NORFLASH_STATUS_REG2_CMP     (1 << 6)
#define NORFLASH_STATUS_REG2_NULL    (1 << 5)
#define NORFLASH_STATUS_REG2_DC      (1 << 4)
#define NORFLASH_STATUS_REG2_LB1     (1 << 3)
#define NORFLASH_STATUS_REG2_LB0     (1 << 2)
#define NORFLASH_STATUS_REG2_QE      (1 << 1)    // Quad Enable
#define NORFLASH_STATUS_REG2_SRP1    (1 << 0)

//NorFlash初始化。支持1线，2线或4线模式。
uint8_t NorFlash_Init(void);

//读取数据。根据配置自动选择1线，2线或4线模式。
uint8_t NorFlash_Read(uint32_t addr,uint8_t *buff, uint32_t len);

//编程数据。支持跨页编程。根据配置自动选择1线、4线模式。
uint8_t NorFlash_Program(uint32_t addr,uint8_t *buff, uint32_t len);

//擦除数据，支持跨页擦除。
uint8_t NorFlash_Erase(uint32_t addr,uint32_t len);

//写入数据（带自动擦除功能）。自动擦除扇区再写入，支持跨页写。根据配置自动选择1线，2线或4线模式。
uint8_t NorFlash_Write(uint32_t addr,uint8_t *buff, uint32_t len);

/******************************************************/

void NorFlash_Read_Manu_Device_ID(uint8_t buff[2]);

void NorFlash_Read_ID(uint8_t buff[3]);

void NorFlash_Read_Unique_ID(uint8_t buff[16]);

uint8_t NorFlash_ReadStatusReg1(void);

uint8_t NorFlash_ReadStatusReg2(void);

uint8_t NorFlash_WriteStatusReg(uint8_t reg1,uint8_t reg2);

uint8_t NorFlash_WaitBusy(uint32_t timeout);

uint8_t NorFlash_QuadEnable(uint8_t quadEnable);

void NorFlash_PowerDown(void);

void NorFlash_Wakeup(void);

void NorFlash_Reset(void);


uint8_t NorFlash_Read_Data_1x(uint32_t addr, uint8_t *buff, uint32_t len);

uint8_t NorFlash_Read_Data_Fast(uint32_t addr, uint8_t *buff, uint32_t len);

uint8_t NorFlash_Read_Dual_Output(uint32_t addr, uint8_t *buff, uint32_t len);

uint8_t NorFlash_Read_Dual_IO(uint32_t addr, uint8_t *buff, uint32_t len);

uint8_t NorFlash_Read_Quad_Output(uint32_t addr, uint8_t *buff, uint32_t len);

uint8_t NorFlash_Read_Quad_IO(uint32_t addr, uint8_t *buff, uint32_t len);


uint8_t NorFlash_PageProgram_1x(uint32_t addr, uint8_t *buff, uint32_t len);

uint8_t NorFlash_PageProgram_Dual(uint32_t addr, uint8_t *buff, uint32_t len);

uint8_t NorFlash_PageProgram_Quad(uint32_t addr, uint8_t *buff, uint32_t len);


uint8_t NorFlash_EraseSector(uint32_t addr);

uint8_t NorFlash_EraseSectors(uint32_t addr,uint16_t num);

uint8_t NorFlash_Erase32KBlock(uint32_t addr);

uint8_t NorFlash_Erase64KBlock(uint32_t addr);

uint8_t NorFlash_EraseChip(void);

#endif
