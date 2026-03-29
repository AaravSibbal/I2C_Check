#include <stdint.h>
#include "Src/gpio.h"
#include "gpio.h"
#include "rcc.h"

// reset and clock control (RCC) Clock control register
// AHB1 enable register 
// GPIOD enables with bit 3
// GPIOA enables with bit 1
#define RCC_AHB1ENR (*(volatile uint32_t *) (RCC_BASE + 0x30))
// RCC ABP1 Enable register
// i2c1 is bit 21
#define RCC_ABP1ENR (*(volatile uint32_t *) (RCC_BASE + 0x40))
/**
 * I2C definitions
 */
 #define I2C_BASE (0x40005400)
//  I2C Clock control register
 #define I2C_CCR (*(volatile uint32_t *) (I2C_BASE + 0x1C))
//  I2C control register 1
#define I2C_CR1 (*(volatile uint32_t *) (I2C_BASE + 0x00))
// I2C Control register 2
#define I2C_CR2 (*(volatile uint32_t *) (I2C_BASE + 0x04))
// I2C TRISE register
#define I2C_TRISE (*(volatile uint32_t *) (I2C_BASE + 0x20))
// I2C Status register 1 
#define I2C_SR1 (*(volatile uint32_t *) (I2C_BASE + 0x14))
// I2C status register 2
#define I2C_SR2 (*(volatile uint32_t *) (I2C_BASE + 0x18))
// I2C Data Register 
#define I2C_DR (*(volatile uint32_t *) (I2C_BASE + 0x10))

#define DAC_WRITE_ADDRESS 0x94
#define DAC_READ_ADDRESS 0x95

#define SYSTICK_BASE (0xE000E010)

#define SYSTICK_CTRL (*(volatile uint32_t *) (SYSTICK_BASE + 0x00))
#define SYSTICK_LOAD (*(volatile uint32_t *) (SYSTICK_BASE + 0x04))
#define SYSTICK_VAL (*(volatile uint32_t *) (SYSTICK_BASE + 0x08))


void turn_green_on(){
    GPIO_set_odr(GPIOD, GPIO_PIN_12, GPIO_OUTPUT_HIGH);
}
void turn_red_on(){
    GPIO_set_odr(GPIOD, GPIO_PIN_14, GPIO_OUTPUT_HIGH);
}
void turn_green_off(){
    GPIO_set_odr(GPIOD, GPIO_PIN_12, GPIO_OUTPUT_LOW);
}
void turn_red_off(){
    GPIO_set_odr(GPIOD, GPIO_PIN_14, GPIO_OUTPUT_LOW);
}

void GPIOx_clock_on(){
    RCC_AHB1_ENR |= ((RCC_AHB1_GPIOD_EN) | (RCC_AHB1_GPIOB_EN));
}

void I2C_clock_on(){
    RCC_APB1_ENR |= RCC_APB1_I2C_EN;
}

void init_clock_on(){
    GPIOx_clock_on();
    I2C_clock_on();
}

void init_GPIOD(){
    GPIO_set_moder(GPIOD, GPIO_PIN_12, GPIO_MODE_OUTPUT);
    GPIO_set_moder(GPIOD, GPIO_PIN_14, GPIO_MODE_OUTPUT);

}

// pin6
void init_SCL(){
    GPIO_set_moder(GPIOB, GPIO_PIN_6, GPIO_MODE_ALT);
    GPIO_set_otyper(GPIOB, GPIO_PIN_6, GPIO_TYPE_OPEN_DRAIN);
    GPIO_set_alt_func(GPIOB, GPIO_PIN_6, AF4);
}

// pin 9 
void init_SDA(){
    GPIO_set_moder(GPIOB, GPIO_PIN_9, GPIO_MODE_ALT);
    GPIO_set_otyper(GPIOB, GPIO_PIN_9, GPIO_TYPE_OPEN_DRAIN);
    GPIO_set_alt_func(GPIOB, GPIO_PIN_9, AF4);
}

void init_GPIOB(){
    init_SCL();
    init_SDA();
}

void init_I2C1(){
    // setting the frequency of the peripheral clock
    I2C_CR2 &= ~(63<<0);
    I2C_CR2 |= (16<<0);

    // setting the time to check for rise and fall
    I2C_CCR &= ~(4095<<0);
    I2C_CCR |= (80<<0);

    // Setting the rise time
    I2C_TRISE &= ~(31<<0);
    I2C_TRISE |= (17<<0);

    // this goes last enabling the I2C engine
    I2C_CR1 |= (1<<0);
}

void write_to_I2C_DR(uint32_t addr){
    I2C_DR = addr;
}

void clear_I2C_ADDR(){
    uint32_t dummy_read;
    dummy_read = I2C_SR1;
    dummy_read = I2C_SR2;
}

void delay_ms(uint32_t ms){
    SYSTICK_LOAD = (16000-1);
    SYSTICK_VAL = 0;
    // make sure there are no exception errors
    SYSTICK_CTRL &= (0<<1);
    // make sure we use the processor clock
    SYSTICK_CTRL |= (1<<2);
    // Enable the clock
    SYSTICK_CTRL |= (1<<0);

    for(uint32_t i=0; i<ms; i++){
        // loop till the clock cycle is done 1ms
        while((SYSTICK_CTRL & (1<<16)) == 0){
               
        }
    }

    // disable the clock
    SYSTICK_CTRL = 0;

}

void ping_audio_chip(){
    // start generation
    I2C_CR1 |= (1<<8);

    while(1){
        // checking for the start bit
        if(I2C_SR1 & (1<<0)){
            break;
        }
    }

    // Now we know that the generation has started
    // we write the dac address to see of it will respond
    write_to_I2C_DR(DAC_WRITE_ADDRESS);
    while(1){
        if(I2C_SR1 & (1<<1)){
            break;
        }
    }
    // we know we recieved an aknowledgement
    clear_I2C_ADDR();

    // stop the generation
    I2C_CR1 |= (1<<9);
    
    turn_green_on();
    delay_ms(1000);
    turn_green_off();
}

void init_audio_chip(){
    GPIO_set_moder(GPIOD, GPIO_PIN_4, GPIO_MODE_OUTPUT);
    GPIO_set_odr(GPIOD, GPIO_PIN_4, GPIO_OUTPUT_HIGH);
}

int main(void) {
    init_clock_on();
    init_GPIOD();
    init_GPIOB();
    init_I2C1();
    init_audio_chip();
    delay_ms(1);

    while(1){
        ping_audio_chip();
        delay_ms(100);
    }
}