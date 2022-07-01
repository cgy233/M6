
#include "Key_Touch.h"
#include "syslock.h"
#include "voice.h"
#include "ac_common.h"
#include "Fgp.h"
#include "card.h"
#include "main.h"
#include "App_define.h"
#include "led_light.h"
#include "flash_manage.h"
#include "lcd.h"
#include "fpm.h"
#include "debug.h"




static uint32_t g_u32KeyMask =  KEY_ALL_ON;
static uint8_t g_u8KeyRegBuf1[MAX_KEYPWD_LEN + 1];
static uint8_t g_u8KeyRegBuf2[MAX_KEYPWD_LEN + 1];
static uint8_t g_u8KeyMatchBuf[MAX_KEYPWD_LEN + 1];      /* 比对时存放密码缓存的数组 */
static uint8_t passwd_state = NO_INPUT;

/*****************************************************************
@原型	void key_mask(uint32_t uwKeyMaskTemporary)
@功能	按键标记
@参数	uwKeyMaskTemporary-标记的按键 
@返回	无
******************************************************************/
void key_mask(uint32_t uwKeyMaskTemporary)
{
    g_u32KeyMask = uwKeyMaskTemporary;
}

/*****************************************************************
@原型	uint16_t is_mask(uint8_t uckey)
@功能	按键标记判断
@参数	uckey - 按键值
@返回	AP_SUCCESS-成功，AP_FAIL-失败
******************************************************************/
static uint16_t is_mask(uint8_t uckey)
{
    uint16_t keyvalue = 0;

    if(uckey == '#')
        keyvalue = FUNCTION_SHARPKEY_ON;
    else if(uckey == '*')
        keyvalue = FUNCTION_STARKEY_ON;
    else if(uckey == '0')
        keyvalue = NUMBER_ZERO_ON;
    else if(uckey == '1')
        keyvalue = NUMBER_ONE_ON;
    else if(uckey == '2')
        keyvalue = NUMBER_TWO_ON;
    else if(uckey == '3')
        keyvalue = NUMBER_THREE_ON;
    else if(uckey == '4')
        keyvalue = NUMBER_FOUR_ON;
    else if(uckey == '5')
        keyvalue = NUMBER_FIVE_ON;
    else if(uckey == '6')
        keyvalue = NUMBER_SIX_ON;
    else if(uckey == '7')
        keyvalue = NUMBER_SEVEN_ON;
    else if(uckey == '8')
        keyvalue = NUMBER_EIGHT_ON;
    else if(uckey == '9')
        keyvalue = NUMBER_NINE_ON;
    else if(uckey == 'A')
		keyvalue = FUNCTION_F1KEY_ON;
    else if(uckey == 'B')
		keyvalue = FUNCTION_F2KEY_ON;

    if(g_u32KeyMask & keyvalue)
        return AP_SUCCESS;
    else
        return AP_FAIL;
}

/*****************************************************************
@原型	void Reset_InputPassword(void)
@功能	按键输入复位
@参数	无 
@返回	无
******************************************************************/
void Reset_InputPassword(void)
{
    passwd_state = NO_INPUT;
}

/*****************************************************************
@原型	void TK_Irq_Callback(void)
@功能	按键睡眠
@参数	无
@返回	无
******************************************************************/
void key_suspend(void)
{
    uint8_t ret = HAL_ERROR;

    ret = TK_Reset();//解决刷卡后TK睡不下去
    if(ret == HAL_OK)
    {
        DBG("TKEY Reset Ok\r\n");
    }
    else
    {
        DBG("TKEY Reset fail\r\n");
    }
    delay_ms(500);
    TK_Config();

    TK_ClearIrqStatus();//进入睡眠前，清除触摸按键状态寄存器，防止还有键值存在
    TK_SlidingWakeUp();
    ret = TK_EnterSleep();
    if(ret == HAL_OK)
    {
        DBG("TKEY Sleep Ok\r\n");
    }
    else
    {
        DBG("TKEY Sleep fail\r\n");
    }
}

