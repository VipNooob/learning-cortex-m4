#pragma once
#include "stdint.h"


void i2c_init();
void i2c_writeBytes(uint8_t addr, uint16_t reg_addr, uint8_t* array, uint16_t length);
void i2c_readBytes(uint8_t addr, uint16_t reg_addr, uint8_t* array, uint16_t length);