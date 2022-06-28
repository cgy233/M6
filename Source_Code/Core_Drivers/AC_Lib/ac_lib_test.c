/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : UCOM.c
 * Description : UCOM  source file
 * Author(s)   : bob
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#include  "ac_common.h"

#define BUFFER_LENGTH    (12*1024)
uint8_t TEST_txBuffer[BUFFER_LENGTH];
uint8_t TEST_rxBuffer[BUFFER_LENGTH];


/* ac_uart_test  *******************************************************/


#if 1
    #define TEST_UARTx          UART1
    #define TEST_UART_TX_PIN    GPIOA9
    #define TEST_UART_TX_AF     GPIOA9_AF_UART1_TX
    #define TEST_UART_RX_PIN    GPIOA10
    #define TEST_UART_RX_AF     GPIOA10_AF_UART1_RX
#else
    #define TEST_UARTx          UART2
    #define TEST_UART_TX_PIN    GPIOA2
    #define TEST_UART_TX_AF     GPIOA2_AF_UART2_TX
    #define TEST_UART_RX_PIN    GPIOA3
    #define TEST_UART_RX_AF     GPIOA3_AF_UART2_RX
#endif

FIFO_TypeDef TEST_UART_rxfifo;
uint8_t TEST_UART_rxBuff[32];

void TEST_UART_IRQCallback(void)
{
    if (TEST_UARTx->IE & UART_IE_RXI) 
    {
        if (TEST_UARTx->RIS & UART_RIS_RXI)
        {
            /* Clear RXI Status */
            TEST_UARTx->ICR = UART_ICR_RXI;

            /* Store Data in buffer */
            FIFO_In(&TEST_UART_rxfifo,TEST_UARTx->DR);                         
        }
    }
}

void TEST_UART_IOInitCallback(void)
{
    GPIO_InitAFIO(TEST_UART_TX_PIN,GPIO_MODE_AF_PP,GPIO_PULLUP,TEST_UART_TX_AF);
    GPIO_InitAFIO(TEST_UART_RX_PIN,GPIO_MODE_AF_PP,GPIO_PULLUP,TEST_UART_RX_AF);      
}

void TEST_UART_Init(void)
{  
    FIFO_Create(&TEST_UART_rxfifo,TEST_UART_rxBuff,sizeof(TEST_UART_rxBuff));
    UART_Init(TEST_UARTx, 115200, 1, TEST_UART_IOInitCallback,TEST_UART_IRQCallback);
 	DEBUG_UartEnable(TEST_UARTx,1);   
	UART_RxiEnable(TEST_UARTx);
    dprintf("\r\ntest uart init ok!\r\n");     
}

void ac_uart_fifo_test(void)
{  
    uint8_t ch;
    uint32_t mstick=0;
    dprintf("\r\nac_uart_fifo_test start ....\r\n");       
    dprintf("please put char in 5s ....\r\n");
    mstick = get_ms_tick();
    while(1)
    {
        if(FIFO_Out(&TEST_UART_rxfifo,&ch) == AC_OK)
        {
            UART_SendByte(TEST_UARTx,ch);
            mstick = get_ms_tick();
        }
        if(get_ms_delay(mstick)>5000)
            break;
    }
    
    dprintf("ac_uart_test timeout,exit ....\r\n");
    
}



/* ac_timer_test  *******************************************************/

#define TEST_TIMERx         TIM6
volatile uint32_t TEST_TIMER_mstick=0;

void TEST_TIMER_IRQCallback(void)
{
    TEST_TIMER_mstick++;
}

void TEST_TIMER_DelayMs(uint32_t ms)
{
    uint32_t mstick = TEST_TIMER_mstick;
    while(TEST_TIMER_mstick-mstick<ms);
}

void ac_timer_test(void)
{
    int i;
    
    dprintf("ac_timer_test start ....\r\n");
    
    TIMER_Init(TEST_TIMERx,1000,TEST_TIMER_IRQCallback);
    TIMER_EnableIRQ(TEST_TIMERx);
    TIMER_Start(TEST_TIMERx);    
    for(i=0;i<5;i++)
    {
        TEST_TIMER_DelayMs(1000);
        dprintf("time:%d\r\n",i);
    }
    TIMER_Stop(TEST_TIMERx);   

}



