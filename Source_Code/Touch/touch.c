/*
 * @Author: cgy233 1781387847@qq.com
 * @Date: 2022-06-29 11:08:01
 * @LastEditors: cgy233 1781387847@qq.com
 * @LastEditTime: 2022-06-30 16:05:08
 * @FilePath: \MDK_Projectd:\Ethan\Project\ACM32\ModulesDemo\UART\demo\Source_Code\Touch\touch.c
 * @Description: 
 * 
 * Copyright (c) 2022 by cgy233 1781387847@qq.com, All Rights Reserved. 
 */
#include "delay.h"
#include "Si14tch.h"
#include "tk_exti.h"
#include "touch.h"
#include "led.h"

// Touch
#define SIZE sizeof(TEXT_Buffer)	
extern uint8_t data_buf[4];
extern uint8_t flag ;
uint16_t times = 0; 
uint16_t tick = 0; 
uint8_t a;


static uint8_t passwd_state = 0;
static uint8_t g_u8KeyRegBuf1[7];
static uint8_t g_u8KeyRegBuf2[7];

void touch_init()
{
	LED_Init();
	delay_ms(200);
	IIC_Init();
	delay_ms(200);
	TK_EXTI_Init();
	delay_ms(200);
	while(SI14TCH_Check())
	{
		delay_ms(500);
		if(times > 5)
		{
			printfS("\r\nNO CHECK SI14TCH!");
			times = 0;
		}
		else times++;
	}

	delay_ms(200);
	SI14TCH_Init(); 

	// EXTI->IENR |=  TK_IRQ_EXTI_LINE;
	// uint8_t led_flag = 13;
			
	// while(1)
	// 	{
	// 		if(flag !=0)
	// 		{
	// 			EXTI->IENR &=  ~TK_IRQ_EXTI_LINE;
	// 			switch (flag)
	// 			{
	// 				case 1 :{ flag = 0;led_flag = 8; printfS("\r\n T1 TOUCH  9"); break; }
	// 				case 2 :{ flag = 0;led_flag = 7; printfS("\r\n T2 TOUCH  8"); break; }
	// 				case 3 :{ flag = 0;led_flag = 4; printfS("\r\n T3 TOUCH  5"); break; }
	// 				case 4 :{ flag = 0;led_flag = 5; printfS("\r\n T4 TOUCH  6"); break; }
	// 				case 5 :{ flag = 0;led_flag = 2; printfS("\r\n T5 TOUCH  3"); break; }
	// 				case 6 :{ flag = 0;led_flag = 1; printfS("\r\n T6 TOUCH  2"); break; }
	// 				case 7 :{ flag = 0;led_flag = 0; printfS("\r\n T7 TOUCH  1"); break; }
	// 				case 8 :{ flag = 0;led_flag = 3; printfS("\r\n T8 TOUCH  4"); break; }
	// 				case 9 :{ flag = 0;led_flag = 6; printfS("\r\n T9 TOUCH  7"); break; }
	// 				case 10:{ flag = 0;led_flag = 9; printfS("\r\n T10 TOUCH *"); break; }
	// 				case 11:{ flag = 0;led_flag = 11;printfS("\r\n T11 TOUCH 0"); break; }
	// 				case 12:{ flag = 0;led_flag = 10;printfS("\r\n T12 TOUCH #"); break; }	
	// 			}
	// 			led_flicker(led_flag);
	// 			do
	// 			{
	// 				led_flicker(led_flag);
	// 				// delay_us(20);
	// 				SI12_ReadData();   //?¨¢¨¨???¡ä??¡Âoutput1,2,3¨ºy?Y
	// 			}while(data_buf [0]!=0 ||data_buf [1]!=0||data_buf [2]!=0||data_buf [3]!=0);
	// 			led_flag = 13;
	// 			EXTI->IENR |=  TK_IRQ_EXTI_LINE;
	// 			// printfS("\r\nTOUCH END!");
	// 		}		
	// 	}
	
}
/*****************************************************************
@??	uint16_t get_touch_key(uint8_t *pucKey, uint32_t time_out)
@??	?????????????
@??	pucKey-??????
@??	time_out-????
@??	AP_SUCCESS-???AP_FAIL-??
******************************************************************/
uint16_t get_touch_key(uint8_t *pucKey, uint32_t time_out)
{
    uint32_t tv = time_out;
    uint8_t release_flag;
    uint8_t  ucKey = 0;
	uint8_t led_flag = 13;
	EXTI->IENR |=  TK_IRQ_EXTI_LINE;

	while(1)
		{
			if(flag != 0)
			{
				EXTI->IENR &=  ~TK_IRQ_EXTI_LINE;
				switch (flag)
				{
					case 1 :{ flag = 0;led_flag = 8; ucKey = '9'; /*printfS("\r\n T1 TOUCH  9");*/ break; }
					case 2 :{ flag = 0;led_flag = 7; ucKey = '8'; /*printfS("\r\n T2 TOUCH  8");*/ break; }
					case 3 :{ flag = 0;led_flag = 4; ucKey = '5'; /*printfS("\r\n T3 TOUCH  5");*/ break; }
					case 4 :{ flag = 0;led_flag = 5; ucKey = '6'; /*printfS("\r\n T4 TOUCH  6");*/ break; }
					case 5 :{ flag = 0;led_flag = 2; ucKey = '3'; /*printfS("\r\n T5 TOUCH  3");*/ break; }
					case 6 :{ flag = 0;led_flag = 1; ucKey = '2'; /*printfS("\r\n T6 TOUCH  2");*/ break; }
					case 7 :{ flag = 0;led_flag = 0; ucKey = '1'; /*printfS("\r\n T7 TOUCH  1");*/ break; }
					case 8 :{ flag = 0;led_flag = 3; ucKey = '4'; /*printfS("\r\n T8 TOUCH  4");*/ break; }
					case 9 :{ flag = 0;led_flag = 6; ucKey = '7'; /*printfS("\r\n T9 TOUCH  7");*/ break; }
					case 10:{ flag = 0;led_flag = 9;ucKey = '*';  /*printfS("\r\n T10 TOUCH *");*/ break; }
					case 11:{ flag = 0;led_flag = 11;ucKey = '0'; /*printfS("\r\n T11 TOUCH 0");*/ break; }
					case 12:{ flag = 0;led_flag = 10;ucKey = '#'; /*printfS("\r\n T12 TOUCH #");*/ break; }	
				}
				led_flicker(led_flag);
				do
				{
					led_flicker(led_flag);
					// delay_us(20);
					SI12_ReadData();   //?¨¢¨¨???¡ä??¡Âoutput1,2,3¨ºy?Y
				}while(data_buf [0]!=0 ||data_buf [1]!=0||data_buf [2]!=0||data_buf [3]!=0);
				led_flag = 13;
				EXTI->IENR |=  TK_IRQ_EXTI_LINE;
				*pucKey	= ucKey; 	
				ucKey = 0;
				return AP_SUCCESS;
				// printfS("\r\nTOUCH END!");
			}		
			// if(time_out)
			// {
			// 	tv--;
			// 	if (tv == 0)
			// 	{
			// 		return 0;
			// 	}
			delay_ms(100);
			// }
		}
}
void Reset_InputPassword(void)
{
    passwd_state = NO_INPUT;
}

