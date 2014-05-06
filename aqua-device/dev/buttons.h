#ifndef BUTTONS_H_
#define BUTTONS_H_

#define S1 	 0x80
#define S2 	 0x40
#define S3 	 0x20
#define S4 	 0x10

void buttonsInit(void);
void readButton(uint8_t button, void (*press)(uint8_t), void (*release)(uint8_t));

#endif
