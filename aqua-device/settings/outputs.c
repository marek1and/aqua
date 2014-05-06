#include "outputs.h"
#include "settings.h"
#include "../dev/eeprom.h"
#include <util/delay.h>

static uint8_t currentOutput = 0;

static uint8_t currentOutputMode;
static uint8_t currentOutputTime[4];
static uint8_t paramIdx = 0;
static double currentOutputTemperature;
static double currentOutputHysteresis;
static uint16_t currentOutputLuminance;

void initOutputsSettings(){
	currentOutput = OUTPUT1;
}

void clearOutputsSettings(){
	currentOutput = 0;
}

void increaseCurrentOutput(){
	if(currentOutput == OUTPUT5){
		currentOutput = OUTPUT1;
	} else {
		currentOutput++;
	}
}

void decreaseCurrentOutput(){
	if(currentOutput == OUTPUT1){
		currentOutput = OUTPUT5;
	} else {
		currentOutput--;
	}
}

uint8_t getCurrentOutput(){
	return currentOutput;
}

void setCurrentOutput(uint8_t currOutput){
	currentOutput = currOutput;
}

uint8_t getCurrentOutputMode(){
	return currentOutputMode;
}

void setCurrentOutputMode(uint8_t mode){
	currentOutputMode = mode;
}

uint8_t *getCurrentOutputTime(){
	return &currentOutputTime[0];
}

void setCurrentOutputTime(uint8_t *time){
	for(uint8_t i = 0; i < 4; i++){
		currentOutputTime[i] = time[i];
	}
}

double getCurrentOutputTemperature(){
	return currentOutputTemperature;
}

void setCurrentOutputTemperature(double temperature){
	currentOutputTemperature = temperature;
}

double getCurrentOutputHysteresis(){
	return currentOutputHysteresis;
}

void setCurrentOutputHysteresis(double hysteresis){
	currentOutputHysteresis = hysteresis;
}

uint16_t getCurrentOutputLuminance(){
	return currentOutputLuminance;
}

void setCurrentOutputLuminance(uint16_t luminance){
	currentOutputLuminance = luminance;
}

void initOutputXSettings(){
	paramIdx = OUT_MODE;
	currentOutputMode = readDataFromMemory(OUTPUT1_MODE + (currentOutput - 1));
	if(currentOutputMode == 0){
		currentOutputMode = OUT_MODE_MANUAL;
	}
	_delay_ms(5);
	readDataBlockFromMemory(OUTPUT1_FROM_HOUR + ((currentOutput - 1) * 4), currentOutputTime, 4);
	_delay_ms(5);

	uint8_t array[4];
	readDataBlockFromMemory(OUTPUT1_TEMPERATURE + ((currentOutput - 1) * 4), array, 4);
	double *f = (double *)&array[0];
	currentOutputTemperature =  *f;
	if(currentOutputTemperature < MIN_TEMPERATURE || isnan(currentOutputTemperature)){
		currentOutputTemperature = MIN_TEMPERATURE;
	} else if (currentOutputTemperature > MAX_TEMPERATURE){
		currentOutputTemperature = MAX_TEMPERATURE;
	}
	_delay_ms(5);

	readDataBlockFromMemory(OUTPUT1_HYSTERESIS + ((currentOutput - 1) * 4), array, 4);
	f = (double *)&array[0];
	currentOutputHysteresis = *f;
	if(currentOutputHysteresis > MAX_HYSTERESIS){
		currentOutputHysteresis = MAX_HYSTERESIS;
	}else if(isnan(currentOutputHysteresis)){
		currentOutputHysteresis = 0;
	}

	_delay_ms(5);

	uint8_t arr[2];
	readDataBlockFromMemory(OUTPUT1_LUMINANCE + ((currentOutput - 1) * 2), arr, 2);
	uint16_t *g = (uint16_t *)&arr[0];
	currentOutputLuminance =  *g;
	if(currentOutputLuminance > MAX_LUMINANCE){
		currentOutputLuminance = MAX_LUMINANCE;
	}
}

