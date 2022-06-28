/*****************************************************************
Copyright(C) 2008 - 2021, Shanghai AisinoChip Co.,Ltd.
@文件名称:	ac_rtc.c
@功能描述:	RTC模块函数。
@开发人员:	sanliu 
@完成日期:	2021.1.21
@当前版本:	1.0
@修改记录:	
修改日期		修改人   		修改说明

******************************************************************/

#include "ac_common.h"
#include "debug.h"


/*****************************************************************
@功能	RTC 初始化函数
@参数	clk:RTC时钟源，RTC_CLOCK_RC32K--内部RC32K，RTC_CLOCK_XTL--外部低速XTL晶振
@返回	无
******************************************************************/
void RTC_Init(uint32_t clk)
{
    RTC_ConfigTypeDef RTC_Handle;

    RTC_Handle.u32_ClockSource       = clk;
    RTC_Handle.u32_Compensation      = COMPENSATION_INCREASE;    // 开始时钟补偿，如不需补偿设置补偿值为0
    RTC_Handle.u32_CompensationValue = 0x05;                     // 开始时钟补偿，如不需补偿设置补偿值为0

    HAL_RTC_Config(&RTC_Handle);

}


/*****************************************************************
@功能	获取RTC日期与时间
@参数	output[OUT]-日期与时间
@返回	AC_OK-成功，AC_FAIL-失败
******************************************************************/
uint8_t RTC_GetDateTime(RTC_DATE_TIME_TypeDef *output)
{
    if(output == NULL)
    {
        return AC_FAIL;
    }
    
    output->year   = Transform_BcdToDec(RTC->YEAR) + YEAR_OFFSET;
    output->month  = Transform_BcdToDec(RTC->MONTH);
    output->day    = Transform_BcdToDec(RTC->DATE);
    output->hour   = Transform_BcdToDec(RTC->HOUR);
    output->minute    = Transform_BcdToDec(RTC->MIN);
    output->second    = Transform_BcdToDec(RTC->SEC);
    output->week   = Transform_BcdToDec(RTC->WEEK);
    return AC_OK;
}

/*****************************************************************
@功能	设置RTC日期与时间
@参数	input[IN]-日期与时间
@返回	AC_OK-成功，AC_FAIL-失败
******************************************************************/
uint8_t RTC_SetDateTime(RTC_DATE_TIME_TypeDef *input)
{
    RTC_DateTypeDef date;
    RTC_TimeTypeDef time;

    if(input == NULL)
    {
        return AC_FAIL;
    }

    memset(&time, 0, sizeof(time));

    date.u8_Year    = Transform_DecToBcd(input->year - YEAR_OFFSET);
    date.u8_Month   = Transform_DecToBcd(input->month);
    date.u8_Date    = Transform_DecToBcd(input->day);
    time.u8_Hours   = Transform_DecToBcd(input->hour);
    time.u8_Minutes = Transform_DecToBcd(input->minute);
    time.u8_Seconds = Transform_DecToBcd(input->second);
    date.u8_WeekDay = Transform_DecToBcd(input->week);
    /* Check RTC Parameter */
    if (!IS_RTC_YEAR(date.u8_Year))
    {       
        return AC_FAIL;
    }
    if (!IS_RTC_MONTH(date.u8_Month))
    {          
        return AC_FAIL;
    }
    if (!IS_RTC_DAY(date.u8_Date))
    {       
        return AC_FAIL;
    }
//    if (!IS_RTC_WEEKDAY(date.u8_WeekDay))
//    {    
//        return AC_FAIL;
//    }

    /* Check RTC Parameter */
    if (!IS_RTC_HOUR(time.u8_Hours))
    {    
        return AC_FAIL;
    }
    if (!IS_RTC_MIN(time.u8_Minutes))
    {    
        return AC_FAIL;
    }
    if (!IS_RTC_SEC(time.u8_Seconds))
    {    
        return AC_FAIL;
    }


    /* Write-Protect Disable */
	System_Enable_Disable_RTC_Domain_Access(1);

    RTC->YEAR  = date.u8_Year;
    RTC->MONTH = date.u8_Month;
    RTC->DATE  = date.u8_Date;
//    RTC->WEEK  = date.u8_WeekDay;

    RTC->HOUR = time.u8_Hours;
    RTC->MIN  = time.u8_Minutes;
    RTC->SEC  = time.u8_Seconds;

    /* Write-Protect Enable */
    System_Enable_Disable_RTC_Domain_Access(0);
    
    return AC_OK;
}

