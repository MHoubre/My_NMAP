#ifndef _ACTIVEDEVICE_H
#define _ACTIVEDEVICE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "portlist.h"

typedef struct ActiveDevice {
    char *address;
    PortList *list;
} ActiveDevice;

typedef struct ActiveDeviceList {
    ActiveDevice **list;
    int size;
} ActiveDeviceList;

ActiveDeviceList *createNewActiveDeviceList();

ActiveDeviceList *addNewActiveDevice(ActiveDeviceList *activeDeviceList, ActiveDevice *activeDevice);

void freeActiveDeviceList(ActiveDeviceList *activeDeviceList);

void debugActiveDeviceList(ActiveDeviceList *activeDeviceList);

/*
 * Create an ActiveDevice.
 * @param{address} is the @IP of the scanned device
 * @param{list} is the list of its listening ports
 */
ActiveDevice *createNewActiveDevice(char *address, PortList *list);

/*
 * Free an ActiveDevice.
 * @param{device} is the activeDevice variable which is to be freed.
 */
void freeActiveDevice(ActiveDevice *device);

/*
 * Print informations about an ActiveDevice.
 * @param{device} is the activeDevice variable we want informations about.
 */
void debugActiveDevice(ActiveDevice *activeDevice);

/*
 * Add a port to the list of an ActiveDevice.
 * @param{device} is the activeDevice variable we want add a port to.
 * @param{port} is the open port.
 */
ActiveDevice *addOpenPort(ActiveDevice *device, int port);

#endif