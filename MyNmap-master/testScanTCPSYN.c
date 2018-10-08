#include <stdio.h>
#include <stdlib.h>
#include "scanTCPSYN.h"

int main(int argc, char **argv) {
    // example for scanTCP()
    struct PortList *list = createNewPortList(0);
    ActiveDevice *dest = createNewActiveDevice("192.168.1.36", list);

    // starting
    dest = scanTCPSYN("192.168.1.36", dest);
    debugActiveDevice(dest);
    freeActiveDevice(dest);
    return 0;
}