#include "ping.h"

ActiveDeviceList* ping(char* arg, ActiveDeviceList *devices){
    printf("Detecting active devices on the network...\n");
    char *tmp=malloc(sizeof(char)*(strlen(arg)+1));
    *tmp='\0';
    tmp = strcat(tmp, arg);
    char **addresses_to_ping = parse(tmp);
    int numb = number(tmp);
    free(tmp);

    /* création du thread de réception */
    createPingThread(devices);

    /* lancement de l'émission */
    for(int i=0; i<numb; i++){
        sendPing(addresses_to_ping[i]);
        free(addresses_to_ping[i]);
    }
    free(addresses_to_ping);
    sleep(10);
    return devices;
}

void sendPing(char *addr) {
    int scanSocket;
    if ((scanSocket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
        perror("Unable to open socket");
    // adresse de destination
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(1234);
    dest_addr.sin_addr.s_addr = inet_addr(addr);

    // header icmp du socket
    struct icmphdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.type = ICMP_ECHO;         
    hdr.code = 0;
    hdr.checksum = 0;  /* required for actual checksum calculation */
    hdr.un.echo.id = htons(0x1234); /* identifier */
    hdr.un.echo.sequence = htons(1); /* sequence no */
    // checksum
    hdr.checksum = icmp_checksum((uint16_t *)&hdr, sizeof(hdr));

    /* broadcast */
    int bd=1;
    if (setsockopt(scanSocket, SOL_SOCKET, SO_BROADCAST, &bd, sizeof(bd))<0) {
        perror("setsockopt");
    }

    /* send the echo request packet */
    const ssize_t sent_bytes = sendto(scanSocket, &hdr, sizeof(hdr), 0,
        (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (sent_bytes != sizeof(hdr))
        perror("sendto");
    close(scanSocket);
}

uint16_t icmp_checksum(const uint16_t *const data, const size_t byte_sz) {
    if (0 != (byte_sz & 1)) {
        perror("icmp_checksum");
    }
    uint32_t accu = 0;
    for (size_t i=0; i < (byte_sz >> 1); ++i) {
        accu = accu + data[i];
    }

     /*  Fold 32-bit sum to 16 bits */
    while (accu >> 16) {
        accu = (accu & 0xffff) + (accu >> 16);
    }
    const uint16_t checksum = ~accu;
    return checksum;
}