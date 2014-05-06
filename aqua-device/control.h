
#ifndef CONTROL_H_
#define CONTROL_H_

#include "settings/settings.h"
#include "settings/outputs.h"
#include <stdint.h>

void readOutputControlParametersFromMemory(uint8_t output);
void loadOutputStatesFromMemory();
void writeOutputStatesToMemory();
void loadControlParametersFromMemory();
uint8_t getOutputRelay(uint8_t output);
void setOutput(uint8_t output, uint8_t state);
void setOutputs(uint8_t s);
void negateOutput(uint8_t output);
uint8_t getStates();
uint8_t getState(uint8_t output);

void controlOutputs(double *temperatures, int *illuminances);

#endif /* CONTROL_H_ */
