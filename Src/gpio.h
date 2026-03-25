#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define __IO volatile

#define GPIOB_BASE (0x40020400)
#define GPIOD_BASE (0x40020C00)

#define GPIO_MODER_OFFSET (0x00)
#define GPIO_OTYPER_OFFSET (0x04)
#define GPIO_AFRL_OFFSET (0x20)
#define GPIO_AFRH_OFFSET (0X24)
#define GPIO_ODR_OFFSET (0X14)


typedef enum{
    GPIO_MODE_INPUT = 0x00,
    GPIO_MODE_OUTPUT = 0x01,
    GPIO_MODE_ALT = 0x02,
    GPIO_MODE_ANALOG = 0x03
} GPIO_Mode_t;

typedef enum{
    GPIO_PUSH_PULL = 0x00,
    GPIO_OPEN_DRAIN = 0X01
} GPIO_OTYPER_t;

typedef enum{
    GPIO_OUTPUT_HIGH = 0x00,
    GPIO_OUTPUT_LOW = 0x01
} GPIO_ODR_t;






typedef struct GPIO{
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t AFRL;
    __IO uint32_t AFRH ;
    __IO uint32_t ODR
} GPIO_t;



void GPIO_set_moder()

GPIO_t

#endif