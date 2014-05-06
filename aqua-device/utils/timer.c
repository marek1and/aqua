#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "timer.h"

volatile int timerCounter = 0;
static int timerTaskDelay = 0;

void cancelTimerTask(){
	TCCR1B &= (0 << CS12);
	TCCR1B &= (0 << CS10);
	TIMSK  &= (0 << OCIE1A);

	timerCounter = 0;
	timerTaskDelay = 0;
}

void scheduleTimerTask(int seconds, void (*task)(void)){

	cancelTimerTask();
	TCNT1H = 0x00;
	TCNT1L = 0x00;

	timerTaskDelay = seconds;
	timerTask = task;

	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);
	OCR1A   = 15625;
	TIMSK |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect){

	timerCounter++;

	if(timerCounter == timerTaskDelay){
		if(timerTask != NULL){
			timerTask();
		}
		timerTask = NULL;
		cancelTimerTask();
	}
}
