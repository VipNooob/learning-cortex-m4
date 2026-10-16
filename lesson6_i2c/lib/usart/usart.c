#include "stm32f407xx.h"
#include "usart.h"

// 115200, 8N1
// PA2 - TX
// PA3 - RX
void USART2_Init(){
  // APB1 = 42MHz
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
 
  USART2->CR1 |= USART_CR1_UE; // Enable USART2
  USART2->CR1 &= ~(USART_CR1_OVER8 | USART_CR1_M | USART_CR1_PCE); // Oversampling = 16, datalength = 8 bit, parity control = off
  USART2->CR1 |= USART_CR1_RXNEIE; // Enable RXNE interrupt
  USART2->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1); // Stopbits = 1
  // USARTDIV = F_APB1 / (16 * UART_BAUD)
  // USARTDIV = 22,786458(3), where mantissa = 22 and fraction = 13
  // USARTDIV = 22.8125, so error = 0.11% 
  USART2->BRR = (0x16 << 4) | 0x13; 
  USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE); // Enable TX and RX lines
  
  NVIC_EnableIRQ(USART2_IRQn);

  // Setup associated GPIO pins: PA2-TX, PA3-RX
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN);
  GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1); // switch PA2, PA3 to alternate function mode
  GPIOA->AFR[0] |= ((GPIO_AFRL_AFSEL3_2 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_0) | (GPIO_AFRL_AFSEL2_2 | GPIO_AFRL_AFSEL2_1| GPIO_AFRL_AFSEL2_0));
}

void Send_Byte(uint8_t ch){
  // Wait until transmit data register is empty
  while ((USART2->SR & USART_SR_TXE) == 0);
  USART2->DR = ch;
}

void Send_Buffer(uint8_t* buf, uint16_t len){
  for (uint16_t i = 0; i < len; i++){
    Send_Byte(buf[i]);
  }
}

void Send_ConstString(char* ch){
  while (*ch) {
    Send_Byte(*ch);
    ch++;
  }
}