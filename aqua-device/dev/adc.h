#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#define ADC_ENABLED		1
#define ADC_DISABLED	0

#define PHOTORESISTOR1 6
#define PHOTORESISTOR2 7

uint8_t isADCEnabled();
void setADCEnabled(uint8_t adcEnabled);
void adcInit(void);
void adcConvert(void);
double adcRead();
int* getIlluminances();

#endif
