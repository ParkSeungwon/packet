#include"packet.h"
#include<cstring>

int main()
{
	Packet packet;
	packet.show();
	packet.set_ipdst("www.naver.com");
	packet.set_ipsrc("localhost");
	strcpy(packet.data, "th is isadata");
	for(int i=0; i<100; i++) packet.send();
}
