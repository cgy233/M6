#ifndef __IIC_H
#define __IIC_H
#include "APP.h"

// IO direction 

#define SDA_OUT()	{GPIOAB->DIR |= GPIO_PIN_4;}
#define SDA_IN()	{GPIOAB->DIR &= ~GPIO_PIN_4;}
HAL_GPIO_MODULE_ENABLED


// IO definition 
#define   IIC_SCL_L		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_CLEAR);
#define   IIC_SCL_H		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

#define   IIC_SDA_L		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_CLEAR);
#define   IIC_SDA_H		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); 
#define 	READ_SDA   HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) 	// SDA in



// Function declaration
void IIC_Init(void);                // 初始化IIC的IO口				 
void IIC_Start(void);				// 发送IIC开始信号
void IIC_Stop(void);	  			// 发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);	// IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 		// IIC等待ACK信号
void IIC_Ack(void);					// IIC发送ACK信号
void IIC_NAck(void);				// IIC不发送ACK信号

void IIC_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr, uint8_t addr);	  

void SI523_I2C_LL_Init(void);
void SI523_I2C_LL_WriteRawRC(unsigned char RegAddr,unsigned char value);
unsigned char SI523_I2C_LL_ReadRawRC(unsigned char RegAddr);

void I_SI523_IO_Init(void);
void I_SI523_IO_Write(unsigned char RegAddr,unsigned char value);
unsigned char I_SI523_IO_Read(unsigned char RegAddr);
void I_SI523_Reset(void);

#endif



















