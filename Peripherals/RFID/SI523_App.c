
//#include "systick.h"
#include <rthw.h>
#include "nfc_iic.h"
#include "delay.h"
#include "SI523_App.h"
#include <stdlib.h>
#include <stdio.h>
#include "exti.h"
#include "power.h"
#include <rtthread.h>

//***********************************//修改新增内容
extern uint8_t PCD_IRQ_flagA ;
unsigned char ACDConfigRegK_Val = 0x2c ;
unsigned char PCDKCONFIG = 0x2c;
unsigned char ACDConfigRegC_Val ;

/////////////////////////////////////////////////////////////////////
//开启天线  
//每次启动或关闭天险发射之间应至少有1ms的间隔
/////////////////////////////////////////////////////////////////////
void PcdAntennaOn(void)
{
    unsigned char i;
    i = I_SI523_IO_Read(TxControlReg);
    if (!(i & 0x03))
    {
        I_SI523_SetBitMask(TxControlReg, 0x03);
    }
}

/////////////////////////////////////////////////////////////////////
//关闭天线
/////////////////////////////////////////////////////////////////////
void PcdAntennaOff(void)
{
	I_SI523_ClearBitMask(TxControlReg, 0x03);
}

/////////////////////////////////////////////////////////////////////
//用MF522计算CRC16函数
/////////////////////////////////////////////////////////////////////
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    I_SI523_ClearBitMask(DivIrqReg,0x04);
    I_SI523_IO_Write(CommandReg,PCD_IDLE);
    I_SI523_SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   I_SI523_IO_Write(FIFODataReg, *(pIndata+i));   }
    I_SI523_IO_Write(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = I_SI523_IO_Read(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = I_SI523_IO_Read(CRCResultRegL);
    pOutData[1] = I_SI523_IO_Read(CRCResultRegH);
}


unsigned char aaa = 0;


/////////////////////////////////////////////////////////////////////
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
/////////////////////////////////////////////////////////////////////
//status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
        case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
    }
   
    //I_SI523_IO_Write(ComIEnReg,irqEn|0x80);
    I_SI523_ClearBitMask(ComIrqReg,0x80);
    I_SI523_IO_Write(CommandReg,PCD_IDLE);
    I_SI523_SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   
		I_SI523_IO_Write(FIFODataReg, pInData[i]);    
	}
    I_SI523_IO_Write(CommandReg, Command);
   
    if (Command == PCD_TRANSCEIVE)
    {    
		I_SI523_SetBitMask(BitFramingReg,0x80);  
	}
    
    //i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
	i = 2000;
    do 
    {
        n = I_SI523_IO_Read(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    I_SI523_ClearBitMask(BitFramingReg,0x80);

    if (i!=0)
    {   
		aaa = I_SI523_IO_Read(ErrorReg);
		
        if(!(I_SI523_IO_Read(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   status = MI_NOTAGERR;   }
            if (Command == PCD_TRANSCEIVE)
            {
               	n = I_SI523_IO_Read(FIFOLevelReg);
              	lastBits = I_SI523_IO_Read(ControlReg) & 0x07;
                if (lastBits)
                {   
					*pOutLenBit = (n-1)*8 + lastBits;   
				}
                else
                {   
					*pOutLenBit = n*8;   
				}
                if (n == 0)
                {   
					n = 1;    
				}
                if (n > MAXRLEN)
                {   
					n = MAXRLEN;   
				}
                for (i=0; i<n; i++)
                {   
					pOutData[i] = I_SI523_IO_Read(FIFODataReg);    
				}
            }
        }
        else
        {   
			status = MI_ERR;   
		}
        
    }
   
    I_SI523_SetBitMask(ControlReg,0x80);           // stop timer now
    I_SI523_IO_Write(CommandReg,PCD_IDLE); 
    return status;
}
                     
/////////////////////////////////////////////////////////////////////
//功    能：寻卡
//参数说明: req_code[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//          pTagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
	char status;  
	unsigned int unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 

	I_SI523_ClearBitMask(Status2Reg,0x08);
	I_SI523_IO_Write(BitFramingReg,0x07);
	I_SI523_SetBitMask(TxControlReg,0x03);
 
	ucComMF522Buf[0] = req_code;

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   
		status = MI_ERR;   
	}
   
	return status;
}