/*****************************************************************
@原型	void TK_Irq_Callback(void)
@功能	按键中断回调函数
@参数	无
@返回	无
******************************************************************/
void TK_Irq_Callback(void)
{
    g_u32Start_Tick = get_ms_tick();
    g_u8KeyTouchBreakFlag = 1; 
}
/*****************************************************************
@原型	void TouchKey_GPIO_Init(void)
@功能	按键IO初始化
@参数	无
@返回	无
******************************************************************/
void TouchKey_GPIO_Init(void)
{
    HAL_StatusTypeDef ret;
    ret = TK_Init();
    if(ret != HAL_OK) 
    {
        DBG("TK_Init Failed !\n");
    }
    // GPIO_InitGpiIRQ(TK_IRQ_PIN, GPIO_NOPULL,GPIO_MODE_IT_RISING, TK_Irq_Callback);
    // NVIC_SetPriority(GPIOCD_IRQn, 1);
}

/*****************************************************************
@原型	void TouchKey_Init(void)
@功能	按键模块参数初始化
@参数	无
@返回	无
******************************************************************/
void TouchKey_Init(void)
{
    HAL_StatusTypeDef ret;
    uint8_t tk_id = 0;
    uint8_t tk_ver = 0;
    //TK模块参数配置
    ret = TK_Config();
    if(ret != HAL_OK) 
    {
        DBG("TK_Config Failed !\r\n");
    }
    ret = TK_GetInfo(&tk_id, &tk_ver);
    if(ret != HAL_OK) 
    {
        DBG("TK_GetInfo Failed !\r\n");
    }
    DBG("get touch key module id = 0x%02x, Version = 0x%02x\r\n", tk_id, tk_ver);
}

/*****************************************************************
@原型	uint16_t get_touch_key(uint8_t *pucKey, uint32_t time_out)
@功能	获取面板输入结束后的按键值
@参数	pucKey-按键存储指针
@参数	time_out-超时时间
@返回	AP_SUCCESS-成功，AP_FAIL-失败
******************************************************************/
uint16_t get_touch_key(uint8_t *pucKey, uint32_t time_out)
{
    uint32_t tv = time_out;
    uint8_t release_flag;
    uint8_t  ucKey = 0;
    while(1)
    {
        ucKey = TK_GetKeyVal(&release_flag);
        if(ucKey != TK_NULL)
        {
            if(is_mask(ucKey) == AP_SUCCESS)
            {
                if(release_flag == 0)//确认是按下（不是抬手）
                {	
                    *pucKey	= ucKey; 	
                    Voice_Play(VOICE_KEY);
                    return AP_SUCCESS;
                } 
            }
            return AP_FAIL;
        }

        if(time_out)
        {
            tv--;
            if (tv == 0)
            {
                return AP_FAIL;
            }
        }
        if(g_u8MachineKeyFlag == 1) /*机械按键按下，*/
        {
            return AP_BREAK;
        }
    }
}