void clearOutputXSettings(){

	for(int i = 0; i < OUT_TO_MINUTE; i++){
		currentOutputTime[i] = 0;
	}
	currentOutputMode = 0;
	currentOutputTemperature = 0;
	currentOutputHysteresis = 0;
	currentOutputLuminance = 0;
	paramIdx = 0;
}

void saveOutputSettings(){
	_delay_ms(5);
	writeDataBlockToMemory(OUTPUT1_FROM_HOUR + ((currentOutput - 1) * 4), currentOutputTime, 4);
	_delay_ms(5);
	writeDataToMemory(OUTPUT1_MODE + (currentOutput - 1), currentOutputMode);
	_delay_ms(5);
	writeDataBlockToMemory(OUTPUT1_TEMPERATURE + ((currentOutput - 1) * 4), (uint8_t *)&currentOutputTemperature, sizeof(double));
	_delay_ms(5);
	writeDataBlockToMemory(OUTPUT1_HYSTERESIS + ((currentOutput - 1) * 4), (uint8_t *)&currentOutputHysteresis, sizeof(double));
	_delay_ms(5);
	writeDataBlockToMemory(OUTPUT1_LUMINANCE + ((currentOutput - 1) * 2), (uint8_t *)&currentOutputLuminance, sizeof(uint16_t));

}

void increaseOutputParamIdx(){
	switch(currentOutputMode){
		case OUT_MODE_TIMER:
			if(paramIdx == OUT_MODE){
				paramIdx = OUT_FROM_HOUR;
			} else {
				paramIdx++;
			}
			break;
		case OUT_MODE_TEMP1_G:
		case OUT_MODE_TEMP1_L:
		case OUT_MODE_TEMP2_G:
		case OUT_MODE_TEMP2_L:
			if(paramIdx == OUT_HYSTERESIS){
				paramIdx = OUT_MODE;
			} else {
				paramIdx++;
			}
			break;
		case OUT_MODE_LUM1_G:
		case OUT_MODE_LUM1_L:
		case OUT_MODE_LUM2_G:
		case OUT_MODE_LUM2_L:
			if(paramIdx == OUT_LUMINANCE){
				paramIdx = OUT_MODE;
			}else{
				paramIdx = OUT_LUMINANCE;
			}
			break;
		default:
			paramIdx = OUT_MODE;
	}
}

void increaseOutputParam(){
	switch(paramIdx){
		case OUT_FROM_HOUR:
		case OUT_TO_HOUR:
			if(currentOutputTime[paramIdx] >= 23){
				currentOutputTime[paramIdx] = 0;
			} else {
				currentOutputTime[paramIdx]++;
			}
			break;
		case OUT_FROM_MINUTE:
		case OUT_TO_MINUTE:
			if(currentOutputTime[paramIdx] >= 59){
				currentOutputTime[paramIdx] = 0;
			} else {
				currentOutputTime[paramIdx]++;
			}
			break;
		case OUT_MODE:
			if(currentOutputMode == OUT_MODE_LUM2_G){
				currentOutputMode = OUT_MODE_MANUAL;
			}else {
				currentOutputMode++;
			}
			break;
		case OUT_TEMPERATURE:
			if(currentOutputTemperature >= MAX_TEMPERATURE){
				currentOutputTemperature = MIN_TEMPERATURE;
			} else{
				currentOutputTemperature += TEMPERATURE_STEP;
			}
			break;
		case OUT_HYSTERESIS:
			if(currentOutputHysteresis >= MAX_HYSTERESIS){
				currentOutputHysteresis = 0;
			} else {
				currentOutputHysteresis += HYSTERESIS_STEP;
			}
			break;
		case OUT_LUMINANCE:
			if(currentOutputLuminance >= MAX_LUMINANCE){
				currentOutputLuminance = 0;
			} else {
				currentOutputLuminance += LUMINANCE_STEP;
			}
	}
}