/* ac_rtc_test  *************************************************************/

void ac_rtc_test(void)
{
    int i,diffsec;
    RTC_DATE_TIME_TypeDef startTime,endTime;
    
    dprintf("ac_rtc_test start ....\r\n");
    
    RTC_Init();
    startTime.year = 2021;
    startTime.month = 3;
    startTime.day = 4;
    startTime.hour = 15;
    startTime.min = 53;
    startTime.sec = 0;
    
    RTC_SetDateTime(&startTime);
    
    for(i=0;i<10;i++)
    {
        delay_ms(500);
        RTC_GetDateTime(&endTime);
        
        dprintf("now time:%04d-%02d-%02d %02d:%02d:%02d\r\n",
            endTime.year,endTime.month,endTime.day,
            endTime.hour,endTime.min,endTime.sec);
    }
    diffsec = RTC_GetTimeDiffSec(&startTime,&endTime);
    dprintf("diff sec: %d\r\n",diffsec);
    
    
    
    dprintf("\r\nenter standby\r\n");
    RTC_ConfigWakeupIO(RTC_WAKEUP_WKUP3,0);  //GPIOA2
    RTC_EnterStandby();
    while(1)
    {
        delay_ms(100);
        dprintf("STANDBY: %d\r\n",i++);
    }    
    
}



/* ac_wdt_test  *************************************************************/

void ac_wdt_test(void)
{
    int i=0;
    
    dprintf("\r\n ac_wdt_test start ....\r\n");
    WDT_Init(WDT_MODE_RST,5);
    WDT_Start();
    while(1)
    {
        delay_ms(1000);
        dprintf("time: %d\r\n",i++);
    }

}

/* ac_gpio_test start *******************************************************/

#define TEST_INPUT_PIN  GPIOB0
#define TEST_OUTPUT_PIN  GPIOB1
void ac_gpio_test(void)
{
    int i;
    uint8_t pinstate;
    GPIO_InitInput(TEST_INPUT_PIN,GPIO_PULLUP);
    GPIO_InitOutput(TEST_OUTPUT_PIN,1);
    for(i=0;i<10;i++)
    {
        GPIO_InvertPin(TEST_OUTPUT_PIN);
        delay_us(1);
        pinstate = GPIO_ReadPin(TEST_INPUT_PIN);
        dprintf("TEST_INPUT_PIN: %d\r\n",pinstate);  
        delay_ms(500);        
    }
}

/* ac_eflash_test start *******************************************************/


