#ifndef I2C_H
#define I2C_H

#include "def.h"
#include "rcc.h"
#include "gpio.h"

#define I2C_BASE (0x40005400)


typedef struct I2C{
    __IO uint32_t I2C_CR1;
    __IO uint32_t I2C_CR2;
    __IO uint32_t I2C_OAR1;
    __IO uint32_t I2C_OAR2;    
    __IO uint32_t I2C_DR;    
    __IO uint32_t I2C_SR1;    
    __IO uint32_t I2C_SR2;    
    __IO uint32_t I2C_CCR;    
    __IO uint32_t I2C_TRISE;    
    __IO uint32_t I2C_FLTR;    
} I2C_t;

#define I2C_ENGINE ((I2C_t *) I2C_BASE)

// talk through i2c
void I2C_init_engine(GPIO_t *GPIO, GPIO_Pin_t scl_pin, GPIO_Pin_t sda_pin, 
GPIO_AFx_t alt_func_val,  uint32_t clock_speed_MHz, uint32_t i2c_freq);
void I2C_start_talking(uint32_t peripheral_addr);
void I2C_write_reg(uint32_t reg_addr, uint32_t value);
void I2C_stop_talking();
void I2C_read();

#endif