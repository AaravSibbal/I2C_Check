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
    GPIOD_ODR |= (1<<12);
}
void turn_red_on(){
    GPIOD_ODR |= (1<<14);
}
void turn_green_off(){
    GPIOD_ODR &= ~(1<<12);
}
void turn_red_off(){
    GPIOD_ODR &= ~(1<<14);
}

void GPIOx_clock_on(){
    RCC_AHB1ENR |= (1<<3);
    RCC_AHB1ENR |= (1<<1);
}

void I2C_clock_on(){
    RCC_ABP1ENR |= (1<<21);
}

void init_clock_on(){
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
    I2C_DR &= ~(255<<0);
    I2C_DR |= (addr << 0);
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

}

int main(void) {
    init_clock_on();
    init_GPIOD();
    init_GPIOB();
    init_I2C1();
    init_audio_chip();

}