#define EFLASH_TEST_ADDR (0x10000)
void ac_eflash_test(void)
{
    uint8_t ret;
    uint8_t manuDeviceID[2];
    uint8_t identifyID[3];    
    uint8_t uniqueID[16];
    int i;
    uint32_t starttick,delay;
    
    dprintf("\r\neflash test,addr:%d, size : %d KB\r\n",EFLASH_TEST_ADDR, BUFFER_LENGTH/1024);
    
    for(i=0;i<BUFFER_LENGTH;i++)
    {
        TEST_txBuffer[i]=i+102;
    }

    
    //擦除FLASH
    
    starttick=get_ms_tick();    
    ret = EFLASH_Erase(EFLASH_TEST_ADDR,BUFFER_LENGTH);     
    dprintf("erase time:%d ms\r\n",get_ms_delay(starttick)); 
    if(ret!=AC_OK)
       dprintf("erase operation fail\r\n");   
    
    //检查是否擦除成功
    
    EFLASH_Read(EFLASH_TEST_ADDR,TEST_rxBuffer, BUFFER_LENGTH);       
    for(i=0;i<BUFFER_LENGTH;i++)
    {
        if(0xff!=TEST_rxBuffer[i])
            break;
    } 
    if(i!=BUFFER_LENGTH)
        dprintf("erase check fail\r\n"); 
    else
        dprintf("erase ok\r\n");     
    
    //编程FLASH
    
    starttick=get_ms_tick(); 
    ret = EFLASH_Program(EFLASH_TEST_ADDR,TEST_txBuffer,BUFFER_LENGTH);    
    dprintf("program time:%d ms\r\n",get_ms_delay(starttick));
    
     if(ret!=AC_OK)
        dprintf("program operation fail\r\n");
     
    //检查是否编程成功 
    starttick=get_ms_tick();      
    EFLASH_Read(EFLASH_TEST_ADDR,TEST_rxBuffer, BUFFER_LENGTH);   
    delay = get_ms_tick() - starttick;
    dprintf("read time:%d ms\r\n",delay);
     
    for(i=0;i<BUFFER_LENGTH;i++)
    {
        if(TEST_txBuffer[i]!=TEST_rxBuffer[i])
            break;
    } 
    if(i!=BUFFER_LENGTH)
        dprintf("program check fail\r\n"); 
    else
        dprintf("program ok\r\n"); 
    
    
    for(i=0;i<BUFFER_LENGTH;i++)
    {
        TEST_txBuffer[i]=i+200;
    } 
    
    //写FLASH（自动擦除）
    
    starttick=get_ms_tick(); 
    ret = EFLASH_Write(EFLASH_TEST_ADDR,TEST_txBuffer,BUFFER_LENGTH);    
    dprintf("write(auto erase) time:%d ms\r\n",get_ms_delay(starttick));
    
    if(ret!=AC_OK)
        dprintf("write(auto erase) operation fail\r\n");
    
    starttick=get_ms_tick();       
    EFLASH_Read(EFLASH_TEST_ADDR,TEST_rxBuffer, BUFFER_LENGTH);
    delay = get_ms_tick() - starttick;
    dprintf("read time:%d ms\r\n",delay);
    
    for(i=0;i<BUFFER_LENGTH;i++)
    {
        if(TEST_txBuffer[i]!=TEST_rxBuffer[i])
            break;
    } 
    if(i!=BUFFER_LENGTH)
        dprintf("write(auto erase) check fail\r\n"); 
    else
        dprintf("write(auto erase) ok\r\n");
      
}

#define NORFLASH_TEST_ADDR (0x10000)
void ac_norflash_test(void)
{
    uint8_t ret;
    uint8_t manuDeviceID[2];
    uint8_t identifyID[3];    
    uint8_t uniqueID[16];
    int i;
    uint32_t starttick,delay;
    
    dprintf("\r\nnorflash test,addr:%d, size : %d KB\r\n",NORFLASH_TEST_ADDR, BUFFER_LENGTH/1024);
    
    ret = NorFlash_Init();
    if(ret!=AC_OK)
    {
        dprintf("norflash init fail\r\n");
        return;
    }
    
    NorFlash_Read_Manu_Device_ID(manuDeviceID);
    dprintf("devide id : %02x %02x\r\n",manuDeviceID[0],manuDeviceID[1]);
    
    NorFlash_Read_ID(identifyID);
    dprintf("identify id : %02x %02x %02x\r\n",identifyID[0],identifyID[1],identifyID[2]); 
    
    dprintf("unique id :");
    NorFlash_Read_Unique_ID(uniqueID);
    dprinth(uniqueID,16);
    dprintf("\r\n"); 

    for(i=0;i<BUFFER_LENGTH;i++)
    {
        TEST_txBuffer[i]=i+102;
    }
  
    //擦除FLASH
    
    starttick=get_ms_tick();    
    ret = NorFlash_Erase(NORFLASH_TEST_ADDR,BUFFER_LENGTH);     
    dprintf("erase time:%d ms\r\n",get_ms_delay(starttick)); 
    if(ret!=AC_OK)
       dprintf("erase operation fail\r\n");   
    
    //检查是否擦除成功
    
    NorFlash_Read(NORFLASH_TEST_ADDR,TEST_rxBuffer, BUFFER_LENGTH);       
    for(i=0;i<BUFFER_LENGTH;i++)
    {
        if(0xff!=TEST_rxBuffer[i])
            break;
    } 
    if(i!=BUFFER_LENGTH)
        dprintf("erase check fail\r\n"); 
    else
        dprintf("erase ok\r\n");     
    
    //编程FLASH
    
    starttick=get_ms_tick(); 
    ret = NorFlash_Program(NORFLASH_TEST_ADDR,TEST_txBuffer,BUFFER_LENGTH);    
    dprintf("program time:%d ms\r\n",get_ms_delay(starttick));
    
     if(ret!=AC_OK)
        dprintf("program operation fail\r\n");
     
    //检查是否编程成功 
    starttick=get_ms_tick();      
    NorFlash_Read(NORFLASH_TEST_ADDR,TEST_rxBuffer, BUFFER_LENGTH);   
    delay = get_ms_tick() - starttick;
    dprintf("read time:%d ms\r\n",delay);
     
    for(i=0;i<BUFFER_LENGTH;i++)
    {
        if(TEST_txBuffer[i]!=TEST_rxBuffer[i])
            break;
    } 
    if(i!=BUFFER_LENGTH)
        dprintf("program check fail\r\n"); 
    else
        dprintf("program ok\r\n"); 
    
    
    for(i=0;i<BUFFER_LENGTH;i++)
    {
        TEST_txBuffer[i]=i+200;
    } 
    
    //写FLASH（自动擦除）
    
    starttick=get_ms_tick(); 
    ret = NorFlash_Write(NORFLASH_TEST_ADDR,TEST_txBuffer,BUFFER_LENGTH);    
    dprintf("write(auto erase) time:%d ms\r\n",get_ms_delay(starttick));
    
    if(ret!=AC_OK)
        dprintf("write(auto erase) operation fail\r\n");
    
    starttick=get_ms_tick();       
    NorFlash_Read(NORFLASH_TEST_ADDR,TEST_rxBuffer, BUFFER_LENGTH);
    delay = get_ms_tick() - starttick;
    dprintf("read time:%d ms\r\n",delay);
    
    for(i=0;i<BUFFER_LENGTH;i++)
    {
        if(TEST_txBuffer[i]!=TEST_rxBuffer[i])
            break;
    } 
    if(i!=BUFFER_LENGTH)
        dprintf("write(auto erase) check fail\r\n"); 
    else
        dprintf("write(auto erase) ok\r\n");     
     
}

