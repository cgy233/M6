/*
  ******************************************************************************
  * @file    APP.c
  * @author  Ellsion
  * @version V1.0.0
  * @date    2021
  * @brief   Muti-UART and Receive_IT demo source code.
  ******************************************************************************        
*/
#include "APP.h"

uint8_t gu8_TxBuffer[512];
uint8_t gu8_RxBuffer[512];


uint8_t gu8_Note[] = {"This is interrupt Mode"};

extern UART_HandleTypeDef  UART1_Handle;
UART_HandleTypeDef  UART2_Handle;
UART_HandleTypeDef  UART3_Handle;
 
DMA_HandleTypeDef  DMA_CH1_Handle;

volatile uint32_t gu32_DMA_Status = false;

volatile uint32_t gu32_TxCpltStatus = false,gu32_TxCpltStatus2 =false,gu32_TxCpltStatus3 =false;

volatile uint32_t gu32_RxCpltStatus = false,gu32_RxCpltStatus2 = false,gu32_RxCpltStatus3 = false;


uint8_t g_send_buff[32];


uint8_t packing(uint8_t *data, uint8_t data_len)
{
    uint16_t check_sum = 0;
    // header
    g_send_buff[0] = 0xef;
    g_send_buff[1] = 0x01;
    // address
    g_send_buff[2] = 0xff;
    g_send_buff[3] = 0xff;
    g_send_buff[4] = 0xff;
    g_send_buff[5] = 0xff;
    // data
    if (0 != data)
    {
        memcpy(g_send_buff + 6, data, data_len);
    }
    for (int i = 0; i < data_len; i++)
    {
        check_sum += data[i];
    }
    if (check_sum > 0xFFFF)
    {
        g_send_buff[data_len + 6] = 0xFF;
        g_send_buff[data_len + 7] = 0xFF;
    }
    else
    {
	    g_send_buff[data_len + 6] = (uint8_t)((check_sum & 0xFF00) >> 8);
	    g_send_buff[data_len + 7] = (uint8_t)(check_sum & 0x00FF);
    }
    return data_len + 8;
}


uint8_t PS_AutoEnroll(uint16_t id, uint8_t count, uint16_t param)
{
    uint8_t data[9];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x08;
    data[3] = 0x31;
    data[4] = (uint8_t)((id & 0xFF00) >> 8);
    data[5] = (uint8_t)(id & 0x00FF);
    data[6] = count;
    data[7] = (uint8_t)((param & 0xFF00) >> 8);
    data[8] = (uint8_t)(param & 0x00FF);
    return packing(data, 9);
}

uint8_t PS_AutoIdentify(uint8_t safety_level, uint16_t id, uint16_t param)
{
    uint8_t data[9];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x08;
    data[3] = 0x32;
    data[4] = safety_level;
    data[5] = (uint8_t)((id & 0xFF00) >> 8);
    data[6] = (uint8_t)(id & 0x00FF);
    data[7] = (uint8_t)((param & 0xFF00) >> 8);
    data[8] = (uint8_t)(param & 0x00FF);
    return packing(data, 9);

}

uint8_t PS_DeletChar(uint16_t page_id, uint16_t n)
{
    uint8_t data[9];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x0c;
    data[5] = (uint8_t)((page_id & 0xFF00) >> 8);
    data[6] = (uint8_t)(page_id & 0x00FF);
    data[7] = (uint8_t)((n & 0xFF00) >> 8);
    data[8] = (uint8_t)(n & 0x00FF);

    return packing(data, 9);
}

uint8_t PS_Empty()
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x0d};

    return packing(data, 4);
}

uint8_t PS_Cancel()
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x30};

    return packing(data, 4);
}

uint8_t PS_Sleep()
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x33};

    return packing(data, 4);
}

uint8_t PS_ValidTempleteNum()
{
    uint8_t data[] = {0x01, 0x00, 0x03, 0x1d};

    return packing(data, 4);
}

uint8_t PS_SetPwd(uint32_t passwd)
{
    uint8_t data[9];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x12;
    data[5] = (uint8_t)((passwd & 0xFF000000) >> 24);
    data[6] = (uint8_t)((passwd & 0x00FF0000) >> 16);
    data[7] = (uint8_t)((passwd  & 0x0000FF00) >> 8);
    data[8] = (uint8_t)(passwd & 0x000000FF);

    return packing(data, 9);
}

