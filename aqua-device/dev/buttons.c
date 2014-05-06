#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "buttons.h"

void buttonsInit(void){
	DDRC = 0x0F;
	PORTC = 0xFF;
}

void readButton(uint8_t button, void (*press)(uint8_t), void (*release)(uint8_t)){
	if(!(PINC & button)){
		_delay_ms(80);
		press(button);
		while(!(PINC & button)) { wdt_reset(); }
		_delay_ms(80);
		release(button);
	}
}
