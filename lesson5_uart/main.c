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
#include "usart.h"

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
  SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PA;  // Set port A on EXTI8
  
  __enable_irq();
  
  EXTI->RTSR |= (EXTI_RTSR_TR0 | EXTI_RTSR_TR1 | EXTI_RTSR_TR8); // Set rising trigger event configuration bit of line 0, 1, 8 
  EXTI->IMR |= (EXTI_IMR_MR0 | EXTI_IMR_MR1 | EXTI_IMR_MR8);     // Enable interrupt on line 0, 1, 8
  
  NVIC_EnableIRQ(EXTI0_IRQn);
  NVIC_EnableIRQ(EXTI1_IRQn);
  NVIC_EnableIRQ(EXTI9_5_IRQn);
  
  GPIO_Init();

  USART2_Init();
  
  while (1) {

  }
}

void EXTI0_IRQHandler(){
  if (EXTI->PR &  EXTI_PR_PR0){
    // Clear pending bit
    EXTI->PR |= EXTI_PR_PR0;
    Send_ConstString("Button S1 ");
  }
}

void EXTI1_IRQHandler(){
  if (EXTI->PR &  EXTI_PR_PR1){
    // Clear pending bit
    EXTI->PR |= EXTI_PR_PR1;
    Send_ConstString("Button S2 ");

  }
}

void EXTI9_5_IRQHandler(){
  if (EXTI->PR &  EXTI_PR_PR8){
    // Clear pending bit
    EXTI->PR |= EXTI_PR_PR8;
    Send_ConstString("Button S3 ");   
  } 
}

void USART2_IRQHandler(){
  if ((USART2->SR & USART_SR_RXNE) != 0){
    uint8_t ch = (uint8_t)(USART2->DR & 0xFF);

    switch (ch){
      case '0':
        LED_OFF(GREEN_LED);
        LED_OFF(ORANGE_LED);
        LED_OFF(RED_LED);
      break;

      case '1':
        LED_ON(GREEN_LED);
      break;

      case '2':
        LED_ON(ORANGE_LED);
      break;

      case '3':
        LED_ON(RED_LED);
      break;

      default:
      break;
    }
  }
}
/*************************** End of file ****************************/