uint8_t PS_VfyPwd(uint32_t passwd)
{
    uint8_t data[9];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x12;
    data[5] = (uint8_t)((passwd & 0xFF000000) >> 24);
    data[6] = (uint8_t)((passwd & 0x00FF0000) >> 16);
    data[7] = (uint8_t)((passwd  & 0x0000FF00) >> 8);
    data[8] = (uint8_t)(passwd & 0x000000FF);

    return packing(data, 9);
}

uint8_t PS_AuraLedConfig(uint8_t cmd, uint8_t speed, uint8_t color, uint8_t count)
{
    uint8_t data[8];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0x07;
    data[3] = 0x3c;
    data[4] = cmd;
    data[5] = speed;
    data[6] = color;
    data[7] = count;
    return packing(data, 8);
}

uint8_t send_cmd[52],cmd,bs,len,ply[20];

//指纹包
void send_cmdToZw(uint8_t *send_buf,uint8_t bs,uint16_t len,uint8_t cmd,uint8_t *ply)
{
	uint8_t i=0;
	uint8_t con=0;
	uint8_t arr[50];
	uint8_t n=0;
	uint16_t sum=0;
	send_buf[i++]=0xEF;
	send_buf[i++]=0x01;
	//芯片地址
	send_buf[i++]=0xFF;
	send_buf[i++]=0xFF;
	send_buf[i++]=0xFF;
	send_buf[i++]=0xFF;
	// 包标识
	send_buf[i++]=bs;
	sum=sum+bs;
	send_buf[i++]=((len) & 0xff00)>>8;
    // 包长度
	send_buf[i++]=len &0x00ff;
	sum=sum+len;
    // 指令码
	send_buf[i++]=cmd;
	sum=sum+cmd;
	for(con=0;con<len-3;con++)
	{
		send_buf[i++]=ply[con];
		sum=sum+ply[con];
	}
	arr[0]=len+2;
	arr[1]=cmd;
	for(n=0;n<len-3;n++)
	{
		arr[n+2]=ply[n];
	}
	send_buf[i++]=(uint8_t)((sum & 0xFF00)>>8);
	send_buf[i++]=(uint8_t)(sum & 0x00FF);
}


void zw_val(uint8_t i)
{
	bs=0x01;
	len=0x08;
	cmd=0x32;
	ply[0]=0x01;
	ply[1]=0x00;
	ply[2]=i;
	ply[3]=0x00;
	ply[4]=0x01;
	send_cmdToZw(send_cmd,bs,len,cmd,ply);
}

void zw_reg(uint8_t n)
{
	bs=0x01;
	len=0x08;
	cmd=0x31;
	ply[0]=0x00;
	ply[1]=n;
	ply[2]=0x03;
	ply[3]=0x00;
	ply[4]=0x0F;
	send_cmdToZw(send_cmd,bs,len,cmd,ply);
}

void zw_del()
{
	bs=0x01;
	len=0x07;
	cmd=0x0c;
	ply[0]=0x00;
	ply[1]=0x01;
	ply[2]=0x00;
	ply[3]=0x01;
	send_cmdToZw(send_cmd,bs,len,cmd,ply);
}

void zw_count()
{
	bs=0x01;
	len=0x03;
	cmd=0x1d;
	ply[0]=0x00;
	ply[1]=0x01;
	ply[2]=0x00;
	ply[3]=0x01;
	send_cmdToZw(send_cmd,bs,len,cmd,ply);
}

/*********************************************************************************
* Function    : UART1_IRQHandler
* Description : UAAR1 Interrupt handler
* Input       : 
* Outpu       : 
* Author      : Chris_Kyle                         Data : 2020年
**********************************************************************************/
void UART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART1_Handle);
}

/*********************************************************************************
* Function    : UART2_IRQHandler
* Description : UART2 Interrupt handler
* Input       : None
* Output      : None
**********************************************************************************/
void UART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART2_Handle);
}

