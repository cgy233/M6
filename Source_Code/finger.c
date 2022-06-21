#include "ACM32Fxx_HAL.h"
#include "finger.h"

uint8_t packing(uint8_t *send_buff, uint8_t *data, uint8_t data_len)
{
    uint16_t check_sum = 0;
    // header
    send_buff[0] = 0xef;
    send_buff[1] = 0x01;
    // address
    send_buff[2] = 0xff;
    send_buff[3] = 0xff;
    send_buff[4] = 0xff;
    send_buff[5] = 0xff;
    // data
    if (0 != data)
    {
        memcpy(send_buff + 6, data, data_len);
    }
    for (int i = 0; i < data_len; i++)
    {
        check_sum += data[i];
    }
    if (check_sum > 0xFFFF)
    {
        send_buff[data_len + 6] = 0xFF;
        send_buff[data_len + 7] = 0xFF;
    }
    else
    {
	    send_buff[data_len + 6] = (uint8_t)((check_sum & 0xFF00) >> 8);
	    send_buff[data_len + 7] = (uint8_t)(check_sum & 0x00FF);
    }
    return data_len + 8;
}


uint8_t PS_AutoEnroll(uint8_t *send_buff, uint16_t id, uint8_t count, uint16_t param)
{
    uint8_t data[9];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x08;
    data[3] = 0x31;
    data[4] = (uint8_t)((id & 0xFF00) >> 8);
    data[5] = (uint8_t)(id & 0x00FF);
    data[6] = count;
    data[7] = (uint8_t)((param & 0xFF00) >> 8);
    data[8] = (uint8_t)(param & 0x00FF);
    return packing(send_buff, data, 9);
}

uint8_t PS_AutoIdentify(uint8_t *send_buff, uint8_t safety_level, uint16_t id, uint16_t param)
{
    uint8_t data[9];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x08;
    data[3] = 0x32;
    data[4] = safety_level;
    data[5] = (uint8_t)((id & 0xFF00) >> 8);
    data[6] = (uint8_t)(id & 0x00FF);
    data[7] = (uint8_t)((param & 0xFF00) >> 8);
    data[8] = (uint8_t)(param & 0x00FF);
    return packing(send_buff, data, 9);

}

uint8_t PS_DeletChar(uint8_t *send_buff, uint16_t page_id, uint16_t n)
{
    uint8_t data[9];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x0c;
    data[5] = (uint8_t)((page_id & 0xFF00) >> 8);
    data[6] = (uint8_t)(page_id & 0x00FF);
    data[7] = (uint8_t)((n & 0xFF00) >> 8);
    data[8] = (uint8_t)(n & 0x00FF);

    return packing(send_buff, data, 9);
}

uint8_t PS_Empty(uint8_t *send_buff)
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x0d};

    return packing(send_buff, data, 4);
}

uint8_t PS_Cancel(uint8_t *send_buff)
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x30};

    return packing(send_buff, data, 4);
}

uint8_t PS_Sleep(uint8_t *send_buff)
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x33};

    return packing(send_buff, data, 4);
}

uint8_t PS_ValidTempleteNum(uint8_t *send_buff)
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x1d};

    return packing(send_buff, data, 4);
}

uint8_t PS_SetPwd(uint8_t *send_buff, uint32_t passwd)
{
    uint8_t data[9];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x12;
    data[5] = (uint8_t)((passwd & 0xFF000000) >> 24);
    data[6] = (uint8_t)((passwd & 0x00FF0000) >> 16);
    data[7] = (uint8_t)((passwd  & 0x0000FF00) >> 8);
    data[8] = (uint8_t)(passwd & 0x000000FF);

    return packing(send_buff, data, 9);
}

uint8_t PS_VfyPwd(uint8_t *send_buff, uint32_t passwd)
{
    uint8_t data[9];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x12;
    data[5] = (uint8_t)((passwd & 0xFF000000) >> 24);
    data[6] = (uint8_t)((passwd & 0x00FF0000) >> 16);
    data[7] = (uint8_t)((passwd  & 0x0000FF00) >> 8);
    data[8] = (uint8_t)(passwd & 0x000000FF);

    return packing(send_buff, data, 9);
}

uint8_t PS_AuraLedConfig(uint8_t *send_buff, uint8_t cmd, uint8_t speed, uint8_t color, uint8_t count)
{
    uint8_t data[8];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x3c;
    data[4] = cmd;
    data[5] = speed;
    data[6] = color;
    data[7] = count;
    return packing(send_buff, data, 8);
}
