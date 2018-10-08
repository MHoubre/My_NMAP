#include "activeDevice.h"
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>   //Provides declarations for udp header
#include <netinet/ip.h>    //Provides declarations for ip header


unsigned short UDPcsum(unsigned short *ptr,int nbytes);

/**
 * Sends the UDP socket that we need to scan the port
 * @param{src} The ActiveDevice from which the socket will be sent
 * @param{dest} The ActiveDevice we want to scan
 * @param{port} The port we want to scan
 */
void sendUdpSocket(ActiveDevice *src,ActiveDevice *dest,int port);

/**
 * Handles the response.
 * Waits for a ICMP_PORT_UNREACH to know if the port is available or not
 */
int waitForIcmpResponse(char *srcAddr);