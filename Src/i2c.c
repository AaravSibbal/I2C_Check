#include "i2c.h"
#include "def.h"
#include "gpio.h"
#include "rcc.h"


static void I2C_set_freq_bits(I2C_t *I2C, uint32_t clock_speed_MHz){
    I2C->I2C_CR2 &= ~(63<<0);
    I2C->I2C_CR2 |= (clock_speed_MHz);
}
static void I2C_set_CCR(I2C_t *I2C, uint32_t CCR_val){
    I2C->I2C_CCR &= ~(4095<<0);
    I2C->I2C_CCR |= (CCR_val<<0);
}
static void I2C_set_TRISE(I2C_t *I2C, uint32_t TRISE_val){
    I2C->I2C_TRISE &= ~(63<<0);
    I2C->I2C_TRISE |= (TRISE_val<<0);
}

static void I2C_enable_engine(I2C_t *I2C){
    I2C->I2C_CR1 |= (1<<0);
}

static void I2C1_clock_on(){
    RCC_APB1_ENR |= RCC_APB1_I2C1_EN;
}


static void init_I2C_pin(GPIO_t *GPIO, GPIO_Pin_t gpio_pin, GPIO_AFx_t alt_func_val){
    GPIO_set_moder(GPIO, gpio_pin, GPIO_MODE_ALT);
    GPIO_set_otyper(GPIO, gpio_pin, GPIO_TYPE_OPEN_DRAIN);
    GPIO_set_alt_func(GPIO, gpio_pin, alt_func_val);
}

/**
 * TRISE (1000ns/Tplock1^-1) + 1 or
 * Tpclock1/1,000,000 + 1
 */
void I2C_init_engine(GPIO_t *GPIO, GPIO_Pin_t scl_pin, GPIO_Pin_t sda_pin, 
GPIO_AFx_t alt_func_val,  uint32_t clock_speed_MHz, uint32_t i2c_freq){
    // turn on peripheral clock
    I2C1_clock_on();

    // turn on sda and scl pins
    init_I2C_pin(GPIO, sda_pin, alt_func_val);
    init_I2C_pin(GPIO, scl_pin, alt_func_val);
    
    // set up the engine
    uint32_t clock_speed_Hz = clock_speed_MHz*1000000;
    uint32_t ccr_val = (clock_speed_Hz)/(2*i2c_freq);
    uint32_t trise_val = (clock_speed_Hz/1000000)+1;
    I2C_set_freq_bits(I2C_ENGINE, clock_speed_MHz);
    I2C_set_CCR(I2C_ENGINE, ccr_val);
    I2C_set_TRISE(I2C_ENGINE, trise_val);
    I2C_ENGINE->I2C_CR1 |=
    // turn on the engine
    I2C_enable_engine(I2C_ENGINE);
}

static i2c_wait_start(){
    while (1){
        if(I2C_ENGINE->I2C_SR1 & (I2C_SR1_SB_FLAG)){
            return TRUE;
        }
    }
}

/**
 * returns 1 if recieved an ACK
 * return 0 if recieved a NACK
 */
static int I2C_is_ACK(){
    while(1){
        if(I2C_ENGINE->I2C_SR1 & (I2C_SR1_ADDR_FLAG)){
            return ACK;
        }else if(I2C_ENGINE->I2C_SR1 & (I2C_SR1_AF_FLAG)){
            return NACK;
        }
    }
}

static void I2C_clear_ADDR(){
    uint32_t dummy_read;
    dummy_read = I2C_ENGINE->I2C_SR1;
    dummy_read = I2C_ENGINE->I2C_SR2;
}

static int I2C_start_talking(uint8_t peripheral_addr){
    I2C_ENGINE->I2C_CR1 |= I2C_START_GENERATION;
    i2c_wait_start();
    I2C_ENGINE->I2C_DR = peripheral_addr;
    if(!I2C_is_ACK()){
        return FALSE;
    }
    I2C_clear_ADDR();
    return TRUE
}


int I2C_write_reg(uint8_t peri_addr, uint8_t reg_addr, uint8_t value){
    int error = I2C_start_talking(peri_addr);
    // peripheral address was wrong
    if(error == FALSE){
        return FALSE;
    }
    while(1){
        if(I2C_ENGINE->I2C_SR1 & (I2C_SR1_TXE_FLAG)){
            // the data regester is empty
            break;
        }
    }
    I2C_ENGINE->I2C_DR = reg_addr;
    while(1){
        if(I2C_ENGINE->I2C_SR1 & (I2C_SR1_TXE_FLAG)){
            // the data regester is empty
            break;
        }
    }
    I2C_ENGINE->I2C_DR = value;
    while(1){
        if(I2C_ENGINE->I2C_SR1 & (I2C_SR1_BTF_FLAG)){
            // data transfor this done
            break;
        }
    } 

    I2C_ENGINE->I2C_CR1 |= I2C_STOP_GENERATION;

}
int I2C_read_reg(uint8_t peri_addr, uint8_t reg_addr, uint8_t *read_val){
    int error = I2C_start_talking(peri_addr);
    if(error == FALSE){
        return FALSE
    }

}