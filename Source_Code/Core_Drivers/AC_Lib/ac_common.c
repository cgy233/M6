/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_common.c
@功能描述:	通用函数。
@开发人员:	bobzhang 
@完成日期:	2021.1.18
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/
#include "ac_common.h"


/*****************************************************************
@功能	获取校验和
@参数	ptr[in]: 计算校验和的数据缓冲区
@参数	len[in]: 数据长度
@返回	校验和	
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
@功能	将uint8_t类型数据转换成字符串
@参数	string  [out]: 转换后的string数据缓冲区
@参数	data    [in] : 要转换的uint8_t类型数据缓冲区
@参数	data_len[in] : 要转换的数据长度
@参数	endian  [in] : 大小端模式
@返回	无	
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
@功能	将字符串数据转换成uint8_t类型数据
@参数	string  [in] : 要转换的string数据缓冲区
@参数	data    [out]: 转换后的uint8_t类型数据缓冲区
@参数	data_len[out]: 转换后的uint8_t类型数据长度
@参数	endian  [in] : 大小端模式
@返回	无	
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
@功能	8421BCD码转换为10进制
@参数	bcd_data[in] : 要转换的8421BCD数据
@返回	转换后的十进制数据	
******************************************************************/
uint8_t Transform_BcdToDec(uint8_t bcd_data)
{
    uint8_t dec_data;
    dec_data = ((bcd_data>>4)&0x0F) * 10;
    dec_data = dec_data + (bcd_data & 0x0F);
    return dec_data;
}

/*****************************************************************
@功能	10进制转换为8421BCD码
@参数	bcd_data[in] : 要转换的10进制数
@返回	转换后的8421BCD码	
******************************************************************/
uint8_t Transform_DecToBcd(uint8_t dec_data)
{
    uint8_t bcd_data;
    bcd_data = dec_data/10;
    bcd_data = (bcd_data<<4) + (dec_data%10);
    return bcd_data;
}



