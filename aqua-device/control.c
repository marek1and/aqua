#include "settings/settings.h"
#include "settings/outputs.h"
#include "dev/eeprom.h"
#include "dev/relay.h"
#include <stdint.h>
#include <util/delay.h>

#include "dev/time.h"

static uint8_t 	outputModes[OUTPUTS_NUMBER];
static uint8_t 	outputTimes[OUTPUTS_NUMBER][4];
static double 	outputTemperatures[OUTPUTS_NUMBER];
static double 	outputHysteresis[OUTPUTS_NUMBER];
static uint16_t outputLuminance[OUTPUTS_NUMBER];
static uint8_t states;


void readOutputControlParametersFromMemory(uint8_t output){

	_delay_ms(5);
	outputModes[output - 1] = readDataFromMemory(OUTPUT1_MODE + (output - 1));
	if(outputModes[output - 1] == 0){
		outputModes[output - 1] = OUT_MODE_MANUAL;
	}
	_delay_ms(5);
	readDataBlockFromMemory(OUTPUT1_FROM_HOUR + ((output - 1) * 4), outputTimes[output - 1], 4);
	_delay_ms(5);

	uint8_t array[4];
	readDataBlockFromMemory(OUTPUT1_TEMPERATURE + ((output - 1) * 4), array, 4);
	double *f = (double *)&array[0];
	outputTemperatures[output - 1] =  *f;
	_delay_ms(5);

	readDataBlockFromMemory(OUTPUT1_HYSTERESIS + ((output - 1) * 4), array, 4);
	f = (double *)&array[0];
	outputHysteresis[output - 1] = *f;
	_delay_ms(5);

	uint8_t arr[2];
	readDataBlockFromMemory(OUTPUT1_LUMINANCE + ((output - 1) * 2), arr, 2);
	uint16_t *g = (uint16_t *)&arr[0];
	outputLuminance[output - 1] =  *g;
	_delay_ms(5);
}

void loadOutputStatesFromMemory(){
	states = readDataFromMemory(OUTPUTS_STATE);
	_delay_ms(5);
	setPCF8574(states);
}

void writeOutputStatesToMemory(){
	states = readRelays();
	_delay_ms(5);
	writeDataToMemory(OUTPUTS_STATE, states);
}

void loadControlParametersFromMemory(){
	for(uint8_t out = OUTPUT1; out <= OUTPUT5; out++){
		readOutputControlParametersFromMemory(out);
	}
	loadOutputStatesFromMemory();
	_delay_ms(5);
}

uint8_t getOutputRelay(uint8_t output){
	return output + 2;
}

void setOutput(uint8_t output, uint8_t state){

	if(((states >> (getOutputRelay(output))) & 0x01) != (~(state) & 0x01)){
		setRelay(getOutputRelay(output), state);
		_delay_ms(5);
		writeOutputStatesToMemory();
		_delay_ms(5);
	}
}

void setOutputs(uint8_t s){
	_delay_ms(5);
	setPCF8574(~s);
	_delay_ms(5);
	writeOutputStatesToMemory();
}

uint8_t getStates(){
	return ~states;
}

uint8_t getState(uint8_t output){
	return (~(states >> (getOutputRelay(output)))) & 0x01;
}

void negateOutput(uint8_t output){
	_delay_ms(5);
	negateRelay(getOutputRelay(output));
	_delay_ms(5);
	writeOutputStatesToMemory();
}

void controlOutput(uint8_t output, double *temperatures, int *illuminances){

	switch(outputModes[output - 1]){
		case OUT_MODE_MANUAL:
			break;
		case OUT_MODE_ON:
			setOutput(output, ON);
			break;
		case OUT_MODE_OFF:
			setOutput(output, OFF);
			break;
		case OUT_MODE_TIMER:

			if(outputTimes[output - 1][0] == getHour()){
				if(outputTimes[output - 1][1] == getMinute()){
					if(getSeconds() >= 0 || getSeconds() < 5){
						setOutput(output, ON);
					}
				}
			}

			if(outputTimes[output -1 ][2] == getHour()){
				if(outputTimes[output - 1][3] == getMinute()){
					if(getSeconds() >= 0 || getSeconds() < 5){
						setOutput(output, OFF);
					}
				}
			}

			break;
		case OUT_MODE_TEMP1_L:

			if(temperatures[0] <= outputTemperatures[output - 1]){
				setOutput(output, ON);
			}else if(temperatures[0] >= (outputTemperatures[output - 1] + outputHysteresis[output - 1])){
				setOutput(output, OFF);
			}

			break;
		case OUT_MODE_TEMP1_G:

			if(temperatures[0] >= outputTemperatures[output - 1]){
				setOutput(output, ON);
			}else if(temperatures[0] <= (outputTemperatures[output - 1] - outputHysteresis[output - 1])){
				setOutput(output, OFF);
			}

			break;
		case OUT_MODE_TEMP2_L:

			if(temperatures[1] <= outputTemperatures[output - 1]){
				setOutput(output, ON);
			}else if(temperatures[1] >= (outputTemperatures[output - 1] + outputHysteresis[output - 1])){
				setOutput(output, OFF);
			}

			break;
		case OUT_MODE_TEMP2_G:

			if(temperatures[1] >= outputTemperatures[output - 1]){
				setOutput(output, ON);
			}else if(temperatures[1] <= (outputTemperatures[output - 1] - outputHysteresis[output - 1])){
				setOutput(output, OFF);
			}

			break;
		case OUT_MODE_LUM1_L:

			if(illuminances[0] <= outputLuminance[output - 1]){
				setOutput(output, ON);
			} else {
				setOutput(output, OFF);
			}

			break;
		case OUT_MODE_LUM1_G:

			if(illuminances[0] >= outputLuminance[output - 1]){
				setOutput(output, ON);
			} else {
				setOutput(output, OFF);
			}

			break;
		case OUT_MODE_LUM2_L:

			if(illuminances[1] <= outputLuminance[output - 1]){
				setOutput(output, ON);
			}else {
				setOutput(output, OFF);
			}

			break;
		case OUT_MODE_LUM2_G:

			if(illuminances[1] >= outputLuminance[output - 1]){
				setOutput(output, ON);
			}else {
				setOutput(output, OFF);
			}

			break;
	}
}

void controlOutputs(double *temperatures, int *illuminances){
	for(uint8_t out = OUTPUT1; out <= OUTPUT5; out++){
		controlOutput(out, temperatures, illuminances);
	}
}