/////////////////////////////////////////////////////////////////////
//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////  
char PcdAnticoll(unsigned char *pSnr, unsigned char anticollision_level)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    I_SI523_ClearBitMask(Status2Reg,0x08);
    I_SI523_IO_Write(BitFramingReg,0x00);
    I_SI523_ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = anticollision_level;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
	{
		for (i=0; i<4; i++)
		{   
			*(pSnr+i)  = ucComMF522Buf[i];
			snr_check ^= ucComMF522Buf[i];
		}
		if (snr_check != ucComMF522Buf[i])
   		{   
			status = MI_ERR;    
		}
    }
    
    I_SI523_SetBitMask(CollReg,0x80);
    return status;
}


/////////////////////////////////////////////////////////////////////
//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回MI_OK
////////////////////////////////////////////////////////////////////
char PcdSelect (unsigned char * pSnr, unsigned char *sak)
{
    char status;
    unsigned char i;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);                                                                      
  
    I_SI523_ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   
		*sak = ucComMF522Buf[0];
		status = MI_OK;  
	}
    else
    {   
		status = MI_ERR;    
	}

    return status;
}

char PcdSelect1 (unsigned char * pSnr, unsigned char *sak)
{
    char status;
    unsigned char i;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    I_SI523_ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   
		*sak = ucComMF522Buf[0];
		status = MI_OK;  
	}
    else
    {   
		status = MI_ERR;    
	}

    return status;
}

char PcdSelect2 (unsigned char * pSnr, unsigned char *sak)
{
    char status;
    unsigned char i;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL2;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    I_SI523_ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   
		*sak = ucComMF522Buf[0];
		status = MI_OK;  
	}
    else
    {   
		status = MI_ERR;    
	}

    return status;
}

char PcdSelect3 (unsigned char * pSnr, unsigned char *sak)
{
    char status;
    unsigned char i;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL2;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    I_SI523_ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   
		*sak = ucComMF522Buf[0];
		status = MI_OK;  
	}
    else
    {   
		status = MI_ERR;    
	}

    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdHalt(void)
{
    char status;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////               
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
	memcpy(&ucComMF522Buf[2], pKey, 6); 
	memcpy(&ucComMF522Buf[8], pSnr, 6); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(I_SI523_IO_Read(Status2Reg) & 0x08)))
    {
		status = MI_ERR;   
	}
    
    return status;
}



/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 
char PcdRead(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
   	{   
		memcpy(pData, ucComMF522Buf, 16);   
	}
    else
    {   
		status = MI_ERR;   
	}
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                  
char PcdWrite(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   
		status = MI_ERR;   
	}
        
    if (status == MI_OK)
    {
        memcpy(ucComMF522Buf, pData, 16);
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   
			status = MI_ERR;   
		}
    }
    
    return status;
}



/*=================================
 函数功能：循环读取A卡UID

=================================*/
void PCD_SI523_TypeA(void)
{
	//LED_OperaSuccess();	// LED indicator
	
	while(1)
	{
		PCD_SI523_TypeA_GetUID();		//读A卡
		
		//PCD_SI523_TypeA_rw_block();		//读A卡扇区

		delay_ms(500);
	}
}

/*================================
 函数功能：循环读取B卡UID

=================================*/
void PCD_SI523_TypeB(void)
{
	//LED_OperaSuccess();	// LED indicator
	
	while(1)
	{
		PCD_SI523_TypeB_GetUID();		//读B卡
		
		//PCD_SI523_IdentityCard_GetUID();		//读身份证

		delay_ms(500);
		
		printfS("%02x",I_SI523_IO_Read(0x37));
	}
}




/*===============================
 函数功能：读A卡初始化配置

 ================================*/
