#include "debounce_handler.h"

extern uint32_t debounce_counter_ms;

void process_debounce(Button_t buttons[], uint8_t length) {
  if (debounce_counter_ms > BUTTON_POLLING_TIME) {
    debounce_counter_ms = 0;

    for (uint8_t i = 0; i < length; i++) {
      buttons[i].last_sampled_state = GET_INPUT_PIN_STATE(buttons[i].gpio);

      if (buttons[i].last_sampled_state == Pressed) {
        buttons[i].consecutive_samples++;
        
        if (buttons[i].consecutive_samples >= DEBOUNCE_СONSECUTIVE_SAMPLES){
            buttons[i].verified_state = Pressed;
        }
      } 
      else {
        buttons[i].consecutive_samples = 0;
        buttons[i].verified_state = Released;
      }
    }
  }
}
