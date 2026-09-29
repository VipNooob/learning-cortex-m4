#include <stm32f4xx.h>
#include "clock.h"

// The order
// 1. Setup flash in order to avoid the situation when the CPU is faster than the flash access
// 2. Enable HSE
// 3. Wait until it starts 
// 4. Disable PLL just in case
// 5. Setup clocking: need to specify the clockings of all buses that dependable from PLL output
// 6. Enable PLL
// 7. Wait until it starts
// 8. Set PLL as SYSCLK 
void ClockInit168MHz(void){
  // Set flash latency 5 wait states, enable prefetch, enable instruction cache and enable data cache
  FLASH->ACR |=  (FLASH_ACR_LATENCY_5WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN);

  // Enable High-speed external oscillator
  RCC->CR |= RCC_CR_HSEON; // | RCC_CR_PLLON | RCC_CR_CSSON  phase-locked loop and clock security system
  while((RCC->CR & RCC_CR_HSERDY) == 0); // Wait until it runs
   
  RCC->CR &= ~(RCC_CR_PLLON); // Explicitly disable PLL before changing its parameters 
  
  RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSE                                             // PLL source clock is HSE
                  | RCC_PLLCFGR_PLLM_2                                               // VCO input clock = 8MHz / 4 = 2MHz 
                  | (RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_3)   // VCO output clock = 2MHz * 168 = 336MHz
                  | (0x0UL << RCC_PLLCFGR_PLLP_Pos)                                  // PLL output clock = 336MHz / 2 = 168Mhz                      
                  | (RCC_PLLCFGR_PLLQ_2 | RCC_PLLCFGR_PLLQ_1 | RCC_PLLCFGR_PLLQ_0);  // Peripheral clock = 168Mhz / 7 = 48Mhz
  
  // AHB = SYSCLK, APB1 = AHB / 4 = 42MHz, APB2 = AHB / 2 = 84MHz
  RCC->CFGR |= (RCC_CFGR_HPRE_0 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2);
  
  // Enable the configured PLL
  RCC->CR |= RCC_CR_PLLON; 
  while((RCC->CR & RCC_CR_PLLRDY) == 0); // Wait until it runs 

  // Set PLL clock source as a system clock
  RCC->CFGR &= ~(RCC_CFGR_SW_PLL); // clear these bits
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // Make sure that the chosen source was set as a system clock
}