void PCD_SI523_TypeA_Init(void)
{
	I_SI523_IO_Init();	// Initializes the interface with Si522	
	delay_ms(500);
	
	I_SI523_ClearBitMask(Status2Reg, 0x08);  
	// Reset baud rates
	I_SI523_IO_Write(TxModeReg, 0x00);
	I_SI523_IO_Write(RxModeReg, 0x00);
	// Reset ModWidthReg
	I_SI523_IO_Write(ModWidthReg, 0x26);
	// RxGain:110,43dB by default;
	I_SI523_IO_Write(RFCfgReg, RFCfgReg_Val);
	// When communicating with a PICC we need a timeout if something goes wrong.
	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
	// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.
	I_SI523_IO_Write(TModeReg, 0x80);// TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
	I_SI523_IO_Write(TPrescalerReg, 0xa9);// TPreScaler = TModeReg[3..0]:TPrescalerReg
	I_SI523_IO_Write(TReloadRegH, 0x03); // Reload timer 
	I_SI523_IO_Write(TReloadRegL, 0xe8); // Reload timer 
	I_SI523_IO_Write(TxASKReg, 0x40);	// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	I_SI523_IO_Write(ModeReg, 0x3D);	// Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
	I_SI523_IO_Write(CommandReg, 0x00);  // Turn on the analog part of receiver   

	PcdAntennaOn();
}



char PCD_SI523_TypeA_GetUID(void)
{
	unsigned char ATQA[2];
	unsigned char UID[12];
	unsigned char SAK = 0;
	unsigned char UID_complate1 = 0;
	unsigned char UID_complate2 = 0;

	// printfS("\r\nTest_Si523_GetUID");
	I_SI523_IO_Write(RFCfgReg, RFCfgReg_Val); //复位接收增益
	
	//寻卡
	if( PcdRequest( PICC_REQIDL, ATQA) != MI_OK )  //寻天线区内未进入休眠状态的卡，返回卡片类型 2字节	
	{
		I_SI523_IO_Write(RFCfgReg, 0x48);
		if(PcdRequest( PICC_REQIDL, ATQA) != MI_OK)
		{
			I_SI523_IO_Write(RFCfgReg, 0x58);
			if(PcdRequest( PICC_REQIDL, ATQA) != MI_OK)
			{	
				printfS("\r\nRequest:fail");
				return 1;
			}
			else
			{
				printfS("\r\nRequest1:ok  ATQA:%02x %02x",ATQA[0],ATQA[1]);
			}	
		}
		else
		{
			printfS("\r\nRequest2:ok  ATQA:%02x %02x",ATQA[0],ATQA[1]);
		}		
	}
	else
	{
		printfS("\r\nRequest3:ok  ATQA:%02x %02x",ATQA[0],ATQA[1]);
	}
	
	
//UID长度=4
	//Anticoll 冲突检测 level1
	if(PcdAnticoll(UID, PICC_ANTICOLL1)!= MI_OK) 
	{
		printfS("\r\nAnticoll1:fail");
		return 1;		
	}
	else
	{
		if(PcdSelect1(UID,&SAK)!= MI_OK)
		{
			printfS("\r\nSelect1:fail");
			return 1;		
		}
		else
		{
			printfS("\r\nSelect1:ok  SAK1:%02x",SAK);
			if(SAK&0x04)                         
			{
				UID_complate1 = 0;
				
				//UID长度=7
				if(UID_complate1 == 0)    
				{
					//Anticoll 冲突检测 level2
					if(PcdAnticoll(UID+4, PICC_ANTICOLL2)!= MI_OK) 
					{
						printfS("\r\nAnticoll2:fail");
						return 1;		
					}
					else
					{
						if(PcdSelect2(UID+4,&SAK)!= MI_OK)  
						{
							printfS("\r\nSelect2:fail");
							return 1;		
						}
						else
						{
							printfS("\r\nSelect2:ok  SAK2:%02x",SAK);
							if(SAK&0x04)                         
							{
								UID_complate2 = 0;
								
								//UID长度=10
								if(UID_complate2 == 0)     
								{
									//Anticoll 冲突检测 level3
									if(PcdAnticoll(UID+8, PICC_ANTICOLL3)!= MI_OK) 
									{
										printfS("\r\nAnticoll3:fail");
										return 1;		
									}
									else
									{
										if(PcdSelect3(UID+8,&SAK)!= MI_OK)  
										{
											printfS("\r\nSelect3:fail");
											return 1;		
										}
										else
										{
											printfS("\r\nSelect3:ok  SAK3:%02x",SAK);
											if(SAK&0x04)                          
											{
//												UID_complate3 = 0;
											}
											else 
											{
	//											UID_complate3 = 1;                   
												printfS("\r\nAnticoll3:ok  UID:%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
												UID[1],UID[2],UID[3],UID[5],UID[6],UID[7],UID[8],UID[9],UID[10],UID[11]);									
											}					
										}							
									}
								}
							}
							else 
							{
								UID_complate2 = 1;                  
								printfS("\r\nAnticoll2:ok  UID:%02x %02x %02x %02x %02x %02x %02x",
								UID[1],UID[2],UID[3],UID[4],UID[5],UID[6],UID[7]);
							}	
						}			
					}
				}
			}
			else 
			{
				UID_complate1 = 1;                   
				printfS("\r\nAnticoll1:ok  UID:%02x %02x %02x %02x",UID[0],UID[1],UID[2],UID[3]);
			}
		}		
	}
	//Halt
//	if(PcdHalt() != MI_OK)
//	{
//		printf("\r\nHalt:fail");
//		return 1;		
//	}
//	else
//	{
//		printf("\r\nHalt:ok");
//	}	
	
	// delay_us(100);
	return 0;
}

