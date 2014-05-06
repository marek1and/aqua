#ifndef OUTPUTSTIMER_H_
#define OUTPUTSTIMER_H_

#include <stdint.h>

#define OUTPUTS_NUMBER		5

#define MIN_TEMPERATURE 	15.0
#define MAX_TEMPERATURE 	35.0
#define TEMPERATURE_STEP	0.1
#define MAX_HYSTERESIS 		2.0
#define HYSTERESIS_STEP		0.1
#define MAX_LUMINANCE		1000
#define LUMINANCE_STEP		5


#define OUTPUT1		1
#define OUTPUT2		2
#define OUTPUT3		3
#define OUTPUT4		4
#define OUTPUT5		5

#define OUT_FROM_HOUR		0
#define OUT_FROM_MINUTE		1
#define OUT_TO_HOUR			2
#define OUT_TO_MINUTE		3
#define OUT_MODE			4
#define OUT_TEMPERATURE		5
#define OUT_HYSTERESIS		6
#define OUT_LUMINANCE		7

//MODES
#define OUT_MODE_MANUAL		1
#define OUT_MODE_ON			2
#define OUT_MODE_OFF		3
#define OUT_MODE_TIMER		4
#define OUT_MODE_TEMP1_L	5
#define OUT_MODE_TEMP1_G	6
#define OUT_MODE_TEMP2_L	7
#define OUT_MODE_TEMP2_G	8
#define OUT_MODE_LUM1_L		9
#define OUT_MODE_LUM1_G		10
#define OUT_MODE_LUM2_L		11
#define OUT_MODE_LUM2_G		12


void initOutputsSettings();
void clearOutputsSettings();
void increaseCurrentOutput();
void decreaseCurrentOutput();

uint8_t getCurrentOutput();
void setCurrentOutput(uint8_t currOutput);
uint8_t getCurrentOutputMode();
void setCurrentOutputMode(uint8_t mode);
uint8_t *getCurrentOutputTime();
void setCurrentOutputTime(uint8_t *time);
double getCurrentOutputTemperature();
void setCurrentOutputTemperature(double temperature);
double getCurrentOutputHysteresis();
void setCurrentOutputHysteresis(double hysteresis);
uint16_t getCurrentOutputLuminance();
void setCurrentOutputLuminance(uint16_t luminance);

void initOutputXSettings();
void clearOutputXSettings();
void saveOutputSettings();
void increaseOutputParamIdx();
void increaseOutputParam();

#endif /* OUTPUTSTIMER_H_ */
