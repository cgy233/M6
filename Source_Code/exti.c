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