void ac_convert_test(void)
{
    uint16_t u16Value;
    uint32_t u32Value;
    uint8_t buffer[32],inbuff[32],outbuff[32],outbuff2[32],*ptr,money[4];
    int len,len2;
    
    
    dprintf("\r\n ac convert test start...\r\n");  
    
    u16Value = 0x5678;    
    BE_Uint16ToBytes(u16Value,buffer);
    dprintf("\r\n BE_Uint16ToBytes: in:0x%04x",u16Value);
    dprintf(", out:");     
    dprinth(buffer,2);
    dprintf("\r\n");
    
    u16Value = BE_BytesToUint16(buffer);
    dprintf("\r\n BE_BytesToUint16: in:");
    dprinth(buffer,2);    
    dprintf(", out:0x%04x\r\n",u16Value);     

    
    u32Value = 0x12345678;     
    BE_Uint32ToBytes(u32Value,buffer);
    dprintf("\r\n BE_Uint32ToBytes: in:0x%04x",u32Value);
    dprintf(", out:");     
    dprinth(buffer,4);
    dprintf("\r\n");    
    
    u32Value = BE_BytesToUint32(buffer);
    dprintf("\r\n BE_BytesToUint32: in:");
    dprinth(buffer,4);    
    dprintf(", out:0x%04x\r\n",u32Value); 
    
    u16Value = 0x5678;    
    LE_Uint16ToBytes(u16Value,buffer);
    dprintf("\r\n LE_Uint16ToBytes: in:0x%04x",u16Value);
    dprintf(", out:");     
    dprinth(buffer,2);
    dprintf("\r\n");
    
    u16Value = LE_BytesToUint16(buffer);
    dprintf("\r\n LE_BytesToUint16: in:");
    dprinth(buffer,2);    
    dprintf(", out:0x%04x\r\n",u16Value);     

    
    u32Value = 0x12345678;     
    LE_Uint32ToBytes(u32Value,buffer);
    dprintf("\r\n LE_Uint32ToBytes: in:0x%04x",u32Value);
    dprintf(", out:");     
    dprinth(buffer,4);
    dprintf("\r\n");    
    
    u32Value = LE_BytesToUint32(buffer);
    dprintf("\r\n LE_BytesToUint32: in:");
    dprinth(buffer,4);    
    dprintf(", out:0x%04x\r\n",u32Value);

    ptr= (uint8_t *)"12 34 AB CD";
    len = HexData_StrtoBin(ptr,outbuff,32);
    dprintf("\r\n HexData_StrtoBin: in:%s",ptr);  
    dprintf(", out:");    
    dprinth(outbuff,len); 
    dprintf("\r\n");

    len2 = HexData_BinToStr(outbuff,len,outbuff2);
    dprintf("\r\n HexData_BinToStr: in:");  
    dprinth(outbuff,len);     
    dprintf(", out:");    
    dprintb(outbuff2,len2); 
    dprintf("\r\n");

    len2 = HexData_BinToStrNoSpace(outbuff,len,outbuff2);
    dprintf("\r\n HexData_BinToStrNoSpace: in:");  
    dprinth(outbuff,len);     
    dprintf(", out:");    
    dprintb(outbuff2,len2); 
    dprintf("\r\n");



    ptr= (uint8_t *)"12345678";
    BcdData_StrToBcd(ptr,outbuff,12);
    len = 12;
    dprintf("\r\n BcdData_StrToBcd: in:");  
    dprintf("%s",ptr);      
    dprintf(", out:");    
    dprinth(outbuff,len); 
    dprintf("\r\n");

    len2 = BcdData_BcdToStr(outbuff,len,outbuff2);
    dprintf("\r\n BcdData_BcdToStr: in:");  
    dprinth(outbuff,len);     
    dprintf(", out:");    
    dprintb(outbuff2,len2); 
    dprintf("\r\n");    
    
    u32Value = BcdData_BcdToInt(outbuff,len);
    dprintf("\r\n BcdData_BcdToInt: in:");  
    dprinth(outbuff,len);     
    dprintf(", out:");    
    dprintf("%d",u32Value);  
    dprintf("\r\n");       
    
    BcdData_IntToBcd(u32Value,outbuff2,12);
    dprintf("\r\n BcdData_IntToBcd: in:");  
    dprintf("%d",u32Value);  
    dprintf(", out:"); 
    dprinth(outbuff2,12);
    dprintf("\r\n");  



    ptr= (uint8_t *)"12345678";
    u32Value = DecData_StrToInt(ptr);
    dprintf("\r\n DecData_StrToInt: in:");  
    dprintf("%s",ptr);      
    dprintf(", out:"); 
    dprintf("%d",u32Value);     
    dprintf("\r\n");
    
    len2 = DecData_IntToStr(u32Value,outbuff2);
    dprintf("\r\n DecData_IntToStr: in:"); 
    dprintf("%d",u32Value);     
    dprintf(", out:");    
    dprintb(outbuff2,len2);
    dprintf("\r\n");       


    
    ptr= (uint8_t *)"123456.78";
    u32Value = Money_StrToMoney(ptr,money);
    dprintf("\r\n DecData_StrToInt: in:");  
    dprintf("%s",ptr);      
    dprintf(", out1:"); 
    dprintf("%d,%x,",u32Value,u32Value); 
    dprinth(money,4);   
    dprintf("\r\n");
    
    u32Value = Money_MoneyToStr(money,outbuff2);
    dprintf("\r\n Money_MoneyToStr: in:"); 
    dprinth(money,4); 
    dprintf(", out:");    
    dprintf("%d,%x,",u32Value,u32Value);          
    dprintf("%s",outbuff2); 
    dprintf("\r\n");   
}


void ac_lib_test(void)
{
    SysTimer_Init(); 
 	GPIO_GlobalInit();    
    TEST_UART_Init(); 
    
    //ac_uart_fifo_test(); 
    //ac_timer_test();
    
    ac_gpio_test();
    
    ac_eflash_test();
    ac_norflash_test();    
    ac_convert_test();
    
     
    ac_rtc_test();  
    //ac_wdt_test();   
    
    
    while(1);
}
