#include "scanTCPSYN.h"

ActiveDevice *scanTCPSYN(char *srcAddr, ActiveDevice *dest) {
    ActiveDevice *source = createNewActiveDevice(srcAddr, NULL);
    /* création du thread de réception */
    //createScanTCPSYNThread(srcAddr, dest);

    /* lancement de l'émission */
    for(int port=1; port<4000; port++) {
        sendCustomSocket(source, dest, port, 1, 1); // create socket, connect
        if (waitForAnyResponse(srcAddr, 0)) {
            addOpenPort(dest, port);
        }
    }
    sleep(20);
    freeActiveDevice(source);
    return dest;
}

ActiveDeviceList *scanTCPSYNNetwork(ActiveDeviceList *devices, ActiveDevice *device) {
    printf("Scanning ports for %s...\n",device->address);
    fflush(stdout);
    char * addr=malloc(sizeof(char)*20);
    *addr='\0';
    strcat(addr, "192.168.150.27");
    device = scanTCPSYN(addr, device);

    return devices;
}