/*****************************************************************
@功能	判断是否为闰年
@参数	year[IN]-年
@返回	1-闰年，0-普通年
******************************************************************/
unsigned char IsRound(unsigned short year)
{
    /*is round year?*/
    if((year%100)&&(year%4==0)) 
        return 1;
    if((year%100==0)&&(year%400==0)) 
        return 1;
    return 0;

}

/*****************************************************************
@功能	将时间转换为自1970年1月1日0时0分0秒以来持续时间的秒数
@参数	input[IN]-日期与时间
@返回	秒数(北京时间)
******************************************************************/
uint32_t RTC_ConvertTimeToUTCSecs(RTC_DATE_TIME_TypeDef *input)
{
    unsigned int tTemp=0;
    unsigned int tSecond=0;
    unsigned char month_s[2][12]={
        {31,28,31,30,31,30,31,31,30,31,30,31},
        {31,29,31,30,31,30,31,31,30,31,30,31}
    };
    int nCount=0;
    int i, j;

    tSecond = input->hour * 3600 + input->minute * 60 + input->second;
 

    for (i = 1970; i < input->year; ++i)
    {
        if (IsRound(i))
        {
            ++nCount;
        }
            
    }
    tTemp += (input->year - 1970 - nCount) * SECONDOFYEAR + nCount * SECONDOFROUNDYEAR;
    if (input->month > 1)
    {
        if (IsRound(input->year))
        {
            for (j = 0; j< (input->month - 1); ++j)
            {
                tTemp += month_s[1][j] * MAXSECONDOFDAY;
            }
            tTemp += (input->day - 1) * MAXSECONDOFDAY + tSecond;
        }
        else
        {
            for (j = 0; j< (input->month - 1); ++j)
            {
                tTemp += month_s[0][j] * MAXSECONDOFDAY;
            }
            tTemp += (input->day - 1) * MAXSECONDOFDAY + tSecond;
        }
    }
    else
    {
        tTemp += (input->day - 1) * MAXSECONDOFDAY + tSecond;
    } 

    return (tTemp - TIME_ZONE_UTC_8);//减去8小时的时区差
}


/*****************************************************************
@功能	将自1970年1月1日0时0分0秒以来持续时间的秒数转换为时间
@参数	utcSecs[IN]-秒
@参数	output[OUT]-日期与时间
@返回	无
******************************************************************/
void RTC_ConvertUTCSecsTOTime(uint32_t utcSecs, RTC_DATE_TIME_TypeDef *output)
{
	int year_s[2]= {365*24*60*60, 366*24*60*60};
	int month_s[2][12]={
		{31,28,31,30,31,30,31,31,30,31,30,31},
		{31,29,31,30,31,30,31,31,30,31,30,31}
	};
	int day_s = 24*60*60;
	int hour_s = 60*60;
	int minute_s = 60;
    int flag;
    int temp = utcSecs;
    //1970年1月1日0时0分0秒
    int year = 1970;
    int month = 1;
    int day = 1;
    int hour = 0;
    int minute = 0;
    int second = 0;
 
    while(temp >= 60)
    {
        flag = IsRound(year);
        if(temp >= year_s[flag]) 
        { 
            year++; 
            temp -= year_s[flag]; 
        }
        else if(temp >= day_s)
        {
            int days = temp / day_s;
            temp = temp % day_s;
            int i = 0;
            flag = IsRound(year);
            int hh=31;
            while(days>= hh)
            {
                days -= month_s[flag][i++];
                hh = month_s[flag][i];
            }
            month += i;
            day += days;

        }
        else if(temp >= hour_s)
        {
            hour = temp / hour_s;
            temp %= hour_s;
        }
        else if(temp >= minute_s)
        {
            minute = temp / minute_s;
            temp %= minute_s;
        }
    }
	second = temp;
    output->year = year;
    output->month = month;
    output->day = day;
    output->hour = hour;
    output->minute = minute;
    output->second = second;
}

