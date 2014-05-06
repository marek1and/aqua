#ifndef RELAY_H_
#define RELAY_H_

#include <stdint.h>

#define RELAY1	3
#define RELAY2	4
#define RELAY3	5
#define RELAY4	6
#define RELAY5	7

#define ON      1
#define OFF     0

uint8_t readRelays();
void setPCF8574(uint8_t state);
void setRelay(uint8_t relay, uint8_t state);
void negateRelay(uint8_t relay);
void setAllRelays();
void clearAllRelays();

#endif
