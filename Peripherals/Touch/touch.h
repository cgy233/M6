#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "ACM32Fxx_HAL.h"

#define AP_FAIL 0
#define AP_SUCCESS 1
#define NO_INPUT 0
#define AP_WELL_KEY_SHORT 1
#define INPUT_PROCESS 1
#define INPUT_END 2
#define AP_STAR_KEY_SHORT 2
#define AP_STAR_KEY_LONG 3
#define AP_Restore 4
#define AP_REG_ADMIN 5
#define AP_DEL_ADMIN 6
#define AP_REG_USER 7 
#define AP_DEL_USER 8
#define AP_RETURN_BOOT 9
#define AP_PWD_LEN_ERROR 10
#define MAX_KEYPWD_LEN 6
#define MIN_KEYPWD_LEN 6
#define AP_TIME_OUT 12
#define AP_CONTINUE 13
#define AP_FULL 14
#define AP_EXSIT 14
#define INPUT_PASSWD_TIMEOUT 10

void touch_init(void);
uint16_t get_touch_key(uint8_t *pucKey, uint32_t time_out);
uint32_t App_KeyReg(uint8_t ucAttr);
uint32_t App_KeyMatch(uint8_t ucRecFlag, uint8_t ucAttr);

#endif