char PCD_SI523_TypeA_rw_block(void)
{
	unsigned char ATQA[2];
	unsigned char UID[12];
	unsigned char SAK = 0;
	unsigned char CardReadBuf[16] = {0};
	unsigned char CardWriteBuf[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	unsigned char DefaultKeyABuf[10] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	printfS("\r\n\r\nTest_Si522_GetCard");
	
	//request 寻卡
	if( PcdRequest( PICC_REQIDL, ATQA) != MI_OK )  //寻天线区内未进入休眠状态的卡，返回卡片类型 2字节	
	{
		printfS("\r\nRequest:fail");
		return 1;		
	}
	else
	{
		printfS("\r\nRequest:ok  ATQA:%02x %02x",ATQA[0],ATQA[1]);
	}
	

	//Anticoll 冲突检测
	if(PcdAnticoll(UID, PICC_ANTICOLL1)!= MI_OK)
	{		
		printfS("\r\nAnticoll:fail");
		return 1;		
	}
	else
	{	
		printfS("\r\nAnticoll:ok  UID:%02x %02x %02x %02x",UID[0],UID[1],UID[2],UID[3]);
	}
	
	//Select 选卡
	if(PcdSelect1(UID,&SAK)!= MI_OK)
	{
		printfS("\r\nSelect:fail");
		return 1;		
	}
	else
	{
		printfS("\r\nSelect:ok  SAK:%02x",SAK);
	}

	//Authenticate 验证密码
	if(PcdAuthState( PICC_AUTHENT1A, 4, DefaultKeyABuf, UID ) != MI_OK )
	{
		printfS("\r\nAuthenticate:fail");
		return 1;		
	}
	else
	{
		printfS("\r\nAuthenticate:ok");
	}

	//读BLOCK原始数据
	if( PcdRead( 4, CardReadBuf ) != MI_OK )
	{
		printfS("\r\nPcdRead:fail");
		return 1;		
	}
	else
	{
		printfS("\r\nPcdRead:ok  ");
		for(unsigned char i=0;i<16;i++)
		{
			printfS(" %02x",CardReadBuf[i]);
		}
	}

	//产生随机数
	for(unsigned char i=0;i<16;i++)
		CardWriteBuf[i] = rand();

	//写BLOCK 写入新的数据
	if( PcdWrite( 4, CardWriteBuf ) != MI_OK )
	{
		printfS("\r\nPcdWrite:fail");
		return 1;	
	}
	else
	{
		printfS("\r\nPcdWrite:ok  ");
		for(unsigned char i=0;i<16;i++)
		{
			printfS(" %02x",CardWriteBuf[i]);
		}
	}
		
	//读BLOCK 读出新写入的数据
	if( PcdRead( 4, CardReadBuf ) != MI_OK )
	{
		printfS("\r\nPcdRead:fail");
		return 1;		
	}
	else
	{
		printfS("\r\nPcdRead:ok  ");
		for(unsigned char i=0;i<16;i++)
		{
			printfS(" %02x",CardReadBuf[i]);
		}
	}
		
//	//Halt
//	if(PcdHalt() != MI_OK)
//	{
//		printf("\r\nHalt:fail");
//		return 1;		
//	}
//	else
//	{
//		printf("\r\nHalt:ok");
//	}	
	
	return 0;
}

void PCD_SI523_TypeB_Init(void)
{
	I_SI523_IO_Init();	// Initializes the interface with Si522	
	delay_ms(500);
	
	I_SI523_ClearBitMask(Status2Reg, 0x08);
	I_SI523_IO_Write(ModeReg, 0x3F);  // For 0xFFFF crc
	I_SI523_IO_Write(TReloadRegL, 30);
	I_SI523_IO_Write(TReloadRegH, 0);
	I_SI523_IO_Write(TModeReg, 0x8D);
	I_SI523_IO_Write(TPrescalerReg, 0x3E); 
	I_SI523_IO_Write(TxASKReg, 0);  // Force 100ASK = 0//		delay_ms(100);
	
	I_SI523_IO_Write(GsNReg, 0xff);  // TX输出电导设置f8 fa N
	I_SI523_IO_Write(CWGsPReg, 0x3f);	 // P_改变1的幅度
	I_SI523_IO_Write(ModGsPReg, 0x07);  // 调制指数设置RegModGsp,, TYPEB ModConductance 0x1A P_改变0的幅度
	I_SI523_IO_Write(TxModeReg, 0x83);  // 编码器设置,106kbps,14443B 03
	I_SI523_IO_Write(BitFramingReg, 0x00);   // 调制脉宽,0x13->2.95us RegTypeBFraming ,,TYPEB
	I_SI523_IO_Write(AutoTestReg, 0x00);   
	// 低二位为接收增益，
	// 00,10,20,30,40,50,60,70
	// 18,23,18,23,33,38,43,48dB
	I_SI523_IO_Write(RFCfgReg, RFCfgReg_Val);          
	I_SI523_IO_Write(RxModeReg, 0x83);                 
	I_SI523_IO_Write(RxThresholdReg, 0x65);     
	
	I_SI523_ClearBitMask(RxSelReg,0x3F);
	I_SI523_SetBitMask(RxSelReg, 0x08);
	I_SI523_ClearBitMask(TxModeReg, 0x80);   // 无CRC,无奇偶校验
	I_SI523_ClearBitMask(RxModeReg, 0x80);
	I_SI523_ClearBitMask(Status2Reg, 0x08);   // MFCrypto1On =0			

	PcdAntennaOn();
}

char PCD_SI523_TypeB_GetUID(void)
{
	//printf("\r\n\r\nTest_B_GetUID");
	
	//I_SI523_IO_Write(0x02, 0xa0); //打开接收中断,则读卡会产生中断
	// Enable external interrupt
	//EXTI->IMR |= 0x00000008;
	
	//request 寻B卡;返回卡号
	uint32_t 		len1;
	unsigned char 	buf1[18] = {0x05,0x00,0x00,0x71,0xFF};
	
	if(PcdComMF522(PCD_TRANSCEIVE, buf1, 5, buf1, &len1) != MI_OK)
	{
		printfS("\r\nRequest:fail");
		return 1;		
	}
	else
	{	
		if( buf1[0] == 0x50 ) //判断是不是ATQB
		printfS("\r\nRequest:ok  UID:%02x %02x %02x %02x",
								buf1[1],buf1[2],buf1[3],buf1[4]);
	}	
	
	return 0;
}



char PCD_SI523_IdentityCard_GetUID(void)
{
	//printf("\r\n\r\nTest_identitycard");
	
	//request 寻B卡
	uint32_t 		len1;
	unsigned char 	buf1[18] = {0x05,0x00,0x00,0x71,0xFF};

	if(PcdComMF522(PCD_TRANSCEIVE, buf1, 5, buf1, &len1) != MI_OK)
	{
		printfS("\r\n\r\nRequest:fail");
		return 1;
	}
	
	//I_SI523_IO_Write(0x02, 0xa0); //打开接收中断,则读卡会产生中断
	// Enable external interrupt
	//EXTI->IMR |= 0x00000008;
	
	//发送二代证非标ATTRIB指令
	uint32_t 		len2;
	unsigned char 	buf2[18] = {0x1D,   0x00,0x00,0x00,0x00,   0x00,  0x08,  0x01,  0x08,  0xF3,  0x10};		
	if(PcdComMF522(PCD_TRANSCEIVE, buf2, 11, buf2, &len2) != MI_OK)
	{
		printfS("\r\nATTRIB:fail");
		return 1;		
	}	
		
	//获取UID	
	uint32_t 		len3;
	unsigned char 	buf3[18] = {0x00,0x36,0x00,0x00, 0x08,0x57,0x44};	

	if(PcdComMF522(PCD_TRANSCEIVE, buf3, 7, buf3, &len3) != MI_OK)
	{
		printfS("\r\nUID:fail");
		return 1;		
	}	
	else
	{	
		if( buf3[8] == 0x90||buf3[9] == 0x00 ) //判断是不是identitycard
		printfS("\r\nUID:ok  UID:%02x %02x %02x %02x %02x %02x %02x %02x ",
								buf3[0],buf3[1],buf3[2],buf3[3],buf3[4],buf3[5],buf3[6],buf3[7]);
	}
	
	return 0;
}




//***********************************//修改新增内容

/*
 * 函数名：PcdReset
 * 描述  ：复位RC522 
 * 输入  ：无
 * 返回  : 无
 * 调用  ：外部调用
 */
void PcdReset ( void )
{
	//hard reset
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_CLEAR);
	delay_us(100);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
	delay_us(100);
	
	I_SI523_IO_Write(CommandReg, 0x0f);			//向CommandReg 写入 0x0f	作用是使RC522复位
	while(I_SI523_IO_Read(CommandReg) & 0x10 );	//Powerdown位为0时，表示RC522已准备好
	delay_us(100);
}

