#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#ifndef NET_H_
#define NET_H_

//PROTOCOLS

#define IP      1
#define ARP     2
#define ICMP    3
#define UDP     4
#define TCP     5


// ETHERNET
#define ETH_HEADER_LEN	14

// ETHERNET FIELDS
#define ETH_DST_MAC     0
#define ETH_SRC_MAC     6
#define ETH_TYPE        12

// ETHERNET ENCAPSULATED PROTOCOLS
#define ETHTYPE_IP      0x0800
#define ETHTYPE_ARP     0x0806

// ARP
#define ARP_LEN         28
// ARP FIELDS
#define ETH_ARP_HTYPE     ETH_HEADER_LEN + 0
#define ETH_ARP_PTYPE     ETH_HEADER_LEN + 2
#define ETH_ARP_HLEN      ETH_HEADER_LEN + 4
#define ETH_ARP_PLEN      ETH_HEADER_LEN + 5
#define ETH_ARP_OPER      ETH_HEADER_LEN + 6
#define ETH_ARP_SRC_MAC   ETH_HEADER_LEN + 8
#define ETH_ARP_SRC_IP    ETH_HEADER_LEN + 14
#define ETH_ARP_DST_MAC   ETH_HEADER_LEN + 18
#define ETH_ARP_DST_IP    ETH_HEADER_LEN + 24

// ARP VALUES
#define ETH_ARP_HTYPE_VAL 0x0001
#define ETH_ARP_PTYPE_VAL 0x0800
#define ETH_ARP_HLEN_VAL  0x06
#define ETH_ARP_PLEN_VAL  0x04
#define ETH_ARP_OPER_REPLY 0x0002

// IP
#define IP_HEADER_LEN	20

// IP FIELDS
#define IP_VER_IHL      ETH_HEADER_LEN + 0
#define IP_DSCP_ECN     ETH_HEADER_LEN + 1
#define IP_TOTAL_LEN    ETH_HEADER_LEN + 2
#define IP_IDENTIFY     ETH_HEADER_LEN + 4
#define IP_FLAGS        ETH_HEADER_LEN + 6
#define IP_OFFSET       ETH_HEADER_LEN + 6
#define IP_TTL          ETH_HEADER_LEN + 8
#define IP_PROTOCOL     ETH_HEADER_LEN + 9
#define IP_CHECKSUM     ETH_HEADER_LEN + 10
#define IP_SRC          ETH_HEADER_LEN + 12
#define IP_DST          ETH_HEADER_LEN + 16
#define IP_DATA         ETH_HEADER_LEN + 20

// IP ENCAPSULATED PROTOCOLS
#define IP_PROTO_ICMP   0x01
#define IP_PROTO_TCP    0x06
#define IP_PROTO_UDP 	0x11
#define IP_PROTO_TEST   0xFD

//IP VALUES
#define IP_VER_IHL_VAL  0x45

// ICMP
// ICMP FIELDS
#define ICMP_TYPE       IP_DATA + 0
#define ICMP_CHECKSUM 	IP_DATA + 2

// ICMP TYPES
#define ICMP_TYPE_ECHOREPLY   0x00
#define ICMP_TYPE_ECHOREQUEST 0x08

// UDP
#define UDP_HEADER_LEN	8

// UDP FIELDS
#define UDP_SRC_PORT 	 IP_DATA + 0
#define UDP_DST_PORT     IP_DATA + 2
#define UDP_LEN			 IP_DATA + 4
#define UDP_CHECKSUM     IP_DATA + 6
#define UDP_DATA         IP_DATA + 8

// TCP
#define TCP_HEADER_LEN	 20

// TCP FIELDS
#define TCP_SRC_PORT     IP_DATA + 0
#define TCP_DST_PORT     IP_DATA + 2
#define TCP_SEQ          IP_DATA + 4
#define TCP_SEQACK       IP_DATA + 8
#define TCP_HDR_LENGTH   IP_DATA + 12
#define TCP_FLAGS        IP_DATA + 13
#define TCP_WIN_SIZE     IP_DATA + 14
#define TCP_CHECKSUM     IP_DATA + 16
#define TCP_PRIORITY     IP_DATA + 18
#define TCP_DATA         IP_DATA + 20

// TCP FLAGS
#define TCP_FLAGS_FIN    0x01
#define TCP_FLAGS_SYN    0x02
#define TCP_FLAGS_RST    0x04
#define TCP_FLAGS_PSH    0x08
#define TCP_FLAGS_ACK    0x10
#define TCP_FLAGS_URG    0x20
#define TCP_FLAGS_ECE    0x40
#define TCP_FLAGS_CWR    0x80
#define TCP_FLAGS_NS     0x01
#define TCP_FLAGS_SYNACK 0x12
#define TCP_FLAGS_PSHACK 0x18

#endif

void networkInit(uint8_t *ipAddr,uint8_t *macAddr, uint16_t devicePort);
uint8_t *getIPAddress();
void setIPAddress(uint8_t *ipAddr);
uint16_t getPort();
void setPort(uint16_t p);

uint16_t receivePacket(unsigned int maxlen, uint8_t* packet);
uint16_t checksum(uint8_t *buf, uint16_t len,uint8_t type);
uint8_t getEthernetDataProtocol(uint8_t *buf);
uint8_t getIPDataProtocol(uint8_t *buf);
uint8_t isMyIP(uint8_t *buf);
uint8_t isMyArpIP(uint8_t *buf);
void clearBuffer(uint8_t *buf, int len);

void doArpAnswer(uint8_t *buf);
void doPingReply(uint8_t *buf,uint16_t len);

void fillIpChecksum(uint8_t *buf);

void sendIPPacket(uint8_t* buf, uint8_t* dstMAC, uint8_t* dstIP, uint8_t ipProto, uint8_t* data, uint16_t dataLength);
void sendUDPPacket(uint8_t* buf, uint8_t* dstMAC, uint8_t* dstIP, uint16_t dstPort, uint8_t* data, uint16_t dataLength);
void replyUDPPacket(uint8_t* buf, uint8_t* data, uint16_t dataLength);
void sendTCPPacket(uint8_t* buf, uint8_t* dstMAC, uint8_t* dstIP, uint16_t dstPort, uint8_t flags, uint8_t* data, uint16_t dataLength);
