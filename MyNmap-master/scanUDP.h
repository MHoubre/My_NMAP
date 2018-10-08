#ifndef _SCANUDP_H
#define _SCANUDP_H

#include "activeDevice.h"
#include "threadUtils.h"
#include "UDPUtils.h"
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

/**
 * Scan the specified ActiveDevice with the specified @IP
 * @param{srcAddr} is the @IP which will be used as the source in the socket
 * @param{dest} is the ActiveDevice we want to scan
 * @return @param{dest} with its portlist filled.
 */
ActiveDevice *scanUDP(char *srcAddr, ActiveDevice *dest);


ActiveDeviceList *scanUDPNetwork(ActiveDeviceList *devices, ActiveDevice *device);

#endif