/*****************************************************************
@功能	计算时间差值
@参数	start_date_time[IN]-起始时间
@参数	end_date_time[IN]-当前时间
@返回	秒数
******************************************************************/
int RTC_GetTimeDiffSec(RTC_DATE_TIME_TypeDef *start_date_time,RTC_DATE_TIME_TypeDef *end_date_time)
{
	uint32_t now_time_sec = 0, old_time_sec = 0;
    
	end_date_time->year = end_date_time->year + YEAR_OFFSET;
	start_date_time->year = start_date_time->year + YEAR_OFFSET;

	
	now_time_sec = RTC_ConvertTimeToUTCSecs(end_date_time);
	old_time_sec = RTC_ConvertTimeToUTCSecs(start_date_time);
    // DBG("now_time_sec = %d, old_time_sec = %d\r\n", now_time_sec, old_time_sec);
	return (now_time_sec - old_time_sec);
}


/*****************************************************************
@功能	StandBy 唤醒脚配置
@参数	wakeup_io[IN]-唤醒源: RTC_WAKEUP_WKUP1, RTC_WAKEUP_WKUP2 ...(详情请见HAL_RTC.h)
@参数	edge[IN]-唤醒方式: 0-上升沿，1-下降沿
@返回	无
******************************************************************/
void Standby_WakeupIO_Config(enum_WKUP_t wakeup_io, WAKEUP_RISING_FALLING_Type trigger)
{
    switch (wakeup_io)
    {
        case RTC_WAKEUP_WKUP1: 
        case RTC_WAKEUP_WKUP2: 
        case RTC_WAKEUP_WKUP3: 
        case RTC_WAKEUP_WKUP4: 
        case RTC_WAKEUP_WKUP5: 
        case RTC_WAKEUP_WKUP6: 
        {
            /* RTC domain write enable */
            SCU->STOPCFG |= (1 << 0);
            /* Standby Mode */
            SCU->STOPCFG |= (1 << 11);
            /* Clear flags、Standby Enable */
            PMU->CR1 |= RPMU_CR_STB_EN | RPMU_CR_CWUF | RPMU_CR_CSBF;

            /* Wakeup IO Filter Enable */
            PMU->CR1 |= wakeup_io << 8;
            /* Wakeup IO Enable */
            PMU->CR1 |= wakeup_io;

            if (wakeup_io & RTC_WAKEUP_WKUP2) 
            {
                 /* PC13 */
                 PMU->IOCR &= ~0x40;//配置为数字接口，禁止上下拉电阻
            }
            
            if(trigger)   //上升沿触发
            {
                PMU->CR2 |= wakeup_io >> 16;
            }
            else    //下降沿触发
            {
                PMU->CR2 &= ~(wakeup_io >> 16);
            }

            PMU->CR1 |= RPMU_CR_CWUF;
        }break;
        
        case RTC_WAKEUP_STAMP2:
        case RTC_WAKEUP_STAMP1:
        case RTC_WAKEUP_32S:
        case RTC_WAKEUP_SEC:
        case RTC_WAKEUP_2HZ:
        case RTC_WAKEUP_MIN:
        case RTC_WAKEUP_HOUR:
        case RTC_WAKEUP_DATE:
        {
            /* Standby Mode */
            SCU->STOPCFG |= (1 << 11);
            /* Clear flags、Standby Enable */
            PMU->CR1 |= RPMU_CR_STB_EN | RPMU_CR_CWUF | RPMU_CR_CSBF;

            RTC->SR |= wakeup_io;
            RTC->IE |= wakeup_io;
        }break;

        default: break;       
    }
}


