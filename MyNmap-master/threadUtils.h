#ifndef _THREADUTILS_H
#define _THREADUTILS_H

#include <pthread.h>
#include "activeDevice.h"
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include "TCPUtils.h"
#include "UDPUtils.h"

typedef struct ScanHelper {
    char *srcAddr;
    ActiveDevice *dest;
} ScanHelper;


void createPingThread(ActiveDeviceList *devices);
void *listenPing(void *arg);

void createScanTCPThread(char* srcAddr, ActiveDevice *dest);
void *listenTCP(void *arg);

void createScanTCPSYNThread(char* srcAddr, ActiveDevice *dest);
void *listenTCPSYN(void *arg);

void createScanUDPThread(char *srcAddr, ActiveDevice *dest);
void *listenUDP(void *arg);

#endif