uint16_t App_KeyInput(uint8_t *pucKeybuf, uint32_t *pudLen)
{
    uint16_t ret;
    uint8_t keyval;
    uint8_t udCnt = 0;
    ret = get_touch_key(&keyval, 10);
	printfS("\r\nInput Password: %c", keyval);
	// printfS("\r\nret: %d, keyval: %c", ret, keyval);

	if (AP_SUCCESS == ret)
	{
        udCnt = strlen((const char*)pucKeybuf);
        printfS("\r\nApp_KeyInput  udCnt = %d",udCnt);
        *(pucKeybuf + udCnt) = keyval;
		if (passwd_state == NO_INPUT)
		{
            if(('#' == pucKeybuf[udCnt])&&(udCnt == 0))
            {
                Reset_InputPassword();
                return AP_WELL_KEY_SHORT;
            }
            passwd_state = INPUT_PROCESS;
		}
		else if(passwd_state == INPUT_PROCESS)
		{
            if('*' == pucKeybuf[udCnt]) /* ??*/
            {
                if(udCnt)/*temp????*/
                {
                    //?*?????????????
//                    DBG("udCnt = %d ", udCnt);
//                    pucKeybuf[udCnt] = 0;
//                    pucKeybuf[--udCnt] = 0;
//                    *pudLen = udCnt;
                    //?*????  //modify by sam 20210309 
                    memset(pucKeybuf,0,17);
                    udCnt =0;
                    *pudLen =0;
                }
				return AP_STAR_KEY_SHORT;
			}
			else if('#' == pucKeybuf[udCnt])
			{
				if(udCnt >= MIN_KEYPWD_LEN)
				{
					passwd_state = INPUT_END;
					Reset_InputPassword();
                    pucKeybuf[udCnt] = '\0';/*??????*/
                    *pudLen = udCnt;
                    Reset_InputPassword();
					return AP_SUCCESS;
				}
				else
				{
					if(udCnt==1)
					{
						if(pucKeybuf[0] == '*')
						{
							Reset_InputPassword();
							return AP_STAR_KEY_LONG;
						}
					}
					else if(udCnt==2)
					{
                        if(pucKeybuf[0]=='1')
                        {
                            Reset_InputPassword();
                            if(pucKeybuf[1]=='9')
                            {
								printfS("\n\rAP_Restore");
                                return AP_Restore;
                            }
                            else if(pucKeybuf[1]=='8')
                            {
								printfS("\n\rAP_REG_ADMIN");
                                return AP_REG_ADMIN;
                            }
                            else if(pucKeybuf[1]=='7')
                            {
								printfS("\n\rAP_DEL_ADMIN");
                                return AP_DEL_ADMIN;
                            }
                            else if(pucKeybuf[1]=='6')
                            {
								printfS("\n\rAP_REG_USER");
                                return AP_REG_USER;
                            }
                            else if(pucKeybuf[1]=='5')
                            {                                
								printfS("\n\rAP_DEL_USER");
                                return AP_DEL_USER;
                            }
                            else if(pucKeybuf[1]=='4')
                            {
								printfS("\n\rAP_RETURN_BOOT");
                                return AP_RETURN_BOOT;
                            }
						}
					}
                    else if(udCnt == 0)
                    {
                        Reset_InputPassword();
						return AP_WELL_KEY_SHORT;
                        // return AP_WELL_KEY_SHORT;
                    }
                    //reset input timeout
                    // input_start_tick = get_ms_tick();
                    Reset_InputPassword();
                    return AP_PWD_LEN_ERROR;
				}
			}
			else
			{

                if(udCnt < MAX_KEYPWD_LEN)
				{

					printfS("\r\nPW: ");
                    for (int i=0; i<udCnt+1; i++)
                    {
                        printfS("%c ", pucKeybuf[i]);
                    }
					printfS("\r\n");
				}
				else
				{
                    memset(pucKeybuf, 0, 17);
                    udCnt =0;
                    *pudLen =0;
                    passwd_state = NO_INPUT;
					return AP_STAR_KEY_SHORT;
				}
			}
		}
	}
	else
	{
	
        if(passwd_state != NO_INPUT)
        {
            if(tick>= INPUT_PASSWD_TIMEOUT)
            {
                Reset_InputPassword();
                return AP_TIME_OUT;
            }
        }
	}
	return AP_CONTINUE;
}

