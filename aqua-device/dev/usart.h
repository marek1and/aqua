#include <stdlib.h>
#include <stdio.h>

#ifndef USART_H_
#define USART_H_

#define BAUD 9600
#define UBRR  F_CPU/BAUD/16-1

void USART_init();
void USART_transmit(uint8_t b);
uint8_t USART_receiveByte(void);
void USART_transmitBytes(uint8_t *bytes, uint8_t len);

#endif
