/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@�ļ�����:	ac_common.c
@��������:	ͨ�ú�����
@������Ա:	bobzhang 
@�������:	2021.1.18
@��ǰ�汾:	1.0
@�޸ļ�¼:	
�޸�����		�޸���   		�޸�˵��

******************************************************************/
#include "ac_common.h"


/*****************************************************************
@����	��ȡУ���
@����	ptr[in]: ����У��͵����ݻ�����
@����	len[in]: ���ݳ���
@����	У���	
******************************************************************/
uint16_t GetCheckSum(const uint8_t *ptr,uint32_t len)
{
    uint32_t i;
    uint16_t checksum=0;
    for(i=0;i<len;i++)
    {
        checksum += *ptr++;
    }
    return checksum;
}

/*****************************************************************
@����	��uint8_t��������ת�����ַ���
@����	string  [out]: ת�����string���ݻ�����
@����	data    [in] : Ҫת����uint8_t�������ݻ�����
@����	data_len[in] : Ҫת�������ݳ���
@����	endian  [in] : ��С��ģʽ
@����	��	
******************************************************************/
void Transform_UcharToString(uint8_t *string, const uint8_t *data, uint8_t data_len, Endian_BigLittleType endian)
{
    uint8_t i;
    uint8_t j;
    uint8_t temp;
    uint8_t offset;  
    
    for (i = 0; i < data_len; i++)
    {
        if (endian == endian_Little)
        {
            offset = data_len - 1 - i;
        }
        else
        {
            offset = i;
        }
        for (j = 0; j < 2; j++)
        {
            temp = (data[offset] >> (4 * (1 - j))) & 0x0f;
            if (temp > 9)
            {
                string[2 * i + j] = temp - 10 + 'A';
            }
            else
            {
                string[2 * i + j] = temp + '0';
            }
        }
    }
}

/*****************************************************************
@����	���ַ�������ת����uint8_t��������
@����	string  [in] : Ҫת����string���ݻ�����
@����	data    [out]: ת�����uint8_t�������ݻ�����
@����	data_len[out]: ת�����uint8_t�������ݳ���
@����	endian  [in] : ��С��ģʽ
@����	��	
******************************************************************/
void Transform_StringToUchar(uint8_t *data, uint8_t *data_len,const uint8_t *string,  Endian_BigLittleType endian)
{
    uint8_t i;
    uint8_t temp;
    uint8_t offset;
    uint8_t len = strlen((char*)string);
    
    *data_len = len/2;
    
    for (i = 0; i < *data_len; i++)
    {
        offset = 2 * i;
        if (offset < len)
        {
            if (string[offset] > '9')
            {
                temp = ((string[offset] + 10 - 'A') << 4);
            }
            else
            {
                temp = ((string[offset] - '0') << 4);
            }
            offset++;
            if (offset < len)
            {
                if (string[offset] > '9')
                {
                    temp += (string[offset] + 10 - 'A');
                }
                else
                {
                    temp += (string[offset] - '0');
                }
            }
            else
            {
                temp += 0x0f;
            }
            if (endian == endian_Little)
            {
                data[*data_len - 1 - i] = temp;
            }
            else
            {
                data[i] = temp;
            }
        }
    }
}


/*****************************************************************
@����	8421BCD��ת��Ϊ10����
@����	bcd_data[in] : Ҫת����8421BCD����
@����	ת�����ʮ��������	
******************************************************************/
uint8_t Transform_BcdToDec(uint8_t bcd_data)
{
    uint8_t dec_data;
    dec_data = ((bcd_data>>4)&0x0F) * 10;
    dec_data = dec_data + (bcd_data & 0x0F);
    return dec_data;
}

/*****************************************************************
@����	10����ת��Ϊ8421BCD��
@����	bcd_data[in] : Ҫת����10������
@����	ת�����8421BCD��	
******************************************************************/
uint8_t Transform_DecToBcd(uint8_t dec_data)
{
    uint8_t bcd_data;
    bcd_data = dec_data/10;
    bcd_data = (bcd_data<<4) + (dec_data%10);
    return bcd_data;
}



