#include "scanTCP.h"

ActiveDevice *scanTCP(char *srcAddr, ActiveDevice *dest) {
    ActiveDevice *source = createNewActiveDevice(srcAddr, NULL);
    /* création du thread de réception */
    createScanTCPThread(srcAddr, dest);

    /* lancement de l'émission */
    for(int port=1; port<4000; port++) {
        sendCustomSocket(source, dest, port, 1, 0); // create socket, connect
    }
    sleep(20);
    freeActiveDevice(source);
    return dest;
}

ActiveDeviceList *scanTCPNetwork(ActiveDeviceList *devices, ActiveDevice *device) {
    printf("Scanning ports for %s...\n",device->address);
    fflush(stdout);
    char * addr=malloc(sizeof(char)*20);
    *addr='\0';
    strcat(addr, "192.168.150.27");
    device = scanTCP(addr, device);

    return devices;
}