/*****************************************************************
@原型	uint16_t App_KeyInput(uint8_t *pucKeybuf, uint32_t *pudLen)
@功能	获取面板输入结束后密码数据
@参数	pucKeybuf：输入的密码存储指针；
@参数	pudLen：输入密码长度；
@返回	AP_SUCCESS：成功；
        AP_FAIL：返回失败；
        AP_WELL_KEY_SHORT：井号键短按；
        AP_PWD_LEN_ERROR：密码长度错误，长度小于6，或者长度大于16；
        AP_TIME_OUT：超时时间到达；
        ...
******************************************************************/
uint16_t App_KeyInput(uint8_t *pucKeybuf, uint32_t *pudLen)
{
    uint8_t i;
    uint8_t x,y;
    char msg[64];
    uint32_t input_interval;
    uint8_t udCnt = 0;
    uint8_t keyval;
    uint16_t ret;
    static uint32_t input_start_tick = 0;
    ret = get_touch_key(&keyval, 10);

    if(AP_SUCCESS == ret)
    {   
        udCnt = strlen((const char*)pucKeybuf);
        DBG("App_KeyInput  udCnt = %d\r\n",udCnt);
        *(pucKeybuf + udCnt) = keyval;
        if(passwd_state == NO_INPUT)
        {
            if(('#' == pucKeybuf[udCnt])&&(udCnt == 0))
            {
                Reset_InputPassword();
                return AP_WELL_KEY_SHORT;
            }
            passwd_state = INPUT_PROCESS;
            Set_ScreenState(SCREEN_PASSWD);
            LED_Light_SetAllStatus(LED_LIGHT_ON);
            LCD_ClearScrn();
            strcpy(msg, "请输入密码");
            x = (TEXT_COL_MAX - strlen(msg)) >> 1;
            y = 0;
            LCD_ShowRowStr(x, y, msg);
            for (i=0; i<udCnt+1; i++)
            {
                // msg[i] = '*';
                msg[i] = pucKeybuf[i];
            }
            msg[i] = 0;
            x = (LCD_W - strlen(msg)*ASC_CHAR_DOT_WIDTH) >> 1;
            y = (LCD_H - ASC_CHAR_WIDTH)>>1;
            LCD_ShowStr(x, y, msg);
            input_start_tick = get_ms_tick();
        }
        else if(passwd_state == INPUT_PROCESS)
        {

            if('*' == pucKeybuf[udCnt]) /* 退格*/
            {
                if(udCnt)/*temp中有数据*/
                {
                    //按*退格，也就是删除上一个按键
//                    DBG("udCnt = %d ", udCnt);
//                    pucKeybuf[udCnt] = 0;
//                    pucKeybuf[--udCnt] = 0;
//                    *pudLen = udCnt;
                    //按*清空输入  //modify by sam 20210309 
                    memset(pucKeybuf,0,17);
                    udCnt =0;
                    *pudLen =0;
                }
//                else
//                {
//                    passwd_state = INPUT_PROCESS;
//                    Set_ScreenState(SCREEN_PASSWD);
//                }
                LCD_ClearScrn();
                strcpy(msg, "请输入密码");
                x = (TEXT_COL_MAX - strlen(msg)) >> 1;
                y = 0;
                LCD_ShowRowStr(x, y, msg);
                for (i=0; i<udCnt+1; i++)
                {
                    msg[i] = pucKeybuf[i];
                }
                msg[i] = 0;
                x = (LCD_W - strlen(msg)*ASC_CHAR_DOT_WIDTH) >> 1;
                y = (LCD_H - ASC_CHAR_WIDTH)>>1;
                LCD_ShowStr(x, y, msg);
                LCD_Update();
                //reset input timeout
                input_start_tick = get_ms_tick();
                
                return AP_STAR_KEY_SHORT;
            }
            else if('#' == pucKeybuf[udCnt])
            {
                if(udCnt >= MIN_KEYPWD_LEN)
                {
                    passwd_state = INPUT_END;
                    Reset_InputPassword();
                    Set_ScreenState(SCREEN_INFO);
                    pucKeybuf[udCnt] = '\0';/*密码保存要求*/
                    *pudLen = udCnt;
                    Reset_InputPassword();
                    Voice_Busy();
                    return AP_SUCCESS;
                }
                else
                {
                    if(udCnt==1)
                    {
                        if(pucKeybuf[0]=='*')//进入管理模式
                        {
                            printfS("\r\nManagement mode.");
                            Reset_InputPassword();
                            return AP_STAR_KEY_LONG;
                        }
                    }		
                    else if(udCnt==2)
                    {
                        if(pucKeybuf[0]=='1')
                        {
                            Reset_InputPassword();
                            if(pucKeybuf[1]=='9')//恢复出厂设置
                            {
                                return AP_Restore;
                            }
                            else if(pucKeybuf[1]=='8')//添加管理员
                            {
                                return AP_REG_ADMIN;
                            }
                            else if(pucKeybuf[1]=='7')//删除管理员
                            {
                                return AP_DEL_ADMIN;
                            }
                            else if(pucKeybuf[1]=='6')//添加用户
                            {
                                return AP_REG_USER;
                            }
                            else if(pucKeybuf[1]=='5')//删除用户
                            {                                
                                return AP_DEL_USER;
                            }
                            else if(pucKeybuf[1]=='4')//回退到Boot
                            {
                                return AP_RETURN_BOOT;
                            }
                        }
                    }
                    else if(udCnt == 0)
                    {
                        Reset_InputPassword();
                        return AP_WELL_KEY_SHORT;
                    }
                    //reset input timeout
                    input_start_tick = get_ms_tick();
                    Reset_InputPassword();
                    return AP_PWD_LEN_ERROR;
                }
            }
            else
            {
                if(udCnt < MAX_KEYPWD_LEN)
                {


//                    LCD_ClearScrn();
//                    strcpy(msg, "请输入密码");
//                    x = (TEXT_COL_MAX - strlen(msg)) >> 1;
//                    y = 0;
//                    LCD_ShowRowStr(x, y, msg); 
          


                    for (i=0; i<udCnt+1; i++)
                    {
                        // msg[i] = '*';
                        msg[i] = pucKeybuf[i];
                    }
                    msg[i] = 0;
                    x = (LCD_W - strlen(msg)*ASC_CHAR_DOT_WIDTH) >> 1;
                    y = (LCD_H - ASC_CHAR_WIDTH)>>1;
                    LCD_ShowStr(x, y, msg);
                }
#if 1                   //modify by sam 20210312 测试按键时关闭，以免按键数量超过16时报错                
                else
                {
                    // input_start_tick = get_ms_tick();
                    // Reset_InputPassword();
                    // return AP_PWD_LEN_ERROR;
                    memset(pucKeybuf,0,17);
                    udCnt =0;
                    *pudLen =0;
                    passwd_state = NO_INPUT;
                    // LCD_ClearScrn();
                    // strcpy(msg, "请输入密码");
                    // x = (TEXT_COL_MAX - strlen(msg)) >> 1;
                    // y = 0;
                    // LCD_ShowRowStr(x, y, msg);
                    // for (i=0; i<udCnt+1; i++)
                    // {
                    //     msg[i] = pucKeybuf[i];
                    // }
                    // msg[i] = 0;
                    // x = (LCD_W - strlen(msg)*ASC_CHAR_DOT_WIDTH) >> 1;
                    // y = (LCD_H - ASC_CHAR_WIDTH)>>1;
                    // LCD_ShowStr(x, y, msg);
                    // LCD_Update();
                    // //reset input timeout
                    // input_start_tick = get_ms_tick();
                    
                    return AP_STAR_KEY_SHORT;
                }
#endif                
                //reset input timeout
                input_start_tick = get_ms_tick();
            }
        }
        LCD_Update();
    }
    else
    {
        if(passwd_state != NO_INPUT)
        {
            input_interval = get_ms_delay(input_start_tick);
            if(input_interval >= INPUT_PASSWD_TIMEOUT)
            {
                LED_Light_SetAllStatus(LED_LIGHT_OFF);
                passwd_state = NO_INPUT;
                Set_ScreenState(SCREEN_IDLE);
                Reset_InputPassword();
                return AP_TIME_OUT;
            }
        }
    }
    return AP_CONTINUE;
}

