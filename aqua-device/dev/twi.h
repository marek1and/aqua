#include <avr/io.h>

#define ACK 1
#define NOACK 0

void twistart(void);
void twistop(void);
void twiwrite(char data);
char twiread(char ack);
