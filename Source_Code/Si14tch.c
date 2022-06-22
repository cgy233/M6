#include "Si14tch.h"
#include "delay.h"
#include "nfc_iic.h"
#include "ACM32Fxx_HAL.h"


extern uint8_t a ;
uint8_t  IICReadData;
/********************************
 * 函数功能  ：SI14TCH初始化
 *
*********************************/
void SI14TCH_Init(void)
{
	delay_ms(200); 
	SI14TCH_WriteOneByte(Ch_hold1,0Xfe);     //delay_ms(10); //1111 1110 ch1 =0 通道一打开
	
	SI14TCH_WriteOneByte(CTRL2,   0X0f);     delay_ms(10); //0000 1111 enable software reset enable sleep mode
 //0000 0111 disable software reset enable sleep mode
 SI14TCH_WriteOneByte(CTRL2,   0X07);     delay_ms(10);
	//灵敏度设置
	SI14TCH_WriteOneByte(Sense1,  0X77);     //delay_ms(10); //0111 0111 低灵敏度
	SI14TCH_WriteOneByte(Sense2,  0X77);     //delay_ms(10);
	SI14TCH_WriteOneByte(Sense3,  0X77);     //delay_ms(10);
	SI14TCH_WriteOneByte(Sense4,  0X77);     //delay_ms(10);
	SI14TCH_WriteOneByte(Sense5,  0X77);     //delay_ms(10);
	SI14TCH_WriteOneByte(Sense6,  0X77);     //delay_ms(10);

	
	SI14TCH_WriteOneByte(CTRL1,   0X1B);


  SI14TCH_WriteOneByte(Ch_hold1,0X00);      //1111 1111 保持操作，不感测不校准
	SI14TCH_WriteOneByte(Ch_hold2,0x30);      //0011 1111    0x30 -- 0x00
  
  SI14TCH_WriteOneByte(Ref_rst1,0X00);      //0000 0000 禁用参考重置
	SI14TCH_WriteOneByte(Ref_rst2,0X00);     
  SI14TCH_WriteOneByte(Cal_hold1,0X00);     
  SI14TCH_WriteOneByte(Cal_hold2,0x00); 
 
	delay_ms(40);
  
}

void Si14TouchKey_Init(void)
{
  delay_ms(100);
//  IIC_EN = 0;
  delay_ms(10);
  SI14TCH_WriteOneByte(CTRL2,   0X0f);
  delay_ms(10);
 //  SI14TCH_WriteOneByte(CTRL2,   0X07);
  SI14TCH_WriteOneByte(Sense1,  Si12T_Sens_User);
  SI14TCH_WriteOneByte(Sense2,  Si12T_Sens_User);
  SI14TCH_WriteOneByte(Sense3,  Si12T_Sens_User);
  SI14TCH_WriteOneByte(Sense4,  Si12T_Sens_User);
  SI14TCH_WriteOneByte(Sense5,  Si12T_Sens_User);
  SI14TCH_WriteOneByte(Sense6,  Si12T_Sens_User);
	SI14TCH_WriteOneByte(Sense7,  Si12T_Sens_User);
  IICReadData=SI14TCH_ReadOneByte(0x02);
  
 
  SI14TCH_WriteOneByte(CTRL1,   0X38);
  SI14TCH_WriteOneByte(Ch_hold1,0x00);  
  SI14TCH_WriteOneByte(Ch_hold2,0x30);  
  SI14TCH_WriteOneByte(Ref_rst1,0X00);     
	SI14TCH_WriteOneByte(Ref_rst2,0X00);  
  
  SI14TCH_WriteOneByte(Cal_hold1,0Xff);     
  SI14TCH_WriteOneByte(Cal_hold2,0x0f);     
	
 
//	SI14TCH_WriteOneByte(CTRL2,   0X07);
////  delay_ms(500);
////  delay_ms(500);
////  delay_ms(500);
//  delay_ms(500);
//  delay_ms(200);
 // SI14TCH_WriteOneByte(CTRL1,   0X03);
//  SI14TCH_WriteOneByte(Ref_rst1,   0X00);
//  SI14TCH_WriteOneByte(Ref_rst2,   0X00);
  SI14TCH_WriteOneByte(Ch_hold1,   0Xfe);
  SI14TCH_WriteOneByte(Ch_hold2,   0X3f);
//  SI14TCH_WriteOneByte(CTRL2,   0X07);
 SI14TCH_WriteOneByte(CTRL2,   0X07);
 delay_ms(40);
   SI14TCH_WriteOneByte(0x3B,   0Xa5);
   SI14TCH_WriteOneByte(0x3d,   0X01);
//  
  delay_us(800);

  
	// IIC_EN = 1;
  
}
/********************************
 * 函数功能  ：SI14TCH写字节
 * 参数1     ：芯片寄存器地址
 * 参数2     ：写入寄存器的值
 *
*********************************/
void SI14TCH_WriteOneByte(uint8_t WriteAddr,uint8_t DataToWrite)
{
	IIC_Start();
	IIC_Send_Byte(SI14TCH_WRITE_ADDR);  //打开SI14TCH设备写地址
	//IIC_Send_Byte(TSM12_WRITE_ADDR1);
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);
	IIC_Wait_Ack();
	IIC_Stop();
	//delay_ms(10);
}



