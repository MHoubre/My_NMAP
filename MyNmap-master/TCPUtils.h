#ifndef _TCPUTILS_H
#define _TCPUTILS_H

#include "activeDevice.h"
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <netinet/tcp.h>   //Provides declarations for tcp header
#include <netinet/ip.h>    //Provides declarations for ip header

typedef struct PseudoHeader
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t tcp_length;
}PseudoHeader;

/**
 * Custom checksum
 * @param{ptr} is the data we want to calculate its checksum
 * @param{nbytes} is its length
 * @return the checksum
 */
unsigned short csum(unsigned short *ptr,int nbytes);

/**
 * Send a custom socket to the specified device
 * @param{src} is the device we want to scan from
 * @param{dst} is the destination device we want to scan
 * @param{port} is the selected port to scan
 * @param{syn} is the SYN flag for the socket
 * @param{ack} is the ACK flag for the socket
 */
void sendCustomSocket(ActiveDevice *src, ActiveDevice *dst, int port, int syn, int ack);

/**
 * Handle the response socket
 * @param{rst} is the RST flag for the socket
 * @param{syn} is the SYN flag for the socket
 * @param{ack} is the ACK flag for the socket
 * @return 1 is port is open, 0 else
 */
int waitForResponse(char* srcAddr, int rst, int ack, int syn);

/**
 * Response detector.
 * @return 0 if no response has been received, 1 else
 */
int waitForAnyResponse(char *srcAddr, int idle);

#endif