void PcdPowerdown (void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_CLEAR);
	delay_us(100);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
	delay_us(100);
}


//SI523_interfaces
void I_SI523_ClearBitMask(unsigned char reg,unsigned char mask)  
{
	char tmp = 0x00;
	tmp = I_SI523_IO_Read(reg);
	I_SI523_IO_Write(reg, tmp & ~mask);  // clear bit mask
} 

void I_SI523_SetBitMask(unsigned char reg,unsigned char mask)  
{
	char tmp = 0x00;
	tmp = I_SI523_IO_Read(reg);
	I_SI523_IO_Write(reg,tmp | mask);  // set bit mask
}

void I_SI523_SiModifyReg(unsigned char RegAddr, unsigned char ModifyVal, unsigned char MaskByte)
{
	unsigned char RegVal;
	RegVal = I_SI523_IO_Read(RegAddr);
	if(ModifyVal)
	{
			RegVal |= MaskByte;
	}
	else
	{
			RegVal &= (~MaskByte);
	}
	I_SI523_IO_Write(RegAddr, RegVal);
}


/*===============================
 函数功能：ACD模式初始化配置

 ================================*/
int ACD_init_Fun(void)
{
	PCD_SI523_TypeA_Init();
	
	PCD_ACD_AutoCalc(); //自动获取阈值
	
	PCD_ACD_Init();
	return 0;
}
INIT_APP_EXPORT(ACD_init_Fun);

