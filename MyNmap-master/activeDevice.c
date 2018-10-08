#include "activeDevice.h"

ActiveDeviceList *createNewActiveDeviceList() {
    ActiveDeviceList *newActiveDeviceList = (ActiveDeviceList *)malloc(sizeof(ActiveDeviceList));
    newActiveDeviceList->size=0;
    newActiveDeviceList->list=NULL;
    return newActiveDeviceList;
}

ActiveDeviceList *addNewActiveDevice(ActiveDeviceList *activeDeviceList, ActiveDevice *activeDevice) {
    if (activeDeviceList->list == NULL) {
        activeDeviceList->list = (ActiveDevice**)malloc(sizeof(ActiveDevice*));
        activeDeviceList->list[0] = activeDevice;
        activeDeviceList->size++;
    } else {
        int exist=0;
        for (int i=0; i<activeDeviceList->size; i++) {
            if (strcmp(activeDeviceList->list[i]->address, activeDevice->address) == 0) {
                exist = 1;
                break;
            }
            
        }
        if (exist)
            return activeDeviceList;
        //activeDeviceList->list=realloc(activeDeviceList->list, sizeof(activeDeviceList->list)+sizeof(ActiveDevice*));
        ActiveDevice **tmp=malloc(sizeof(ActiveDevice *)*(activeDeviceList->size+1));
        for (int i=0; i<activeDeviceList->size; i++) {
            tmp[i]=activeDeviceList->list[i];
        }
        free(activeDeviceList->list);
        activeDeviceList->list = tmp;
        activeDeviceList->list[activeDeviceList->size]=activeDevice;
        activeDeviceList->size++;
    }
    return activeDeviceList;
}

void freeActiveDeviceList(ActiveDeviceList *activeDeviceList) {
    for (int i=0; i<activeDeviceList->size; i++) {
        freeActiveDevice(activeDeviceList->list[i]);
    }
    free(activeDeviceList->list);
    free(activeDeviceList);
}

void debugActiveDeviceList(ActiveDeviceList *activeDeviceList) {
    for (int i=0; i<activeDeviceList->size; i++) {
        printf("[%d] %s is up.\n",i, activeDeviceList->list[i]->address);
        fflush(stdout);
    }
}

ActiveDevice *createNewActiveDevice(char *address, PortList *list) {
    ActiveDevice *newActiveDevice = (ActiveDevice *)malloc(sizeof(ActiveDevice));
    newActiveDevice->address = address;
    if (list == NULL)
        newActiveDevice->list = createNewPortList(0);
    else
        newActiveDevice->list = list;
    return newActiveDevice;
}

void freeActiveDevice(ActiveDevice *device) {
    freePortList(device->list);
    free(device->address);
    free(device);
}

ActiveDevice *addPortToDevice(ActiveDevice *device, int value) {
    device->list = addToList(device->list, value);
    return device;
}

void debugActiveDevice(ActiveDevice *activeDevice) {
    int *ports = getPorts(activeDevice->list);
    int size = getListSize(activeDevice->list);

    printf("Debug for an ActiveDevice :\n");
    printf("IP address : %s\n", activeDevice->address);
    printf("List of listening ports :\n");
    for (int i=0; i<size; i++) {
        if (ports[i] == 0) {
            printf("    None.\n");
        } else {
            printf("    - port %d ;\n", ports[i]);
        }
    }
    free(ports);
    printf("End of debugging.\n");
    fflush(stdout);
}

ActiveDevice *addOpenPort(ActiveDevice *device, int port) {
    device->list = addToList(device->list, port);
    return device;
}