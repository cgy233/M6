/*
  ******************************************************************************
  * @file    APP.c
  * @author  Chris_Kyle
  * @version V1.0.0
  * @date    2020
  * @brief   RTC demo source code.
  ******************************************************************************
*/
#include "power.h"
#include "SI523_App.h"

RTC_ConfigTypeDef RTC_Handle;

/*********************************************************************************
* Function    : APP_RTC_Test
* Description : 
* Input       : 
* Outpu       : 
* Author      : Chris_Kyle                         Data : 2020年
**********************************************************************************/
void APP_Standby_Test(void)
{
    printfS("MCU Standby Test \r\n");
    
    if (HAL_RTC_Get_StandbyStatus())
    {
        printfS("MCU Entered Standby Mode     ");

        if (HAL_RTC_Get_StandbyWakeupSource() & RTC_WAKEUP_SOURCE_WKUP1) 
            printfS("MCU Wakeup source --> WKUP1 \r\n");
        if (HAL_RTC_Get_StandbyWakeupSource() & RTC_WAKEUP_SOURCE_WKUP2) 
            printfS("MCU Wakeup source --> WKUP2 \r\n");
        if (HAL_RTC_Get_StandbyWakeupSource() & RTC_WAKEUP_SOURCE_WKUP3) 
            printfS("MCU Wakeup source --> WKUP3 \r\n");
        if (HAL_RTC_Get_StandbyWakeupSource() & RTC_WAKEUP_SOURCE_WKUP4) 
            printfS("MCU Wakeup source --> WKUP4 \r\n");
        if (HAL_RTC_Get_StandbyWakeupSource() & RTC_WAKEUP_SOURCE_WKUP5) 
            printfS("MCU Wakeup source --> WKUP5 \r\n");
        if (HAL_RTC_Get_StandbyWakeupSource() & RTC_WAKEUP_SOURCE_WKUP6) 
            printfS("MCU Wakeup source --> WKUP6 \r\n");
        if (HAL_RTC_Get_StandbyWakeupSource() & RTC_WAKEUP_SOURCE_RTCWUF) 
            printfS("MCU Wakeup source --> RTC \r\n");
        if (HAL_RTC_Get_StandbyWakeupSource() & RTC_WAKEUP_SOURCE_RSTWUF) 
            printfS("MCU Wakeup source --> Reset \r\n");
        if (HAL_RTC_Get_StandbyWakeupSource() & RTC_WAKEUP_SOURCE_IWDTWUF) 
            printfS("MCU Wakeup source --> IWDT \r\n");
        if (HAL_RTC_Get_StandbyWakeupSource() & RTC_WAKEUP_SOURCE_BORWUF) 
            printfS("MCU Wakeup source --> BOR Reset \r\n");
    }

    System_Delay_MS(5000);

    /********************************************/
    /**************** RTC Config ****************/
    /********************************************/
    RTC_Handle.u32_ClockSource       = RTC_CLOCK_XTL;
    RTC_Handle.u32_Compensation      = COMPENSATION_INCREASE;    // 开时钟补偿，如不需补偿设置补偿值为0
    RTC_Handle.u32_CompensationValue = 0x05;                     // 开时钟补偿，如不需补偿设置补偿值为0

    HAL_RTC_Config(&RTC_Handle);

    printfS("MCU enter standby mode \r\n");
		PCD_ACD_Init();
		System_Delay_MS(1);

    HAL_RTC_Standby_Wakeup(RTC_WAKEUP_WKUP3, STANDBY_WAKEUP_FALLING);    

    while(1)
    {
        printfS("MCU never run here \r\n");
    }
}

