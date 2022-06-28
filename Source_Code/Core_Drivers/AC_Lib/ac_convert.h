#ifndef _AC_CONVERT_H_
#define _AC_CONVERT_H_

#include "ac_common.h"

void BE_Uint16ToBytes(uint16_t intValue, uint8_t buff[2]);
void BE_Uint32ToBytes(uint32_t intValue, uint8_t buff[4]);
uint16_t BE_BytesToUint16(uint8_t buff[2]);
uint32_t BE_BytesToUint32(uint8_t buff[4]);

void LE_Uint16ToBytes(uint16_t intValue, uint8_t buff[2]);
void LE_Uint32ToBytes(uint32_t intValue, uint8_t buff[4]);
uint16_t LE_BytesToUint16(uint8_t buff[2]);
uint32_t LE_BytesToUint32(uint8_t buff[4]);

int HexData_StrtoBin(void *str, void *binBuff,int maxBinLen);
int HexData_BinToStr(void *binBuff,int binLen,void *str);
int HexData_BinToStrNoSpace(void *binBuff,int binLen,void *str);

void BcdData_StrToBcd(void *str,void *bcdBuff,int bcdLen);
int BcdData_BcdToStr(void *bcdBuff,int bcdLen,void *str);
uint32_t BcdData_BcdToInt(void *bcdBuff,int bcdLen);
void BcdData_IntToBcd(uint32_t intValue,void *bcdBuff, int bcdLen);

uint32_t DecData_StrToInt(void *str);
int DecData_IntToStr(uint32_t intValue,void *str);

uint32_t Money_StrToMoney(void *str,uint8_t money[4]);
uint32_t Money_MoneyToStr(uint8_t money[4], void *str);


#endif