#include <stdio.h>
#include <stdlib.h>

#include "activeDevice.h"

int main(int argc, char **argv) {
    printf("ActiveDevice - Test n°1 :\n");
    // example for defining an ActiveDevice
    PortList *list = createNewPortList(2222);
    list = addToList(list, 80);
    list = addToList(list, 3306);

    //creating
    ActiveDevice *ad = createNewActiveDevice("127.0.0.1", list);
    debugActiveDevice(ad);
    //freeing
    freeActiveDevice(ad);

    printf("\nActiveDevice - Test n°2 :\n");
    ActiveDevice *ad2 = createNewActiveDevice("127.0.0.1", NULL);
    ad2 = addOpenPort(ad2, 2222);
    debugActiveDevice(ad2);
    freeActiveDevice(ad2);

    printf("\nActiveDevice - Test n°3 :\n");
    ActiveDeviceList *adl=createNewActiveDeviceList();
    ad = createNewActiveDevice("127.0.0.1", NULL);
    ad2 = createNewActiveDevice("1.2.3.4",NULL);
    adl = addNewActiveDevice(adl,ad);
    adl = addNewActiveDevice(adl,ad2);
    debugActiveDeviceList(adl);
    freeActiveDeviceList(adl);
    return 0;
}