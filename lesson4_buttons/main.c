/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/
#include <stm32f4xx.h>
#include "clock.h"
#include "gpio.h"

#define TASK1
//#define TASK2
//#define TASK3  // the idea is to toggle pin at rising and falling edge in order to track all signal changes (such as a wire connect/disconnect)

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {
  SystemInit();
  ClockInit168MHz();
  
  // Switch on clock on SYSCFG controller
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;  // Set port A on EXTI0
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;  // Set port A on EXTI1
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;  // Set port A on EXTI3
  SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PA;  // Set port A on EXTI8
  
  
  __enable_irq();
  
  EXTI->RTSR |= (EXTI_RTSR_TR0 | EXTI_RTSR_TR1| EXTI_RTSR_TR3 | EXTI_RTSR_TR8); // Set rising trigger event configuration bit of line 0, 1, 3, 8 
  EXTI->FTSR |= (EXTI_FTSR_TR1); // Set falling trigger event configuration bit of line 1
  EXTI->IMR |= (EXTI_IMR_MR0 | EXTI_IMR_MR1| EXTI_IMR_MR3 | EXTI_IMR_MR8);     // Enable interrupt on line 0, 1, 3, 8
  
  NVIC_EnableIRQ(EXTI0_IRQn);
  NVIC_EnableIRQ(EXTI1_IRQn);
  NVIC_EnableIRQ(EXTI3_IRQn);
  NVIC_EnableIRQ(EXTI9_5_IRQn);

  GPIO_Init();

  while (1) {

  }

}

void EXTI0_IRQHandler(){
  if (EXTI->PR &  EXTI_PR_PR0){
    // Clear pending bit
    EXTI->PR |= EXTI_PR_PR0;

     #if defined (TASK1)
    if (GET_LED_STATE(GREEN_LED)){
      LED_OFF(GREEN_LED);
    }
    else{
      LED_ON(GREEN_LED);
    }
  #elif defined(TASK2)
    switch (GET_LED_STATE(GREEN_LED)) {
      case ON:
        LED_OFF(GREEN_LED);
      break;
      case OFF:
        LED_ON(GREEN_LED);
      break;
    }
  #elif defined(TASK3)
    TOGGLE_PIN(B0_OUTPUT);
    LED_TOGGLE(GREEN_LED);
  #endif
  }
}

void EXTI1_IRQHandler(){
  if (EXTI->PR &  EXTI_PR_PR1){
    // Clear pending bit
    EXTI->PR |= EXTI_PR_PR1;

    LED_TOGGLE(BLUE_LED);
  }
}

void EXTI3_IRQHandler(){
  if (EXTI->PR &  EXTI_PR_PR3){
    // Clear pending bit
    EXTI->PR |= EXTI_PR_PR3;

    #if defined (TASK1)
    if (GET_LED_STATE(ORANGE_LED)){
      LED_OFF(ORANGE_LED);
    }
    else{
      LED_ON(ORANGE_LED);
    }
  #elif defined(TASK2)
    switch (GET_LED_STATE(ORANGE_LED)) {
      case ON:
        LED_OFF(ORANGE_LED);
      break;
      case OFF:
        LED_ON(ORANGE_LED);
      break;
    }
  #elif defined(TASK3)
  #endif
  }
}

void EXTI9_5_IRQHandler(){
  if (EXTI->PR &  EXTI_PR_PR8){
    // Clear pending bit
    EXTI->PR |= EXTI_PR_PR8;

    #if defined (TASK1)
    if (GET_LED_STATE(RED_LED)){
      LED_OFF(RED_LED);
    }
    else{
      LED_ON(RED_LED);
    }
  #elif defined(TASK2)
    switch (GET_LED_STATE(RED_LED)) {
      case ON:
        LED_OFF(RED_LED);
      break;
      case OFF:
        LED_ON(RED_LED);
      break;
    }
  #elif defined(TASK3)
  #endif
  } 
}
/*************************** End of file ****************************/
