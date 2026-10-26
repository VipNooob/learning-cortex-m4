#include "i2c.h"
#include "stm32f407xx.h"


// TODO: add error handler and timer instead of while()  


void i2c_init(void){
  // Setup GPIO pins
  // PB6 -SCL, PB9 - SDA
  // Switch on clock on specified GPIO
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOBEN);
  GPIOB->MODER |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER9_1); // alternate function
  GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2; // PB6 (AF4)
  GPIOB->AFR[1] |= GPIO_AFRH_AFSEL9_2; // PB9 (AF4)

  RCC->AHB1ENR |= RCC_APB1ENR_I2C1EN;
  I2C1->CR2 |= (I2C_CR2_FREQ_4 | I2C_CR2_FREQ_5); // Set peripheral clock frequency equals to APB1 clock frequency (42 MHz)
  I2C1->CCR |= (210 << I2C_CCR_CCR_Pos); // Standard mode, CCR = 210
  I2C1->TRISE |= (43 << I2C_TRISE_TRISE_Pos); // Set T_rise - the time mcu should wait before make sure that SCL is high and start counting until CCR
  I2C1->CR1 |= I2C_CR1_PE; // Enable I2C module
}

void i2c_writeBytes(uint8_t addr, uint16_t reg_addr, uint8_t* array, uint16_t length){
  // Wait if I2C1 is busy
  while(I2C1->SR2 & I2C_SR2_BUSY);
  
  I2C1->CR1 |= I2C_CR1_START; // Send START condition
  
  // Wait until Start bit is set (SB=1)
  while ((I2C1->SR1 & I2C_SR1_SB) == 0);
  (void)I2C1->CR1; // Dummy CR1 read in order to clear SB flag 
  I2C1->DR = (addr << 1) | 1; // Send target address in write mode
  
  // Wait until Address bit is set (ADDR=1)
  while((I2C1->SR1 & I2C_SR1_ADDR) == 0);
  // Dummy SR1,SR2 read in order to clear ADDR flag 
  (void)I2C1->SR1; 
  (void)I2C1->SR2;
  
  // Send register address of the device
  while((I2C1->SR1 & I2C_SR1_TXE) == 0);
  I2C1->DR = (uint8_t)((reg_addr >> 8) & 0xFF00); // send first word address 
  while((I2C1->SR1 & I2C_SR1_TXE) == 0);
  I2C1->DR = (uint8_t)(reg_addr & 0xFF); // send second word address

  for (uint16_t i = 0; i < length; i++){
    // Wait until tx buffer is empty
    while((I2C1->SR1 & I2C_SR1_TXE) == 0);
    I2C1->DR = array[i]; // send data
  }

  // Wait until byte transfer finished flag is set
  while((I2C1->SR1 & I2C_SR1_BTF) == 0);
  I2C1->CR1 |= I2C_CR1_STOP;
}

void i2c_readBytes(uint8_t addr, uint16_t reg_addr, uint8_t* array, uint16_t length){
  // Wait if I2C1 is busy
  while(I2C1->SR2 & I2C_SR2_BUSY);
  
  I2C1->CR1 |= I2C_CR1_ACK; // Enable auto-acknowledge

  I2C1->CR1 |= I2C_CR1_START; // Send START condition
  
  // Wait until Start bit is set (SB=1)
  while ((I2C1->SR1 & I2C_SR1_SB) == 0);
  (void)I2C1->CR1; // Dummy CR1 read in order to clear SB flag 
  I2C1->DR = (addr << 1) | 1; // Send target address in write mode
  
  // Wait until Address bit is set (ADDR=1)
  while((I2C1->SR1 & I2C_SR1_ADDR) == 0);
  // Dummy SR1,SR2 read in order to clear ADDR flag 
  (void)I2C1->SR1; 
  (void)I2C1->SR2;
  
  // Send register address of the device
  while((I2C1->SR1 & I2C_SR1_TXE) == 0);
  I2C1->DR = (uint8_t)(reg_addr >> 8); // send first word address 
  while((I2C1->SR1 & I2C_SR1_TXE) == 0);
  I2C1->DR = (uint8_t)(reg_addr & 0xFF); // send second word address
  
  // Make sure that data was sent 
  while ((I2C1->SR1 & I2C_SR1_BTF) == 0);
  I2C1->CR1 |= I2C_CR1_START; // Send repeated start

  // Wait until Start bit is set (SB=1)
  while ((I2C1->SR1 & I2C_SR1_SB) == 0);
  (void)I2C1->CR1; // Dummy CR1 read in order to clear SB flag 
  I2C1->DR = (addr << 1); // Send target address in read mode

  // Wait until Address bit is set (ADDR=1)
  while((I2C1->SR1 & I2C_SR1_ADDR) == 0);
  
  // Three different cases should be considered in accordance to RM0090
  if (length == 1){
    I2C1->CR1 &= ~I2C_CR1_ACK; // NACK

    // Dummy SR1,SR2 read in order to clear ADDR flag 
    (void)I2C1->SR1; 
    (void)I2C1->SR2;

    I2C1->CR1 |= I2C_CR1_STOP; // Send STOP

    while ((I2C1->SR1 & I2C_SR1_RXNE) == 0);
    array[0] = I2C1->DR;
  }
  else if (length == 2){
    I2C1->CR1 &= ~I2C_CR1_ACK; // NACK
    I2C1->CR1 |= I2C_CR1_POS; // Set POS (ACK bit controls the (N)ACK of the next byte which is received in the shift register)
    
    // Dummy SR1,SR2 read in order to clear ADDR flag 
    (void)I2C1->SR1; 
    (void)I2C1->SR2;

    // Wait until BTF=1 (Data 1 in DR, Data2 in shift register)
    while ((I2C1->SR1 & I2C_SR1_BTF) == 0);
    I2C1->CR1 |= I2C_CR1_STOP; // Send STOP
    
    array[0] = I2C1->DR;
    array[1] = I2C1->DR;
  }
  else{
    // Dummy SR1,SR2 read in order to clear ADDR flag 
    (void)I2C1->SR1; 
    (void)I2C1->SR2;

    // Read before N-2 bytes as usual
    for (uint16_t i = 0; i < length - 3; i++){
      while((I2C1->SR1 & I2C_SR1_RXNE) == 0);
      array[i] = I2C1->DR;
    }
    
    // Wait until BTF=1 (data N-2 in DR, data N-1 in shift register)
    while ((I2C1->SR1 & I2C_SR1_BTF) == 0);
    I2C1->CR1 &= ~I2C_CR1_ACK; // NACK
    
    array[length - 3] = I2C1->DR; // Read data N-2

    // Wait until BTF=1 (data N-1 in DR, data N in shift register)
    while ((I2C1->SR1 & I2C_SR1_BTF) == 0);
    I2C1->CR1 |= I2C_CR1_STOP; // Send STOP

    array[length - 2] = I2C1->DR; // Read data N-1
    array[length - 1] = I2C1->DR; // Read data N
  }
}

