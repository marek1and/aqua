#ifndef DS18B20_H
#define DS18B20_H

#define SET_ONEWIRE_PORT     PORTD  |=  _BV(PD6)
#define CLR_ONEWIRE_PORT     PORTD  &= ~_BV(PD6)
#define IS_SET_ONEWIRE_PIN   PIND   &   _BV(PD6)
#define SET_OUT_ONEWIRE_DDR  DDRD   |=  _BV(PD6)
#define SET_IN_ONEWIRE_DDR   DDRD   &= ~_BV(PD6)

#define NUM_OF_THERMS        2
unsigned char ROM[NUM_OF_THERMS][8];

unsigned char ds18b20_ConvertT(void);
int ds18b20_Read();
void OneWireStrong(char);
unsigned char OneWireReset(void);
void OneWireWriteByte(unsigned char);
unsigned char OneWireReadByte(void);
void OneWireWriteBit(unsigned char);
unsigned char OneWireReadBit(void);
void OneWireSearchRom(void);
int ds18b20_SetResolution(char resolution);
void readTemperatures();
double* getTemperatures();
void OneWireInit();

#endif
