#include "adc.h"
#include <avr/io.h>
#include <math.h>
#include <avr/interrupt.h>
#include <stdint.h>

static int illuminances[2];
static uint8_t currentRes = PHOTORESISTOR1;
static uint8_t adcEnabled = ADC_DISABLED;

uint8_t isADCEnabled(){
	return adcEnabled;
}

void setADCEnabled(uint8_t state){
	adcEnabled = state;
	if(adcEnabled == ADC_ENABLED){
		ADCSRA |= (1 << ADEN);
	}else {
		ADCSRA &= ~(1 << ADEN);
	}
}

void adcInit(void){
	DDRA &= ~_BV(PA6) & ~_BV(PA7);
	PORTA &= ~_BV(PA6) & ~_BV(PA7);

	ADMUX  |= (0 << REFS1) | (1 << REFS0) | (6 << MUX0);
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | ( 7 << ADPS0);

	adcEnabled = ADC_ENABLED;
}

void adcConvert(void){
	if(adcEnabled == ADC_ENABLED){
		if(currentRes == PHOTORESISTOR1){
			ADMUX &= ~(1<<MUX0);
		}
		ADMUX  |= (currentRes << MUX0);
		ADCSRA |= (1 << ADSC) | (1 << ADIE);
	}
}

double adcRead(){
	if(adcEnabled == ADC_ENABLED){
		uint16_t adc_value;
		adc_value = ADCL;
		adc_value += (ADCH << 8);
		double voltage= 5.0 * ((double)adc_value / 1024.0);
		double resistance = (5 * 5.0) / voltage - 5;
		return (255.84 * pow(resistance, 10/9));
	}
	return 0;
}

int* getIlluminances(){
	return &illuminances[0];
}

ISR(ADC_vect){

	illuminances[currentRes - PHOTORESISTOR1] = adcRead();

	if(currentRes == PHOTORESISTOR1){
		currentRes = PHOTORESISTOR2;
	}else {
		currentRes = PHOTORESISTOR1;
	}
}
