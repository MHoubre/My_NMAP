#include "scanUDP.h"

ActiveDevice *scanUDP(char *srcAddr, ActiveDevice *dest) {
    ActiveDevice *source = createNewActiveDevice(srcAddr, NULL);
    // thread creation
    //createScanUDPThread(srcAddr, dest);

    // for each port
    for(int port=442; port<447; port++) {
        sendUdpSocket(source, dest, port); // create socket, connect
        int resp = waitForIcmpResponse(srcAddr);
        if (resp>0) {
            addOpenPort(dest, port);
        }
    }
    //sleep(20);
    freeActiveDevice(source);
    return dest;
}

ActiveDeviceList *scanUDPNetwork(ActiveDeviceList *devices, ActiveDevice *device) {
    printf("Scanning ports for %s...\n",device->address);
    fflush(stdout);
    char * addr=malloc(sizeof(char)*20);
    *addr='\0';
    strcat(addr, "192.168.150.27");
    device = scanUDP(addr, device);

    return devices;
}
