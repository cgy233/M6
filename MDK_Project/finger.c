#include "finger.h"

/**
 * @description: 封包
 * 
 * @param {uint8_t} *send_buff 封好的数据包
 * @param {uint8_t} *data 指令和参数
 * @param {uint8_t} data_len  指令和参数的长度
 * @return {*}
 */
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

/**
 * @description: 自动注册模板
 * 一站式注册指纹，包含采集指纹、生成特征、组合模板、存储模板等功能。
 * @param {uint8_t} *send_buff
 * @param {uint16_t} id ID 号
 * @param {uint8_t} count 录入次数
 * @param {uint16_t} param
 * 最低位为 bit0。
 * 1) bit0：采图背光灯控制位，0-LED 长亮，1-LED 获取图像成功后灭；
 * 2) bit1：采图预处理控制位，0-关闭预处理，1-打开预处理；
 * 3) bit2：注册过程中，是否要求模块在关键步骤，返回当前状态，0-要求返回，1-不要 求返回；
 * 4) bit3：是否允许覆盖 ID 号，0-不允许，1-允许；
 * 5) bit4：允许指纹重复注册控制位，0-允许，1-不允许；
 * 6) bit5：注册时，多次指纹采集过程中，是否要求手指离开才能进入下一次指纹图像 采集， 0-要求离开；1-不要求离开；
 * 7) bit6：0-不同手指可以录入，1-仅允许相同手指录入；
 * 8) bit7~bit15：预留。
 * @return {*}
 */
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

/**
 * @description: 自动验证指纹
 * 自动采集指纹，在指纹库中搜索目标模板或整个指纹模板，并返回搜索结果。
 * 如果目标模板同当前采集的指纹比对得分大于最高阀值，并且目标模板为不完
 * 整特征则以采集的特征更新目标模板的空白区域。一站式搜索包含获取图像，
 * 生成特征，搜索指纹等功能
 * @param {uint8_t} *send_buff
 * @param {uint8_t} safety_level 安全等级
 * @param {uint16_t} id ID 号
 * @param {uint16_t} param
 * 1) bit0：采图背光灯控制位，0-LED 长亮，1-LED 获取图像成功后灭；
 * 2) bit1：采图预处理控制位，0-关闭预处理，1-打开预处理；
 * 3) bit2：注册过程中，是否要求模块在关键步骤，返回当前状态，0-要求返回，1-不要求返回；
 * 4) bit3~bit15：预留。
 * @return {*}
 */
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

/**
 * @description: 删除 flash 指纹库中的一个特征文件
 * @param {uint8_t} *send_buff
 * @param {uint16_t} page_id 指纹库模板号
 * @param {uint16_t} n 删除的模板个数
 * @return {*}
 */
uint8_t PS_DeletChar(uint8_t *send_buff, uint16_t page_id, uint16_t n)
{
    uint8_t data[8];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x0c;
    data[4] = (uint8_t)((page_id & 0xFF00) >> 8);
    data[5] = (uint8_t)(page_id & 0x00FF);
    data[6] = (uint8_t)((n & 0xFF00) >> 8);
    data[7] = (uint8_t)(n & 0x00FF);

    return packing(send_buff, data, 8);
}

/**
 * @description: 清空 flash 指纹库
 * @param {uint8_t} *send_buff
 * @return {*}
 */
uint8_t PS_Empty(uint8_t *send_buff)
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x0d};

    return packing(send_buff, data, 4);
}

/**
 * @description: 取消指令
 * @param {uint8_t} *send_buff
 * @return {*}
 */
uint8_t PS_Cancel(uint8_t *send_buff)
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x30};

    return packing(send_buff, data, 4);
}
/**
 * @description: 休眠指令
 * @param {uint8_t} *send_buff
 * @return {*}
 */
uint8_t PS_Sleep(uint8_t *send_buff)
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x33};

    return packing(send_buff, data, 4);
}

/**
 * @description:  读有效模板个数
 * @param {uint8_t} *send_buff
 * @return {*}
 */
uint8_t PS_ValidTempleteNum(uint8_t *send_buff)
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x1d};

    return packing(send_buff, data, 4);
}

/**
 * @description: 设置设备握手口令
 * @param {uint8_t} *send_buff
 * @param {uint32_t} passwd 口令
 * @return {*}
 */
uint8_t PS_SetPwd(uint8_t *send_buff, uint32_t passwd)
{
    uint8_t data[8];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x12;
    data[4] = (uint8_t)((passwd & 0xFF000000) >> 24);
    data[5] = (uint8_t)((passwd & 0x00FF0000) >> 16);
    data[6] = (uint8_t)((passwd  & 0x0000FF00) >> 8);
    data[7] = (uint8_t)(passwd & 0x000000FF);

    return packing(send_buff, data, 9);
}

/**
 * @description: 验证设备握手口令
 * @param {uint8_t} *send_buff
 * @param {uint32_t} passwd 口令
 * @return {*}
 */
uint8_t PS_VfyPwd(uint8_t *send_buff, uint32_t passwd)
{
    uint8_t data[8];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x12;
    data[4] = (uint8_t)((passwd & 0xFF000000) >> 24);
    data[5] = (uint8_t)((passwd & 0x00FF0000) >> 16);
    data[6] = (uint8_t)((passwd  & 0x0000FF00) >> 8);
    data[7] = (uint8_t)(passwd & 0x000000FF);

    return packing(send_buff, data, 8);
}

/**
 * @description: 功能说明： 光环 LED 控制。
 * @param {uint8_t} *send_buff
 * @param {uint8_t} cmd 控制码
 * @param {uint8_t} speed 速度
 * @param {uint8_t} color 颜色
 * @param {uint8_t} count 次数
 * @return {*}
 */
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
uint8_t PS_SetChipAddr(uint8_t *send_buff,  uint8_t chip_addr)
{
    uint8_t data[9];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x15;
    data[4] = chip_addr;
    data[5] = (uint8_t)((chip_addr & 0xFF000000) >> 24);
    data[6] = (uint8_t)((chip_addr & 0x00FF0000) >> 16);
    data[7] = (uint8_t)((chip_addr  & 0x0000FF00) >> 8);
    data[8] = (uint8_t)(chip_addr & 0x000000FF);
    return packing(send_buff, data, 9);
}
