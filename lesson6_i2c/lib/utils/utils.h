#pragma once
#include "stdint.h"


#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
 
extern volatile uint32_t delay_counter_ms; 

void delay_ms(uint32_t ms){
    delay_counter_ms = ms;
    while (delay_counter_ms != 0); 
}