/*****************************************************************
@原型	uint32_t App_KeyRegInput(uint8_t *pucKeybuf, uint32_t udType)
@功能	注册时获取当前输入的按键数值
@参数	pucKeybuf：输入的密码存储指针；
@参数	udType：密码类型（普通用户/管理员）；
@返回	AP_SUCCESS：成功；
        AP_FAIL：返回失败；
        AP_WELL_KEY_SHORT：井号键短按；
        AP_PWD_LEN_ERROR：密码长度错误，长度小于6，或者长度大于16；
        AP_TIME_OUT：超时时间到达；
        ...
******************************************************************/
uint32_t App_KeyRegInput(uint8_t *pucKeybuf, uint32_t udType)
{
    uint32_t udRtn = AP_FAIL;

    uint32_t udLen;
    uint16_t udUserNum;
    uint32_t start_tick;
    uint32_t pass_tick; 
    start_tick = get_ms_tick();
    while(1)
    {
        udRtn = App_KeyInput(pucKeybuf, &udLen);/* 获取面板输入的密码*/

        if((AP_CONTINUE != udRtn)&& (AP_STAR_KEY_SHORT != udRtn) )
        {
            DBG(" udLen = %d udRtn = %d ", udLen, udRtn);
            // break;
            return udRtn;
        }
        else
        {
           if(0 < udLen)
            {
                gfm_get_user_num(&udUserNum,udType);  /* 获取该类型下所有的注册用户*/
                if(udUserNum >= gfm_get_maxnum(udType))/*密码用户大于等于最大值*/
                {
                    return AP_FULL;
                    // break;
                }
            }
        }
        if(AP_STAR_KEY_SHORT == udRtn)
        {
            start_tick = get_ms_tick();
        }
//        if((g_u8CardBreakFlag)||(READ_FPS_IRQ_STATUS() == 1)||(g_u8MachineKeyFlag))
        if((g_u8CardBreakFlag)||(g_u8FgpBreakFlag)||(READ_FPS_IRQ_STATUS() == 1)||(g_u8MachineKeyFlag))  
        {					
            return  AP_BREAK;
        }
        pass_tick = get_ms_delay(start_tick);
        if(pass_tick > GLOBAL_TIMEOUT)
        {
            return AP_TIME_OUT;
        }
    }
    return udRtn;
}

