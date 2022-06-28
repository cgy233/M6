#ifndef _AC_GPIO_H_
#define _AC_GPIO_H_

#include "ac_common.h"

typedef enum {
    GPIOA0,  // 0x00
    GPIOA1,  // 0x01
    GPIOA2,  // 0x02
    GPIOA3,  // 0x03
    GPIOA4,  // 0x04
    GPIOA5,  // 0x05
    GPIOA6,  // 0x06
    GPIOA7,  // 0x07
    GPIOA8,  // 0x08
    GPIOA9,  // 0x09
    GPIOA10, // 0x0A
    GPIOA11, // 0x0B
    GPIOA12, // 0x0C
    GPIOA13, // 0x0D
    GPIOA14, // 0x0E
    GPIOA15, // 0x0F

    GPIOB0,  // 0x10
    GPIOB1,  // 0x11
    GPIOB2,  // 0x12
    GPIOB3,  // 0x13
    GPIOB4,  // 0x14
    GPIOB5,  // 0x15
    GPIOB6,  // 0x16
    GPIOB7,  // 0x17
    GPIOB8,  // 0x18
    GPIOB9,  // 0x19
    GPIOB10, // 0x1A
    GPIOB11, // 0x1B
    GPIOB12, // 0x1C
    GPIOB13, // 0x1D
    GPIOB14, // 0x1E
    GPIOB15, // 0x1F

    GPIOC0,  // 0x20
    GPIOC1,  // 0x21
    GPIOC2,  // 0x22
    GPIOC3,  // 0x23
    GPIOC4,  // 0x24
    GPIOC5,  // 0x25
    GPIOC6,  // 0x26
    GPIOC7,  // 0x27
    GPIOC8,  // 0x28
    GPIOC9,  // 0x29
    GPIOC10, // 0x2A
    GPIOC11, // 0x2B
    GPIOC12, // 0x2C
    GPIOC13, // 0x2D
    GPIOC14, // 0x2E
    GPIOC15, // 0x2F

    GPIOD0,  // 0x30
    GPIOD1,  // 0x31
    GPIOD2,  // 0x32
    GPIOD3,  // 0x33
    GPIOD4,  // 0x34
    GPIOD5,  // 0x35
    GPIOD6,  // 0x36
    GPIOD7,  // 0x37
    GPIOD8,  // 0x38
    GPIOD9,  // 0x39
    GPIOD10, // 0x3A
    GPIOD11, // 0x3B
    GPIOD12, // 0x3C
    GPIOD13, // 0x3D
    GPIOD14, // 0x3E
    GPIOD15, // 0x3F

    GPIOE0,  // 0x40
    GPIOE1,  // 0x41
    GPIOE2,  // 0x42
    GPIOE3,  // 0x43
    GPIOE4,  // 0x44
    GPIOE5,  // 0x45
    GPIOE6,  // 0x46
    GPIOE7,  // 0x47
    GPIOE8,  // 0x48
    GPIOE9,  // 0x49
    GPIOE10, // 0x4A
    GPIOE11, // 0x4B
    GPIOE12, // 0x4C
    GPIOE13, // 0x4D
    GPIOE14, // 0x4E
    GPIOE15, // 0x4F

    GPIOF0,  // 0x50
    GPIOF1,  // 0x51
    GPIOF2,  // 0x52
    GPIOF3,  // 0x53
    GPIOF4,  // 0x54
    GPIOF5,  // 0x55
    GPIOF6,  // 0x56
    GPIOF7,  // 0x57
    GPIOF8,  // 0x58
    GPIOF9,  // 0x59
    GPIOF10, // 0x5A
    GPIOF11, // 0x5B
    GPIOF12, // 0x5C
    GPIOF13, // 0x5D
    GPIOF14, // 0x5E
    GPIOF15, // 0x5F
} GPIO_Type;


void GPIO_GlobalInit(void);

void GPIO_InitOutput(GPIO_Type gpioNum, enum_PinState_t PinState);

void GPIO_InitInput(GPIO_Type gpioNum, uint32_t pull);

void GPIO_InitGpiIRQ(GPIO_Type gpioNum, uint32_t pull,uint32_t mode, void (*GPIOxx_IRQCallback)(void));

void GPIO_EnableGpiIRQ(GPIO_Type gpioNum);

void GPIO_DisableGpiIRQ(GPIO_Type gpioNum);

void GPIO_WritePin(GPIO_Type gpioNum, enum_PinState_t PinState);

void GPIO_SetPin(GPIO_Type gpioNum);

void GPIO_ClrPin(GPIO_Type gpioNum);

void GPIO_InvertPin(GPIO_Type gpioNum);

enum_PinState_t GPIO_ReadPin(GPIO_Type gpioNum);

void GPIO_InitAFIO(GPIO_Type gpioNum, uint32_t gpioMode,uint32_t gpioPull, uint32_t gpioAF);



#endif
