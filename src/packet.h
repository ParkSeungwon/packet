#pragma once
#include<string>
#include"netheaders.h"

class Packet
{
public:
	struct ether_header ethernet_header = {
		{0x00, 0x08, 0x7c, 0x12, 0xac, 0x0a},//dest mac
		{0x10, 0x02, 0xb5, 0x13, 0x32, 0xa1},//src mac wlp1s0
		0x0800//type ip, 0x0806-arp
	};
	struct ip ip_header = {
		5,//IHL Little endian 작은 수가 앞에 온다.
		4,//version
		0b11111100,//type of service, priority, ECN
		65535,//length header+payload
		999,//id, to gather fragmented segment
		0,//fragment option + fragment offset
		128,//time to live
		6,//protocol
		0,//checksum
		{0},	//source in_addr
		{0}	//dest in_addr	
	};
	struct tcpheader tcp_header = { 80, 80, 0, 0, 0, 5,
		0x02,//flags 0x01 FIN, 0x02 SYN, 0x04 RST, 0x08 PUSH,0x10 ACK, 0x20 URG
		0, 0, 0 };
	char data[4096];

	Packet();
	void show();
	
protected:
	unsigned char gateway_mac_address[6];
	std::string device;
};
