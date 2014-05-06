#include "twi.h"
#include "eeprom.h"
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t readDataFromMemory(uint16_t address){

	uint8_t lowAddr = address & 0x00FF;
	uint8_t highAddr = (address >> 8) & 0x00FF;
	uint8_t data;

	cli();
	twistart();
	twiwrite(0xA2);
	twiwrite(highAddr);
	twiwrite(lowAddr);
	twistart();
	twiwrite(0xA3);
	data = twiread(NOACK);
	twistop();
	sei();

	return data;
}

void readDataBlockFromMemory(uint16_t address, uint8_t *data, int length) {

	uint8_t lowAddr = address & 0x00FF;
	uint8_t highAddr = (address >> 8) & 0x00FF;

	cli();
	twistart();
	twiwrite(0xA2);
	twiwrite(highAddr);
	twiwrite(lowAddr);
	twistart();
	twiwrite(0xA3);
	for(int i = 0; i < length; i++){
		if(i == length -1){
			data[i] = twiread(NOACK);
		}else {
			data[i] = twiread(ACK);
		}
	}
	twistop();
	sei();
}

void writeDataToMemory(uint16_t address, uint8_t data){

	uint8_t lowAddr = address & 0x00FF;
	uint8_t highAddr = (address >> 8) & 0x00FF;

	twistart();
	twiwrite(0xA2);
	twiwrite(highAddr);
	twiwrite(lowAddr);
	twiwrite(data);
	twistop();
}

void writeDataBlockToMemory(uint16_t address, uint8_t *data, int length){

	uint8_t lowAddr = address & 0x00FF;
	uint8_t highAddr = (address >> 8) & 0x00FF;

	twistart();
	twiwrite(0xA2);
	twiwrite(highAddr);
	twiwrite(lowAddr);
	for(int i = 0; i < length; i++){
		twiwrite(data[i]);
	}
	twistop();
}

void clearEEPROM() {
	twistart();
	twiwrite(0xA2);
	twiwrite(0x00);
	twiwrite(0x00);
	for(uint16_t i = 0; i < MEMORY_MAX_ADDRESS; i++){
		twiwrite(0x00);
	}
	twistop();
}