/********************************
 * 函数功能 ：SI14TCH读字节
 * 参数     ：芯片寄存器地址
 * 返回值   ：芯片寄存器地址值
 *
*********************************/
uint8_t SI14TCH_ReadOneByte(uint8_t ReadAddr)
{
	uint8_t temp=0;
	IIC_Start();
	IIC_Send_Byte(SI14TCH_WRITE_ADDR);

	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr);
	IIC_Wait_Ack();
	IIC_Stop();
	//delay_ms(10);
	IIC_Start();
	IIC_Send_Byte(SI14TCH_READ_ADDR);

	IIC_Wait_Ack();
	temp=IIC_Read_Byte(0); //读取一个字节后，产生NAck
	IIC_Stop();
	return temp;
}



/*=================================================================
 读取寄存器output1,2,3数据

	        output3           output2          output1
     |     8bit         |    8bit        |   8bit
     | 00   00  00   00 | 00  00  00  00 | 00  00  00  00
     | t12  t11 t10  t9 | t8  t7  t6  t5 | t4  t3  t2  t1

=====================================================================*/
uint8_t data_buf[4] = {0};


void SI12_ReadData(void)
{
	//IIC_EN = 0 ; //使能 
	
  delay_ms (50);
	data_buf[0] = SI14TCH_ReadOneByte(Output1);
	printf("\r\n output1=0x%2x",data_buf[0]); 
	
	data_buf[1] = SI14TCH_ReadOneByte(Output2);
	 printf("   output2=0x%2x",data_buf[1]); 
	
	data_buf[2] = SI14TCH_ReadOneByte(Output3);
	 printf("   output3=0x%2x",data_buf[2]); 
	
	data_buf[3] = SI14TCH_ReadOneByte(Output4);
	 printf("   output4=0x%2x",data_buf[3]); 
	
	//IIC_EN = 1 ; //关闭 
}

/*=================================================================
 读取寄存器output1,2,3数据

	        output3           output2          output1
     |     8bit         |    8bit        |   8bit
     | 00   00  00   00 | 00  00  00  00 | 00  00  00  00
     | t12  t11 t10  t9 | t8  t7  t6  t5 | t4  t3  t2  t1

=====================================================================*/

uint32_t SI14TCH_ReadData(void)
{
	uint32_t SI14TCH_data_buf=0;
	uint8_t data_SI14TCH;
	
	data_SI14TCH=SI14TCH_ReadOneByte(Output4);
	SI14TCH_data_buf|=data_SI14TCH;
	
	SI14TCH_data_buf=SI14TCH_data_buf<<8;
	data_SI14TCH=SI14TCH_ReadOneByte(Output3);
	SI14TCH_data_buf|=data_SI14TCH;
	
	SI14TCH_data_buf=SI14TCH_data_buf<<8;
	data_SI14TCH=SI14TCH_ReadOneByte(Output2);
	SI14TCH_data_buf|=data_SI14TCH;
	
	SI14TCH_data_buf=SI14TCH_data_buf<<8;
	data_SI14TCH=SI14TCH_ReadOneByte(Output1);
	SI14TCH_data_buf|=data_SI14TCH;
	
	return SI14TCH_data_buf;	
}

/********************************
 * 函数功能：芯片检测
 * 返回值  1：检查失败
 *         0：检查成功
*********************************/
uint8_t SI14TCH_Check(void)
{
	uint8_t temp;
	SI14TCH_WriteOneByte(Sense1,0xAA);
  
  a=SI14TCH_ReadOneByte(Sense1);
	temp=SI14TCH_ReadOneByte(Sense1);
	printf("\r\n temp11111111111111111111=0x%02X \r\n",temp);
	
	if(temp==0xAA)
	{
		SI14TCH_WriteOneByte(Sense1,0x77);
		return 0;
	}
	return 1;
}


/********************************
 * 函数功能：芯片硬复位
 *
*********************************/
void SI14TCH_hard_Reset(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);//rst =0
	delay_ms(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_CLEAR); //rst = 1
	delay_ms(2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);//rst =0
	delay_ms(1);
}

void  Si12T_EnterSleep(void)
{
  //IIC_EN=0;
//  SI14TCH_WriteOneByte(CTRL1,   0X07);
  delay_ms(200);
  SI14TCH_WriteOneByte(CTRL2,   0X07);
  SI14TCH_WriteOneByte(Ref_rst1,   0X00);
  SI14TCH_WriteOneByte(Ref_rst2,   0X00);
  SI14TCH_WriteOneByte(Ch_hold1,   0X00);
  SI14TCH_WriteOneByte(Ch_hold2,   0X30);
  SI14TCH_WriteOneByte(CTRL2,   0X07);
 //SI14TCH_WriteOneByte(CTRL1,   0X07);
   SI14TCH_WriteOneByte(0x3B,   0Xa5);
   SI14TCH_WriteOneByte(0x3d,   0X01);
//  
  delay_us(800);
//  EXTI->IMR |=0x00000008;     //Enable external interrupt
}
void  Si12T_WaitInit()
{
  uint16_t timeout =300;
  while(SI14TCH_ReadData()  !=0)
  {
    delay_ms(10);
    timeout--;
    if(timeout  == 0)
    {
      break;
    }
  }
}
