/*
  ******************************************************************************
  * @file    main.c
  * @brief   main source File.
  ******************************************************************************
*/
#include "APP.h" 

#define UART_BAUD_RATE  115200

UART_HandleTypeDef UART1_Handle;

static uint8_t gu8_UART2Test[] = {"This is UART2 Test Data"};
static uint8_t gu8_UART3Test[] = {"This is UART3 Test Data"};

/************************************************************************
 * function   : Uart_Init
 * Description: Uart Initiation. 
 ************************************************************************/ 
void Uart_Init(void) 
{
    UART1_Handle.Instance        = UART1;    
    UART1_Handle.Init.BaudRate   = UART_BAUD_RATE; 
    UART1_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    UART1_Handle.Init.StopBits   = UART_STOPBITS_1;
    UART1_Handle.Init.Parity     = UART_PARITY_NONE;
    UART1_Handle.Init.Mode       = UART_MODE_TX_RX_DEBUG;
    UART1_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    
    HAL_UART_Init(&UART1_Handle);  
    
    /* UART_DEBUG_ENABLE control printfS */     
    //printfS("MCU is running, HCLK=%dHz, PCLK=%dHz\n", System_Get_SystemClock(), System_Get_APBClock());       
}


/*********************************************************************************
* Function    : main
* Description : 
* Input       : 
* Outpu       : 
* Author      : PJ                        Date : 2021  
**********************************************************************************/
int main(void)
{
    System_Init();
    
    Uart_Init();
		UART2_Init();
		UART3_Init();
    
		/* UARTx Tx */
		//HAL_UART_Transmit(&UART2_Handle, gu8_UART2Test, strlen((char *)gu8_UART2Test),0);
		//HAL_UART_Transmit(&UART3_Handle, gu8_UART3Test, strlen((char *)gu8_UART3Test),0);
	
    /* Select Mode: TEST_LOOP、TEST_UART1_IT、TEST_DMA、TEST_UART_ABORT、TEST_UART2,TEST_UART3*/
		// memset(send_cmd,0,sizeof(send_cmd));  
		// zw_reg(1);
		// for (int i = 0; i < len+9; i++)
		// printfS("%02x ", send_cmd[i]);
		// printfS("\n");
		// HAL_UART_Transmit(&UART2_Handle, send_cmd, len+9,0);
		
		APP_Uart_Test(TEST_UART1_IT);
		while(1)
		{
			
		};
}

