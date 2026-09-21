#pragma once
#include "stm32f407xx.h"


// Hardware setup:
// PA0 - user button on the development board
// PA3 - external user button1
// PA8 - external user button2
// PD12 - green led
// PD13 - orange led
// PD14 - red led
// PD15 - blue led
// PA2 -  B0 (in my case)
// PA1 -  B1 (in my case)

typedef struct {
    GPIO_TypeDef *port;
    uint8_t pin;
} GPIO_t;

enum {OFF, ON};

#define GREEN_LED     ((GPIO_t){GPIOD, 12})
#define ORANGE_LED    ((GPIO_t){GPIOD, 13})
#define RED_LED       ((GPIO_t){GPIOD, 14})
#define BLUE_LED      ((GPIO_t){GPIOD, 15})
#define B0_OUTPUT     ((GPIO_t){GPIOA, 2})
#define B1_INPUT      ((GPIO_t){GPIOA, 1})

#define SET_PIN(GPIO_STRUCT)        ((GPIO_STRUCT).port->BSRR = (1 << (GPIO_STRUCT).pin))
#define RESET_PIN(GPIO_STRUCT)      ((GPIO_STRUCT).port->BSRR = (1 << ((GPIO_STRUCT).pin + 16)))
#define TOGGLE_PIN(GPIO_STRUCT)     ((GPIO_STRUCT).port->ODR ^= (1 << (GPIO_STRUCT).pin))

#define GET_OUTPUT_PIN_STATE(GPIO_STRUCT) ((GPIO_STRUCT).port->ODR == (1 << (GPIO_STRUCT).pin))
#define GET_INPUT_PIN_STATE(GPIO_STRUCT)  ((GPIO_STRUCT).port->IDR == (1 << (GPIO_STRUCT).pin))

#define LED_ON(LED)      SET_PIN(LED)
#define LED_OFF(LED)     RESET_PIN(LED)
#define LED_TOGGLE(LED)  TOGGLE_PIN(LED)

#define GET_LED_STATE(LED)    GET_OUTPUT_PIN_STATE(LED)

void GPIO_Init(void);
