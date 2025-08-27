#pragma once

#include "../../TextPrint.h"
#include "../../libc/stdtypes.h"
#include "../../Memory.h"
#include "../../cpu/io.h"
#include "../pci.h"
#include "rtl8139.h"



typedef struct netinfo_t{
    uint8_t mac[6];
    uint32_t ip;
    uint32_t mask;
    uint32_t gateway;
} netinfo_t;

extern netinfo_t net_info;


//Frame 2
typedef struct eth_frame_t {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype;  // 0x0800 pour IPv4
    uint8_t payload[];   // header IP + data
} eth_frame_t;

//Frame 3
typedef struct ipv4_header_t {
    uint8_t ihl:4;
    uint8_t version:4;
    uint8_t tos;
    uint16_t total_length;
    uint16_t identification;
    uint16_t flags_fragment_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    uint32_t source_ip;
    uint32_t dest_ip;
} ipv4_header_t;

typedef struct icm_echo_t {
    uint8_t type;      // 8 = Echo Request
    uint8_t code;      // 0
    uint16_t checksum; // à calculer
    uint16_t id;       // identifiant
    uint16_t seq;      // numéro de séquence
    uint8_t payload[]; // données optionnelles
} icmp_echo_t;


//ARP
#pragma pack(push, 1)
typedef struct arp_packet_t {
    uint16_t htype;     // Hardware type (Ethernet = 1)
    uint16_t ptype;     // Protocol type (IPv4 = 0x0800)
    uint8_t  hlen;      // Hardware size (MAC = 6)
    uint8_t  plen;      // Protocol size (IPv4 = 4)
    uint16_t oper;      // Operation (1=request, 2=reply)
    uint8_t  sha[6];    // Sender MAC address
    uint32_t spa;       // Sender IP address
    uint8_t  tha[6];    // Target MAC address
    uint32_t tpa;       // Target IP address
} arp_packet_t;
#pragma pack(pop)



void setIP();
void setMACAddress();
uint8_t* getMACAddress();

void testPing(uint16_t seq);
void broadcast_ping(const uint8_t* payload, uint16_t size, uint16_t seq);
void testARP();

eth_frame_t* createARPRequest(uint32_t target_ip, uint16_t operation, const uint8_t* sha);
void handleARPPacket(eth_frame_t* frame);

void handleEthernetFrame(uint8_t* frame, uint16_t length);


uint16_t htle16(uint16_t nb);
uint32_t htle32(uint32_t nb) ;