/*===============================
 函数功能：ACD寻卡

 ================================*/
int ACD_Fun(void)
{
	//EXTI->IMR |= 0x00000008;	// Enable external interrupt
	EXTI->IENR |=  RF_IRQ_EXTI_LINE;
	PCD_IRQ_flagA = 0;	//clear IRQ flag
	//LED_OperaSuccess();	// LED indicator
	//APP_Standby_Test();
	
	while(1)
	{	
		if(PCD_IRQ_flagA)
		{
			// printfS("\r\nPCD_IRQ_flagA");
			EXTI->IENR &= ~RF_IRQ_EXTI_LINE;
			//EXTI->IMR &= 0xFFFFFFF7;		// Disable external interrupt			

			switch( PCD_IRQ() )
			{
				case 0:	//Other_IRQ 			
					printfS("\r\nOther IRQ Occur");
					PCD_SI523_TypeA_GetUID();
					PcdReset();			//软复位				
					// PcdPowerdown();			//硬复位
					PCD_SI523_TypeA_Init();
					PCD_ACD_Init();
					break;
						
				case 1:	//ACD_IRQ
					I_SI523_SiModifyReg(0x01, 0, 0x20);	// Turn on the analog part of receiver 			
					PCD_SI523_TypeA_GetUID();
					//I_SI523_IO_Write(ComIEnReg, 0x80);		//复位02寄存器,在读卡函数中被改动
					I_SI523_IO_Write(CommandReg, 0xb0);	 	//进入软掉电,重新进入ACD（ALPPL）
				break;
				
				case 2:	//ACDTIMER_IRQ			
					printfS("\r\nACDTIMER_IRQ:Reconfigure the register");
					PcdReset();			//软复位
					//PcdPowerdown();		//硬复位
					PCD_SI523_TypeA_Init();
					PCD_ACD_Init();
					break;
				
			}		
			
			EXTI->IENR |=  RF_IRQ_EXTI_LINE;
			//EXTI->IMR |= 0x00000008;		// Enable external interrupt
			PCD_IRQ_flagA = 0;
			delay_ms(500);
			//APP_Standby_Test();			
		}
		else
		{
			// APP_Standby_Test();
			delay_ms(500);
		}
	}
}
INIT_APP_EXPORT(ACD_Fun);


