#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>

#define CALENDAR_START_YEAR 	2012

void clockInit();
void readTime(void);
void writeTime(unsigned char seconds,unsigned char minutes, unsigned char hours);
void writeDate(unsigned char day,unsigned char month, uint8_t year);
uint8_t *getTime(void);

uint8_t getHour();
uint8_t getMinute();
uint8_t getSeconds();
uint8_t getDay();
uint8_t getMonth();
uint16_t getYear();

#endif /* TIME_H_ */
