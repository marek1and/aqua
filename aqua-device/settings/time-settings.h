#ifndef TIMESETTINGS_H_
#define TIMESETTINGS_H_

#include <stdint.h>

#define HOUR		0
#define	MINUTE		1
#define	DAY			2
#define	MONTH		3
#define YEAR		4

uint8_t *getTmpTime();
uint8_t getTmpTimeIndex();
void initTimeSettings();
void saveTimeSettings();
void clearTimeSettings();
void nextTimeParam();
void increaseTimeParam();

#endif /* TIMESETTINGS_H_ */
