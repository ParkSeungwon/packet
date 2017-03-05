#pragma once
#include<arpa/inet.h>
/*struct in_addr {
	u_int32_t s_addr;
};
*/
struct ether_header {
	u_int8_t  ether_dhost[6];	/* destination eth addr	*/
	u_int8_t  ether_shost[6];	/* source ether addr	*/
	u_int16_t ether_type;		        /* packet type ID field	*/
}__attribute__((packed)); 

struct ip {
	unsigned int ip_hl:4;		/* header length */
	unsigned int ip_v:4;		/* version */
	u_int8_t ip_tos;			/* type of service */
	u_short ip_len;			/* total length */
	u_short ip_id;			/* identification */
	u_short ip_off;			/* fragment offset field */
#define	IP_RF 0x8000			/* reserved fragment flag */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	u_int8_t ip_ttl;			/* time to live */
	u_int8_t ip_p;			/* protocol */
	u_short ip_sum;			/* checksum */
	struct in_addr ip_src, ip_dst;	/* source and dest address */
}__attribute__((packed));//need this to get rid of auto padding

struct tcpheader {
	unsigned short int th_sport, th_dport;
	unsigned int th_seq, th_ack;
	unsigned char th_x2:4, th_off:4;//always write together, to avoid endian prob
	unsigned short int th_flags;
	unsigned short int th_win, th_sum, th_urp;
}__attribute__((packed));

 struct sockaddr_ll {
	 unsigned short sll_family;   /* Always AF_PACKET */
	 unsigned short sll_protocol; /* Physical-layer protocol */
	 int            sll_ifindex;  /* Interface number */
	 unsigned short sll_hatype;   /* ARP hardware type */
	 unsigned char  sll_pkttype;  /* Packet type */
	 unsigned char  sll_halen;    /* Length of address */
	 unsigned char  sll_addr[8];  /* Physical-layer address */
 };
