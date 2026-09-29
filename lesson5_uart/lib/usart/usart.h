#pragma once
#include "stdio.h"


void USART2_Init();
void Send_Byte(uint8_t ch);
void Send_Buffer(uint8_t* buf, uint16_t len);
void Send_ConstString(char* ch);