#include "UDPUtils.h"
#define BUFFER_LEN 4096

unsigned short UDPcsum(unsigned short *ptr,int nbytes) {
    long sum;
    short oddbyte;
    short answer;
    sum=0;
    while (nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }
    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
    return(answer);
}

void sendUdpSocket(ActiveDevice *src, ActiveDevice *dest, int port){
    int udp_socket;
    if((udp_socket= socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP)) < 0){
        perror("Unable to create socket");
    }

    char datagram[4096],source_ip[32], *data;
    memset(datagram, 0, 4096);

    struct iphdr *ip_h = (struct iphdr *) datagram ;
    struct udphdr *udp_h = (struct udphdr *) (datagram + sizeof (struct ip));
    struct sockaddr_in sin;

    //Data preparation
    data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
    //strcpy(data, "lemme scan ur ports baby");

    strcpy(source_ip, src->address);
    sin.sin_family=AF_INET;
    sin.sin_port=htons(port);
    sin.sin_addr.s_addr= inet_addr(dest->address);

    ip_h->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + strlen(data);

    // IP header
    ip_h->ihl = 5;
    ip_h->version = 4;
    ip_h->tos = 0;
    ip_h->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + strlen(data);
    ip_h->id = htonl (54321); //Id of this packet
    ip_h->frag_off = 0;
    ip_h->ttl = 255;
    ip_h->protocol = IPPROTO_UDP;
    ip_h->check = 0;      //Set to 0 before calculating checksum
    ip_h->saddr = inet_addr(source_ip);    //Spoof the source ip address
    ip_h->daddr = sin.sin_addr.s_addr;
    //Ip checksum
    ip_h->check = UDPcsum ((unsigned short *) datagram, ip_h->tot_len);


    //UDP header
    udp_h->source=htons(1234);
    udp_h->dest=htons(port);
    udp_h->check=0;
    if (sendto (udp_socket, datagram, ip_h->tot_len ,  0, (struct sockaddr *) &sin, sizeof (sin)) < 0) {
            perror("sendto failed");
    }
    printf("Requete envoyée port %d\n",port);
    fflush(stdout);
    close(udp_socket);
}

int waitForIcmpResponse(char *srcAddr){
    int sock;
    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
        perror("Unable to open socket");
    }
    struct sockaddr_in addr;
    memset((char*)&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(1234);

    struct timeval timeout;
    timeout.tv_sec=1;
    timeout.tv_usec=0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
        perror("setsockopt failed");
    }
    if (bind(sock,(struct sockaddr *)&addr, sizeof(addr) ) <0) {
        perror ("servecho: erreur bind");
    }

    char buffer[BUFFER_LEN];
    if (recv(sock, (void *)buffer, BUFFER_LEN-1, 0) <= 0) {
        close(sock);
        return 0;
    }
    //printf("REPONSE \n");
    //fflush(stdout);
    close(sock);
    buffer[BUFFER_LEN-1]='\0';
    // maintenant on lit le socket qu'on a recu
    struct iphdr *iph = (struct iphdr *) buffer;
    in_addr_t ad = iph->daddr;
    char *d = inet_ntoa(*(struct in_addr *)&ad);
    //printf("dest attendue: %s & dest : %s\n",srcAddr, d);
    //fflush(stdout);
    if (strncmp(srcAddr, d, 15) != 0)
        return 0;
    //printf("    ADRESSE OK\n");
    //fflush(stdout);
    // verifie que le port est ouvert
    struct icmphdr *icmph = (struct icmphdr *) (buffer + sizeof(struct iphdr));
    //printf("        ICMP %d\n",icmph->type);
    //fflush(stdout);
    //printf("WTF: %d@\n", icmph->un.echo.id);
    //printf("WTF: %d@\n", icmph->un.echo.sequence);
    //fflush(stdout);
    if (icmph->type == ICMP_PORT_UNREACH){
        return 0;
    }
    else {/*
        struct udphdr *udph = (struct udphdr *) icmph;
        int port = ntohs(udph->source);
        printf("        PORT %d\n",port);
        fflush(stdout);
        return port;*/
        return 1;
    }
}