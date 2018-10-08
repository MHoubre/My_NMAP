#include "scanIDLE.h"

int scanIDLE(ActiveDevice *device, ActiveDevice *zombie, int port) {
    char * addr=malloc(sizeof(char)*20);
    *addr='\0';
    strcat(addr, "192.168.150.27");
    ActiveDevice *source = createNewActiveDevice(addr, NULL);
    // etape 1 - récupérer l'id
    sendCustomSocket(source, zombie, port, 1, 1);
    int id = waitForAnyResponse(addr, 1);
    // etape 2 envoyer le faux SYN
    sendCustomSocket(zombie, device, port, 1, 0);
    sleep(1); // histoire detre sur 
    //etape 3 - id
    sendCustomSocket(source, zombie, port, 1, 1);
    int idbis = waitForAnyResponse(addr, 1);
    return (idbis-id-1);
}