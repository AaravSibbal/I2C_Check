#include <stdint.h>
#include "gpio.h"
#include "rcc.h"

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

void init_clock_on(){
    GPIOx_clock_on();
}

void init_GPIOD(){
    GPIO_set_moder(GPIOD, GPIO_PIN_12, GPIO_MODE_OUTPUT);
    GPIO_set_moder(GPIOD, GPIO_PIN_14, GPIO_MODE_OUTPUT);
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

void init_audio_chip(){
    GPIO_set_moder(GPIOD, GPIO_PIN_4, GPIO_MODE_OUTPUT);
    GPIO_set_bssr(GPIOD, GPIO_PIN_4, BSSR_RESET);
    delay_ms(1);
    GPIO_set_bssr(GPIOD, GPIO_PIN_4, BSSR_SET);
    
}

int main(void) {
    init_clock_on();
    init_GPIOD();
    init_audio_chip();
    delay_ms(1);

    while(1){
        ping_audio_chip();
        delay_ms(100);
    }
}