/*===============================
 函数功能：自动获取阈值

 ================================*/
void PCD_ACD_AutoCalc(void)
{
	unsigned char temp; 
	unsigned char temp_Compare=0; 
	unsigned char VCON_TR[8]={ 0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};//acd灵敏度调节
	unsigned char TR_Compare[4]={ 0x00, 0x00, 0x00, 0x00};
	ACDConfigRegC_Val = 0x7f;
	
	I_SI523_IO_Write(TxControlReg, 0x83);	//打开天线
	I_SI523_SetBitMask(CommandReg, 0x06);	//开启ADC_EXCUTE
	delay_us(200);
	
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigK << 2) | 0x40);		//手动设置一个K值
	I_SI523_IO_Write(ACDConfigReg, PCDKCONFIG);
		
	while(1)
	{
		I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigG << 2) | 0x40);		
		temp_Compare = I_SI523_IO_Read(ACDConfigReg);
		printf("\r\nG:%x\r\n",temp_Compare);
		delay_ms(100);
	}
	
	
	for(int i=7; i>0; i--)
	{	
		I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigK << 2) | 0x40);		
		I_SI523_IO_Write(ACDConfigReg, VCON_TR[i]);
		
		I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigG << 2) | 0x40);
		temp_Compare = I_SI523_IO_Read(ACDConfigReg);
		for(int m=0;m<100;m++)
		{
			I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigG << 2) | 0x40);		
			temp = I_SI523_IO_Read(ACDConfigReg);
			temp_Compare=(temp_Compare+temp)/2;		
			delay_us(100);
		}		
		
		if(temp_Compare == 0 || temp_Compare == 0x7f) //比较当前值和所存值
		{

		}
		else
		{
			if(temp_Compare < ACDConfigRegC_Val)
			{
				ACDConfigRegC_Val = temp_Compare;
				ACDConfigRegK_Val = VCON_TR[i];
			}
		}
	}//取得最接近的参考电压VCON
	
	for(int j=0; j<4; j++)
	{
		I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigK << 2) | 0x40);		
		I_SI523_IO_Write(ACDConfigReg, j*32+ACDConfigRegK_Val);
		
		I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigG << 2) | 0x40);
		temp_Compare = I_SI523_IO_Read(ACDConfigReg);
		for(int n=0;n<100;n++)
		{
			I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigG << 2) | 0x40);		
			temp = I_SI523_IO_Read(ACDConfigReg);
			temp_Compare=(temp_Compare+temp)/2;		
			delay_us(100);
		}		
		TR_Compare[j] = temp_Compare;
	}//再调TR的档位，将采集值填入TR_Compare[]

	for(int z=0; z<4; z++)
	{
		if(TR_Compare[z] == 0x7f)
		{
			
		}
		else
		{
			ACDConfigRegC_Val = TR_Compare[z];//最终选择的配置
			ACDConfigRegK_Val = 0x0c + z*32;
		}
	}//再选出一个非7f大值
	
	//ACDConfigRegK_Val = 0x4c;
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigK << 2) | 0x40);
	printfS("\r\n ACDConfigRegK_Val:%02x ",ACDConfigRegK_Val);	
	
	I_SI523_SetBitMask(CommandReg, 0x06);		//关闭ADC_EXCUTE
}


