#include "../../includes/drivers/network/netstack.h"

static uint16_t packet_id = 0;
netinfo_t net_info;


uint16_t htle16(uint16_t nb) {
    return (nb>>8) | (nb<<8);
}

uint32_t htle32(uint32_t nb) {
    return ((nb>>24)&0xff)      |
            ((nb<<8)&0xff0000)   |
            ((nb>>8)&0xff00)     |
            ((nb<<24)&0xff000000);
}


void setMACAddress(){
    uint32_t bar0 = getBar0(getRtlBus(), getRtlDevice(), 0) & 0xFFFFFFFC; //masking the last 2 bits (to get the address)
    for(int i = 0; i < 6; i++){
        net_info.mac[i] = inb(bar0 + i);
    }
    return;
}

uint8_t* getMACAddress(){
    setMACAddress();
    uint8_t* mac = (uint8_t*)malloc(6);
    for(int i = 0; i<6; i++){
        mac[i]=net_info.mac[i];
    }
    return mac;
}

//Todo use DHCP to get an IP address, pour l'instant c'est hardcodé
void setIP(){
    net_info.ip = 0xC0A80064;      // 192.168.0.100
    net_info.mask = 0xFFFFFF00;    // 255.255.255.0
    net_info.gateway = 0xC0A80001; // 192.168.0.1
}


static uint16_t checksum(const void *data, uint32_t len) {
    const uint16_t *word = (const uint16_t *)data;
    uint32_t sum = 0;

    while (len > 1) {
        sum += *word++;
        if (sum & 0x10000)
            sum = (sum & 0xFFFF) + 1;
        len -= 2;
    }

    if (len > 0) {
        uint16_t last = 0;
        *(uint8_t*)&last = *(const uint8_t*)word;
        sum += last;
        if (sum & 0x10000)
            sum = (sum & 0xFFFF) + 1;
    }

    return ~sum;
}

uint16_t ipv4_checksum(const ipv4_header_t* ip_hdr, uint32_t ip_hdr_len) {
    // Le champ checksum doit être mis à zéro avant
    return checksum((const void*)ip_hdr, ip_hdr_len);
}


