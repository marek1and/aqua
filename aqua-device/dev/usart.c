#include "usart.h"
#include <avr/io.h>
#include <stdint.h>

void USART_init(){
	unsigned int ubrr = UBRR;
    UBRRH = (unsigned char)(ubrr>>8);
    UBRRL = (unsigned char)ubrr;
    UCSRB = (1<<RXEN)|(1<<TXEN)|(1 << RXCIE);
    UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void USART_transmit(uint8_t b){
    while(!(UCSRA & (1<<UDRE)));
    UDR = b;
}

uint8_t USART_receiveByte(void){
	while(!(UCSRA&(1<<RXC)));
	return UDR;
}

void USART_transmitBytes(uint8_t *bytes, uint8_t len){
	for(uint8_t i=0; i < len; i++){
		USART_transmit(bytes[i]);
	}
}
