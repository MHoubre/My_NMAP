#include "threadUtils.h"

void createPingThread(ActiveDeviceList *devices) {
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (pthread_create(&thread, NULL, listenPing, (void *)*&devices) < 0)
        perror("Unable to create thread");
}

void *listenPing(void *arg) {
    ActiveDeviceList *devices = (ActiveDeviceList *)arg;
    int origin_time = time(NULL);
    int current_time = origin_time;
    // pendant 10 secs on analyse ce qu'on recoit
    while (origin_time + 10 > current_time) {
        int sock;
        if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
            perror("Unable to open socket");
        struct sockaddr_in addr;
        memset((char*)&addr, 0, sizeof(addr));
        addr.sin_family = PF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(1234);
        /* a priori plus besoin de ce timeout la
        struct timeval timeout;
        timeout.tv_sec=0;
        timeout.tv_usec=500;
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
            perror("setsockopt failed");*/
        if (bind(sock,(struct sockaddr *)&addr, sizeof(addr) ) <0) 
            perror ("servecho: erreur bind");
        // lecture de ce qu'on reçoit 
        char buffer[4096];
        if (recv(sock, (void *)buffer, 4095, 0) > 0) {
            struct iphdr *header = (struct iphdr *)buffer;
            in_addr_t ad = header->saddr;
            char *address = inet_ntoa(*(struct in_addr *)&ad); // in_addr
            char *copy = malloc(sizeof(char)*(strlen(address)+1));
            copy = strcpy(copy, address);            
            devices = addNewActiveDevice(devices, createNewActiveDevice(copy, NULL));
        }
        close(sock);

        current_time = time(NULL);
    }
    pthread_exit(0);
    return NULL;
}

void createScanTCPThread(char* srcAddr, ActiveDevice *dest) {
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ScanHelper *helper = malloc(sizeof(ScanHelper));
    helper->dest=dest;
    helper->srcAddr=srcAddr;
    if (pthread_create(&thread, NULL, listenTCP, (void *)helper) < 0)
        perror("Unable to create thread");    
}

void *listenTCP(void *arg) {
    char* src = (char *)(((ScanHelper *)arg)->srcAddr);
    ActiveDevice* dest = (ActiveDevice *)(((ScanHelper *)arg)->dest);
    int origin_time = time(NULL);
    int current_time = origin_time;
    while (current_time < origin_time + 20) {
        int resp = waitForResponse(src, 0, 1, 1);
        if (resp > 0) {
            addOpenPort(dest, resp);
        }
        current_time = time(NULL);
    }
    free(arg);
    pthread_exit(0);
    return NULL;
}

void createScanTCPSYNThread(char* srcAddr, ActiveDevice *dest) {
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ScanHelper *helper = malloc(sizeof(ScanHelper));
    helper->dest=dest;
    helper->srcAddr=srcAddr;
    if (pthread_create(&thread, NULL, listenTCPSYN, (void *)helper) < 0)
        perror("Unable to create thread");    
}

void *listenTCPSYN(void *arg) {
    char* src = (char *)(((ScanHelper *)arg)->srcAddr);
    ActiveDevice* dest = (ActiveDevice *)(((ScanHelper *)arg)->dest);
    int origin_time = time(NULL);
    int current_time = origin_time;
    while (current_time < origin_time + 20) {
        int resp = waitForAnyResponse(src, 0);
        if (resp > 0) {
            addOpenPort(dest, resp);
        }
        current_time = time(NULL);
    }
    free(arg);
    pthread_exit(0);
    return NULL;
}

void createScanUDPThread(char* srcAddr, ActiveDevice *dest) {
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ScanHelper *helper = malloc(sizeof(ScanHelper));
    helper->dest=dest;
    helper->srcAddr=srcAddr;
    if (pthread_create(&thread, NULL, listenUDP, (void *)helper) < 0)
        perror("Unable to create thread");    
}

void *listenUDP(void *arg) {
    char* src = (char *)(((ScanHelper *)arg)->srcAddr);
    ActiveDevice* dest = (ActiveDevice *)(((ScanHelper *)arg)->dest);
    int origin_time = time(NULL);
    int current_time = origin_time;
    while (current_time < origin_time + 20) {
        int resp = waitForIcmpResponse(src);
        if (resp > 0) {
            addOpenPort(dest, resp);
        }
        current_time = time(NULL);
    }
    free(arg);
    pthread_exit(0);
    return NULL;
}