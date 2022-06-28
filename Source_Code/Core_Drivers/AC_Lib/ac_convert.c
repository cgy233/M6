#include "ac_common.h"
#include <ctype.h>
/*****************************************************************
@功能	把十六进制字符转换为对应的整数（1位）  比如 把字符 '4' 转换为 数字 4
@参数	ch - 字符
@返回	转换结果
******************************************************************/
uint8_t HexChar_CharToInt(uint8_t ch)
{
	if(ch<=0x39)
		ch -= 0x30;
	else if(ch<='F')
		ch -= 0x37;
	else
		ch -= 0x57;

	return ch;
}

/*****************************************************************
@功能	把十六进制整数（1位）转换为对应的字符 比如： 把  数字 4 转换为 字符 '4'
@参数	ch - 字符
@返回	转换结果
******************************************************************/
uint8_t HexChar_IntToChar(uint8_t ch)
{
	if(ch<=9)
		ch += 0x30;
	else
		ch += 0x37;
	return ch;
}


void BE_Uint16ToBytes(uint16_t intValue, uint8_t buff[2])
{
    buff[0]= intValue >> 8;
    buff[1]= intValue;  
}

void BE_Uint32ToBytes(uint32_t intValue, uint8_t buff[4])
{
    buff[0]= intValue >> 24;
    buff[1]= intValue >> 16;
    buff[2]= intValue >> 8;
    buff[3]= intValue;  
}

uint16_t BE_BytesToUint16(uint8_t buff[2])
{
    return ((buff[0] << 8) | buff[1]);
}

uint32_t BE_BytesToUint32(uint8_t buff[4])
{
    return ((buff[0] << 24) | (buff[1] << 16) | (buff[2] << 8) | buff[3]);
}

void LE_Uint16ToBytes(uint16_t intValue, uint8_t buff[2])
{
    buff[0]= intValue;     
    buff[1]= intValue >> 8; 
}

void LE_Uint32ToBytes(uint32_t intValue, uint8_t buff[4])
{
    buff[0]= intValue;     
    buff[1]= intValue >> 8;    
    buff[2]= intValue >> 16;      
    buff[3]= intValue >> 24;
}

uint16_t LE_BytesToUint16(uint8_t buff[2])
{
    return (buff[0] | (buff[1] << 8));
}

uint32_t LE_BytesToUint32(uint8_t buff[4])
{
    return (buff[0] | (buff[1] << 8) | (buff[2] << 16) | (buff[3] << 24));
}

/*****************************************************************
@功能	把十六进制字符串转换为二进制数组，比如  "34 F5" 转换为  {0x34,0xf5}
        自动过滤：空格，逗号，0x前缀，回车换行
@参数	str - 字符串
@参数	binBuff - 结果数组
@参数	maxBinLen - 最大二进制字节长度
@返回	返回二进制字节长度
******************************************************************/
int HexData_StrtoBin(void *str, void *binBuff,int maxBinLen)
{
	int i,j;
	int flag=0;
	int inlen = strlen((char *)str);
	uint8_t ch,ch2,halfbyte;
	uint8_t *ptrin,*ptrout;
	ptrin = (uint8_t *)str;
	ptrout = (uint8_t *)binBuff;

	j=0;
	for(i=0;i<inlen;i++)
	{
		ch=ptrin[i];		
		ch2 = ptrin[i+1];
		if((ch == '0') &&(ch2=='x'))
		{
			i++;
			continue;
		}
		else if(isxdigit(ch))
		{
			if(flag==0)
			{
				halfbyte = (HexChar_CharToInt(ch) << 4);
				flag++;
			}
			else
			{
				ptrout[j] = halfbyte + HexChar_CharToInt(ch);
				j++;
				flag=0;

				if(j==maxBinLen)
					break;
			}
		}
		else if((ch==0x0d)||(ch==0x0a)||(ch==0x20)||(ch==','))
		{
			continue;
		}
		else
		{
			break;
		}
	}

	return j;
}

/*****************************************************************
@功能	二进制数组转换为十六进制字符串（自动加入空格）。比如 {0x34,0xf5} 转换为 " 34 F5"
@参数	binBuff - 二进制数组
@参数	binLen - 二进制数组长度
@参数	str - 字符串
@返回	返回字符串长度
******************************************************************/
int HexData_BinToStr(void *binBuff,int binLen,void *str)
{
	int i,j;
	uint8_t ch;
	uint8_t *ptrin,*ptrout;
	int firstFlag=1;
	ptrin = (uint8_t *)binBuff;
	ptrout = (uint8_t *)str;

	for(i=0,j=0;i<binLen;i++)
	{
		if(firstFlag==0)
		{
			ptrout[j++] = 0x20;
		}
		else
		{
			firstFlag = 0;
		}

		ch=ptrin[i];
		ptrout[j++] = HexChar_IntToChar(ch>>4);
		ptrout[j++] = HexChar_IntToChar(ch&0xf);
	}
	ptrout[j]=0x00;
	return j;
}

