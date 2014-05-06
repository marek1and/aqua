#include <avr/interrupt.h>
#include "twi.h"
#include "../utils/utils.h"
#include "time.h"
#include <stdint.h>

static uint8_t time[11];

void clockInit(){
	cli();
	twistart();
	twiwrite(0xA0);
	twiwrite(0x00);
	twiwrite(0x00);
	twistop();
	sei();
}

void readTime(void) {
	uint8_t pcfTime[5];
	cli();
	twistart();
	twiwrite(0xA0);
	twiwrite(0x02);
	twistart();
	twiwrite(0xA1);
	pcfTime[2]=twiread(ACK);
	pcfTime[1]=twiread(ACK);
	pcfTime[0]=twiread(ACK);
	pcfTime[3]=twiread(ACK);
	pcfTime[4]=twiread(NOACK);
	twistop();
	int j=0;
	for(uint8_t i=0;i<5;i++){
		if(i == 3) {
			time[10] = (pcfTime[i] >> 6) & 0x03;
			pcfTime[i] &= 0x3F;
		} else if(i == 4) {
			pcfTime[i] &= 0x1F;
		}
		time[j] = pcfTime[i];
		time[j] >>= 4 & 0x0F;
		j++;
		time[j]=pcfTime[i];
		time[j]&=0x0F;
		j++;
	}
	sei();
}

void writeTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {

	twistart();
	twiwrite(0xA0);
	twiwrite(0x02);
	twiwrite(intToBCD(seconds));
	twiwrite(intToBCD(minutes));
	twiwrite(intToBCD(hours));
	twistop();
}

void writeDate(uint8_t day, uint8_t month, uint8_t year) {

	twistart();
	twiwrite(0xA0);
	twiwrite(0x05);
	twiwrite(intToBCD(day) | ((year << 6) & 0xC0));
	twiwrite(intToBCD(month));
	twistop();
}

uint8_t *getTime(){
	return time;
}

uint8_t getHour(){
	return time[0] * 10 +  time[1];
}

uint8_t getMinute(){
	return time[2] * 10 +  time[3];
}

uint8_t getSeconds(){
	return time[4] * 10 +  time[5];
}

uint8_t getDay(){
	return time[6] * 10 +  time[7];
}

uint8_t getMonth(){
	return time[8] * 10 +  time[9];
}

uint16_t getYear(){
	return CALENDAR_START_YEAR + time[10];
}
