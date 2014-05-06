#include "connection.h"
#include "settings.h"
#include <stdint.h>
#include <util/delay.h>
#include "../dev/eeprom.h"


static uint8_t connectionParam;
static uint8_t connectionIP[4];
static uint8_t connectionPort;

static uint8_t connectionIPIdx;

void initConnectionSettings(){
	connectionParam = CONNECTION_PARAM_IP;
	connectionIPIdx = IP_D;
	loadConnectionParamsFromMemory();
}

void clearConnectionSetings(){
	connectionParam = 0;
	connectionIPIdx = 0;
	connectionPort = 0;
	for(uint8_t i = 0; i < 4; i++){
		connectionIP[i] = 0;
	}
}

void nextConnectionParam(){
	if(connectionParam == CONNECTION_PARAM_IP){
		connectionParam++;
	} else {
		connectionParam = CONNECTION_PARAM_IP;
	}
}

void prevConnectionParam(){
	if(connectionParam == CONNECTION_PARAM_PORT){
		connectionParam--;
	} else {
		connectionParam = CONNECTION_PARAM_PORT;
	}
}

uint8_t getConnectionParam(){
	return connectionParam;
}

uint8_t *getConnectionIP(){
	return &connectionIP[0];
}

void setConnectionIP(uint8_t *ip){
	for(uint8_t i = 0; i < 4; i++){
		connectionIP[i] = ip[i];
	}
}

uint8_t getConnectionPort(){
	return connectionPort;
}

void setConnectionPort(uint8_t port){
	connectionPort = port;
}

void increaseConnectionIPIdx(){
	if(connectionParam == CONNECTION_PARAM_IP){
		if(connectionIPIdx >= IP_D){
			connectionIPIdx = IP_A;
		} else{
			connectionIPIdx++;
		}
	}
}

void increaseConnectionParam(){
	if(connectionParam == CONNECTION_PARAM_IP){

		connectionIP[connectionIPIdx - 1]++;

	} else if(connectionParam == CONNECTION_PARAM_PORT){
		if(connectionPort >= 255){
			connectionPort = 1;
		} else{
			connectionPort++;
		}
	}
}

void decreaseConnectionParam(){
	if(connectionParam == CONNECTION_PARAM_PORT){
		if(connectionPort <= 1){
			connectionPort = 255;
		} else{
			connectionPort--;
		}
	}
}

void loadConnectionParamsFromMemory(){

	readDataBlockFromMemory(IP_PARAM_ADDR, connectionIP, 4);
	_delay_ms(5);
	connectionPort = readDataFromMemory(PORT_PARAM_ADDR);
	_delay_ms(5);
}

void saveConnectionParamsToMemory(){

	writeDataBlockToMemory(IP_PARAM_ADDR, connectionIP, 4);
	_delay_ms(5);
	writeDataToMemory(PORT_PARAM_ADDR, connectionPort);
	_delay_ms(5);
}

