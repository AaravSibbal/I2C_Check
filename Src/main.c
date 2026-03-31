#include <stdint.h>
#include "gpio.h"
#include "i2c.h"

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

void get_chip_id(uint8_t *ready_val){
    GPIO_set_moder(GPIOD, GPIO_PIN_4, GPIO_MODE_OUTPUT);
    GPIO_set_bssr(GPIOD, GPIO_PIN_4, BSSR_RESET);
    delay_ms(1);
    GPIO_set_bssr(GPIOD, GPIO_PIN_4, BSSR_SET);
    I2C_init_engine(GPIOB, GPIO_PIN_6, GPIO_PIN_9, AF4, 16, 100000);
    I2C_read_reg(DAC_WRITE_ADDRESS, 0x01, ready_val);
}

int main(void) {
    init_clock_on();
    init_GPIOD();
    uint8_t chip_id = 0;
    get_chip_id(&chip_id);
    if(chip_id == 0xE0){
        while(1){
            turn_green_on();
            delay_ms(100);
            turn_green_off();
            delay_ms(100);
        }
    }else if(chip_id == 0xE1){
        while(1){
            turn_red_on();
            delay_ms(100);
            turn_red_off();
            delay_ms(100);
        }
    } else if(chip_id == 0xE2){
        while(1){
            turn_green_on();
            delay_ms(1000);
            turn_green_off();
            delay_ms(1000);
        }
    }else if(chip_id == 0xE3){
        while(1){
            turn_red_on();
            delay_ms(1000);
            turn_red_off();
            delay_ms(1000);
        }
    }else{
        while(1){
            turn_red_on();
        }
    }


}