/*===============================
 函数功能：ACD初始化配置

 ================================*/
void PCD_ACD_Init(void)
{
	I_SI523_IO_Write(DivIrqReg, 0x60);	//清中断
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigJ << 2) | 0x40);		
	I_SI523_IO_Write(ACDConfigReg, 0x55);	//Clear ACC_IRQ
	
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigA << 2) | 0x40);
	I_SI523_IO_Write(ACDConfigReg, ACDConfigRegA_Val );
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigB << 2) | 0x40);
	I_SI523_IO_Write(ACDConfigReg, ACDConfigRegB_Val );
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigC << 2) | 0x40);
	I_SI523_IO_Write(ACDConfigReg, ACDConfigRegC_Val );
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigD << 2) | 0x40);
	I_SI523_IO_Write(ACDConfigReg, ACDConfigRegD_Val );
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigH << 2) | 0x40);
	I_SI523_IO_Write(ACDConfigReg, ACDConfigRegH_Val );
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigI << 2) | 0x40);
	I_SI523_IO_Write(ACDConfigReg, ACDConfigRegI_Val );	
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigK << 2) | 0x40);
	// I_SI523_IO_Write(ACDConfigReg, ACDConfigRegK_Val );
	I_SI523_IO_Write(ACDConfigReg, PCDKCONFIG);
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigM << 2) | 0x40);
	I_SI523_IO_Write(ACDConfigReg, ACDConfigRegM_Val );
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigO << 2) | 0x40);
	I_SI523_IO_Write(ACDConfigReg, ACDConfigRegO_Val );
	
	I_SI523_IO_Write(ComIEnReg, ComIEnReg_Val);	//IRqInv
	I_SI523_IO_Write(DivIEnReg, DivIEnReg_Val);
	
	I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigJ << 2) | 0x40);
	I_SI523_IO_Write(ACDConfigReg, ACDConfigRegJ_Val );
	
	I_SI523_IO_Write(CommandReg, 0xb0);	//进入ACD
}

char PCD_IRQ(void)
{
	unsigned char status_SI523CD_IRQ;  
	unsigned char temp_SI523CD_IRQ; 
	
	temp_SI523CD_IRQ = I_SI523_IO_Read(DivIrqReg);
	if	( temp_SI523CD_IRQ & 0x40)	//ACD中断
	{
		I_SI523_IO_Write(DivIrqReg, 0x40);		//Clear ACDIRq
		
		status_SI523CD_IRQ =1;
		return status_SI523CD_IRQ;
	}
	
	if ( temp_SI523CD_IRQ & 0x20)	//ACD看门狗中断
	{
		I_SI523_IO_Write(DivIrqReg, 0x20);		//Clear ACDTIMER_IRQ
		
		status_SI523CD_IRQ = 2;
		return status_SI523CD_IRQ;
	}
	
	I_SI523_IO_Write(DivIrqReg, 0x40);		//Clear ACDIRq
	I_SI523_IO_Write(DivIrqReg, 0x20);		//Clear ACDTIMER_IRQ
	I_SI523_IO_Write(0x20, (0x0f << 2) | 0x40);		
	I_SI523_IO_Write(0x0f, 0x0a);	//Clear OSCMon_IRQ,RFLowDetect_IRQ
	I_SI523_IO_Write(0x20, (0x09 << 2) | 0x40);		
	I_SI523_IO_Write(0x0f, 0x55);	//Clear ACC_IRQ

	return status_SI523CD_IRQ = 0;

}


