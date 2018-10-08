#ifndef _PING_H
#define _PING_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "threadUtils.h"
#include "parser.h"

#define BUFFER_LEN 4096

void sendPing(char *addr);

ActiveDeviceList* ping(char* arg, ActiveDeviceList *devices);

uint16_t icmp_checksum(const uint16_t *const data, const size_t byte_sz);

#endif