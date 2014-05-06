#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ds18b20.h"

static double temperatures[NUM_OF_THERMS];

unsigned char ds18b20_ConvertT(void){
	if (!OneWireReset()) return 0;
	OneWireWriteByte(0xcc); // SKIP ROM
	OneWireWriteByte(0x44); // CONVERT T
	return -1;
}

int ds18b20_Read(){
  for(int i=0;i < NUM_OF_THERMS;i++){
  		unsigned char scratchpad[2];
		if (!OneWireReset()) {return 0;}
		if(NUM_OF_THERMS > 1){
			OneWireWriteByte(0x55);
			for(int j=0;j<8;j++){
				OneWireWriteByte(ROM[i][j]);
			}
		} else{
			OneWireWriteByte(0xcc);
		}
  		OneWireWriteByte(0xBE);
  		for(int k=0; k<2; k++) scratchpad[k] = OneWireReadByte();
		temperatures[i]= ((scratchpad[1] << 8) + scratchpad[0]) / 16.0;
  }
  return 1;
}

void readTemperatures(){
	if(ds18b20_ConvertT()) {
    	_delay_ms(375);
       cli();
       ds18b20_Read();
       sei();
	}
}

double* getTemperatures(){
	return &temperatures[0];
}

int ds18b20_SetResolution(char resolution)
{
	if(resolution == 12){
		resolution = 0x7F;
	}
	else if(resolution == 11){
		 resolution = 0x5F;
	}
	else if(resolution == 10){
		 resolution = 0x3F;
	}
	else if(resolution == 9){
		  resolution = 0x1F;
	}
	else{
		 resolution = 0x7F;
	}

  for(int i=0;i < NUM_OF_THERMS;i++){
		if (!OneWireReset()) {return 0;}
		if(NUM_OF_THERMS > 1){
			OneWireWriteByte(0x55);
			for(int j=0;j<8;j++){
				OneWireWriteByte(ROM[i][j]);
			}
		} else{
			OneWireWriteByte(0xcc);
		}
  		OneWireWriteByte(0x4E);
  		OneWireWriteByte(0x00);
  		OneWireWriteByte(0x00);
  		OneWireWriteByte(resolution);

  		if (!OneWireReset()) {return 0;}
  		OneWireWriteByte(0x55);
  		for(int j=0;j<8;j++){
  			OneWireWriteByte(ROM[i][j]);
  		}
  		OneWireWriteByte(0x48);
  		_delay_ms(10);
  }
  return 1;
}

void OneWireStrong(char s)
{
  if (s)
  {
     SET_ONEWIRE_PORT; 
     SET_OUT_ONEWIRE_DDR; 
  }
  else
  {
     SET_IN_ONEWIRE_DDR; 
  }
}

unsigned char OneWireReset()
{
  CLR_ONEWIRE_PORT; 
  if (!(IS_SET_ONEWIRE_PIN)) return 0;
  SET_OUT_ONEWIRE_DDR; 
  _delay_us(500);
  SET_IN_ONEWIRE_DDR; 
  _delay_us(70);
  if(!(IS_SET_ONEWIRE_PIN))
  {
    _delay_us(500);
    return 1;
  }

  _delay_us(500);
return 0;
}

void OneWireWriteByte(unsigned char byte)
{
   unsigned char i;

   CLR_ONEWIRE_PORT; 

   for (i=0; i<8; i++)
   {
     SET_OUT_ONEWIRE_DDR; 

     if (byte & 0x01)
     {
       _delay_us(7);
       SET_IN_ONEWIRE_DDR; 
       _delay_us(70);
     }
     else
     {
        _delay_us(70);
        SET_IN_ONEWIRE_DDR; 
        _delay_us(7);
     }

     byte >>= 1;
   }
}

unsigned char OneWireReadByte(void)
{
  unsigned char i, byte = 0;

  SET_IN_ONEWIRE_DDR; 
  
  for (i=0; i<8; i++)
  {
     SET_OUT_ONEWIRE_DDR; 
     _delay_us(7);
     SET_IN_ONEWIRE_DDR; 
     _delay_us(7);
     byte >>= 1;
     
     if(IS_SET_ONEWIRE_PIN) byte |= 0x80;

     _delay_us(70);
  }

  return byte;
}

unsigned char OneWireReadBit(void)    
{ 
   unsigned char data=0; 
	SET_IN_ONEWIRE_DDR;
	SET_OUT_ONEWIRE_DDR; 
   	_delay_us(7); 
    SET_IN_ONEWIRE_DDR; 
   	_delay_us(7); 
	if(IS_SET_ONEWIRE_PIN) data |= 0x01; 
   _delay_us(70); 
   return data; 
} 

void OneWireWriteBit(unsigned char data) 
{ 
   CLR_ONEWIRE_PORT; 
   SET_OUT_ONEWIRE_DDR; 
   _delay_us(7); 
   if(data&0x01) 
    	SET_IN_ONEWIRE_DDR;     
   		_delay_us(70);  
   SET_IN_ONEWIRE_DDR; 
   _delay_us(7);
} 

void OneWireSearchRom(void) {

	unsigned char bit,bit_complementary,pozycja,buffer[NUM_OF_THERMS];
	for(unsigned char i=0;i < NUM_OF_THERMS;i++) {
		buffer[i]=0;
	}
	for(unsigned char i=0;i < NUM_OF_THERMS;i++) {
		pozycja=0;
		if (!OneWireReset()) return;
		OneWireWriteByte(0xF0);
		for(unsigned char bitNo=0;bitNo<64;bitNo++) {
		bit=OneWireReadBit();
		bit_complementary=OneWireReadBit();
		if ((!(bit|bit_complementary))&1) {
			if(buffer[pozycja+1]==0) {
			buffer[pozycja]++;
			}
		OneWireWriteBit((buffer[pozycja]-1));
		ROM[i][bitNo/8]|=((buffer[pozycja]-1))<<(bitNo%8);
		pozycja++;
		} 
		else {
			OneWireWriteBit(bit);
			ROM[i][bitNo/8]|=bit<<(bitNo%8);
		}
		}
		while(buffer[pozycja-1]==2) {
			buffer[pozycja]=0;
			pozycja--;
		}
	}
}

void OneWireInit() {
	SET_OUT_ONEWIRE_DDR;
	SET_ONEWIRE_PORT;

	OneWireSearchRom();
	ds18b20_SetResolution(11);
}
