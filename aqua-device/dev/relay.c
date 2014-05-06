#include "twi.h"
#include "relay.h"
#include <stdint.h>

uint8_t readRelays(){
	uint8_t state;
	twistart();
	twiwrite(0x75);
	state = twiread(NOACK);
	twistop();
	return state;
}

void setPCF8574(uint8_t state){
	twistart();
	twiwrite(0x74);
	twiwrite(state);
	twistop();
}

void setRelay(uint8_t relay, uint8_t state){

	uint8_t st = readRelays();

	if(state){
		st &= ~(1 << relay);
	} else {
		st |= (1 << relay);
	}
	setPCF8574(st);
}

void negateRelay(uint8_t relay){
	uint8_t st = readRelays();
	st ^= (1 << relay);
	setPCF8574(st);
}

void setAllRelays(){
	setPCF8574(0x00);
}

void clearAllRelays(){
	setPCF8574(0xFF);
}