uint32_t App_KeyRegInput(uint8_t *pucKeybuf, uint32_t udType)
{
    uint32_t udRtn = AP_FAIL;
    uint32_t udLen;
    uint16_t udUserNum;
    uint32_t start_tick;
    uint32_t pass_tick; 
    // start_tick = get_ms_tick();
	while (1)
	{
		udRtn = App_KeyInput(pucKeybuf, &udLen);

		if((AP_CONTINUE != udRtn)&& (AP_STAR_KEY_SHORT != udRtn) )
		{
			// DBG(" udLen = %d udRtn = %d ", udLen, udRtn);
			// break;
			return udRtn;
		}
		else
		{
			if(0 < udLen)
			{
				// gfm_get_user_num(&udUserNum,udType);  /* ?????????????*/
				// if(udUserNum >= gfm_get_maxnum(udType))/*???????????*/
				// {
					// return AP_FULL;
					// break;
				// }
			}
		}
		if(AP_STAR_KEY_SHORT == udRtn)
		{
			// start_tick = get_ms_tick();
		}
        // if((g_u8CardBreakFlag)||(g_u8FgpBreakFlag)||(READ_FPS_IRQ_STATUS() == 1)||(g_u8MachineKeyFlag))  
        // {					
        //     return  AP_BREAK;
        // }
        // pass_tick = get_ms_delay(start_tick);
        // if(pass_tick > GLOBAL_TIMEOUT)
        // {
        //     return AP_TIME_OUT;
        // }
	}
	return udRtn;
}