ipv4_header_t* createIPv4Header(uint32_t dest_ip, uint8_t protocol, uint16_t data_length){
    ipv4_header_t* header = (ipv4_header_t*)malloc(sizeof(ipv4_header_t));
    if(header == 0){
        PrintString("Failed to allocate memory for IPv4 header\n\r", BACKGROUND_RED);
        return 0;
    }
    header->version = 4;
    header->ihl = 5; //no options
    header->tos = 0;
    header->total_length = htle16(sizeof(ipv4_header_t) + data_length);
    header->identification = htle16(packet_id++); //todo
    header->flags_fragment_offset = 0; //todo
    header->ttl = 64; //default
    header->protocol = protocol;
    header->header_checksum = 0; //todo
    header->source_ip = htle32(net_info.ip);
    header->dest_ip = htle32(dest_ip);

    //Calculate checksum
    uint16_t* ptr = (uint16_t*)header;
    uint32_t sum = 0;
    for(int i = 0; i < sizeof(ipv4_header_t)/2; i++){
        sum += htle16(ptr[i]);
    }
    while(sum >> 16){
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    header->header_checksum = 0;
    header->header_checksum = ipv4_checksum(header, sizeof(ipv4_header_t));

    return header;
}

eth_frame_t* createEthernetFrame(const uint8_t* dst_mac, uint16_t ethertype, uint16_t payload_length){
    eth_frame_t* frame = (eth_frame_t*)malloc(sizeof(eth_frame_t) + payload_length);
    if(frame == 0){
        PrintString("Failed to allocate memory for Ethernet frame\n\r", BACKGROUND_RED);
        return 0;
    }
    //Set destination MAC
    for(int i = 0; i < 6; i++){
        frame->dst_mac[i] = dst_mac[i];
    }
    //Set source MAC
    for(int i = 0; i < 6; i++){
        frame->src_mac[i] = net_info.mac[i];
    }
    frame->ethertype = htle16(ethertype);
    //Payload will be set later
    return frame;
}

eth_frame_t* createIPv4EthernetFrame(uint8_t* dst_mac, uint32_t dest_ip, uint8_t protocol, uint16_t data_length, uint8_t* data){
    ipv4_header_t* ip_header = createIPv4Header(dest_ip, protocol, data_length);


    eth_frame_t* frame = createEthernetFrame(dst_mac, 0x0800, sizeof(ipv4_header_t) + data_length);

    //Copy IP header to payload
    for(int i = 0; i < sizeof(ipv4_header_t); i++){
        frame->payload[i] = ((uint8_t*)ip_header)[i];
    }

    //Copy data to payload
    for(int i = 0; i < data_length; i++){
        frame->payload[sizeof(ipv4_header_t) + i] = data[i];
    }
    //Free the IP header
    free(ip_header);

    return frame;
}

uint16_t icmp_checksum(uint16_t* data, int length){
    uint32_t sum = 0;
    for(int i = 0; i < length/2; i++){
        sum += htle16(data[i]); // host order
    }
    if(length % 2) sum += ((uint8_t*)data)[length-1] << 8;
    while(sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    return htle16(~sum);
}

icm_echo_t* createICMPEchoRequest(uint16_t id, uint16_t seq, const uint8_t* payload, uint16_t payload_length){
    icm_echo_t* echo = (icmp_echo_t*)malloc(sizeof(icmp_echo_t) + payload_length);

    echo->type = htle16(8); // Echo Request
    echo->code = 0;
    echo->checksum = 0; // will be calculated later
    echo->id = htle16(id);
    echo->seq = htle16(seq);
    //Copy payload
    for(int i = 0; i < payload_length; i++){
        echo->payload[i] = payload[i];
    }
    //Calculate checksum
    echo->checksum = icmp_checksum((uint16_t*)echo, sizeof(icm_echo_t) + payload_length);
    return echo;
}


eth_frame_t* createARPRequest(uint32_t target_ip) {
    // ARP payload
    arp_packet_t* arp = (arp_packet_t*)malloc(sizeof(arp_packet_t));
    if (!arp) return 0;

    arp->htype = htle16(1);         // Ethernet
    arp->ptype = htle16(0x0800);    // IPv4
    arp->hlen = 6;                  // MAC length
    arp->plen = 4;                  // IPv4 length
    arp->oper = htle16(1);          // ARP Request

    // Sender info
    for (int i = 0; i < 6; i++) arp->sha[i] = net_info.mac[i];
    arp->spa = htle32(net_info.ip);

    // Target info
    for (int i = 0; i < 6; i++) arp->tha[i] = 0x00;
    arp->tpa = htle32(target_ip);

    // Ethernet frame (broadcast destination)
    uint8_t broadcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    eth_frame_t* frame = createEthernetFrame(broadcast, 0x0806, sizeof(arp_packet_t));

    // Copy ARP payload
    memcopy(frame->payload, arp, sizeof(arp_packet_t));
    free(arp);

    return frame;
}

eth_frame_t* createARPReply(uint32_t target_ip, const uint8_t* target_mac) {
    arp_packet_t* arp = (arp_packet_t*)malloc(sizeof(arp_packet_t));
    if (!arp) return 0;

    arp->htype = htle16(1);
    arp->ptype = htle16(0x0800);
    arp->hlen  = 6;
    arp->plen  = 4;
    arp->oper  = htle16(2);

    //Nous
    memcopy(arp->sha, net_info.mac, 6);
    arp->spa = htle32(net_info.ip);

    //Eux
    memcopy(arp->tha, target_mac, 6);
    arp->tpa = htle32(target_ip);

    eth_frame_t* frame = createEthernetFrame(target_mac, 0x0806, sizeof(arp_packet_t));

    memcopy(frame->payload, arp, sizeof(arp_packet_t));
    free(arp);

    return frame;
}




uint16_t udp_checksum(uint32_t src_ip, uint32_t dst_ip, 
                      const uint8_t *udp_packet, uint32_t udp_len) {
    udp_pseudo_header_t ph;
    ph.src_addr = htle32(src_ip);
    ph.dst_addr = htle32(dst_ip);
    ph.zero = 0;
    ph.protocol = 17; // UDP
    ph.udp_length = htle32(udp_len);

    // Taille = pseudo header + UDP header + data
    uint32_t total_len = sizeof(ph) + udp_len;
    uint8_t buf[1500]; // assez grand pour un paquet Ethernet
    memcopy(buf, &ph, sizeof(ph));
    memcopy(buf + sizeof(ph), udp_packet, udp_len);

    return checksum(buf, total_len);
}

eth_frame_t* createIPv4UDPFrame(uint8_t* dst_mac, uint32_t dst_ip,
                                uint16_t src_port, uint16_t dst_port,
                                const uint8_t* payload, uint16_t payload_len) {
    // IPv4 header
    ipv4_header_t* ip_hdr = createIPv4Header(dst_ip, 17 /*UDP*/, sizeof(udp_header_t) + payload_len);

    // UDP header
    udp_header_t* udp_hdr = (udp_header_t*)malloc(sizeof(udp_header_t));
    udp_hdr->src_port = htle16(src_port);
    udp_hdr->dst_port = htle16(dst_port);
    udp_hdr->length = htle16(sizeof(udp_header_t) + payload_len);

    udp_hdr->checksum = 0; 
    udp_hdr->checksum = udp_checksum(net_info.ip, dst_ip, (const uint8_t*)udp_hdr, sizeof(udp_header_t) + payload_len);


    // Ethernet frame
    eth_frame_t* frame = createEthernetFrame(dst_mac, 0x0800, sizeof(ipv4_header_t) + sizeof(udp_header_t) + payload_len);

    // Copie IPv4 header dans payload
    memcopy(frame->payload, ip_hdr, sizeof(ipv4_header_t));
    // Copie UDP header juste après
    memcopy(frame->payload + sizeof(ipv4_header_t), udp_hdr, sizeof(udp_header_t));
    // Copie le payload utilisateur
    memcopy(frame->payload + sizeof(ipv4_header_t) + sizeof(udp_header_t), payload, payload_len);


    free(ip_hdr);
    free(udp_hdr);

    return frame;
}


void testUDP() {
    //uint32_t dst_ip =  0xC0A80001; // 192.168.0.1
    uint32_t dst_ip = 0x0A000201; // 10.0.2.2 = host NAT IP
    uint16_t src_port = 1234;
    uint16_t dst_port = 5555;
    uint8_t message[] = "HELLO FROM OS!";

    eth_frame_t* frame = createIPv4UDPFrame(net_info.mac, dst_ip,
                                            src_port, dst_port,
                                            message, sizeof(message)-1);

    rtl8139_transmit_packet((char*)frame, sizeof(eth_frame_t) + sizeof(ipv4_header_t) + sizeof(udp_header_t) + sizeof(message)-1);
    free(frame);
}

uint32_t ip_list_to_uint32(uint8_t a, uint8_t b, uint8_t c, uint8_t d){
    return (a<<24) | (b<<16) | (c<<8) | (d);
}


//TODO check si ça s'addresse à moi (check if who has ip = my ip !!!)
void handleARPPacket(eth_frame_t* frame) {
    arp_packet_t* arp = (arp_packet_t*)frame->payload;


    if(htle16(arp->oper) == 1){ //Request
        printf("ARP Request: IP %d.%d.%d.%d -> MAC %x%x%x%x%x%x\n",
               arp->spa & 0xFF, (arp->spa >> 8) & 0xFF,
               (arp->spa >> 16) & 0xFF, (arp->spa >> 24) & 0xFF,
               arp->sha[0], arp->sha[1], arp->sha[2],
               arp->sha[3], arp->sha[4], arp->sha[5]);
        //on envoie la réponse
        eth_frame_t* arp_req = createARPReply(htle32(arp->spa), arp->sha);
        rtl8139_transmit_packet((char*)arp_req, sizeof(eth_frame_t) + sizeof(arp_packet_t));

        uint8_t* payload = (uint8_t*)malloc(sizeof(eth_frame_t) + sizeof(arp_packet_t));
        memcopy(payload, arp_req, sizeof(eth_frame_t) + sizeof(arp_packet_t));
        //broadcast_ping(payload, sizeof(eth_frame_t) + sizeof(arp_packet_t), 4);
        free(payload);

        free(arp_req);
        printf("ARP Reply Sent.\n");
        return;        
    }

    if (htle16(arp->oper) == 2) { // Reply
        //uint8_t* gateway_mac = (uint8_t*)malloc(6);
        printf("ARP Reply: IP %d.%d.%d.%d -> MAC %x%x%x%x%x%x\n",
               arp->spa & 0xFF, (arp->spa >> 8) & 0xFF,
               (arp->spa >> 16) & 0xFF, (arp->spa >> 24) & 0xFF,
               arp->sha[0], arp->sha[1], arp->sha[2],
               arp->sha[3], arp->sha[4], arp->sha[5]);

        // Tu peux stocker la MAC dans une petite ARP cache
        //memcopy(gateway_mac, arp->sha, 6);
        return;
    }

    printf("Received non-reply ARP packet: %x\n", htle16(arp->oper));

}

void handleIPv4Packet(eth_frame_t* frame){
    ipv4_header_t* ipv4_packet = (ipv4_header_t*)frame->payload;
    
    
    if(ipv4_packet->protocol == 1){
        printf("ICMP packet\n");
        return;
    }

    printf("unhandled protocol: %x", ipv4_packet->protocol);

}


void broadcast_ping(const uint8_t* payload, uint16_t size, uint16_t seq){
    icm_echo_t* echo = createICMPEchoRequest(1, seq, payload, size);
    uint8_t dst_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; //broadcast for testing
    eth_frame_t* frame = createIPv4EthernetFrame(dst_mac, net_info.gateway, 1, sizeof(icm_echo_t) + size, (uint8_t*)echo); //pinging the gateway for testing
    free(echo);
    rtl8139_transmit_packet((char*)frame, sizeof(eth_frame_t) + sizeof(ipv4_header_t) + sizeof(icm_echo_t) + size);
    free(frame);
}


void testPing(uint16_t seq){
    icm_echo_t* echo = createICMPEchoRequest(1, seq, (uint8_t*)"Hello", 5);
    uint8_t dst_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; //broadcast for testing
    eth_frame_t* frame = createIPv4EthernetFrame(dst_mac, net_info.gateway, 1, sizeof(icm_echo_t) + 5, (uint8_t*)echo); //pinging the gateway for testing
    free(echo);
    rtl8139_transmit_packet((char*)frame, sizeof(eth_frame_t) + sizeof(ipv4_header_t) + sizeof(icm_echo_t) + 5);
    free(frame);
}


void testARP(){
    printf("testing ARP...\n");
    eth_frame_t* arp_req = createARPRequest(net_info.gateway);
    rtl8139_transmit_packet((char*)arp_req, sizeof(eth_frame_t) + sizeof(arp_packet_t));
    free(arp_req);
    printf("ARP request sent\n");
}

void handleEthernetFrame(uint8_t* frame, uint16_t length){
 if (length < 14){ // Ethernet header minimal = 14 octets
        printf("Frame too short: %d bytes\n", (uint8_t)length);
        return;
 }
 

    // Cast sur header Ethernet
    eth_frame_t* eth = (eth_frame_t*)frame;

    // Afficher MAC source/destination pour debug
    printf("Ethernet frame: dst %x%x%x%x%x%x, src %x%x%x%x%x%x\n",
           eth->dst_mac[0], eth->dst_mac[1], eth->dst_mac[2],
           eth->dst_mac[3], eth->dst_mac[4], eth->dst_mac[5],
           eth->src_mac[0], eth->src_mac[1], eth->src_mac[2],
           eth->src_mac[3], eth->src_mac[4], eth->src_mac[5]);

    uint16_t ethertype = htle16(eth->ethertype); // convertir si nécessaire

    switch (ethertype) {
        case 0x0806: // ARP
            printf("Received ARP packet\n");
            handleARPPacket(eth);
            break;

        case 0x0800: // IPv4
            printf("Received IPv4 packet\n");
            handleIPv4Packet(eth);
            break;
        default:
            printf("Unknown Ethernet type: 0x%x, treating as raw\n", ethertype);
            //print packet payload:

            break;
    }
}