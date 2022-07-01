
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
static uint8_t g_u8KeyMatchBuf[MAX_KEYPWD_LEN + 1];      /* �ȶ�ʱ������뻺������� */
static uint8_t passwd_state = NO_INPUT;

/*****************************************************************
@ԭ��	void key_mask(uint32_t uwKeyMaskTemporary)
@����	�������
@����	uwKeyMaskTemporary-��ǵİ��� 
@����	��
******************************************************************/
void key_mask(uint32_t uwKeyMaskTemporary)
{
    g_u32KeyMask = uwKeyMaskTemporary;
}

/*****************************************************************
@ԭ��	uint16_t is_mask(uint8_t uckey)
@����	��������ж�
@����	uckey - ����ֵ
@����	AP_SUCCESS-�ɹ���AP_FAIL-ʧ��
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
@ԭ��	void Reset_InputPassword(void)
@����	�������븴λ
@����	�� 
@����	��
******************************************************************/
void Reset_InputPassword(void)
{
    passwd_state = NO_INPUT;
}

/*****************************************************************
@ԭ��	void TK_Irq_Callback(void)
@����	����˯��
@����	��
@����	��
******************************************************************/
void key_suspend(void)
{
    uint8_t ret = HAL_ERROR;

    ret = TK_Reset();//���ˢ����TK˯����ȥ
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

    TK_ClearIrqStatus();//����˯��ǰ�������������״̬�Ĵ�������ֹ���м�ֵ����
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
@ԭ��	void TK_Irq_Callback(void)
@����	�����жϻص�����
@����	��
@����	��
******************************************************************/
void TK_Irq_Callback(void)
{
    g_u32Start_Tick = get_ms_tick();
    g_u8KeyTouchBreakFlag = 1; 
}
/*****************************************************************
@ԭ��	void TouchKey_GPIO_Init(void)
@����	����IO��ʼ��
@����	��
@����	��
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
@ԭ��	void TouchKey_Init(void)
@����	����ģ�������ʼ��
@����	��
@����	��
******************************************************************/
void TouchKey_Init(void)
{
    HAL_StatusTypeDef ret;
    uint8_t tk_id = 0;
    uint8_t tk_ver = 0;
    //TKģ���������
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
@ԭ��	uint16_t get_touch_key(uint8_t *pucKey, uint32_t time_out)
@����	��ȡ������������İ���ֵ
@����	pucKey-�����洢ָ��
@����	time_out-��ʱʱ��
@����	AP_SUCCESS-�ɹ���AP_FAIL-ʧ��
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
                if(release_flag == 0)//ȷ���ǰ��£�����̧�֣�
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
        if(g_u8MachineKeyFlag == 1) /*��е�������£�*/
        {
            return AP_BREAK;
        }
    }
}

