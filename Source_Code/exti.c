#include "exti.h"
#include  "Si14tch.h"
#include  "delay.h"

extern uint8_t PCD_IRQ_flagA ;
extern uint8_t data_buf[4];

uint8_t flag ;

EXTI_HandleTypeDef EXTI_Handle;

GPIO_InitTypeDef GPIO_Handle;

void EXTI_IRQHandler(void)
{
		PCD_IRQ_flagA = 1;
    printfS("\r\nEXTI_IRQHandler");
		HAL_EXTI_IRQHandler(&EXTI_Handle);
		EXTI->IENR &=  ~IRQ_EXTI_LINE;
		SI12_ReadData();   // ??Output??
		 
		 if( (data_buf[0] & 0x03)  != 0 )   flag = 1;
		 if( ((data_buf[0] >>2) & 0x03) != 0 ) flag = 2;
		 if( ((data_buf[0] >>4) & 0x03) != 0 ) flag = 3;
		 if( ((data_buf[0] >>6) & 0x03) != 0 ) flag = 4;
		 
		 
		 if( (data_buf[1] & 0x03) != 0 )      flag = 5;
		 if( ((data_buf[1]>>2) & 0x03) != 0 ) flag = 6;  
		 if( ((data_buf[1]>>4) & 0x03) != 0 ) flag = 7; 
		 if( ((data_buf[1]>>6) & 0x03) != 0 ) flag = 8;  
		 
		 if( (data_buf[2] & 0x03) != 0 )      flag = 9;  
		 if( ((data_buf[2]>>2) & 0x03) != 0 ) flag = 10; 
		 if( ((data_buf[2]>>4) & 0x03) != 0 ) flag = 11; 
		 if( ((data_buf[2]>>6) & 0x03) != 0 ) flag = 12;
		 EXTI->IENR |=  IRQ_EXTI_LINE;
}

void EXTIX_Init()
{
		System_Module_Enable(EN_EXTI);

    /* Initialization GPIO */
    GPIO_Handle.Pin       = IRQ_PIN;
    GPIO_Handle.Mode      = GPIO_MODE_INPUT;
    GPIO_Handle.Pull      = GPIO_PULLUP;
    GPIO_Handle.Alternate = GPIO_FUNCTION_0;
		
    HAL_GPIO_Init(IRQ_PORT, &GPIO_Handle);
		HAL_GPIO_WritePin(IRQ_PORT, IRQ_PIN, GPIO_PIN_CLEAR);
    
    /* Config EXTI */
    EXTI_Handle.u32_Line    = IRQ_EXTI_LINE;
    EXTI_Handle.u32_Mode    = EXTI_MODE_INTERRUPT;
    EXTI_Handle.u32_Trigger = EXTI_TRIGGER_FALLING;
    EXTI_Handle.u32_GPIOSel = IRQ_EXTI_PORT;

    HAL_EXTI_SetConfigLine(&EXTI_Handle);
		
		printfS("\n\rEXTI INIT OK.");
    HAL_EXTI_ClearAllPending();
		//printfS("MCU Enter Lowpower, Press User Button To Wakeup MCU!\n");	
    //System_Enter_Stop_Mode(STOPENTRY_WFI);
}