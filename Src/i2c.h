#ifndef I2C_H
#define I2C_H

#include "def.h"
#include "rcc.h"
#include "gpio.h"

#define I2C_BASE (0x40005400)
#define I2C_START_GENERATION (1<<8)
#define I2C_NACK_EN_R (1<<10)

#define I2C_SR1_AF_FLAG (1<<10)
#define I2C_SR1_SB_FLAG (1<<0) 
#define I2C_SR1_ADDR_FLAG (1<<1)
#define I2C_SR1_TXE_FLAG (1<<7)
#define I2C_SR1_BTF_FLAG (1<<0)

#define I2C_CR1_ACK_EN (1<<10)
#define I2C_STOP_GENERATION (1<<9)
#define ACK 1
#define NACK 0

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
int I2C_write_reg(uint8_t peri_addr, uint8_t reg_addr, uint8_t value);
int I2C_read_reg(uint8_t peri_addr, uint8_t reg_addr, uint8_t *read_val);

#endif