/*********************************************************************************
* Function    : UART1_IRQHandler
* Description : UAAR1 Interrupt handler
* Input       : None
* Output      : NOne
**********************************************************************************/
void UART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART3_Handle);
}

/*********************************************************************************
* Function    : UART2 Init function
* Description : UART3 Init function
* Input       : None
* Output      : None
**********************************************************************************/
void UART2_Init(void) 
{
    UART2_Handle.Instance        = UART2;    
    UART2_Handle.Init.BaudRate   = 57600; 
    UART2_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    UART2_Handle.Init.StopBits   = UART_STOPBITS_2;
    UART2_Handle.Init.Parity     = UART_PARITY_NONE;
    UART2_Handle.Init.Mode       = UART_MODE_TX_RX;
    UART2_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    
    HAL_UART_Init(&UART2_Handle);               
}

/*********************************************************************************
* Function    : UART2 Init function
* Description : UART3 Init function
* Input       : None
* Output      : NOne
**********************************************************************************/
void UART3_Init(void) 
{
    UART3_Handle.Instance        = UART3;    
    UART3_Handle.Init.BaudRate   = 9600; 
    UART3_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    UART3_Handle.Init.StopBits   = UART_STOPBITS_1;
    UART3_Handle.Init.Parity     = UART_PARITY_NONE;
    UART3_Handle.Init.Mode       = UART_MODE_TX_RX;
    UART3_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    
    HAL_UART_Init(&UART3_Handle);               
}