/*****************************************************************
@ԭ��	uint16_t App_KeyInput(uint8_t *pucKeybuf, uint32_t *pudLen)
@����	��ȡ��������������������
@����	pucKeybuf�����������洢ָ�룻
@����	pudLen���������볤�ȣ�
@����	AP_SUCCESS���ɹ���
        AP_FAIL������ʧ�ܣ�
        AP_WELL_KEY_SHORT�����ż��̰���
        AP_PWD_LEN_ERROR�����볤�ȴ��󣬳���С��6�����߳��ȴ���16��
        AP_TIME_OUT����ʱʱ�䵽�
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
            strcpy(msg, "����������");
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

            if('*' == pucKeybuf[udCnt]) /* �˸�*/
            {
                if(udCnt)/*temp��������*/
                {
                    //��*�˸�Ҳ����ɾ����һ������
//                    DBG("udCnt = %d ", udCnt);
//                    pucKeybuf[udCnt] = 0;
//                    pucKeybuf[--udCnt] = 0;
//                    *pudLen = udCnt;
                    //��*�������  //modify by sam 20210309 
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
                strcpy(msg, "����������");
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
                    pucKeybuf[udCnt] = '\0';/*���뱣��Ҫ��*/
                    *pudLen = udCnt;
                    Reset_InputPassword();
                    Voice_Busy();
                    return AP_SUCCESS;
                }
                else
                {
                    if(udCnt==1)
                    {
                        if(pucKeybuf[0]=='*')//�������ģʽ
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
                            if(pucKeybuf[1]=='9')//�ָ���������
                            {
                                return AP_Restore;
                            }
                            else if(pucKeybuf[1]=='8')//��ӹ���Ա
                            {
                                return AP_REG_ADMIN;
                            }
                            else if(pucKeybuf[1]=='7')//ɾ������Ա
                            {
                                return AP_DEL_ADMIN;
                            }
                            else if(pucKeybuf[1]=='6')//����û�
                            {
                                return AP_REG_USER;
                            }
                            else if(pucKeybuf[1]=='5')//ɾ���û�
                            {                                
                                return AP_DEL_USER;
                            }
                            else if(pucKeybuf[1]=='4')//���˵�Boot
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
//                    strcpy(msg, "����������");
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
#if 1                   //modify by sam 20210312 ���԰���ʱ�رգ����ⰴ����������16ʱ����                
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
                    // strcpy(msg, "����������");
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
@ԭ��	uint32_t App_KeyRegInput(uint8_t *pucKeybuf, uint32_t udType)
@����	ע��ʱ��ȡ��ǰ����İ�����ֵ
@����	pucKeybuf�����������洢ָ�룻
@����	udType���������ͣ���ͨ�û�/����Ա����
@����	AP_SUCCESS���ɹ���
        AP_FAIL������ʧ�ܣ�
        AP_WELL_KEY_SHORT�����ż��̰���
        AP_PWD_LEN_ERROR�����볤�ȴ��󣬳���С��6�����߳��ȴ���16��
        AP_TIME_OUT����ʱʱ�䵽�
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
        udRtn = App_KeyInput(pucKeybuf, &udLen);/* ��ȡ������������*/

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
                gfm_get_user_num(&udUserNum,udType);  /* ��ȡ�����������е�ע���û�*/
                if(udUserNum >= gfm_get_maxnum(udType))/*�����û����ڵ������ֵ*/
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
@ԭ��	uint32_t App_KeyReg(uint8_t ucAttr)
@����	����ע������   ������ʽ
@����	ucAttr���������ԣ���ͨ�û�/����Ա����
@����	AP_SUCCESS���ɹ���
        AP_FAIL������ʧ�ܣ�
        AP_WELL_KEY_SHORT�����ż��̰���
        AP_PWD_LEN_ERROR�����볤�ȴ��󣬳���С��6�����߳��ȴ���16��
        AP_TIME_OUT����ʱʱ�䵽�
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

    if(AP_SUCCESS == udRtn)/*��һ����������Ϲ�*/
    {
#if 1
        DBG(" pwd len = %d\r\n",strlen((char *)g_u8KeyRegBuf1));

        DBG(" g_u8KeyRegBuf1=%s",g_u8KeyRegBuf1);

        DBG(" \r\n");
#endif
		
 /* �ж�����������Ƿ���ڼ� */
#if 0
        udRtn = App_ChkEqualDifference(g_u8KeyRegBuf1, strlen((const char *)g_u8KeyRegBuf1));
        DBG("�Ƿ�Ϊ�Ȳ����У�udRtn=%d.\r\n",udRtn);
        if(TA_SUCCESS == udRtn)
        {
#if defined(LEDBAND)
            Drv_Epwm0Stop();
            Drv_LedOn(LED_RED_ON);
#endif
            KEY_BACK_LIGHT_PIN = KEY_BACK_LIGHT_OFF;
            delay_ms(200);
            Api_VoiceBroadcast(VOICE_PLEASE_NOT_USE_CONTINUOUSPWD);    /* ���벻����Ҫ�� */
            while(0 != Api_VoiceAcquire());
            KEY_BACK_LIGHT_PIN = KEY_BACK_LIGHT_ON;
#if defined(LEDBAND)
            /* �����Ƶ�PWM */
            Drv_Epwm0Init();
            Drv_LedOn(LED_BLUE_ON);                       
#endif   
            return TA_FAIL;
        }

#endif
        LCD_ClearScrn();
        strcpy(msg, "��������ͬһ����");
        x = (TEXT_COL_MAX - strlen(msg)) >> 1;
        LCD_ShowRowStr(x, 0, msg);
        LCD_Update();	
        LED_Light_SetAllStatus(LED_LIGHT_OFF);	
        Tkeyirq_disable();
        Voice_Busy();        
        Voice_Play(VOICE_PWD_ENTR_AGAIN);/*��ʾ��������ͬһ����*/
        Tkeyirq_enable();
        delay_ms(200); 
        LED_Light_SetAllStatus(LED_LIGHT_ON);
        
        udRtn = App_KeyRegInput(g_u8KeyRegBuf2, ucAttr);
        if(AP_SUCCESS == udRtn)/*�ڶ�����������Ϲ�*/
        {
#if 1
            DBG(" pwd len = %d ", strlen((char*)g_u8KeyRegBuf2));

            DBG("g_u8KeyRegBuf2 =%s", g_u8KeyRegBuf2);

            DBG("\n");
#endif
            if(0 == strcmp((char*)g_u8KeyRegBuf1, (char*)g_u8KeyRegBuf2))/*��������һ�� memcmp*/        
            {
                g_u8KeyRegBuf1[strlen((char*)g_u8KeyRegBuf1)] = '\0';
                
                udRtn = gfm_verify_data(g_u8KeyRegBuf1,&udUserId,ATTR_PWD_ADMIN,TYPE_STRING);//�ȱȶԹ���Ա
                if(AP_FAIL == udRtn)
                {//����Ա�ȶ�ʧ�ܣ��ٱȶ��û�  
                    udRtn = gfm_verify_data(g_u8KeyRegBuf1,&udUserId,ATTR_PWD_USER,TYPE_BYTE);
                }
                if(udRtn == AP_SUCCESS)
                {
                    DBG("password exist\r\n");
                    Lcd_Center_Display((char *)"�������Ѵ���");
                    Voice_Play(VOICE_PWD_EXIST);
                    Voice_Busy();
                    return AP_EXSIT;
                }
                
                udUserId = 0;
                udRtn = gfm_add(g_u8KeyRegBuf1,strlen((char*)g_u8KeyRegBuf1),&udUserId,ucAttr);/*����������*/
                if(AP_SUCCESS == udRtn)
                { 	
                    Voice_Busy();						
                    return AP_SUCCESS;
                }
                else /*����ʧ��*/
                {
                    DBG("pwd save fail;%d \r\n",udRtn);
                    Lcd_Center_Display((char *)"����ʧ��");
                    Voice_Play(VOICE_FIRE_ALARM);
                    Voice_Busy();                 
                    return AP_FAIL;
                }
            }
            else  /*�������벻һ��*/
            {	
                Lcd_Center_Display((char *)"�������벻һ��");            
                Voice_Play(VOICE_PWD_NOTFIT);
                Voice_Busy();						
                return AP_FAIL;
            }
        }
	  	else/*�������벻�Ϲ�*/
		{	
            Lcd_Center_Display((char *)"�������벻�Ϲ�");
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
            /* �رհ����ж� */
            Tkeyirq_disable();
            LED_Light_SetAllStatus(LED_LIGHT_OFF);
            Lcd_Center_Display((char *)"�������벻�Ϲ�");
            Voice_Play(VOICE_ENTR_FITBIT12);
            Voice_Busy();
            LED_Light_SetAllStatus(LED_LIGHT_ON); 
            /* �򿪰����ж� */
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
@ԭ��	uint32_t App_KeyMatch(uint8_t ucRecFlag, uint8_t ucAttr)
@����	����ȶԺ��� 
@����	ucRecFlag�� 0 ������ӿ��ż�¼  1,����ӿ��ż�¼��  
@����	ucAttr���������ԣ���ͨ�û�/����Ա����
@����	AP_SUCCESS���ɹ���
        AP_FAIL������ʧ�ܣ�
        AP_WELL_KEY_SHORT�����ż��̰���
        AP_PWD_LEN_ERROR�����볤�ȴ��󣬳���С��6�����߳��ȴ���16��
        AP_TIME_OUT����ʱʱ�䵽�
        ...
******************************************************************/
uint32_t App_KeyMatch(uint8_t ucRecFlag, uint8_t ucAttr)
{
    
    uint32_t udRtn = AP_FAIL;

    uint16_t ID;
    uint32_t udLen = 0,i=0;

    udRtn = App_KeyInput(g_u8KeyMatchBuf, &udLen);

 
    if(AP_SUCCESS == udRtn)/*��ȡ����ȷ������*/
    {   
        /* ��ͨ�ȶ� */
        if(ATTR_PWD_ALL == ucAttr)
        { 
            DBG("g_u8NO_mode = %d\n",g_u8NO_mode);
            // if (g_u8NO_mode == 1)  //�ж��ǲ���ͨ��ģʽ
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
            if(AP_FAIL == udRtn)/*������user ������*/
            {
                udRtn = AP_FAIL;
                udRtn = gfm_verify_data(g_u8KeyMatchBuf,&ID,ATTR_PWD_ADMIN,TYPE_STRING);
                DBG("jata_verify_password TYPE_KEY_ADMIN = %d ", udRtn);            
                if(AP_FAIL == udRtn)
                {
                    /* ��������ݵ�������� */
                    Reset_InputPassword();
                    memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf));
                    return AP_FAIL;
                }
                else
                {
                    /* �ȶԳɹ�����������ݵ�������� */
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
                /* �ȶԳɹ�����������ݵ�������� */
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
            /* �ȶ������û� */
            udRtn = gfm_verify_data(g_u8KeyMatchBuf,&ID,ucAttr,TYPE_STRING);
            if(AP_FAIL == udRtn)/*�����ڴ�����*/
            {
                DBG("key user MatchFailed");
                udRtn = AP_FAIL;
                /* ��������ݵ�������� */
                Reset_InputPassword();
                memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf));
                return AP_FAIL;
            }
            else
            {
                /*�����û��ȶԳɹ�����������¼���� */
                Reset_InputPassword();
                memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf));
                g_u8Typeopendoor = ucAttr;
                g_u16IDopendoor = ID;
                udRtn = AP_SUCCESS;
            }
        }
        else if(ATTR_PWD_ADMIN == ucAttr)    /* �ȶԹ���Ա */
        {
            /*�ȶԹ���Ա */
            if (g_u8NO_mode ==1)  //�ж��ǲ���ͨ��ģʽ
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
            if(AP_FAIL == udRtn)/*�����ڴ�����*/
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
        /*������뻺��*/
        Reset_InputPassword();
        memset(g_u8KeyMatchBuf, 0, sizeof(g_u8KeyMatchBuf));
    }

    return udRtn;
}

/*****************************************************************
@ԭ��	void Tkeyirq_disable(void)
@����	�رհ����ж�
@����	��
@����	��
******************************************************************/
void Tkeyirq_disable(void)
{
    GPIO_DisableGpiIRQ(TK_IRQ_PIN);
}

/*****************************************************************
@ԭ��	void Tkeyirq_enable(void)
@����	�򿪰����ж�
@����	��
@����	��
******************************************************************/
void Tkeyirq_enable(void)
{
    // GPIO_EnableGpiIRQ(TK_IRQ_PIN);
    GPIO_InitGpiIRQ(TK_IRQ_PIN, GPIO_NOPULL,GPIO_MODE_IT_RISING, TK_Irq_Callback);
    NVIC_SetPriority(GPIOCD_IRQn, 1);
}


