#pragma once
#include "stdint.h"
#include "gpio.h"


#define BUTTON_POLLING_TIME                  5  // ms
#define DEBOUNCE_СONSECUTIVE_SAMPLES         5 


typedef enum {

  Pressed,
  Released,

} ButtonState_t;

typedef struct{

  GPIO_t gpio;
  
  ButtonState_t last_sampled_state;
  ButtonState_t verified_state;
  
  uint32_t consecutive_samples; 

} Button_t;

void process_debounce(Button_t buttons[], uint8_t length);