/*****************************************************************
@功能	二进制数组转换为十六进制字符串（无空格分开）。比如 {0x34,0xf5} 转换为 " 34F5"
@参数	binBuff - 二进制数组
@参数	binLen - 二进制数组长度
@参数	str - 字符串
@返回	返回字符串长度
******************************************************************/
int HexData_BinToStrNoSpace(void *binBuff,int binLen,void *str)
{
	int i,j;
	uint8_t ch;
	uint8_t *ptrin,*ptrout;
	ptrin = (uint8_t *)binBuff;
	ptrout = (uint8_t *)str;

	for(i=0,j=0;i<binLen;i++)
	{
		ch=ptrin[i];
		ptrout[j++] = HexChar_IntToChar(ch>>4);
		ptrout[j++] = HexChar_IntToChar(ch&0xf);
	}
	ptrout[j]=0x00;
	return j;
}


/*****************************************************************
@功能	把字符串转换为指定长度的压缩BCD格式数组。比如 "31256" 转换为 {0x03,0x12,0x56 }
        不足时高位补0，溢出则抛弃高位
@参数	str - 字符串
@参数	bcdBuff - BCD数组
@参数	bcdLen - BCD长度
@返回	无
******************************************************************/
void BcdData_StrToBcd(void *str,void *bcdBuff,int bcdLen)
{
	int i,j;
	int flag=0;
	int inlen;
	uint8_t ch;
	uint8_t *ptrin,*ptrout;
	ptrin = (uint8_t *)str;
	ptrout = (uint8_t *)bcdBuff;
	
	j=0;
    
    //有效BCD字符串长度
    for(i=0;;i++)
    {
		ch=ptrin[i];
		if(isdigit(ch))
		{
			continue;
		}		
		else
		{
			break;
		}        
    }
    inlen = i;

    //从末尾往前解析
    flag = 0;
    memset(bcdBuff,0,bcdLen);
    i=inlen-1;
    j= bcdLen-1;     
	for(;i>=0;i--)
	{
		ch=ptrin[i];
		
        if(flag==0)
        {
            ptrout[j] = HexChar_CharToInt(ch);
            flag++;
        }
        else
        {
            ptrout[j] = ptrout[j] + (HexChar_CharToInt(ch)<<4);
            j--;
            if(j<0)
                break;
            flag=0;
        }
	}
}

/*****************************************************************
@功能	压缩BCD编码数组转换为字符串。比如 {0x12,0x56 } 转换为 "1256"
@参数	bcdBuff - BCD数组
@参数	bcdLen - BCD数组长度
@参数	str - 字符串
@返回	返回字符串长度
******************************************************************/
int BcdData_BcdToStr(void *bcdBuff,int bcdLen,void *str)
{
	int i,j;
	uint8_t ch;
	uint8_t *ptrin,*ptrout;
	ptrin = (uint8_t *)bcdBuff;
	ptrout = (uint8_t *)str;

	for(i=0,j=0;i<bcdLen;i++)
	{
		ch=ptrin[i];
		ptrout[j++] = HexChar_IntToChar(ch>>4);
		ptrout[j++] = HexChar_IntToChar(ch&0xf);
	}
	ptrout[j]=0x00;
	return j;
}

/*****************************************************************
@功能	压缩BCD编码数组转换为UINT32数,比如 {0x12,0x56 } 转换为 十进制数 1256
@参数	bcdBuff - BCD数组
@参数	bcdLen - BCD数据长度
@返回	UINT32数值
******************************************************************/
uint32_t BcdData_BcdToInt(void *bcdBuff,int bcdLen)
{
	uint8_t *ptrin;
	ptrin = (uint8_t *)bcdBuff;

	int i;
	uint8_t ch;
	uint32_t out;
	out=0;

	for(i=0;i<bcdLen;i++)
	{
		ch=ptrin[i];
		out = out*10 + (ch>>4);
		out = out*10 + (ch&0xf);
	}
	return out;
}

