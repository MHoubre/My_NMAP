#include <stdio.h>
#include <stdlib.h>
#include "scanUDP.h"
#include "threadUtils.h"

int main(int argc, char **argv) {
    // example for scanTCP()
    struct PortList *list = createNewPortList(0);
    ActiveDevice *dest = createNewActiveDevice("192.168.1.1", list);

    // starting
    dest = scanTCP("192.168.1.20", dest);
    debugActiveDevice(dest);
    freeActiveDevice(dest);
    return 0;
}