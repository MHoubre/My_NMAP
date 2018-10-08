#ifndef _SCANIDLE_H
#define _SCANIDLE_H

#include "TCPUtils.h"
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "TCPUtils.h"

int scanIDLE(ActiveDevice *device, ActiveDevice *zombie, int port);

#endif