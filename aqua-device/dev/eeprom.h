#ifndef EEPROM_H_
#define EEPROM_H_

#include "twi.h"

#define MEMORY_MAX_ADDRESS 	0x7FFF


uint8_t readDataFromMemory(uint16_t address);
void readDataBlockFromMemory(uint16_t address, uint8_t *data, int length);
void writeDataToMemory(uint16_t address, uint8_t data);
void writeDataBlockToMemory(uint16_t address, uint8_t *data, int length);
void clearEEPROM();

#endif /* EEPROM_H_ */