/*****************************************************************
@功能	进入低功耗StandBy模式
@参数	无
@返回	无
******************************************************************/
void System_Enter_StandBy(void)
{
   /* Clear Wakeup flag,Clear STANDBY flag */
   PMU->CR1 |= RPMU_CR_CWUF | RPMU_CR_CSBF;
   System_Enter_Standby_Mode();


    // /* Clear Wakeup flag,Clear STANDBY flag */
    // PMU->CR1 |= RPMU_CR_CWUF | RPMU_CR_CSBF; //如有唤醒标志，清除后，MCU依然能进入到Standby。不清除，会产生唤醒复位
    // /* Set SLEEPDEEP bit of Cortex System Control Register */
    // SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
    // SCU->STOPCFG |= BIT11;  // set PDDS=1  
    // /* Set SLEEPONEXIT bit of Cortex System Control Register */
    // SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
    // //防止Cache中有指令没执行完
    // __DSB(); //数据隔离
    // __ISB(); //指令隔离
    // System_DisableIAccelerate(); //关闭指令Cache
    // SysTick->CTRL = 0x00; //关闭系统定时器
    // SysTick->VAL = 0x00;  //清零
    // __set_PRIMASK(1); //关闭全局中断,只关闭了ISR中断处理函数
    // //diasable所有外设中断
    // NVIC->ICER[0] = 0xFFFFFFFF;
    // NVIC->ICER[1] = 0xFFFFFFFF;
    // //清除所有外设pedding
    // NVIC->ICPR[0] = 0xFFFFFFFF;
    // NVIC->ICPR[1] = 0xFFFFFFFF;
    // //清除SYSTICK pedding
    // SCB->ICSR = BIT25;
    // //Clear Wakeup flag,Clear STANDBY flag
    // PMU->CR1 |= RPMU_CR_CWUF | RPMU_CR_CSBF;
    // __DSB(); //数据隔离
    // __ISB(); //指令隔离
    // __WFI();

    // printfS("Enter Standby Fial\r\n");
}


/*****************************************************************
@功能	获取StandBy唤醒源
@参数	无
@返回	0-PowerON，非零-唤醒源
******************************************************************/
uint32_t Get_StandbyWakeupSource(void)
{
    uint32_t reslut = RTC_WAKEUP_SOURCE_POWERON;
    
    if (PMU->SR & RPMU_SR_SBF)
    {
        if (PMU->SR & RTC_WAKEUP_SOURCE_WKUP1)
        { 
            reslut = RTC_WAKEUP_SOURCE_WKUP1;
        }
        else if (PMU->SR & RTC_WAKEUP_SOURCE_WKUP2)
        { 
            reslut = RTC_WAKEUP_SOURCE_WKUP2;
        }
        else if (PMU->SR & RTC_WAKEUP_SOURCE_WKUP3) 
        {
            reslut = RTC_WAKEUP_SOURCE_WKUP3;
        }
        else if (PMU->SR & RTC_WAKEUP_SOURCE_WKUP4) 
        {
            reslut = RTC_WAKEUP_SOURCE_WKUP4;
        }  
        else if (PMU->SR & RTC_WAKEUP_SOURCE_WKUP5) 
        {
            reslut = RTC_WAKEUP_SOURCE_WKUP5;
        }  
        else if (PMU->SR & RTC_WAKEUP_SOURCE_WKUP6)
        {        
            reslut = RTC_WAKEUP_SOURCE_WKUP6;
        }
        else if (PMU->SR & RTC_WAKEUP_SOURCE_RSTWUF)
        {        
            reslut = RTC_WAKEUP_SOURCE_RSTWUF;
        }
        else if (PMU->SR & RTC_WAKEUP_SOURCE_BORWUF)
        {        
            reslut = RTC_WAKEUP_SOURCE_BORWUF;
        }
        else if(PMU->SR & RTC_WAKEUP_SOURCE_RTCWUF)
        {
            return RTC_WAKEUP_SOURCE_RTCWUF;
        }
        else
        {        
            reslut = PMU->SR;
        }
    }
    
    /* Clear Wakeup flag,Clear STANDBY flag */
    PMU->CR1 |= RPMU_CR_CWUF | RPMU_CR_CSBF;
    
    return reslut;
}

