#include "nfc_iic.h"
#include "delay.h"
#include "SI523_App.h"
 //PA.6 --- SCK ---(SPI.MISO)
 //PA.4 --- SDA ---(SPI.CSN)
 
// Si523
#define SLA_ADDR 0x50
// Si12T
//#define SLA_ADDR 0xD0

//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;

//	System_SysTick_Init();
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE );
//	RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
	
	GPIO_InitStructure.Pin = IO_IIC_SCL | IO_IIC_SDA ;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;			//Set output type
	GPIO_InitStructure.Alternate = GPIO_FUNCTION_0;
	HAL_GPIO_Init(IO_IIC_PORT, &GPIO_InitStructure);
	
 	HAL_GPIO_WritePin(IO_IIC_PORT, IO_IIC_SCL | IO_IIC_SDA, GPIO_PIN_SET);	
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_H;	  	  
	IIC_SCL_H;
	delay_us(10);
 	IIC_SDA_L;//START:when CLK is high,DATA change form high to low 
	delay_us(10);
	IIC_SCL_L;//钳住I2C总线，准备发送或接收数据 
}	
 
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_L;
	IIC_SDA_L;//STOP:when CLK is high DATA change form low to high
 	delay_us(10);
	IIC_SCL_H; 
	delay_us(10);
	IIC_SDA_H;//发送I2C总线结束信号
	delay_us(10);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();//sda线输出
	IIC_SDA_H;delay_us(10);	   
	IIC_SCL_H;delay_us(10);	
	 
	while(READ_SDA!=0)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_L;//时钟输出0 	   
	return 0;  
}
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_L;
	SDA_OUT();
	IIC_SDA_L;
	delay_us(8);
	IIC_SCL_H;
	delay_us(8);
	IIC_SCL_L;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL_L;
	SDA_OUT();
	IIC_SDA_H;
	delay_us(8);
	IIC_SCL_H;
	delay_us(8);
	IIC_SCL_L;
}				 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
  uint8_t t; 
	uint8_t  getpin;
	
	SDA_OUT(); 	    
    IIC_SCL_L;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		getpin=(txd&0x80)>>7;
		if(getpin ==0)
		{
			IIC_SDA_L;
		}
		else 
		{
			IIC_SDA_H;
		}
		
        txd<<=1; 	  
		delay_us(8);   //对TEA5767这三个延时都是必须的
		IIC_SCL_H;
		delay_us(8); 
		IIC_SCL_L;	
		delay_us(8);
    }	
}  	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_L; 
        delay_us(8);
		IIC_SCL_H;
        receive<<=1;
        if(READ_SDA!=0)receive++;   
		delay_us(10); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


void SI523_I2C_LL_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE );
	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE );
	
	GPIO_InitStructure.Pin = GPIO_PIN_3;	//CE(NRST)
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  			
	GPIO_InitStructure.Alternate = GPIO_FUNCTION_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_CLEAR);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	//GPIO_SetBits(GPIOA,GPIO_PIN_2); 
	
//	GPIO_InitStructure.Pin =  GPIO_Pin_3 ;	//IRQ
//	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 			 //Set GPIO Direction : Out
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd = IRQMODE;     //新增
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	
	IIC_Init();
}

void SI523_I2C_LL_WriteRawRC(unsigned char RegAddr,unsigned char value)
{
	IIC_Start();  				 
	IIC_Send_Byte(SLA_ADDR);     	//发送写器件指令	 
	IIC_Wait_Ack();	   
  IIC_Send_Byte(RegAddr);   			//发送寄存器地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(value);     		//发送值					   
	IIC_Wait_Ack();  		    	   
  IIC_Stop();						//产生一个停止条件 	
}


unsigned char SI523_I2C_LL_ReadRawRC(unsigned char RegAddr)
{
	uint8_t RegVal=0;
	IIC_Start(); 

	IIC_Send_Byte(SLA_ADDR);	//发送写器件指令	 
	RegVal = IIC_Wait_Ack();

	IIC_Send_Byte(RegAddr);   		//发送寄存器地址
	RegVal = IIC_Wait_Ack(); 

	IIC_Start();  	 	   		//重新启动
	IIC_Send_Byte(SLA_ADDR + 1);	//发送读器件指令	

	RegVal = IIC_Wait_Ack();	   
	RegVal = IIC_Read_Byte(0);		//读取一个字节,不继续再读,发送NAK 	  	
	IIC_Stop();					//产生一个停止条件 
	return RegVal;
}

void I_SI523_IO_Init(void)
{
    SI523_I2C_LL_Init();
}
void I_SI523_IO_Write(unsigned char RegAddr,unsigned char value)
{
    SI523_I2C_LL_WriteRawRC(RegAddr,value);
}
unsigned char I_SI523_IO_Read(unsigned char RegAddr)
{
    return SI523_I2C_LL_ReadRawRC(RegAddr);
}

void I_SI523_Reset(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_CLEAR);
	delay_us(100);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
	delay_us(100);
	
}














