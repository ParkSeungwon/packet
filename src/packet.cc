#include<regex>
#include<iostream>
#include"packet.h"
using namespace std;

string psstm(string command)
{//return system call output as string
	char buf[1000];
	FILE* f = popen(command.c_str(), "r");
	fgets(buf, sizeof(buf), f);
	pclose(f);
	return buf;
}

Packet::Packet()
{
	string s = psstm("arp -a");
	regex e{R"(([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}).+ on (\S+))"};
	cout << "here" << endl;
	smatch m;
	if(regex_search(s, m, e)) {
		for(int i=0; i<6; i++) gateway_mac_address[i] = stoi(m[i+1].str(), nullptr, 16);
		device = m[7].str();
	}
}

