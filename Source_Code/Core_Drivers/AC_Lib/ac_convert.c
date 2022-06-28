#include "ac_common.h"
#include <ctype.h>
/*****************************************************************
@����	��ʮ�������ַ�ת��Ϊ��Ӧ��������1λ��  ���� ���ַ� '4' ת��Ϊ ���� 4
@����	ch - �ַ�
@����	ת�����
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
@����	��ʮ������������1λ��ת��Ϊ��Ӧ���ַ� ���磺 ��  ���� 4 ת��Ϊ �ַ� '4'
@����	ch - �ַ�
@����	ת�����
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
@����	��ʮ�������ַ���ת��Ϊ���������飬����  "34 F5" ת��Ϊ  {0x34,0xf5}
        �Զ����ˣ��ո񣬶��ţ�0xǰ׺���س�����
@����	str - �ַ���
@����	binBuff - �������
@����	maxBinLen - ���������ֽڳ���
@����	���ض������ֽڳ���
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
@����	����������ת��Ϊʮ�������ַ������Զ�����ո񣩡����� {0x34,0xf5} ת��Ϊ " 34 F5"
@����	binBuff - ����������
@����	binLen - ���������鳤��
@����	str - �ַ���
@����	�����ַ�������
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
@����	����������ת��Ϊʮ�������ַ������޿ո�ֿ��������� {0x34,0xf5} ת��Ϊ " 34F5"
@����	binBuff - ����������
@����	binLen - ���������鳤��
@����	str - �ַ���
@����	�����ַ�������
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
@����	���ַ���ת��Ϊָ�����ȵ�ѹ��BCD��ʽ���顣���� "31256" ת��Ϊ {0x03,0x12,0x56 }
        ����ʱ��λ��0�������������λ
@����	str - �ַ���
@����	bcdBuff - BCD����
@����	bcdLen - BCD����
@����	��
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
    
    //��ЧBCD�ַ�������
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

    //��ĩβ��ǰ����
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
@����	ѹ��BCD��������ת��Ϊ�ַ��������� {0x12,0x56 } ת��Ϊ "1256"
@����	bcdBuff - BCD����
@����	bcdLen - BCD���鳤��
@����	str - �ַ���
@����	�����ַ�������
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
@����	ѹ��BCD��������ת��ΪUINT32��,���� {0x12,0x56 } ת��Ϊ ʮ������ 1256
@����	bcdBuff - BCD����
@����	bcdLen - BCD���ݳ���
@����	UINT32��ֵ
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
@����	��UINT32����ת��Ϊָ�����ȵ�ѹ��BCD��������,���� ʮ������ 1256 ת��Ϊ  {0x00,0x12,0x56 }
        ����ʱ��λ��0�������������λ
@����	intValue - UINT32����
@����	bcdBuff - BCD����
@����	bcdLen - BCD���ݳ���
@����	��
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
@����	��ʮ�����ַ���ת��ΪUINT32������������ "1234" ת��Ϊ 1234 
        �Զ����˿ո��ַ�
@����	str - �ַ�������
@����	UINT32������
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
        else if(ch==0x20)   //���˿ո��ַ�
        {
            continue;
        }            
		else  //�Ƿ��ַ�����ֹת��
		{
			break;
		}
	}
	return dwData;
}

/*****************************************************************
@����	��UINT32������ת��Ϊʮ�����ַ��������� 1234 ת��Ϊ "1234"
@����	intValue - UINT32����
@����	str - �ַ�������
@����	�ַ������ȡ�������������󳤶ȣ��򷵻�0��
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

	//ǰ���ַ�����
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
@����	���ַ���ת��Ϊ���Ҹ�ʽ���飨�Է�Ϊ��λ��4�ֽ����飩�� "2365.87" ת��Ϊ  {0x00,0x03,0x9c,0x2b} ,ע��0x00039c2b==236587 
@����	str - �����ַ�������
@����	money - ���Ҹ�ʽ���飨4�ֽڣ��Է�Ϊ��λ����λ��ǰ��
@����	UINT32�����������Ҹ�ʽ��
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
@����	�ѻ������飨��Ϊ��λ��4�ֽ����飩ת��Ϊ�ַ�������λС���� {0x00,0x03,0x9c,0x2b} ת��Ϊ"2365.87"  ,ע��0x00039c2b==236587 
@����	money - ���Ҹ�ʽ���飨4�ֽڣ��Է�Ϊ��λ����λ��ǰ��
@����	str - �����ַ������飬��λС��
@����	UINT32�����������Ҹ�ʽ��
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

