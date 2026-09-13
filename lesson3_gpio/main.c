/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include "stm32f407xx.h"

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/

#define CHASING_LIGHTS_DELAY               1000000
#define COLUMN_LIKE_DELAY                  (CHASING_LIGHTS_DELAY / 4)
#define SIMULTANEOUS_BLINKING_DELAY        CHASING_LIGHTS_DELAY
#define MY_BLINK_SEQUENCE_DELAY            CHASING_LIGHTS_DELAY

int main(void) {
  uint32_t i = 0;

  SystemInit();
  
  // Enable GPIOD clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
  // Set PD12-PD15 in general purpose output mode
  GPIOD->MODER |= (GPIO_MODER_MODE12_0 | GPIO_MODER_MODE13_0 | GPIO_MODER_MODE14_0 | GPIO_MODER_MODE15_0);

  // chasing lights
  // 3 leds * 2 states * 3 times 
  for (i = 0; i < 18; i++){
    GPIOD->ODR ^= (0x1UL << (GPIO_ODR_OD12_Pos + i % 3));
    for (uint32_t i = 0; i < CHASING_LIGHTS_DELAY; i++);
  }

  // column-like lighting 
  for (i = 0; i < 5; i++){
    // movement upward
    for (uint8_t k = 0; k < 3; k++){
      GPIOD->BSRR = (0x1UL << (GPIO_BSRR_BS12_Pos + k));
      for (uint32_t i = 0; i < COLUMN_LIKE_DELAY; i++);
    }
    // movement downward
    for (uint8_t k = 0; k < 3; k++){
      GPIOD->BSRR = (0x1UL << (GPIO_BSRR_BR14_Pos - k));
      for (uint32_t i = 0; i < COLUMN_LIKE_DELAY; i++);
    }
  }
  
  // Simultaneous blinking
  for (uint8_t i = 0; i < 20; i++){
    // Toggle PD12-PD15 pins using XOR operation
    GPIOD->ODR ^= (GPIO_ODR_OD12_Msk | GPIO_ODR_OD13_Msk | GPIO_ODR_OD14_Msk | GPIO_ODR_OD15_Msk);
    for (uint32_t i = 0; i < SIMULTANEOUS_BLINKING_DELAY; i++);
  }

  // my blink sequence: two LEDs blink, then one LED blinks
  for (uint8_t i = 0; i < 10; i++){
    // Toggle PD12-PD13 pins using XOR operation
    GPIOD->ODR ^= (GPIO_ODR_OD12_Msk | GPIO_ODR_OD13_Msk);
    for (uint32_t i = 0; i < MY_BLINK_SEQUENCE_DELAY; i++);  
  }
  
  for (uint32_t i = 0; i < MY_BLINK_SEQUENCE_DELAY / 2; i++);
  
  for (uint8_t i = 0; i < 10; i++){
    // Toggle PD12-PD13 pins using XOR operation
    GPIOD->ODR ^= (GPIO_ODR_OD14_Msk);
    for (uint32_t i = 0; i < MY_BLINK_SEQUENCE_DELAY; i++);  
  }

  while (1) {

  }

  
  
  
}

/*************************** End of file ****************************/