/*********************************************************************************
* Function    : HAL_UART_MspInit
* Description : Initialize the UART MSP.
* Input       : huart: UART handle.
* Output      : 
**********************************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{

    GPIO_InitTypeDef    GPIO_InitStruct ={0};
    
    if (huart->Instance == UART1) 
    {
        /* Enable Clock */
        System_Module_Enable(EN_UART1);
       
        /* Initialization GPIO */
        /* A9:Tx  A10:Rx */
        GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Alternate = GPIO_FUNCTION_2;       
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
        /* NVIC Config */
        NVIC_ClearPendingIRQ(UART1_IRQn);
        NVIC_SetPriority(UART1_IRQn, (1U << __NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(UART1_IRQn);
    }
	else if (huart->Instance == UART2) 
    {
        /* Enable Clock */
        System_Module_Enable(EN_UART2);
       
        /* Initialization GPIO */
        /* A2:Tx  A3:Rx */
        GPIO_InitStruct.Pin       = GPIO_PIN_2|GPIO_PIN_3;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Alternate = GPIO_FUNCTION_2;       
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  
		      			
        /* NVIC Config */
        NVIC_ClearPendingIRQ(UART2_IRQn);
        NVIC_SetPriority(UART2_IRQn, (1U << __NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(UART2_IRQn);
    }
	else if (huart->Instance == UART3) 
    {
        /* Enable Clock */
        System_Module_Enable(EN_UART3);
       
        /* Initialization GPIO */
        /* B10:Tx  B11:Rx */
        GPIO_InitStruct.Pin       = GPIO_PIN_10|GPIO_PIN_11;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Alternate = GPIO_FUNCTION_2;       
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  
		
        /* NVIC Config */
        NVIC_ClearPendingIRQ(UART3_IRQn);
        NVIC_SetPriority(UART3_IRQn, (1U << __NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(UART3_IRQn);
    }
}

/*********************************************************************************
* Function    : DMA_IRQHandler
* Description : DMA Interrupt handler
* Input       : 
* Outpu       : 
* Author      : Chris_Kyle                         Data : 2020年
**********************************************************************************/
void DMA_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&DMA_CH1_Handle);
}

/*********************************************************************************
* Function    : HAL_UART_TxCpltCallback
* Description : Tx Transfer completed callbacks.
* Input       : 
* Outpu       : 
* Author      : PJ                         Data : 2021
**********************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance ==UART1)  // Transfor Data By UART1
    {
		gu32_TxCpltStatus= true;
    }
    else if(huart->Instance ==UART2) //Transfor Data By UART2 
	{
		gu32_TxCpltStatus2 =true;
	}
	else if(huart->Instance ==UART3) // Transfor Data By UART3
	{
		gu32_TxCpltStatus3 =true;
	}
}

/*********************************************************************************
* Function    : UART1_DMA_ITC_Callback
* Description : 
* Input       : 
* Outpu       : 
* Author      : PJ                       Data : 2021
**********************************************************************************/
void UART1_DMA_ITC_Callback(void)
{
    gu32_DMA_Status = true;
}

/*********************************************************************************
* Function    : HAL_UART_RxCpltCallback
* Description : Rx Receive completed callbacks.
* Input       : None
* Outpu       : None
**********************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
    if(huart->Instance ==UART1)  // receive data by UART1
    {
		gu32_RxCpltStatus= true;
    }
    else if(huart->Instance ==UART2) //receive data by UART2  
	{
		gu32_RxCpltStatus2 =true;
	}
	else if(huart->Instance ==UART3) // receive data by UART3  
	{
		gu32_RxCpltStatus3 =true;
	}
}


/*********************************************************************************
* Function    : APP_Uart_Test
* Description : 
* Input       : 
* Outpu       : 
* Author      : PJ                       Data : 2021年
**********************************************************************************/
void APP_Uart_Test(enum_TEST_MODE_t fe_Mode)
{
    printfS("\r\n---------- UART Test ----------\r\n");
    // printfS("Please enter any String/Data \r\n");

	uint8_t lu8_ret;
	uint8_t pack_len;
    
    switch (fe_Mode)
    {
        /* 循环模式 */
        case TEST_LOOP: 
        {
            for(;;)
            {
                HAL_UART_Receive(&UART1_Handle, gu8_RxBuffer, sizeof(gu8_RxBuffer), 200);

                HAL_UART_Transmit(&UART1_Handle, gu8_RxBuffer, UART1_Handle.lu32_RxCount, 0);  

                memset(gu8_RxBuffer, 0, sizeof(gu8_RxBuffer)); 
            }
        }break;

        /* 中断模式 */
        case TEST_UART1_IT: 
        {
            while(1)
            {
                /*receive variable length data by intrrupt method*/
                HAL_UART_Receive_IT(&UART1_Handle, gu8_RxBuffer, sizeof(gu8_RxBuffer), UART_RX_FIFO_1_2);    // length should <= sizeof(gu8_RxBuffer)   
						
                while(1)  
                {
                    while (!gu32_RxCpltStatus);
                
                    gu32_RxCpltStatus=false;
                
                    memset(send_cmd,0,sizeof(send_cmd));  
                    memcpy(gu8_TxBuffer, gu8_RxBuffer, UART1_Handle.lu32_RxCount);
                    printfS("Receive data: %s\n", gu8_RxBuffer);
                    if(strstr((char *)gu8_RxBuffer,"1"))
                    {
                        pack_len = PS_AutoEnroll(0x1, 0x03, 0x000F);
                    }
                    else if(strstr((char *)gu8_RxBuffer,"2"))
                    {
                        pack_len = PS_AutoIdentify(1, 0xFFFF, 0x0001);
                    }
                    else if(strstr((char *)gu8_RxBuffer,"3"))
                    {
                        pack_len = PS_Empty();
                    }
                    else if(strstr((char *)gu8_RxBuffer,"4"))
                    {
                        pack_len = PS_ValidTempleteNum();
                    }
                    else if(strstr((char *)gu8_RxBuffer,"5"))
                    {
                        pack_len = PS_AuraLedConfig(0x04, 0, 0, 0);
                        System_Delay_MS(500);
                        pack_len = PS_AuraLedConfig(0x01, 0xF0, 0x01, 0xF0);
                    }
                    for (int i = 0; i < pack_len; i++)
                    printfS("%02X ", g_send_buff[i]);
                    printfS("\n");
                    printfS("**********************************************\n");
                    HAL_UART_Transmit(&UART2_Handle, g_send_buff, pack_len, 0);
                
                    HAL_UART_Transmit_IT(&UART1_Handle, gu8_TxBuffer, UART1_Handle.lu32_RxCount, UART_TX_FIFO_1_2); 
                
                    while (!gu32_TxCpltStatus);  
				
                    gu32_TxCpltStatus =false;
				
                    break;
           
                }
            }
        }break;
        
        /* DMA模式 */
        case TEST_DMA: 
        {
            DMA_CH1_Handle.Instance              = DMA_Channel0;
            DMA_CH1_Handle.Init.Data_Flow        = DMA_DATA_FLOW_M2P;
            DMA_CH1_Handle.Init.Request_ID       = REQ5_UART1_SEND;
            DMA_CH1_Handle.Init.Source_Inc       = DMA_SOURCE_ADDR_INCREASE_ENABLE;
            DMA_CH1_Handle.Init.Desination_Inc   = DMA_DST_ADDR_INCREASE_DISABLE;
            DMA_CH1_Handle.Init.Source_Width     = DMA_SRC_WIDTH_BYTE;
            DMA_CH1_Handle.Init.Desination_Width = DMA_DST_WIDTH_BYTE;
            
            DMA_CH1_Handle.DMA_ITC_Callback = UART1_DMA_ITC_Callback;
            
            HAL_DMA_Init(&DMA_CH1_Handle);
            
            __HAL_LINK_DMA(UART1_Handle, HDMA_Tx, DMA_CH1_Handle);
            
            
            gu32_DMA_Status = true;
            
            for(;;)
            {
                if (gu32_DMA_Status) 
                {
                    gu32_DMA_Status = false;
                    
                    memset(gu8_RxBuffer, 0, 512);
                }
                
                lu8_ret=HAL_UART_Receive(&UART1_Handle, gu8_RxBuffer, sizeof(gu8_RxBuffer), 100);

                lu8_ret=HAL_UART_Transmit_DMA(&UART1_Handle, gu8_RxBuffer, UART1_Handle.lu32_RxCount);

            }
        }break;
		case TEST_UART_ABORT:
    	{
			 volatile uint8_t gu8_TestAbortCnt;
			 
			 memset(gu8_TxBuffer, 0, 200);

			 for(gu8_TestAbortCnt=0;gu8_TestAbortCnt<100;gu8_TestAbortCnt++)
			 {
				gu8_TxBuffer[gu8_TestAbortCnt] =gu8_TestAbortCnt;

				HAL_UART_Transmit(&UART2_Handle,&gu8_TxBuffer[gu8_TestAbortCnt],1,0);

				if(gu8_TestAbortCnt>=50)
				{
					if(HAL_UART_Abort(&UART2_Handle)==HAL_OK)
					{
						printfS("UART2 Abort Ok\r\n");
						
						break;
					}
				}
			 }
		}break;
		case TEST_UART2:
		{
			while(1)
			{
				HAL_UART_Receive_IT(&UART2_Handle, gu8_RxBuffer, sizeof(gu8_RxBuffer), UART_RX_FIFO_1_2);    // length should <= sizeof(gu8_RxBuffer)   
						
				while(1)  
				{
					while (!gu32_RxCpltStatus2);
				
					gu32_RxCpltStatus2=false;
				
					memcpy(gu8_TxBuffer, gu8_RxBuffer, UART2_Handle.lu32_RxCount);
				
					HAL_UART_Transmit_IT(&UART2_Handle, gu8_TxBuffer, UART2_Handle.lu32_RxCount, UART_TX_FIFO_1_2); 
				
					while (!gu32_TxCpltStatus2);  
				
					gu32_TxCpltStatus2 =false;
				
					break;		   
				}
			}
		}break;
		case TEST_UART3:
		{
			while(1)
			{
				HAL_UART_Receive_IT(&UART3_Handle, gu8_RxBuffer, sizeof(gu8_RxBuffer), UART_RX_FIFO_1_2);    // length should <= sizeof(gu8_RxBuffer)   
						
				while(1)  
				{
					while (!gu32_RxCpltStatus3);
				
					gu32_RxCpltStatus3=false;
				
					memcpy(gu8_TxBuffer, gu8_RxBuffer, UART3_Handle.lu32_RxCount);
				
					HAL_UART_Transmit_IT(&UART3_Handle, gu8_TxBuffer, UART3_Handle.lu32_RxCount, UART_TX_FIFO_1_2);    
				
					while (!gu32_TxCpltStatus3);  
				
					gu32_TxCpltStatus3 =false;
				
					break;		   
				}
			}
		}break;

        default: break; 
    }
}
