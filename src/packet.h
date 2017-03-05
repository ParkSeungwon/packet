#pragma once
#include<string>
#include<sys/socket.h>
#include"netheaders.h"

class Packet
{
public:
	struct ether_header ethernet_header = { 
		{0x00, }, {0x00, }, htons(0x0800)//type ip, 0x0806-arp
	};
	struct ip ip_header = {
		5,//IHL Little endian 작은 수가 앞에 온다.
		4,//version
		0,//type of service, priority, ECN
		0,//length header+payload
		0,//id, to gather fragmented segment
		0,//fragment option + fragment offset
		128,//time to live
		6,//protocol
		0,//checksum
		{0},	//source in_addr
		{0}	//dest in_addr	
	};
	struct tcpheader tcp_header = { htons(80), htons(80), 0, 0, 0, 5,
		0x02,//flags 0x01 FIN, 0x02 SYN, 0x04 RST, 0x08 PUSH,0x10 ACK, 0x20 URG
		0, 0, 0 };
	char data[4096];
	Packet();
	void show();
	void send();
	void set_ipdst(const char* s);
	
protected:
	std::string device;
	struct sockaddr_ll devll;
	int sd;

private:
	void fill_gateway_mac();
	void fill_my_mac();
	void fill_devll();
};
