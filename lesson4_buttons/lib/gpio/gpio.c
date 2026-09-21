#include "stm32f407xx.h"
#include "gpio.h"


void GPIO_Init(void){
  // Switch on clock on specified GPIO
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN);
  GPIOD->MODER |= (GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0); // PD12-PD15 push-pull output
  GPIOA->MODER |= (GPIO_MODER_MODER2_0); // PA2 push-pull output 
  GPIOA->MODER &= ~(GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0| GPIO_MODER_MODER3_0 | GPIO_MODER_MODER8_0); // PA0, PA1, PA3, PA8, input
  // Enable pull-up on inputs
  GPIOA->PUPDR |= (GPIO_PUPDR_PUPD3_0 | GPIO_PUPDR_PUPD8_0);
  // Enable pull-down on inputs
  GPIOA->PUPDR |= (GPIO_PUPDR_PUPD1_1);
}


