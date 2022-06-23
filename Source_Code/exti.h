#ifndef __EXTI_H__
#define __EXTI_H__

#include "ACM32Fxx_HAL.h"

// Si523
//#define IRQ_PORT GPIOA
//#define IRQ_PIN GPIO_PIN_2

//#define IRQ_EXTI_PORT EXTI_GPIOA
//#define IRQ_EXTI_LINE EXTI_LINE_2

// Si14T
#define IRQ_PORT GPIOC
#define IRQ_PIN GPIO_PIN_12

#define IRQ_EXTI_PORT EXTI_GPIOC
#define IRQ_EXTI_LINE EXTI_LINE_12

void EXTIX_Init();
#endif /* __MAIN_H__ */