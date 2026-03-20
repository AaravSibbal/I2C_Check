#include <stdint.h>

// GPIOB 
#define GPIOB_BASE (0x40020400)
// alternate function 10 or 2
#define GPIOB_MODER (*(volatile uint32_t *) (GPIOB_BASE + 0X00))
#define GPIOB_OTYPER (*(volatile uint32_t *) (GPIOB_BASE + 0x04)) 
#define GPIOB_AFRL (*(volatile uint32_t *) (GPIOB_BASE + 0x20)) // scl pin 6
#define GPIOB_AFRH (*(volatile uint32_t *) (GPIOB_BASE + 0x24)) // sda pin 9

// red and green lights both are on port D
// green is on PD12 red is on PD 14
#define GPIOD_BASE (0x40020C00)
// this would be 1 as both red and green light would need to be in
// output mode
#define GPIOD_MODER (*(volatile uint32_t *) (GPIOD_BASE + 0X00))
// this will control the output of the lights green and red
#define GPIOD_ODR (*(volatile uint32_t *) (GPIOD_BASE + 0x14))

// reset and clock control (RCC) Clock control register
#define RCC_BASE (0x40023800)
#define RCC_CCR (*(volatile uint32_t *) (RCC_BASE + 0x00))
// AHB1 enable register 
// GPIOD enables with bit 3
// GPIOA enables with bit 1
#define RCC_AHB1ENR (*(volatile uint32_t *) (RCC_BASE + 0x30))
// RCC ABP1 Enable register
// i2c1 is bit 21
#define RCC_ABP1ENR (*(volatile uint32_t *) (RCC_BASE + 0x40))

void turn_green_on(){
    GPIOD_ODR |= (1<<12);
}
void turn_red_on(){
    GPIOD_ODR |= (1<<14);
}
void turn_green_off(){
    GPIOD_ODR &= ~(1<<24);
}
void turn_red_off(){
    GPIOD_ODR &= ~(1<<28);
}

void sys_clock_on(){
    RCC_CCR |= (1<<0);
}

void GPIOx_clock_on(){
    RCC_AHB1ENR |= (1<<3);
    RCC_AHB1ENR |= (1<<1);
}

void I2C_clock_on(){
    RCC_ABP1ENR |= (1<<21);
}

void init_clock_on(){
    sys_clock_on();
    GPIOx_clock_on();
    I2C_clock_on();
}

void init_GPIOD(){
    // lights infrastructure

    // clear the pin 12 section, i.e. green
    GPIOD_MODER &= ~(3<<24);
    GPIOD_MODER |= (1<<24);

    GPIOD_MODER &= ~(3<<28);
    GPIOD_MODER |= (1<<28);
}

// pin6
void init_SCL(){
    GPIOB_MODER &= ~(3<<12);
    GPIOB_MODER |= (2<<12);
    GPIOB_OTYPER |= (1<<6);
    GPIOB_AFRL &= ~(15<<24);
    GPIOB_AFRL |= (4<<24);    
}

// pin 9 
void init_SDA(){
    GPIOB_MODER &= ~(3<<18);
    GPIOB_MODER |= (2<<18);
    GPIOB_OTYPER |= (1<<9);
    GPIOB_AFRH &= ~(15<<4);
    GPIOB_AFRH |= (4<<4);    
}

void init_GPIOB(){
    init_SCL();
    init_SDA();
}

int main(void) {
    init_clock_on();
    init_GPIOD();
    init_GPIOB();


}