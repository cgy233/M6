#include "nfc_iic.h"
#include "ACM32Fxx_HAL.h"
/*======================================================
TSM12寄存器定义
=======================================================*/
#define SI14TCH_WRITE_ADDR     0xD0    // if ID_SEL =0 Address = 0xD0 else Address = 0xF0  b[7:1]=address b0 = R/W  1 == R 0==W
#define SI14TCH_READ_ADDR      0XD1

#define SI14TCH_WRITE_ADDR2    0xF0
#define SI14TCH_READ_ADDR2     0xF1


#define Sense1               0x02    //channel 2 and 1  灵敏度控制寄存器
#define Sense2               0x03    //channel 4 and 3
#define Sense3               0x04    //channel 6 and 5
#define Sense4               0x05    //channel 8 and 7
#define Sense5               0x06    //channel 10 and 9
#define Sense6               0x07    //channel 12 and 11
#define Sense7               0x22    //channel 14 and 13  新增
#define CTRL1                0x08    //通用控制寄存器1
#define CTRL2                0x09		 //通用控制寄存器2
#define Ref_rst1             0x0A    //通道参考复位控制寄存器
#define Ref_rst2             0x0B
#define Ch_hold1             0x0C    //通道感应控制寄存器
#define Ch_hold2             0x0D
#define Cal_hold1            0x0E    //通道校准控制寄存器
#define Cal_hold2            0x0F
#define Output1              0x10     //channel 1 , 2 , 3 , 4
#define Output2              0x11     //channel 5 , 6 , 7 , 8
#define Output3              0x12     //channel 9 , 10 , 11 , 12
#define Output4              0x13     //新增 channel 13 , 14 

#define Si12T_Sens_User       0x99

//TSM12操作函数
void SI14TCH_Init(void);

void SI14TCH_WriteOneByte(uint8_t WriteAddr,uint8_t DataToWrite);

uint8_t SI14TCH_ReadOneByte(uint8_t ReadAddr);

uint8_t SI14TCH_Check(void);

uint32_t SI14TCH_ReadData(void);


void SI14TCH_hard_Reset(void);

void SI12_ReadData(void);


void Si14TouchKey_Init(void);
void  Si12T_EnterSleep(void);

void  Si12T_WaitInit(void);
