#ifndef BACKLIGHT_H_
#define BACKLIGHT_H_

#include <stdint.h>

void setLCDBacklight(uint8_t val);
uint8_t getLCDBackLight();
void increaseLCDBacklight();
void decreaseLCDBacklight();
void saveLCDBacklighToMemory();
void loadLCDBacklightFromMemory();

#endif /* BACKLIGHT_H_ */
