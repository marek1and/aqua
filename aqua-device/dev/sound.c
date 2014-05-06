#include "sound.h"
#include "avr/io.h"
#include <util/delay.h>
#include <stdlib.h>

static uint8_t playing;
static uint8_t currentMelody;
static uint8_t repeat;

int beeper[][2] = {
					{3400, 200 },
					{0   , 1000}};

int koziolek[][2] = {
                    {523,125},
                    {587,125},
                    {659,250},
                    {698,125},
                    {659,125},
                    {587,250},
                    {523,250},
                    {1047,250},
                    {784,250},
                    {523,250},
                    {1047,250},
                    {784,250},
                    {523,250},
                    {1047,250},
                    {784,1000}};

void soundInit(){
	BUZZER_PORT_DDR |= BUZZER_PORT_OUT;
	playing = NO_PLAYING;
	currentMelody = MELODY_STOP;
	repeat = 0;
}

void setMelody(uint8_t melody) {
	currentMelody = melody;
}

void setPlaying(uint8_t p) {
	playing = p;
}

void setRepeatCount(uint8_t r) {
	repeat = r;
}

uint8_t isPlaying(){
	return playing;
}

void beep(unsigned int frequency, unsigned int duration) {
  unsigned int i,t,n;
  t = 125000/frequency;
  n = (250UL*duration)/t;

  BUZZER_PORT |= _BV(BUZZER_PIN1);
  BUZZER_PORT &= ~_BV(BUZZER_PIN2);
  for(i=0; i < n; i++) {
    BUZZER_PORT ^= _BV(BUZZER_PIN1);
    BUZZER_PORT ^= _BV(BUZZER_PIN2);
    _delay_loop_2(t*16);
  }
}

void playNotes(int nots[][2], uint8_t len) {

   for(int n = 0; n < len; n++) {
	   beep(nots[n][0], nots[n][1]);
   }
}

void stopPlay(){
	playing = NO_PLAYING;
	currentMelody = MELODY_STOP;
	repeat = 0;
	BUZZER_PORT &= ~_BV(BUZZER_PIN1);
	BUZZER_PORT &= ~_BV(BUZZER_PIN2);
}

void play() {

	if(repeat == 0){
		return;
	}

	do {
		if(playing == PLAYING){
			switch(currentMelody) {
				case MELODY_1:
					playNotes(beeper, 2);
					break;
				case MELODY_2:
					playNotes(koziolek, 15);
					break;
			}
		} else {
			break;
		}
		repeat--;
	} while(repeat > 0);
	stopPlay();
}
