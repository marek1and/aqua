#include "twi.h"

void twistart(void){
	TWBR = 0x0C;
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void twistop(void){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while ((TWCR & (1<<TWSTO)));
}

void twiwrite(char data){
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

char twiread(char ack){
	TWCR = ack
			? ((1 << TWINT) | (1 << TWEN) | (1 << TWEA))
			: ((1 << TWINT) | (1 << TWEN)) ;
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}
