#ifndef PROTO_H_
#define PROTO_H_

#include <stdint.h>

//HEADER
// aaa b cccc dddddddd

// aaa  - OUTPUT/PARAM NUMBER
// b    - R(1)/W(0) or GET(1)/SET(0)
// cccc - FUNCTION
// d    - data

#define PROTO_HEADER_LEN	1

#define PROTO_HEADER	0
#define PROTO_DATA		1

#define PROTO_OUT0		0 // RESERVED
#define PROTO_OUT1		1
#define PROTO_OUT2		2
#define PROTO_OUT3		3
#define PROTO_OUT4		4
#define PROTO_OUT5		5
#define PROTO_OUT6		6 // RESERVED
#define PROTO_OUT7		7 // RESERVED

#define PROTO_PARAM0		0 //RESERVED
#define PROTO_PARAM1		1
#define PROTO_PARAM2		2
#define PROTO_PARAM3		3
#define PROTO_PARAM4		4
#define PROTO_PARAM5		5
#define PROTO_PARAM6		6
#define PROTO_PARAM7		7 //RESERVED

#define PROTO_READ		1
#define PROTO_WRITE		0
#define PROTO_GET		1
#define PROTO_SET		0

#define PROTO_FUNC_IS_ALIVE			0
#define PROTO_FUNC_DATETIME			1
#define PROTO_FUNC_TEMPERATURE		2
#define PROTO_FUNC_ILLUMINANCE		3
#define PROTO_FUNC_BACKLIGHT		4
#define PROTO_FUNC_OUT_MODE			5
#define PROTO_FUNC_OUT_TIMER		6
#define PROTO_FUNC_OUT_TEMPERATURE	7
#define PROTO_FUNC_OUT_HYSTERESIS	8
#define PROTO_FUNC_OUT_LUMINANCE	9
#define PROTO_FUNC_IP_ADDRESS		10
#define PROTO_FUNC_PORT				11
#define PROTO_FUNC_CONTROL			12
#define PROTO_FUNC_DEFAULT_SETTINGS	13

#define PROTO_FUNC_PLAY_MELODY		14
#define PROTO_FUNC_DEVICE_RESET		15

#define PROTO_USB_START_BYTE		0xEE
#define PROTO_USB_STOP_BYTE			0xEF

#define PROTO_SUCCESS			0
#define PROTO_FAILURE			1
#define PROTO_INVALID_OPTION	2

uint8_t* getProtoHeader(uint8_t *data);
uint8_t getProtoHeaderLength();
uint8_t getProtoOutput(uint8_t *data);
uint8_t getProtoMode(uint8_t *data);
uint8_t getProtoFunction(uint8_t *data);
uint8_t* getProtoHeader(uint8_t *data);
uint8_t* getProtoData(uint8_t *data);
uint8_t getProtoDataLength(uint16_t dataLength);



#endif /* PROTO_H_ */
