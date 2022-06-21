/*
  ******************************************************************************
  * @file    main.c
  * @brief   main source File.
  ******************************************************************************
*/
#include "APP.h" 
#include "nfc_iic.h"
#include "delay.h"
#include "exti.h"
#include "SI523_App.h"

#include "finger.h"

uint8_t  PCD_IRQ_flagA = 0;

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
	
  printfS("\n\rAPP START.");
	/* UARTx Tx */
	//HAL_UART_Transmit(&UART2_Handle, gu8_UART2Test, strlen((char *)gu8_UART2Test),0);
	//HAL_UART_Transmit(&UART3_Handle, gu8_UART3Test, strlen((char *)gu8_UART3Test),0);
	
    /* Select Mode: TEST_LOOP、TEST_UART1_IT、TEST_DMA、TEST_UART_ABORT、TEST_UART2,TEST_UART3*/
	APP_Uart_Test(TEST_UART1_IT);

	//EXTIX_Init();
	//ACD_init_Fun();
	//ACD_Fun();
	// PCD_SI523_TypeA_Init();
	// PCD_SI523_TypeA();
	
	//SI523_I2C_LL_Init();
	while(1)
	{
		uint8_t k = 0x6c;
		printfS("\n\r********wirte:%02x*******", k);
		I_SI523_IO_Write(ACDConfigSelReg, (ACDConfigK << 2) | 0x40);
		I_SI523_IO_Write(ACDConfigReg, k );
		printfS("\n\r********read********");
		char version = I_SI523_IO_Read(ACDConfigReg);
		printfS("\n\rACDConfigReg: %02x", version);
		//IIC_Send_Byte(0xff);
		delay_ms(500);
	};
}

