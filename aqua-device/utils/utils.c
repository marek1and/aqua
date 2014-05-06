#include <stdint.h>
#include "utils.h"

char intToBCD(uint8_t num){
	return (((num / 10) << 4) & 0xF0) + ((num % 10) & 0x0F);
}

uint8_t bcdToInt(char bcd){
	return ((0x0F & (bcd >> 4)) * 10) + (0x0F & bcd);
}
