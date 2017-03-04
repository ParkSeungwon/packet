#include"packet.h"
#include<cstring>

int main()
{
	Packet packet;
	packet.show();
	strcpy(packet.data, "thisis a data");
	packet.send();
}
