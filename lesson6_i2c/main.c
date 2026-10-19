#include <stdio.h>
#include <stm32f407xx.h>
#include "clock.h"
#include "gpio.h"
#include "debounce_handler.h"
#include "utils.h"


volatile uint32_t debounce_counter_ms = 0;
volatile uint32_t systick_counter_ms = 0;


int main(void) {
  // Setup clock: AHB = SYSCLK = 168Mhz, APB1 = 42MHz, APB2 = 84MHz
  ClockInit168MHz();
  
  // PD12 - green led, PD13 - orange led, PD14 - red led, PD15 - blue led
  // PA0 - button1, PA1 - button2, PA2 - button3
  GPIO_Init();
  
  // debounce handler
  Button_t buttons[] = {
    {
        .gpio = BUTTON1,
        .last_sampled_state = Released,
        .verified_state = Released,
        .consecutive_samples = 0
    },
    {
        .gpio = BUTTON2,
        .last_sampled_state = Released,
        .verified_state = Released,
        .consecutive_samples = 0
    },
    {
        .gpio = BUTTON3,
        .last_sampled_state = Released,
        .verified_state = Released,
        .consecutive_samples = 0
    }
  };
  
  // Setup SYSTYCK
  // By default Systick clock is an external clock = HCLK / 8
  SysTick_Config(21000); 
  
  
  while (1){
    process_debounce(buttons, ARRAY_SIZE(buttons));
    
  }
}

void SysTick_Handler(void){
  debounce_counter_ms++;
  systick_counter_ms++;
}

/*************************** End of file ****************************/
