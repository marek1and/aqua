#include <avr/interrupt.h>
#include "../utils/utils.h"
#include "../dev/time.h"
#include "time-settings.h"
#include <stdint.h>

static uint8_t tmpTime[5];
static uint8_t tmpTimeIdx = 0;

uint8_t *getTmpTime(){
	return tmpTime;
}

uint8_t getTmpTimeIndex(){
	return tmpTimeIdx;
}

void initTimeSettings(){
	tmpTime[HOUR]   = getHour();
	tmpTime[MINUTE] = getMinute();
	tmpTime[DAY]    = getDay();
	tmpTime[MONTH]  = getMonth();
	tmpTime[YEAR]   = (uint8_t)(getYear() % 4);
}

void saveTimeSettings(){
	writeTime(tmpTime[HOUR],
			  tmpTime[MINUTE],
			  0);
	writeDate(tmpTime[DAY],
			  tmpTime[MONTH],
			  tmpTime[YEAR]);
}

void clearTimeSettings(){
	for(uint8_t i = 0; i < 5; i++){
		tmpTime[i] = 0;
	}
	tmpTimeIdx = 0;
}

void nextTimeParam(){
	if(tmpTimeIdx == YEAR){
		tmpTimeIdx = HOUR;
	}else {
		tmpTimeIdx++;
	}
}

void increaseTimeParam(){

	uint8_t numOfDaysInMonth = 31;

	switch(tmpTimeIdx){
		case HOUR:
			if(tmpTime[HOUR] >= 23){
				tmpTime[HOUR] = 0;
			} else {
				tmpTime[HOUR]++;
			}
			break;
		case MINUTE:
			if(tmpTime[MINUTE] >= 59){
				tmpTime[MINUTE] = 0;
			} else {
				tmpTime[MINUTE]++;
			}
			break;
		case DAY:
			if(tmpTime[MONTH] == 4 || tmpTime[MONTH] == 6 ||
			   tmpTime[MONTH] == 9 || tmpTime[MONTH] == 11){
				numOfDaysInMonth = 30;
			} else if(tmpTime[MONTH] == 2){
				if((tmpTime[YEAR] % 4) == 0){
					numOfDaysInMonth = 29;
				} else {
					numOfDaysInMonth = 28;
				}
			}
			if(tmpTime[DAY] >= numOfDaysInMonth){
				tmpTime[DAY] = 1;
			} else {
				tmpTime[DAY]++;
			}

			break;
		case MONTH:
			if(tmpTime[MONTH] >= 12){
				tmpTime[MONTH] = 1;
			} else {
				tmpTime[MONTH]++;
			}
			break;
		case YEAR:
			if(tmpTime[YEAR] == 3){
				tmpTime[YEAR] = 0;
			} else {
				tmpTime[YEAR]++;
			}
			break;
	}
}
