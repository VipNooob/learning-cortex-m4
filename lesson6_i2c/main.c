#include <stdio.h>
#include <stm32f407xx.h>
#include "clock.h"
#include "gpio.h"
#include "debounce_handler.h"
#include "utils.h"
#include "i2c.h"



#define BUTTON_COOLDOWN_MS 3000
#define AT24C64_DEVICE_ADDRESS 0x50
#define AT24C64_PAGE_ADDRESS 0x00

volatile uint32_t debounce_counter_ms = 0;
volatile uint32_t systick_counter_ms = 0;
volatile uint32_t delay_counter_ms = 0;

uint32_t last_button_press_ms = 0;
int8_t pressed_buttons[5]; // It stores the index of the first pressed button, if no button is pressed, index = -1 
uint8_t is_pressed = 0;
int8_t eeprom_rx_buffer[5];

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
        .consecutive_samples = 0,
    },
    {
        .gpio = BUTTON2,
        .last_sampled_state = Released,
        .verified_state = Released,
        .consecutive_samples = 0,
    },
    {
        .gpio = BUTTON3,
        .last_sampled_state = Released,
        .verified_state = Released,
        .consecutive_samples = 0,
    }
  };
  
  // Setup SYSTYCK
  SysTick_Config(168000); 
  
  while (1){
    // Process 5 button presses
    for (uint8_t i = 0; i < 5; i++){
      while (systick_counter_ms - last_button_press_ms <= BUTTON_COOLDOWN_MS){
        if (is_pressed == 0){
          int16_t first_pressed_button = process_debounce(buttons, ARRAY_SIZE(buttons));
        
          if (first_pressed_button > -1){
            pressed_buttons[i] = first_pressed_button;
            reset_states(buttons, ARRAY_SIZE(buttons));
            is_pressed = 1;
          }
        }
      }

      last_button_press_ms = systick_counter_ms;
      reset_states(buttons, ARRAY_SIZE(buttons));
      is_pressed = 0; 
    }

    // Как собрал 5 нажатий, надо закинуть их в eeprom
    i2c_writeBytes(AT24C64_DEVICE_ADDRESS, AT24C64_PAGE_ADDRESS, (uint8_t*)pressed_buttons, ARRAY_SIZE(pressed_buttons));
    
    // Read data from EEPROM and blink led sequence
    i2c_readBytes(AT24C64_DEVICE_ADDRESS, AT24C64_PAGE_ADDRESS, (uint8_t*)eeprom_rx_buffer, ARRAY_SIZE(eeprom_rx_buffer));

    for(;;){
      for (uint8_t i = 0; i < 5; i++){
        // Switch on, switch off cycle with an equal duration
        for (uint8_t j = 0; j < 2; j++){
          switch (eeprom_rx_buffer[i]) {
            case 0:
              LED_TOGGLE(GREEN_LED);
              break;
            case 1:
              LED_TOGGLE(ORANGE_LED);
              break;
            case 2:
              LED_TOGGLE(RED_LED);          
              break;
            default:
            break;                
          }
        delay_ms(250);
        }
      }
    } 
  }
}

void SysTick_Handler(void){
  debounce_counter_ms++;
  systick_counter_ms++;

  if (delay_counter_ms != 0x00) {
        delay_counter_ms--;
  }
}

/*************************** End of file ****************************/
