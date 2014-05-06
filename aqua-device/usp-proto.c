#include "proto.h"
#include "usb-proto.h"

#include "remote-control.h"
#include "dev/usart.h"

#include <stdint.h>

static uint8_t usbBuffer[USB_BUFFER_SIZE];
static uint8_t usbBufferIdx = 0;

void clearUSBBuffer(){

	usbBufferIdx = 0;

	for(uint8_t i = 0; i < USB_BUFFER_SIZE; i++){
		usbBuffer[i] = 0;
	}
}

void sendBufferOverflow(){
	USART_transmit(PROTO_USB_START_BYTE);
	USART_transmit(USB_BUFFER_OVERFLOW);
	USART_transmit(PROTO_USB_STOP_BYTE);
}

void processUSBByte(uint8_t receivedByte){

	if(usbBufferIdx == USB_BUFFER_SIZE - 1){
		clearUSBBuffer();
		sendBufferOverflow();
	}

	if(receivedByte == PROTO_USB_START_BYTE){
		usbBufferIdx = 0;

	} else if(receivedByte == PROTO_USB_STOP_BYTE) {

		uint8_t l = processProtoData(&usbBuffer[0], 0);
		USART_transmit(PROTO_USB_START_BYTE);
		USART_transmitBytes(usbBuffer, l);
		USART_transmit(PROTO_USB_STOP_BYTE);

		clearUSBBuffer();
	} else {
		usbBuffer[usbBufferIdx++] = receivedByte;
	}

}
