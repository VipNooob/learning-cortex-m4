#include "debounce_handler.h"

extern uint32_t debounce_counter_ms;

int16_t process_debounce(Button_t buttons[], uint8_t length){
    int16_t pressed_button = -1;

    if (debounce_counter_ms >= BUTTON_POLLING_TIME){
        debounce_counter_ms = 0;

        for (uint8_t i = 0; i < length; i++){
            buttons[i].last_sampled_state = GET_INPUT_PIN_STATE(buttons[i].gpio);

            if (buttons[i].last_sampled_state == Pressed){
                if (buttons[i].consecutive_samples < DEBOUNCE_СONSECUTIVE_SAMPLES){
                    buttons[i].consecutive_samples++;
                }

                if (buttons[i].consecutive_samples >= DEBOUNCE_СONSECUTIVE_SAMPLES){
                    // Before being pressed, the button has a verified state - released
                    // Such way, we won't take into account long presses as separate events
                    if (buttons[i].verified_state == Released){
                        buttons[i].verified_state = Pressed;
                        
                        if (pressed_button == -1){
                            pressed_button = i;
                        }
                    }
                }
            }
            else{
                buttons[i].consecutive_samples = 0;
                buttons[i].verified_state = Released;
            }
        }
    }

    return pressed_button;
}

void reset_states(Button_t buttons[], uint8_t length){
  for (uint8_t i = 0; i < length; i++){
    buttons[i].consecutive_samples = 0;
    buttons[i].last_sampled_state = Released;
    buttons[i].verified_state = Released;
  }
}