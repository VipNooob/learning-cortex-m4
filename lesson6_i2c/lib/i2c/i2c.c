#include "i2c.h"
#include "stm32f407xx.h"


void i2c_init(void){
  RCC->AHB1ENR |= RCC_APB1ENR_I2C1EN;
  

}