/*****************************************************************
@功能	RTC Backup区按字节读
@参数	addr - 起始地址(0x0-0x19)
@参数	buf-数据缓存，字节地址 
@参数	len-读取的字节数(MAX:20) 
@返回	AC_OK-成功,AC_FAIL-失败
******************************************************************/
uint8_t RTC_Backup_ReadByte(uint32_t addr, uint8_t *buff, uint8_t len)
{
	uint32_t i;
	uint32_t rData,offset,readAddr,readLen;
    uint8_t rtc_pos, readBuf[4];  

    if(addr + len > 0x19)
    {
        return AC_FAIL;
    }
    
    offset = addr & 0x03;
    readAddr = addr - offset; 
    
    SCU->STOPCFG |= SCU_STOPCFG_RTC_WE;     
    /* Write-Protect Disable */
    RTC->WP = 0xCA53CA53;
    while(len > 0)
    {
        if(len <= (4 - offset))
        {
            readLen = len;
        }
        else
        {
            readLen = 4 - offset;
        } 
        
        rtc_pos = readAddr / 4;
        rData = RTC->BAKUP[rtc_pos];
        
        TRANSFORM_UINT32_TO_BYTES(rData, readBuf);
        for (i = 0; i < readLen; i++)
        {
            *buff++ = readBuf[i + offset];	
        }

        offset = 0;			    		//偏移位置为0 
        readAddr += 4;	                //写地址偏移        
        len -= readLen;	    	        //字节数递减
    }
    /* Write-Protect Enable */
    RTC->WP = 0;
    SCU->STOPCFG &= (~SCU_STOPCFG_RTC_WE);    
    return AC_OK;
}


/*****************************************************************
@功能	RTC Backup区按字节写
@参数	addr - 起始地址(0x0-0x19)
@参数	buff-数据缓存，字节地址 
@参数	len-写入的字节数(MAX:20)
@返回	AC_OK-成功,AC_FAIL-失败
******************************************************************/
uint8_t RTC_Backup_ProgramByte(uint32_t addr, uint8_t *buff, uint8_t len)
{
	uint32_t wData,offset,writeAddr,writeLen;
    uint8_t rtc_pos, writeBuf[4]; 

    if(addr + len > 0x19)
    {
        return AC_FAIL;
    }
    
    offset = addr & 0x03;
    writeAddr = addr - offset;
    
    SCU->STOPCFG |= SCU_STOPCFG_RTC_WE;     
    /* Write-Protect Disable */
    RTC->WP = 0xCA53CA53;
    while(len > 0)
    {
        if(len <= (4 - offset))
        {
            writeLen = len;
        }
        else
        {
            writeLen = 4 - offset;
        } 
        
        rtc_pos = writeAddr / 4;
        wData = RTC->BAKUP[rtc_pos];
        TRANSFORM_UINT32_TO_BYTES(wData, writeBuf);
        memcpy(writeBuf + offset, buff, writeLen);
        wData = TRANSFORM_BYTES_TO_UINT32(writeBuf);
		RTC->BAKUP[rtc_pos] = wData;
		    
        offset = 0;			    		//偏移位置为0 
        writeAddr += 4;	                //写地址偏移        
        buff += writeLen;  	            //指针偏移
        len -= writeLen;	    	    //字节数递减
    }    
    /* Write-Protect Enable */
    RTC->WP = 0;
    SCU->STOPCFG &= (~SCU_STOPCFG_RTC_WE);
	return AC_OK; 
}