/*****************************************************************
@功能	把UINT32整数转换为指定长度的压缩BCD编码数组,比如 十进制数 1256 转换为  {0x00,0x12,0x56 }
        不足时高位补0，溢出则抛弃高位
@参数	intValue - UINT32整数
@参数	bcdBuff - BCD数组
@参数	bcdLen - BCD数据长度
@返回	无
******************************************************************/
void BcdData_IntToBcd(uint32_t intValue,void *bcdBuff, int bcdLen)
{
	int i;
	uint8_t ch;
	uint32_t temp;
	uint8_t *ptrout;
	ptrout = (uint8_t *)bcdBuff + bcdLen - 1;

	for(i=0;i<bcdLen;i++)
	{
		temp = intValue%10;
		intValue = intValue/10;
		ch = (uint8_t)temp;
		temp = intValue%10;
		intValue = intValue/10;
		ch = ch + ((uint8_t)temp<<4);
		*ptrout-- = ch;
	}
}

/*****************************************************************
@功能	把十进制字符串转换为UINT32整形数。比如 "1234" 转换为 1234 
        自动过滤空格字符
@参数	str - 字符串数组
@返回	UINT32整形数
******************************************************************/
uint32_t DecData_StrToInt(void *str)
{
	uint8_t ch;
	uint8_t *ptr = (uint8_t *)str;
	uint32_t dwData = 0;
	while(1)
	{
		ch = *(uint8_t *)ptr++;
		if((ch>=0x30) && (ch<=0x39))
		{
			ch  = ch - 0x30;
			dwData = dwData * 10 + ch;
		}
        else if(ch==0x20)   //过滤空格字符
        {
            continue;
        }            
		else  //非法字符，终止转换
		{
			break;
		}
	}
	return dwData;
}

/*****************************************************************
@功能	把UINT32整型数转换为十进制字符串。比如 1234 转换为 "1234"
@参数	intValue - UINT32整数
@参数	str - 字符串数组
@返回	字符串长度。如果结果超过最大长度，则返回0。
******************************************************************/
int DecData_IntToStr(uint32_t intValue,void *str)
{
	int i,strlen;
	uint8_t ch;
	uint8_t *ptrout;
	ptrout = (uint8_t *)str;

	for(i=0;;)
	{
		ptrout[i] = (uint8_t)(intValue % 10) + 0x30;
		intValue = intValue / 10;
		i++;
		if(intValue == 0)
			break;
	}

	//前后字符交换
	strlen = i;
	for(i=0;i<strlen/2;i++)
	{
		ch = ptrout[i];
		ptrout[i] = ptrout[strlen - 1 - i];
		ptrout[strlen - 1 - i] = ch;
	}

	ptrout[strlen]=0x00;
	return strlen;
}


/*****************************************************************
@功能	把字符串转换为货币格式数组（以分为单位的4字节数组）， "2365.87" 转换为  {0x00,0x03,0x9c,0x2b} ,注：0x00039c2b==236587 
@参数	str - 浮点字符串数组
@参数	money - 货币格式数组（4字节，以分为单位，高位在前）
@返回	UINT32整形数（货币格式）
******************************************************************/
uint32_t Money_StrToMoney(void *str,uint8_t money[4])
{
	uint32_t value;
	char *ptr = (char *)str;
	double fvalue = atof(ptr);

	fvalue = fvalue*100;
	value = (uint32_t)fvalue;

	money[0] = (uint8_t )( (value>>24) & 0xff);
	money[1] = (uint8_t )( (value>>16) & 0xff);
	money[2] = (uint8_t )( (value>>8) & 0xff);
	money[3] = (uint8_t )( value & 0xff);
	return value;


}
/*****************************************************************
@功能	把货币数组（分为单位的4字节数组）转换为字符串（两位小数） {0x00,0x03,0x9c,0x2b} 转换为"2365.87"  ,注：0x00039c2b==236587 
@参数	money - 货币格式数组（4字节，以分为单位，高位在前）
@参数	str - 浮点字符串数组，两位小数
@返回	UINT32整形数（货币格式）
******************************************************************/
uint32_t Money_MoneyToStr(uint8_t money[4], void *str)
{
	uint32_t value;
	double fValue; 
	char *ptr = (char *)str;    
	value = ((uint32_t)money[0]<<24) + ((uint32_t)money[1]<<16) + ((uint32_t)money[2]<<8) + money[3];
	fValue = value;
	fValue = fValue/100;
	sprintf(ptr,"%.2f",fValue);
	return value;
}

