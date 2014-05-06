#include "proto.h"
#include <stdint.h>


uint8_t* getProtoHeader(uint8_t *data){
	return &data[PROTO_HEADER];
}

uint8_t getProtoHeaderLength(){
	return PROTO_HEADER_LEN;
}

uint8_t getProtoOutput(uint8_t *data){
	return (data[PROTO_HEADER] >> 5) & 0x07;
}

uint8_t getProtoMode(uint8_t *data){
	return (data[PROTO_HEADER] >> 4) & 0x01;
}

uint8_t getProtoFunction(uint8_t *data){
	return data[PROTO_HEADER] & 0x0F;
}

uint8_t* getProtoData(uint8_t *data){
	return &data[PROTO_DATA];
}

uint8_t getProtoDataLength(uint16_t dataLength){
	return (uint8_t)(dataLength - PROTO_HEADER_LEN);
}

