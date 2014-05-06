#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <stdint.h>

#define CONNECTION_PARAM_IP		1
#define CONNECTION_PARAM_PORT	2

#define IP_A		1
#define IP_B		2
#define IP_C		3
#define IP_D		4

void initConnectionSettings();
void clearConnectionSetings();
void nextConnectionParam();
void prevConnectionParam();
uint8_t getConnectionParam();

uint8_t *getConnectionIP();
void setConnectionIP(uint8_t *ip);
uint8_t getConnectionPort();
void setConnectionPort(uint8_t port);

void increaseConnectionIPIdx();
void increaseConnectionParam();
void decreaseConnectionParam();

void loadConnectionParamsFromMemory();
void saveConnectionParamsToMemory();

#endif /* CONNECTION_H_ */
