#include<regex>
#include<iomanip>
#include<iostream>
#include<net/if.h>
#include<netdb.h>
#include"packet.h"
using namespace std;

uint16_t checksum (const uint16_t *addr, int len);
uint16_t tcp4_checksum (struct ip iphdr, struct tcpheader tcphdr, 
		const uint8_t *payload, int payloadlen);

static string psstm(string command)
{//return system call output as string
	char buf[1000];
	FILE* f = popen(command.c_str(), "r");
	fgets(buf, sizeof(buf), f);
	pclose(f);
	return buf;
}

Packet::Packet()
{
	fill_gateway_mac();
	if(!device.empty()) fill_my_mac();
	if ((sd = socket (PF_PACKET, SOCK_RAW, htons(3))) < 0) {
		perror ("socket() failed to get socket descriptor for using ioctl() ");
		exit (EXIT_FAILURE);
	}
	fill_devll();
}

void Packet::set_ipsrc(const char* s)
{
	auto* a = gethostbyname(s);
	memcpy(&ip_header.ip_src, (struct in_addr*)a->h_addr, sizeof(struct in_addr));
}

void Packet::set_ipdst(const char* s)
{
	auto* a = gethostbyname(s);
	memcpy(&ip_header.ip_dst, (struct in_addr*)a->h_addr, sizeof(struct in_addr));
}

void Packet::send()
{
	int len = strlen(data);
	ip_header.ip_len = htons(40 + len);
	ip_header.ip_sum = checksum((const uint16_t*)&ip_header, 20);
	tcp_header.th_sum = tcp4_checksum(ip_header, tcp_header,(const uint8_t*)data, len);
	cout << sizeof(ethernet_header) << endl;
	for(int i=0; i<54+len; i++) cout << hex << +*((unsigned char*)this+i) << ' ';
	if(sendto(sd, this, 54+len, 0, (struct sockaddr*)&devll, sizeof(devll)) <= 0) {
		perror ("sendto() failed");
		exit (EXIT_FAILURE);
	}
}

void Packet::fill_gateway_mac()
{
	string s = psstm("arp -a");
	regex e{R"(([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}).+ on (\S+))"};
	smatch m;
	if(regex_search(s, m, e)) {
		for(int i=0; i<6; i++) 
			ethernet_header.ether_dhost[i] = stoi(m[i+1].str(), 0, 16);
		device = m[7].str();
	}
}

void Packet::fill_my_mac()
{
	string s = device;
	s += R"raw(.+HWaddr ([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}):([a-f\d]{2}))raw";
	regex e{s};
	smatch m;
	string r = psstm("ifconfig");
	if(regex_search(r, m, e)) 
		for(int i=0; i<6; i++) 
			ethernet_header.ether_shost[i] = stoi(m[i+1].str(), 0, 16);
}

void Packet::fill_devll()
{
	memset(&devll, 0, sizeof(devll));
	if(!(devll.sll_ifindex = if_nametoindex(device.data()))) {
		perror("if_nametoindex() failed to obtain interface index ");
		exit(EXIT_FAILURE);
	}
	devll.sll_family = AF_PACKET;
	memcpy(devll.sll_addr, ethernet_header.ether_shost, sizeof(uint8_t));
	devll.sll_halen = 6;
}

void Packet::show()
{
	cout << device << " : ";
	for(int i=0; i<6; i++) cout << hex << +ethernet_header.ether_shost[i] << ':';
	cout << "\b \nGateway : " ;
	for(int i=0; i<6; i++) cout << hex << +ethernet_header.ether_dhost[i] << ':';
	cout << "\b \n";
}