/*****************************************************************
@原型	uint32_t App_KeyReg(uint8_t ucAttr)
@功能	密码注册流程   阻塞方式
@参数	ucAttr：密码属性（普通用户/管理员）；
@返回	AP_SUCCESS：成功；
        AP_FAIL：返回失败；
        AP_WELL_KEY_SHORT：井号键短按；
        AP_PWD_LEN_ERROR：密码长度错误，长度小于6，或者长度大于16；
        AP_TIME_OUT：超时时间到达；
        ...
******************************************************************/
uint32_t App_KeyReg(uint8_t ucAttr)
{

    uint32_t udRtn = AP_FAIL;

    uint16_t udUserId = 0;
    char msg[64];
    uint8_t x;
    
    memset(g_u8KeyRegBuf1, 0, sizeof(g_u8KeyRegBuf1));
    memset(g_u8KeyRegBuf2, 0, sizeof(g_u8KeyRegBuf2));

    if(g_u8Forbid_Touch_flag == 0)     
    {
        udRtn = App_KeyRegInput(g_u8KeyRegBuf1, ucAttr);
    }
    else
    {
        if((g_u8Forbid_Finger_flag ==0) || (g_u8Forbid_Card_flag ==0))
            return AP_CONTINUE;
        return AP_FULL;
    }
    DBG(" App_KeyRegInput udRtn = %02x\n", udRtn);

    if(AP_SUCCESS == udRtn)/*第一次密码输入合规*/
    {
#if 1
        DBG(" pwd len = %d\r\n",strlen((char *)g_u8KeyRegBuf1));

        DBG(" g_u8KeyRegBuf1=%s",g_u8KeyRegBuf1);

        DBG(" \r\n");
#endif
		
 /* 判断输入的密码是否过于简单 */
#if 0
        udRtn = App_ChkEqualDifference(g_u8KeyRegBuf1, strlen((const char *)g_u8KeyRegBuf1));
        DBG("是否为等差数列：udRtn=%d.\r\n",udRtn);
        if(TA_SUCCESS == udRtn)
        {
#if defined(LEDBAND)
            Drv_Epwm0Stop();
            Drv_LedOn(LED_RED_ON);
#endif
            KEY_BACK_LIGHT_PIN = KEY_BACK_LIGHT_OFF;
            delay_ms(200);
            Api_VoiceBroadcast(VOICE_PLEASE_NOT_USE_CONTINUOUSPWD);    /* 密码不符合要求 */
            while(0 != Api_VoiceAcquire());
            KEY_BACK_LIGHT_PIN = KEY_BACK_LIGHT_ON;
#if defined(LEDBAND)
            /* 打开蓝灯的PWM */
            Drv_Epwm0Init();
            Drv_LedOn(LED_BLUE_ON);                       
#endif   
            return TA_FAIL;
        }

#endif
        LCD_ClearScrn();
        strcpy(msg, "请再输入同一密码");
        x = (TEXT_COL_MAX - strlen(msg)) >> 1;
        LCD_ShowRowStr(x, 0, msg);
        LCD_Update();	
        LED_Light_SetAllStatus(LED_LIGHT_OFF);	
        Tkeyirq_disable();
        Voice_Busy();        
        Voice_Play(VOICE_PWD_ENTR_AGAIN);/*提示请再输入同一密码*/
        Tkeyirq_enable();
        delay_ms(200); 
        LED_Light_SetAllStatus(LED_LIGHT_ON);
        
        udRtn = App_KeyRegInput(g_u8KeyRegBuf2, ucAttr);
        if(AP_SUCCESS == udRtn)/*第二次密码输入合规*/
        {
#if 1
            DBG(" pwd len = %d ", strlen((char*)g_u8KeyRegBuf2));

            DBG("g_u8KeyRegBuf2 =%s", g_u8KeyRegBuf2);

            DBG("\n");
#endif
            if(0 == strcmp((char*)g_u8KeyRegBuf1, (char*)g_u8KeyRegBuf2))/*密码输入一致 memcmp*/        
            {
                g_u8KeyRegBuf1[strlen((char*)g_u8KeyRegBuf1)] = '\0';
                
                udRtn = gfm_verify_data(g_u8KeyRegBuf1,&udUserId,ATTR_PWD_ADMIN,TYPE_STRING);//先比对管理员
                if(AP_FAIL == udRtn)
                {//管理员比对失败，再比对用户  
                    udRtn = gfm_verify_data(g_u8KeyRegBuf1,&udUserId,ATTR_PWD_USER,TYPE_BYTE);
                }
                if(udRtn == AP_SUCCESS)
                {
                    DBG("password exist\r\n");
                    Lcd_Center_Display((char *)"该密码已存在");
                    Voice_Play(VOICE_PWD_EXIST);
                    Voice_Busy();
                    return AP_EXSIT;
                }
                
                udUserId = 0;
                udRtn = gfm_add(g_u8KeyRegBuf1,strlen((char*)g_u8KeyRegBuf1),&udUserId,ucAttr);/*保存新密码*/
                if(AP_SUCCESS == udRtn)
                { 	
                    Voice_Busy();						
                    return AP_SUCCESS;
                }
                else /*保存失败*/
                {
                    DBG("pwd save fail;%d \r\n",udRtn);
                    Lcd_Center_Display((char *)"增加失败");
                    Voice_Play(VOICE_FIRE_ALARM);
                    Voice_Busy();                 
                    return AP_FAIL;
                }
            }
            else  /*密码输入不一致*/
            {	
                Lcd_Center_Display((char *)"两次输入不一致");            
                Voice_Play(VOICE_PWD_NOTFIT);
                Voice_Busy();						
                return AP_FAIL;
            }
        }
	  	else/*密码输入不合规*/
		{	
            Lcd_Center_Display((char *)"密码输入不合规");
            Voice_Play(VOICE_PWD_NOTFIT);
            Voice_Busy();	
			return AP_FAIL;
		}
    }
    switch(udRtn)/*the first time to deal with the error/interrupt case from key reg*/
    {
        case AP_CONTINUE:
        break;
        case AP_FULL:
            key_mask(KEY_ALL_OFF);
            Reset_InputPassword();            
            LED_Light_SetAllStatus(LED_LIGHT_OFF); 
            if(ATTR_PWD_ADMIN == ucAttr)
            {
                Voice_Play(VOICE_PWD_ADMIN_FULL);
                Voice_Busy();
            }
            else
            {
                Voice_Play(VOICE_PWD_USER_FULL);
                Voice_Busy();
            }                    
            LED_Light_SetAllStatus(LED_LIGHT_ON);                    
            delay_ms(100);
        break;
        case AP_FAIL:               
            DBG(" TA_FAIL ");           
            LED_Light_SetAllStatus(LED_LIGHT_OFF);           
            Voice_Play(VOICE_FIRE_ALARM);
            Voice_Busy();         
            LED_Light_SetAllStatus(LED_LIGHT_ON);
            Reset_InputPassword();
        break;
        case AP_PWD_LEN_ERROR:             
            /* 关闭按键中断 */
            Tkeyirq_disable();
            LED_Light_SetAllStatus(LED_LIGHT_OFF);
            Lcd_Center_Display((char *)"密码输入不合规");
            Voice_Play(VOICE_ENTR_FITBIT12);
            Voice_Busy();
            LED_Light_SetAllStatus(LED_LIGHT_ON); 
            /* 打开按键中断 */
            Tkeyirq_enable();                    
            Reset_InputPassword();
            memset(g_u8KeyRegBuf1, 0, sizeof(g_u8KeyRegBuf1));
            udRtn = AP_CONTINUE;
        break;
        default:
        
        break;
    }

    return udRtn;
}

