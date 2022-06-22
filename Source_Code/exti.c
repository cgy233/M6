#include "exti.h"

extern uint8_t PCD_IRQ_flagA ;

uint8_t flag ;

EXTI_HandleTypeDef EXTI_Line2_Handle;

GPIO_InitTypeDef GPIO_PA2_Handle;

void EXTI_IRQHandler(void)
{
    printfS("\r\nEXTI_IRQHandler");
		PCD_IRQ_flagA = 1;  
    HAL_EXTI_IRQHandler(&EXTI_Line2_Handle);
}

void EXTIX_Init()
{
		System_Module_Enable(EN_EXTI);

    /* Initialization GPIO */
    GPIO_PA2_Handle.Pin       = GPIO_PIN_2;
    GPIO_PA2_Handle.Mode      = GPIO_MODE_INPUT;
    GPIO_PA2_Handle.Pull      = GPIO_PULLUP;
    GPIO_PA2_Handle.Alternate = GPIO_FUNCTION_0;

    HAL_GPIO_Init(GPIOA, &GPIO_PA2_Handle);
    
    /* Config EXTI */
    EXTI_Line2_Handle.u32_Line    = EXTI_LINE_2;
    EXTI_Line2_Handle.u32_Mode    = EXTI_MODE_INTERRUPT;
    EXTI_Line2_Handle.u32_Trigger = EXTI_TRIGGER_FALLING;
    EXTI_Line2_Handle.u32_GPIOSel = EXTI_GPIOA;

    HAL_EXTI_SetConfigLine(&EXTI_Line2_Handle);
		
		printfS("EXTI INIT OK.\n");
    HAL_EXTI_ClearAllPending();
		//printfS("MCU Enter Lowpower, Press User Button To Wakeup MCU!\n");	
    //System_Enter_Stop_Mode(STOPENTRY_WFI);
}