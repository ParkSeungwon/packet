#include"packet.h"
#include<cstring>

int main()
{
	Packet packet;
	packet.show();
	packet.ethernet_header.ether_dhost = {0x00,0x1d,0xbc,0x49,0x86,0x15};
	packet.set_ipdst("localhost");
	packet.tcp_header.th_dport = htons(2001);
	strcpy(packet.data, "th is isadata");
	for(int i=0; i<10; i++) packet.send();
}