/*****************************************************************
@原型	uint32_t App_KeyMatch(uint8_t ucRecFlag, uint8_t ucAttr)
@功能	密码比对函数 
@参数	ucRecFlag： 0 代表不添加开门记录  1,：添加开门记录；  
@参数	ucAttr：密码属性（普通用户/管理员）；
@返回	AP_SUCCESS：成功；
        AP_FAIL：返回失败；
        AP_WELL_KEY_SHORT：井号键短按；
        AP_PWD_LEN_ERROR：密码长度错误，长度小于6，或者长度大于16；
        AP_TIME_OUT：超时时间到达；
        ...
******************************************************************/
uint32_t App_KeyMatch(uint8_t ucRecFlag, uint8_t ucAttr)
{
    
    uint32_t udRtn = AP_FAIL;

    uint16_t ID;
    uint32_t udLen = 0,i=0;

    udRtn = App_KeyInput(g_u8KeyMatchBuf, &udLen);

 
    if(AP_SUCCESS == udRtn)/*获取到正确的密码*/
    {   
        /* 普通比对 */
        if(ATTR_PWD_ALL == ucAttr)
        { 
            DBG("g_u8NO_mode = %d\n",g_u8NO_mode);
            // if (g_u8NO_mode == 1)  //判断是不是通开模式
            if(AP_NO_ADMIN == App_Is_Admin_Set())
            {
                // if (strstr((char*)g_u8KeyMatchBuf, (char*)DEFAULT_PWD))
                {
                    g_u8Typeopendoor = 0;
                    g_u16IDopendoor = 0;
                    Reset_InputPassword();
                    memset(g_u8KeyMatchBuf,0,sizeof(g_u8KeyMatchBuf));
                    return AP_SUCCESS;
                }
                // else
                // {
                //     Reset_InputPassword();
                //     memset(g_u8KeyMatchBuf,0,sizeof(g_u8KeyMatchBuf));
                //     return AP_FAIL;
                // }					
            }				
            else
            {
                udRtn = gfm_verify_data(g_u8KeyMatchBuf,&ID,ATTR_PWD_USER,TYPE_STRING); 
            }                
            if(AP_FAIL == udRtn)/*不存在user 此密码*/
            {
                udRtn = AP_FAIL;
                udRtn = gfm_verify_data(g_u8KeyMatchBuf,&ID,ATTR_PWD_ADMIN,TYPE_STRING);
                DBG("jata_verify_password TYPE_KEY_ADMIN = %d ", udRtn);            
                if(AP_FAIL == udRtn)
                {
                    /* 将存放数据的数组清空 */
                    Reset_InputPassword();
                    memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf));
                    return AP_FAIL;
                }
                else
                {
                    /* 比对成功，将存放数据的数组清空 */
                    Reset_InputPassword();
                    memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf));
                    g_u8Typeopendoor = ATTR_PWD_ADMIN;
                    g_u16IDopendoor = ID;
                    udRtn = AP_SUCCESS;
                }
            }
            else
            {
                DBG(" Type key user match Success\r\n");
                /* 比对成功，将存放数据的数组清空 */
                Reset_InputPassword();
                memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf));
                g_u8Typeopendoor = ATTR_PWD_USER;
                g_u16IDopendoor = ID;
                udRtn = AP_SUCCESS;
            }
        }
        else if(ATTR_PWD_USER == ucAttr)
        {
            DBG(" match user pwd ucAttr = %d ", ucAttr);
            /* 比对密码用户 */
            udRtn = gfm_verify_data(g_u8KeyMatchBuf,&ID,ucAttr,TYPE_STRING);
            if(AP_FAIL == udRtn)/*不存在此密码*/
            {
                DBG("key user MatchFailed");
                udRtn = AP_FAIL;
                /* 将存放数据的数组清空 */
                Reset_InputPassword();
                memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf));
                return AP_FAIL;
            }
            else
            {
                /*密码用户比对成功，将开锁记录保存 */
                Reset_InputPassword();
                memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf));
                g_u8Typeopendoor = ucAttr;
                g_u16IDopendoor = ID;
                udRtn = AP_SUCCESS;
            }
        }
        else if(ATTR_PWD_ADMIN == ucAttr)    /* 比对管理员 */
        {
            /*比对管理员 */
            if (g_u8NO_mode ==1)  //判断是不是通开模式
            {
                if (strstr((char*)g_u8KeyMatchBuf, (char*)DEFAULT_PWD))
                {
                    Reset_InputPassword();
                    memset(g_u8KeyMatchBuf,0,sizeof(g_u8KeyMatchBuf));
                    return AP_SUCCESS;
                }
                else
                {
                    Reset_InputPassword();
                    memset(g_u8KeyMatchBuf,0,sizeof(g_u8KeyMatchBuf));
                    return AP_FAIL;
                }					
            }				
            else
            {
                udRtn = gfm_verify_data(g_u8KeyMatchBuf,&ID,ucAttr,TYPE_STRING);
            }
            if(AP_FAIL == udRtn)/*不存在此密码*/
            {
                DBG("MatchFailed");
                Reset_InputPassword();
                memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf));
                udRtn = AP_FAIL;
            }
            else
            {
                Reset_InputPassword();
                memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf)); 
                g_u8Typeopendoor = ucAttr;
                g_u16IDopendoor = ID;              
                udRtn = AP_SUCCESS;
            }

        }
    }
    else if(udRtn == AP_Machine_Break)
    {
        Reset_InputPassword();
        memset(g_u8KeyMatchBuf,0,sizeof(g_u8KeyMatchBuf));
        return AP_Machine_Break;
    }
    if(AP_CONTINUE != udRtn)
    {
        DBG("else key\r\n");
        /*清空密码缓存*/
        Reset_InputPassword();
        memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf));
    }

    return udRtn;
}

/*****************************************************************
@原型	void Tkeyirq_disable(void)
@功能	关闭按键中断
@参数	无
@返回	无
******************************************************************/
void Tkeyirq_disable(void)
{
    GPIO_DisableGpiIRQ(TK_IRQ_PIN);
}

/*****************************************************************
@原型	void Tkeyirq_enable(void)
@功能	打开按键中断
@参数	无
@返回	无
******************************************************************/
void Tkeyirq_enable(void)
{
    // GPIO_EnableGpiIRQ(TK_IRQ_PIN);
    GPIO_InitGpiIRQ(TK_IRQ_PIN, GPIO_NOPULL,GPIO_MODE_IT_RISING, TK_Irq_Callback);
    NVIC_SetPriority(GPIOCD_IRQn, 1);
}


