#ifndef _AC_WDT_H_
#define _AC_WDT_H_

#include "ac_common.h"

#define ENABLE_WDT

void WDT_Feed(void);

void WDT_Start(void);

void WDT_Stop(void);

void WDT_Int_Enable(void);

void WDT_Int_Disable(void);

void WDT_Init(WDT_MODE mode,uint16_t wdt_time);


















#endif


