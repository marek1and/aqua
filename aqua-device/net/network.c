#include "network.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "enc28j60.h"
#include "avrlibdefs.h"

static uint8_t  macAddress[6];
static uint8_t  ipAddress[4];
static uint16_t port;

void networkInit(uint8_t *ipAddr, uint8_t *macAddr, uint16_t devicePort){
	uint8_t i=0;

	while(i<4){
			ipAddress[i] = ipAddr[i];
			i++;
	}
	i=0;
	while(i<6){
			macAddress[i] = macAddr[i];
			i++;
	}
	port = devicePort;

	enc28j60Init(macAddr);
	enc28j60clkout(2);
	_delay_ms(10);
	enc28j60PhyWrite(PHLCON,0x476);
	_delay_ms(100);
}

uint8_t *getIPAddress(){
	return &ipAddress[0];
}

void setIPAddress(uint8_t *ipAddr){
	for(uint8_t i = 0; i < 4; i++){
		ipAddress[i] = ipAddr[i];
	}
}

uint16_t getPort(){
	return port;
}

void setPort(uint16_t p){
	port = p;
}

uint16_t receivePacket(unsigned int maxlen, uint8_t* packet){
	return enc28j60PacketReceive(maxlen, packet);
}

uint16_t checksum(uint8_t *buf, uint16_t len,uint8_t type){
	uint32_t sum = 0;

	if(type == UDP){
		sum += IP_PROTO_UDP;
		sum += len-8;
	}
	if(type == TCP){
		sum += IP_PROTO_TCP;
		sum += len-8;
	}
	while(len > 1){
		sum += 0xFFFF & (((uint32_t)*buf<<8)|*(buf+1));
		buf+=2;
		len-=2;
	}
	if (len){
		sum += ((uint32_t)(0xFF & *buf))<<8;
	}
	while (sum>>16){
		sum = (sum & 0xFFFF)+(sum >> 16);
	}
	return( (uint16_t) sum ^ 0xFFFF);
}

uint8_t getEthernetDataProtocol(uint8_t *buf){
	uint16_t protocol = 0;

	protocol = 8 << buf[ETH_TYPE];
	protocol |= buf[ETH_TYPE + 1];

	if(protocol == ETHTYPE_IP){
		return IP;
	} else if(protocol == ETHTYPE_ARP){
		return ARP;
	}

	return 0;
}

uint8_t getIPDataProtocol(uint8_t *buf){
	uint8_t protocol = 0;

	protocol = buf[IP_PROTOCOL];

	if(protocol == IP_PROTO_ICMP){
		return ICMP;
	} else if(protocol == IP_PROTO_UDP){
		return UDP;
	} else if(protocol == IP_PROTO_TCP){
		return TCP;
	}

	return 0;
}

uint8_t checkMyIP(uint8_t *buf, uint8_t ipPosition){

	uint8_t i=0;

	while(i<4){
		if(buf[ipPosition + i] != ipAddress[i]){
			return 0;
		}
		i++;
	}
	return 1;
}

uint8_t isMyIP(uint8_t *buf){
	return checkMyIP(buf, IP_DST);
}

uint8_t isMyArpIP(uint8_t *buf){
	return checkMyIP(buf, ETH_ARP_DST_IP);
}

void clearBuffer(uint8_t *buf, int len){
	for(int i = 1; i < len; i++){
		buf[i] = 0;
	}
}

void fillIpChecksum(uint8_t *buf) {
	uint16_t ck;

	buf[IP_CHECKSUM]=0;
	buf[IP_CHECKSUM + 1]=0;

	ck = checksum(&buf[ETH_HEADER_LEN], IP_HEADER_LEN,IP);
	buf[IP_CHECKSUM]= ck >> 8;
	buf[IP_CHECKSUM + 1] = ck& 0xff;
}


void doArpAnswer(uint8_t *buf) {
	uint8_t i=0;

	while(i<6) {
		buf[ETH_DST_MAC + i] = buf[ETH_SRC_MAC +i];
		buf[ETH_SRC_MAC + i] = macAddress[i];
		i++;
	}

	buf[ETH_ARP_OPER] = ETH_ARP_OPER_REPLY >> 8;
	buf[ETH_ARP_OPER + 1] = ETH_ARP_OPER_REPLY & 0xFF;

	i=0;
	while(i<6){
			buf[ETH_ARP_DST_MAC + i] = buf[ETH_ARP_SRC_MAC + i];
			buf[ETH_ARP_SRC_MAC + i] = macAddress[i];
			i++;
	}
	i=0;
	while(i<4){
			buf[ETH_ARP_DST_IP + i] = buf[ETH_ARP_SRC_IP + i];
			buf[ETH_ARP_SRC_IP + i] = ipAddress[i];
			i++;
	}
	enc28j60PacketSend(ETH_HEADER_LEN + ARP_LEN, buf);
}

void doPingReply(uint8_t *buf,uint16_t len) {
	uint8_t i=0;

	while(i<6) {
		buf[ETH_DST_MAC + i] = buf[ETH_SRC_MAC +i];
		buf[ETH_SRC_MAC + i] = macAddress[i];
		i++;
	}
	i=0;

	while(i<4){
		buf[IP_DST + i] = buf[IP_SRC + i];
		buf[IP_SRC + i] = ipAddress[i];
		i++;
	}
	i=0;
	fillIpChecksum(buf);


	buf[ICMP_TYPE] = ICMP_TYPE_ECHOREPLY;

	if (buf[ICMP_CHECKSUM] > (0xff-0x08)){
		buf[ICMP_CHECKSUM + 1]++;
	}
	buf[ICMP_CHECKSUM]+=0x08;

	enc28j60PacketSend(len,buf);
}