uint32_t App_KeyReg(uint8_t ucAttr)
{
    uint32_t udRtn = AP_FAIL;
    uint16_t udUserId = 0;

    memset(g_u8KeyRegBuf1, 0, sizeof(g_u8KeyRegBuf1));
    memset(g_u8KeyRegBuf2, 0, sizeof(g_u8KeyRegBuf2));

    // if(g_u8Forbid_Touch_flag == 0)     
    // {
	udRtn = App_KeyRegInput(g_u8KeyRegBuf1, ucAttr);
    // }
    // else
    // {
    //     if((g_u8Forbid_Finger_flag ==0) || (g_u8Forbid_Card_flag ==0))
    //         return AP_CONTINUE;
    //     return AP_FULL;
    // }
    printfS("\r\nApp_KeyRegInput udRtn = %02x", udRtn);
    if(AP_SUCCESS == udRtn)
    {
        printfS("\n\rpwd len = %d\r\n",strlen((char *)g_u8KeyRegBuf1));

        printfS("\n\rg_u8KeyRegBuf1=%s",g_u8KeyRegBuf1);

		printfS("\r\nPlease input again.");

        udRtn = App_KeyRegInput(g_u8KeyRegBuf2, ucAttr);
        if(AP_SUCCESS == udRtn)
		{
			printfS("\n\rpwd len = %d\r\n",strlen((char *)g_u8KeyRegBuf2));

			printfS("\n\rg_u8KeyRegBuf2=%s",g_u8KeyRegBuf2);

			if(0 == strcmp((char*)g_u8KeyRegBuf1, (char*)g_u8KeyRegBuf2))//?????? memcmp        
			{
				g_u8KeyRegBuf1[strlen((char*)g_u8KeyRegBuf1)] = '\0';
				
				// udRtn = gfm_verify_data(g_u8KeyRegBuf1,&udUserId,ATTR_PWD_ADMIN,TYPE_STRING);//??????
				if(AP_FAIL == udRtn)
				{//?????????????  
					// udRtn = gfm_verify_data(g_u8KeyRegBuf1,&udUserId,ATTR_PWD_USER,TYPE_BYTE);
				}
				/*
				if(udRtn == AP_SUCCESS)
				{
					printfS("\r\nPassword exist.");
					// Lcd_Center_Display((char *)"??????");
					// Voice_Play(VOICE_PWD_EXIST);
					// Voice_Busy();
					return AP_EXSIT;
				}
				*/
				
				udUserId = 0;
				printfS("\r\nThe password has been saved.");
				// udRtn = gfm_add(g_u8KeyRegBuf1,strlen((char*)g_u8KeyRegBuf1),&udUserId,ucAttr);//?????
				if(AP_SUCCESS == udRtn)
				{ 	
					// Voice_Busy();						
					return AP_SUCCESS;
				}
				else //????
				{
					printfS("Pwd Save Fail;%d \r\n",udRtn);
					// Lcd_Center_Display((char *)"????");
					// Voice_Play(VOICE_FIRE_ALARM);
					// Voice_Busy();                 
					return AP_FAIL;
				}
			}
		}
		else  //???????
		{	
			printfS("\r\nThe two passwords do not match.");
			// Lcd_Center_Display((char *)"???????");            
			// Voice_Play(VOICE_PWD_NOTFIT);
			// Voice_Busy();						
			return AP_FAIL;
		}
	}
	else//???????
	{	
		printfS("\r\nPassword fomats error.");
		// Lcd_Center_Display((char *)"???????");
		// Voice_Play(VOICE_PWD_NOTFIT);
		// Voice_Busy();	
		return AP_FAIL;
	}
	switch(udRtn)/*the first time to deal with the error/interrupt case from key reg*/
	{
		case AP_CONTINUE:
		break;
		case AP_FULL:
			// key_mask(KEY_ALL_OFF);
			Reset_InputPassword();            
			// LED_Light_SetAllStatus(LED_LIGHT_OFF); 
			/*
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
		*/
		break;
		case AP_FAIL:               
			printfS("\n\rTA_FAIL.");           
			// LED_Light_SetAllStatus(LED_LIGHT_OFF);           
			// Voice_Play(VOICE_FIRE_ALARM);
			// Voice_Busy();         
			// LED_Light_SetAllStatus(LED_LIGHT_ON);
			Reset_InputPassword();
		break;
		case AP_PWD_LEN_ERROR:             
			/* ?????? */
			// Tkeyirq_disable();
			// LED_Light_SetAllStatus(LED_LIGHT_OFF);
			// Lcd_Center_Display((char *)"???????");
			// Voice_Play(VOICE_ENTR_FITBIT12);
			// Voice_Busy();
			// LED_Light_SetAllStatus(LED_LIGHT_ON); 
			/* ?????? */
			// Tkeyirq_enable();                    
			Reset_InputPassword();
			memset(g_u8KeyRegBuf1, 0, sizeof(g_u8KeyRegBuf1));
			udRtn = AP_CONTINUE;
		break;
		default:
		
		break;
	}

    return udRtn;
}