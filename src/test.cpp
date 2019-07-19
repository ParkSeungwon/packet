#include"packet.h"
#include<cstring>

int main()
{
	Packet packet;
	packet.show();
 	uint8_t dh[] = {0x00,0x1d,0xbc,0x49,0x86,0x15};
	for(int i=0; i<6; i++) packet.ethernet_header.ether_dhost[i] = dh[i];
	packet.set_ipdst("localhost");
	packet.tcp_header.th_dport = htons(2001);
	strcpy(packet.data, "th is isadata");
	for(int i=0; i<10; i++) packet.send();
}
