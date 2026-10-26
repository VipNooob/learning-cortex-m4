#pragma once
#include "stm32f407xx.h"


// Hardware setup:
// LEDs
// PD12 - green led
// PD13 - orange led
// PD14 - red led
// PD15 - blue led
// Buttons
// PA0 - first user button  (integrated on the development board)
// PA1 - second user button (external)
// PA2 - third user button  (external)


typedef struct {
    GPIO_TypeDef *port;
    uint8_t pin;
} GPIO_t;

enum {OFF, ON};

#define GREEN_LED     ((GPIO_t){GPIOD, 12})
#define ORANGE_LED    ((GPIO_t){GPIOD, 13})
#define RED_LED       ((GPIO_t){GPIOD, 14})
#define BLUE_LED      ((GPIO_t){GPIOD, 15})

#define BUTTON1       ((GPIO_t){GPIOA, 0})
#define BUTTON2       ((GPIO_t){GPIOA, 1})
#define BUTTON3       ((GPIO_t){GPIOA, 2})

#define SET_PIN(GPIO_STRUCT)        ((GPIO_STRUCT).port->BSRR = (1 << (GPIO_STRUCT).pin))
#define RESET_PIN(GPIO_STRUCT)      ((GPIO_STRUCT).port->BSRR = (1 << ((GPIO_STRUCT).pin + 16)))
#define TOGGLE_PIN(GPIO_STRUCT)     ((GPIO_STRUCT).port->ODR ^= (1 << (GPIO_STRUCT).pin))

#define GET_OUTPUT_PIN_STATE(GPIO_STRUCT) (((GPIO_STRUCT).port->ODR >> (GPIO_STRUCT).pin) & 1U)
#define GET_INPUT_PIN_STATE(GPIO_STRUCT)  (((GPIO_STRUCT).port->IDR >> (GPIO_STRUCT).pin) & 1U)

#define LED_ON(LED)      SET_PIN(LED)
#define LED_OFF(LED)     RESET_PIN(LED)
#define LED_TOGGLE(LED)  TOGGLE_PIN(LED)

#define GET_LED_STATE(LED)    GET_OUTPUT_PIN_STATE(LED)

void GPIO_Init(void);
