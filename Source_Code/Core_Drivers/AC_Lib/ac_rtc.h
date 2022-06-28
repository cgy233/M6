
#ifndef __AC_RTC_H__
#define __AC_RTC_H__

#include "ac_common.h"


//Check RTC wakeup source form standby
#define RTC_WAKEUP_SOURCE_POWERON   (0x00000000)

#define YEAR_OFFSET     2000

/*the seconds of round year = 3600*24*366 */
#define SECONDOFROUNDYEAR 31622400 

/*the seconds of general year = 3600*24*365 */
#define SECONDOFYEAR      31536000

/*the seconds of one day = 3600*24 */
#define MAXSECONDOFDAY    86400 

//北京时间=UTC+8
#define TIME_ZONE_UTC_8   28800

typedef struct
{
    uint8_t second;      //秒钟
    uint8_t minute;      //分钟
    uint8_t hour;     //时钟
    uint8_t day;      //日
    uint8_t month;    //月
    uint16_t year;    //年
    uint8_t week;     //星期
} RTC_DATE_TIME_TypeDef;


typedef enum
{
    WAKEUP_EDGE_RISING,
    WAKEUP_EDGE_FALLING,
}WAKEUP_RISING_FALLING_Type;

void RTC_Init(uint32_t clk);


uint8_t RTC_GetDateTime(RTC_DATE_TIME_TypeDef *output);


uint8_t RTC_SetDateTime(RTC_DATE_TIME_TypeDef *input);


uint32_t RTC_ConvertTimeToUTCSecs(RTC_DATE_TIME_TypeDef *input);


void RTC_ConvertUTCSecsTOTime(uint32_t utcSecs, RTC_DATE_TIME_TypeDef *output);


int RTC_GetTimeDiffSec(RTC_DATE_TIME_TypeDef *start_date_time,RTC_DATE_TIME_TypeDef *end_date_time);


void Standby_WakeupIO_Config(enum_WKUP_t wakeup_io, WAKEUP_RISING_FALLING_Type trigger);

void System_Enter_StandBy(void);

uint32_t Get_StandbyWakeupSource(void);

uint8_t RTC_Backup_ReadByte(uint32_t addr, uint8_t *buff, uint8_t len);

uint8_t RTC_Backup_ProgramByte(uint32_t addr, uint8_t *buff, uint8_t len);



#endif


