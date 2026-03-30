#ifndef RCC_H
#define RCC_H

#include "def.h"

#define RCC_BASE (0x40023800)
#define CCR_OFFSET (0x00)
#define AHB1_ENR_OFFSET (0x30)
#define APB1_ENR_OFFSET (0x40)

#define RCC_CCR CREATE_ADDRESS(RCC_BASE + CCR_OFFSET)
#define RCC_AHB1_ENR  (*(__IO uint32_t *)((RCC_BASE) + (AHB1_ENR_OFFSET))) 
#define RCC_APB1_ENR (*(__IO uint32_t *)((RCC_BASE) + (APB1_ENR_OFFSET)))  

#define I2C1_BIT 21
#define GPIOB_BIT 1
#define GPIOD_BIT 3
#define RCC_AHB1_GPIOB_EN (1<<GPIOB_BIT)
#define RCC_AHB1_GPIOD_EN (1<<GPIOD_BIT)
#define RCC_APB1_I2C1_EN (1<<I2C1_BIT)

#endif