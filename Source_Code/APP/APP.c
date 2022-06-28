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
#include "finger.h"

uint8_t guint8_t_TxBuffer[512];
uint8_t guint8_t_RxBuffer[512];
uint8_t guint8_t_TxBufferF[512];
uint8_t guint8_t_RxBufferF[512];

uint8_t guint8_t_Note[] = {"This is interrupt Mode"};

extern UART_HandleTypeDef  UART1_Handle;
UART_HandleTypeDef  UART2_Handle;
UART_HandleTypeDef  UART3_Handle;
 
DMA_HandleTypeDef  DMA_CH1_Handle;

volatile uint32_t guint32_t_DMA_Status = false;

volatile uint32_t guint32_t_TxCpltStatus = false,guint32_t_TxCpltStatus2 =false,guint32_t_TxCpltStatus3 =false;

volatile uint32_t guint32_t_RxCpltStatus = false,guint32_t_RxCpltStatus2 = false,guint32_t_RxCpltStatus3 = false;


uint8_t g_send_buff[32];

/*********************************************************************************
* Function    : UART1_IRQHandler
* Description : UAAR1 Interrupt handler
* Input       : 
* Outpu       : 
* Author      : Chris_Kyle                         Data : 2020?
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
    UART1_Handle.Instance        = UART1;    
    UART1_Handle.Init.BaudRate   = 57600; 
    UART1_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    UART1_Handle.Init.StopBits   = UART_STOPBITS_2;
    UART1_Handle.Init.Parity     = UART_PARITY_NONE;
    UART1_Handle.Init.Mode       = UART_MODE_TX_RX;
    UART1_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    
    HAL_UART_Init(&UART1_Handle);               
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
    UART3_Handle.Init.BaudRate   = 57600; 
    UART3_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    UART3_Handle.Init.StopBits   = UART_STOPBITS_2;
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
        GPIO_InitStruct.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Alternate = GPIO_FUNCTION_1;       
				HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
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
* Author      : Chris_Kyle                         Data : 2020�?
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
		guint32_t_TxCpltStatus= true;
    }
    else if(huart->Instance ==UART2) //Transfor Data By UART2 
	{
		guint32_t_TxCpltStatus2 =true;
	}
	else if(huart->Instance ==UART3) // Transfor Data By UART3
	{
		guint32_t_TxCpltStatus3 =true;
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
    guint32_t_DMA_Status = true;
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
		guint32_t_RxCpltStatus= true;
    }
    else if(huart->Instance ==UART2) //receive data by UART2  
	{
		guint32_t_RxCpltStatus2 =true;
	}
	else if(huart->Instance ==UART3) // receive data by UART3  
	{
		guint32_t_RxCpltStatus3 =true;
	}
}


/*********************************************************************************
* Function    : APP_Uart_Test
* Description : 
* Input       : 
* Outpu       : 
* Author      : PJ                       Data : 2021�?
**********************************************************************************/
void APP_Uart_Test(enum_TEST_MODE_t fe_Mode)
{
    printfS("\r\nUART Test.");
		printfS("\r\n**********************************************");
		printfS("\r\nPlease enter Finger command:");

		uint8_t luint8_t_ret;
		uint8_t pack_len;
    
    switch (fe_Mode)
    {
        /* 循环模式 */
        case TEST_LOOP: 
        {
            for(;;)
            {
                HAL_UART_Receive(&UART1_Handle, guint8_t_RxBuffer, sizeof(guint8_t_RxBuffer), 200);

                HAL_UART_Transmit(&UART1_Handle, guint8_t_RxBuffer, UART1_Handle.lu32_RxCount, 0);  

                memset(guint8_t_RxBuffer, 0, sizeof(guint8_t_RxBuffer)); 
            }
        }break;

        /* 中断模式 */
        case TEST_UART1_IT: 
        {
            while(1)
            {
                /*receive variable length data by intrrupt method*/
                HAL_UART_Receive_IT(&UART1_Handle, guint8_t_RxBuffer, sizeof(guint8_t_RxBuffer), UART_RX_FIFO_1_2);    // length should <= sizeof(guint8_t_RxBuffer)   
                HAL_UART_Receive_IT(&UART2_Handle, guint8_t_RxBufferF, sizeof(guint8_t_RxBufferF), UART_RX_FIFO_1_2);    // length should <= sizeof(guint8_t_RxBuffer)   
						
                while(1)  
                {
                    if (guint32_t_RxCpltStatus2){
                
                        guint32_t_RxCpltStatus2=false;
                        // memcpy(guint8_t_TxBuffer, guint8_t_RxBuffer, UART1_Handle.luint32_t_RxCount);
                        printfS("\r\nReceive cmd: %s", guint8_t_RxBufferF);
                        if(strstr((char *)guint8_t_RxBufferF,"1"))
                        {
                            pack_len = PS_AutoEnroll(g_send_buff, 0x0001, 0x03, 0x000F);
                        }
                        else if(strstr((char *)guint8_t_RxBufferF,"2"))
                        {
                            pack_len = PS_AutoIdentify(g_send_buff, 0x05, 0xFFFF, 0x0001);
                        }
                        else if(strstr((char *)guint8_t_RxBufferF,"3"))
                        {
                            pack_len = PS_Empty(g_send_buff);
                        }
                        else if(strstr((char *)guint8_t_RxBufferF,"4"))
                        {
                            pack_len = PS_ValidTempleteNum(g_send_buff);
                        }
                        else if(strstr((char *)guint8_t_RxBufferF,"5"))
                        {
                            pack_len = PS_AuraLedConfig(g_send_buff, 0x04, 0, 0, 0);
                            System_Delay_MS(200);
                            pack_len = PS_AuraLedConfig(g_send_buff, 0x01, 0x6F, 0x02, 0x00);
                        }
                        printfS("\r\nPackege:");
                        for (int i = 0; i < pack_len; i++)
                        printfS("0x%02X, ", g_send_buff[i]);
                                            
                        HAL_UART_Transmit(&UART1_Handle, g_send_buff, pack_len, 0);
                        memset(g_send_buff, 0, sizeof(g_send_buff));
                        memset(guint8_t_RxBufferF, 0, sizeof(guint8_t_RxBufferF));
                        System_Delay_MS(20);
                        break;

                        // HAL_UART_Transmit_IT(&UART1_Handle, guint8_t_TxBuffer, UART1_Handle.luint32_t_RxCount, UART_TX_FIFO_1_2);
                                            
                        // while (!guint32_t_TxCpltStatus);  
                        // guint32_t_TxCpltStatus =false;
                        // break;
                    }
                    if (guint32_t_RxCpltStatus)
                    {
                        guint32_t_RxCpltStatus = false;
                        printfS("\r\nFinger Result:");
                        for (int i = 0; i < UART1_Handle.lu32_RxCount; i++)
                        {
                            printfS("%02X ", guint8_t_RxBuffer[i]);
                        }
                        memset(guint8_t_RxBuffer, 0, sizeof(guint8_t_RxBuffer));
                        System_Delay_MS(20);
                        break;
                    }
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
            
            
            guint32_t_DMA_Status = true;
            
            for(;;)
            {
                if (guint32_t_DMA_Status) 
                {
                    guint32_t_DMA_Status = false;
                    
                    memset(guint8_t_RxBuffer, 0, 512);
                }
                
                luint8_t_ret=HAL_UART_Receive(&UART1_Handle, guint8_t_RxBuffer, sizeof(guint8_t_RxBuffer), 100);

                luint8_t_ret=HAL_UART_Transmit_DMA(&UART1_Handle, guint8_t_RxBuffer, UART1_Handle.lu32_RxCount);

            }
        }break;
		case TEST_UART_ABORT:
    	{
			 volatile uint8_t guint8_t_TestAbortCnt;
			 
			 memset(guint8_t_TxBuffer, 0, 200);

			 for(guint8_t_TestAbortCnt=0;guint8_t_TestAbortCnt<100;guint8_t_TestAbortCnt++)
			 {
				guint8_t_TxBuffer[guint8_t_TestAbortCnt] =guint8_t_TestAbortCnt;

				HAL_UART_Transmit(&UART2_Handle,&guint8_t_TxBuffer[guint8_t_TestAbortCnt],1,0);

				if(guint8_t_TestAbortCnt>=50)
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
				HAL_UART_Receive_IT(&UART2_Handle, guint8_t_RxBuffer, sizeof(guint8_t_RxBuffer), UART_RX_FIFO_1_2);    // length should <= sizeof(guint8_t_RxBuffer)   
						
				while(1)  
				{
					while (!guint32_t_RxCpltStatus2);
				
					guint32_t_RxCpltStatus2=false;
				
					memcpy(guint8_t_TxBuffer, guint8_t_RxBuffer, UART2_Handle.lu32_RxCount);
				
					HAL_UART_Transmit_IT(&UART2_Handle, guint8_t_TxBuffer, UART2_Handle.lu32_RxCount, UART_TX_FIFO_1_2); 
				
					while (!guint32_t_TxCpltStatus2);  
				
					guint32_t_TxCpltStatus2 =false;
				
					break;		   
				}
			}
		}break;
		case TEST_UART3:
		{
			while(1)
			{
				HAL_UART_Receive_IT(&UART3_Handle, guint8_t_RxBuffer, sizeof(guint8_t_RxBuffer), UART_RX_FIFO_1_2);    // length should <= sizeof(guint8_t_RxBuffer)   
						
				while(1)  
				{
					while (!guint32_t_RxCpltStatus3);
				
					guint32_t_RxCpltStatus3=false;
				
					memcpy(guint8_t_TxBuffer, guint8_t_RxBuffer, UART3_Handle.lu32_RxCount);
				
					HAL_UART_Transmit_IT(&UART3_Handle, guint8_t_TxBuffer, UART3_Handle.lu32_RxCount, UART_TX_FIFO_1_2);    
				
					while (!guint32_t_TxCpltStatus3);  
				
					guint32_t_TxCpltStatus3 =false;
				
					break;		   
				}
			}
		}break;

        default: break; 
    }
}
