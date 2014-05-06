#include "backlight.h"
#include "settings.h"
#include "../dev/eeprom.h"
#include <stdint.h>


static uint8_t backlightValue = 128;

void setLCDBacklight(uint8_t val){
	backlightValue = val;
}

uint8_t getLCDBackLight(){
	return backlightValue;
}

void increaseLCDBacklight(){
	if(backlightValue < 240){
		backlightValue += 16;
	} else {
		backlightValue = 255;
	}
}

void decreaseLCDBacklight(){
	if(backlightValue == 255){
		backlightValue = 240;
	} else if(backlightValue >= 16){
		backlightValue -= 16;
	} else {
		backlightValue = 0;
	}
}

void saveLCDBacklighToMemory(){
	writeDataToMemory(LCD_BACKLIGHT_PARAM_ADDR, backlightValue);
}

void loadLCDBacklightFromMemory(){
	backlightValue = readDataFromMemory(LCD_BACKLIGHT_PARAM_ADDR);
}
