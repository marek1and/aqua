#ifndef USB_PROTO_H_
#define USB_PROTO_H_

#define USB_BUFFER_SIZE		50

#define USB_BUFFER_OVERFLOW		0xFF

void processUSBByte(uint8_t receivedByte);

#endif /* USB_PROTO_H_ */
