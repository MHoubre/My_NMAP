#include "TCPUtils.h"

#define PACKET_LEN 4096
#define BUFFER_LEN 4096

unsigned short csum(unsigned short *ptr,int nbytes) {
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

void sendCustomSocket(ActiveDevice *src, ActiveDevice *dst, int port, int syn, int ack) {
    int scanSocket;
    if ((scanSocket = socket(PF_INET, SOCK_RAW, IPPROTO_RAW)) == -1) {
        perror("Unable to open socket");
        //exit(-1);
    }
    //Datagram to represent the packet
    char datagram[4096] , source_ip[32] , *data , *pseudogram;
    //zero out the packet buffer
    memset (datagram, 0, 4096);
    //IP header
    struct iphdr *iph = (struct iphdr *) datagram;
    //TCP header
    struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
    struct sockaddr_in sin;
    PseudoHeader psh;
    //Data part
    data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
    strcpy(data , "This is not a friendly one.");
    //some address resolution
    strcpy(source_ip , src->address);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = inet_addr (dst->address);
    //Fill in the IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
    iph->id = htonl (54321); //Id of this packet
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0;      //Set to 0 before calculating checksum
    iph->saddr = inet_addr(source_ip);    //Spoof the source ip address
    iph->daddr = sin.sin_addr.s_addr;
    //Ip checksum
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);
    //TCP Header
    tcph->source = htons(1234);
    tcph->dest = htons(port);
    tcph->seq = 0;
    tcph->ack_seq = 0;
    tcph->doff = 5;  //tcp header size
    tcph->fin=0;
    tcph->syn=syn;
    tcph->rst=0;
    tcph->psh=0;
    tcph->ack=ack;
    tcph->urg=0;
    tcph->window = htons (5840); /* maximum allowed window size */
    tcph->check = 0; //leave checksum 0 now, filled later by pseudo header
    tcph->urg_ptr = 0;
    //Now the TCP checksum
    psh.source_address = inet_addr( source_ip );
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data) );
    int psize = sizeof(PseudoHeader) + sizeof(struct tcphdr) + strlen(data);
    pseudogram = malloc(psize);
    memcpy(pseudogram , (char*) &psh , sizeof (PseudoHeader));
    memcpy(pseudogram + sizeof(PseudoHeader) , tcph , sizeof(struct tcphdr) + strlen(data));
    tcph->check = csum( (unsigned short*) pseudogram , psize);
    //IP_HDRINCL to tell the kernel that headers are included in the packet
    int one = 1;
    const int *val = &one;
    if (setsockopt (scanSocket, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
        perror("Error setting socket option");
        exit(0);
    }
    if (sendto (scanSocket, datagram, iph->tot_len ,  0, (struct sockaddr *) &sin, sizeof (sin)) < 0) {
            perror("sendto failed");
    }
    free(pseudogram);
    close(scanSocket);
}

int waitForResponse(char *srcAddr, int rst, int ack, int syn) {
    int sock;
    if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1) {
        perror("Unable to open socket");
    }
    struct sockaddr_in addr;
    memset((char*)&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(1234);

    struct timeval timeout;
    timeout.tv_sec=0;
    timeout.tv_usec=500;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        perror("setsockopt failed");

    if (bind(sock,(struct sockaddr *)&addr, sizeof(addr) ) <0) {
        perror ("servecho: erreur bind");
    }

    char buffer[BUFFER_LEN];
    if (recv(sock, (void *)buffer, BUFFER_LEN-1, 0) <= 0) {
        //perror("Unable to listen");
        close(sock);
    } else {
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
        struct tcphdr *tcph = (struct tcphdr *) (buffer + sizeof (struct ip));

        if (tcph->rst == rst && tcph->syn == syn && tcph->ack == ack) {
            int port = ntohs(tcph->source);
            return port;
        }
        else
            return 0;
    }
    close(sock);
    return 0;
}

int waitForAnyResponse(char *srcAddr, int idle) {
    int sock;
    if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1) {
        perror("Unable to open socket");
    }
    struct sockaddr_in addr;
    memset((char*)&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(1234);

    struct timeval timeout;
    timeout.tv_sec=0;
    timeout.tv_usec=100;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        perror("setsockopt failed");
    
    if (bind(sock,(struct sockaddr *)&addr, sizeof(addr) ) <0) {
        perror ("servecho: erreur bind");
    }

    char buffer[BUFFER_LEN];
    if (recv(sock, (void *)buffer, BUFFER_LEN-1, 0) <= 0) {
        close(sock);
        return 1;
    } else { 
        struct iphdr *iph = (struct iphdr *) buffer;
        if (!idle) {
            close(sock);
            buffer[BUFFER_LEN-1]='\0';
            // maintenant on lit le socket qu'on a recu
            in_addr_t ad = iph->daddr;
            char *d = inet_ntoa(*(struct in_addr *)&ad);
            //printf("dest attendue: %s & dest : %s\n",srcAddr, d);
            //fflush(stdout);
            if (strncmp(srcAddr, d, 15) != 0)
                return 0;
            struct tcphdr *tcph = (struct tcphdr *) (buffer + sizeof (struct ip));

            if (tcph->rst == 0) {
                int port = ntohs(tcph->source);
                return port;
            }
            else
                return 0;
        } else {
            return iph->id;
        }
    }
}