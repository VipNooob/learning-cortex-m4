#include <stdio.h>
#include <stm32f407xx.h>
#include "clock.h"
#include "gpio.h"


int main(void) {
  // Setup clock: AHB = SYSCLK = 168Mhz, APB1 = 42MHz, APB2 = 84MHz
  ClockInit168MHz();
  
  // PD12 - green led, PD13 - orange led, PD14 - red led, PD15 - blue led
  // PA0 - button1, PA1 - button2, PA2 - button3
  GPIO_Init();
  
  // Setup SYSTYCK
  // By default Systick clock is an external clock = HCLK / 8
  SysTick_Config(21000); 
  

  


  while (1){
  }
}

/*************************** End of file ****************************/