void prepareEthernetHeader(uint8_t* buf, uint8_t* dstMAC, uint16_t ethType) {

	buf[ETH_TYPE] =  ethType >> 8 ;
	buf[ETH_TYPE + 1] = ethType & 0xFF;

	for(int i = 0; i < 6; i++){
		buf[ETH_DST_MAC +i] = dstMAC[i];
		buf[ETH_SRC_MAC +i] = macAddress[i];
	}
}

void prepareIPHeader(uint8_t* buf, uint8_t* dstIP, uint8_t ipProto) {

	for(int i = 0; i < 4; i++) {
		buf[IP_DST+i] = dstIP[i];
		buf[IP_SRC+i] = ipAddress[i];
	}

	buf[IP_VER_IHL]    = IP_VER_IHL_VAL;
	buf[IP_PROTOCOL]   = ipProto;

    buf[IP_FLAGS]      = 0x40;  // don't fragment
    buf[IP_OFFSET + 1] = 0;     // fragement offset
    buf[IP_TTL]        = 64;    // ttl

}

void prepareTCPHeader(uint8_t* buf, uint16_t dstPort, uint8_t flags){

	buf[TCP_SRC_PORT]     = port >> 8;
	buf[TCP_SRC_PORT + 1] = port & 0xFF;
	buf[TCP_DST_PORT    ] = dstPort >> 8;
	buf[TCP_DST_PORT + 1] = dstPort & 0xFF;

	buf[TCP_HDR_LENGTH  ] = 0x50;
	buf[TCP_FLAGS       ] = flags;
	buf[TCP_WIN_SIZE    ] = 0x05;
	buf[TCP_WIN_SIZE + 1] = 0xB4;
	buf[TCP_PRIORITY    ] = 0;
}

void prepareUDPHeader(uint8_t* buf, uint16_t dstPort){

	buf[UDP_SRC_PORT]     = port >> 8;
	buf[UDP_SRC_PORT + 1] = port & 0xFF;
	buf[UDP_DST_PORT]     = dstPort >> 8;
	buf[UDP_DST_PORT + 1] = dstPort & 0xFF;;

}

void sendIPPacket(uint8_t* buf, uint8_t* dstMAC, uint8_t* dstIP, uint8_t ipProto, uint8_t* data, uint16_t dataLength) {

	prepareEthernetHeader(buf, dstMAC, ETHTYPE_IP);
	prepareIPHeader(buf, dstIP, ipProto);

	uint16_t length = IP_HEADER_LEN + dataLength;

	if(ipProto == IP_PROTO_TCP) {
		buf[TCP_CHECKSUM] = 0;
		buf[TCP_CHECKSUM + 1] = 0;

		uint16_t ck;
		ck = checksum(&buf[IP_SRC], 8 + dataLength, TCP);
		buf[TCP_CHECKSUM] = ck >> 8;
		buf[TCP_CHECKSUM + 1] = ck & 0xff;

	} else if(ipProto == IP_PROTO_UDP) {
		buf[UDP_CHECKSUM] = 0;
		buf[UDP_CHECKSUM + 1] = 0;

        uint16_t ck;
        ck = checksum(&buf[IP_SRC], 8 + dataLength, UDP);
        buf[UDP_CHECKSUM] = ck >> 8;
        buf[UDP_CHECKSUM + 1] = ck & 0xff;

	} else {
		for(uint16_t i = 0; i < dataLength; i++){
			buf[IP_DATA + i] = data[i];
		}
	}

	buf[IP_TOTAL_LEN]     = length >> 8;
	buf[IP_TOTAL_LEN + 1] = length & 0xFF;

	fillIpChecksum(buf);
    enc28j60PacketSend(ETH_HEADER_LEN + length, buf);
    clearBuffer(buf, ETH_HEADER_LEN + length);
}

void sendUDPPacket(uint8_t* buf, uint8_t* dstMAC, uint8_t* dstIP, uint16_t dstPort, uint8_t* data, uint16_t dataLength){

	prepareUDPHeader(buf, dstPort);

	uint16_t length = UDP_HEADER_LEN + dataLength;

	buf[UDP_LEN]     = length >> 8;
	buf[UDP_LEN + 1] = length & 0xFF;

	for(uint16_t i = 0; i < dataLength; i++){
		buf[UDP_DATA + i] = data[i];
	}

	sendIPPacket(buf, dstMAC, dstIP, IP_PROTO_UDP, &buf[IP_DATA], length);
}

void replyUDPPacket(uint8_t* buf, uint8_t* data, uint16_t dataLength){

	uint16_t dstPort = ((buf[UDP_SRC_PORT] << 8) & 0xFF00) | (buf[UDP_SRC_PORT + 1] & 0x00FF);
	sendUDPPacket(buf, &buf[ETH_SRC_MAC], &buf[IP_SRC], dstPort, data, dataLength);

}

// Not finished and not used TCP mechanism. Written for testing purpose
void sendTCPPacket(uint8_t* buf, uint8_t* dstMAC, uint8_t* dstIP, uint16_t dstPort, uint8_t flags, uint8_t* data, uint16_t dataLength){

	prepareTCPHeader(buf, dstPort, flags);

	uint16_t length = TCP_HEADER_LEN + dataLength;

	for(uint16_t i = 0; i < dataLength; i++){
		buf[TCP_DATA + i] = data[i];
	}

	buf[TCP_SEQ         ] =  0;
	buf[TCP_SEQACK      ] =  0;

	//#define TCP_SEQ          IP_DATA + 4
	//#define TCP_SEQACK       IP_DATA + 8

	sendIPPacket(buf, dstMAC, dstIP, IP_PROTO_TCP, &buf[IP_DATA], length);
}
