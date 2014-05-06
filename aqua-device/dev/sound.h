#ifndef SOUND_H_
#define SOUND_H_

#include <avr/io.h>

#define BUZZER_PORT			PORTD
#define BUZZER_PORT_DDR		DDRD
#define BUZZER_PORT_OUT		0x30
#define BUZZER_PIN1			PD4
#define BUZZER_PIN2			PD5

#define MELODY_STOP			0
#define MELODY_1			1
#define MELODY_2			2
#define MELODY_3			3
#define MELODY_4			4
#define MELODY_5			5
#define MELODY_6			6
#define MELODY_7			7

#define PLAYING				1
#define NO_PLAYING			0

void soundInit();
void setMelody(uint8_t melody);
void setPlaying(uint8_t p);
void setRepeatCount(uint8_t r);
uint8_t isPlaying();
void play();

#endif /* SOUND_H_ */
