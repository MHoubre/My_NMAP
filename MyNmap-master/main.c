#ifndef _MAIN_C
#define _MAIN_C

#include <stdio.h>
#include <stdlib.h>

#include "ping.h"
#include "scanTCP.h"
#include "scanTCPSYN.h"
#include "scanUDP.h"
#include "scanIDLE.h"

int main(int argc, char** argv) {
    char *network = malloc(sizeof(char)*20);
    strcpy(network, argv[1]);
    printf("#########################\n");
    printf("#                       #\n");
    printf("#        MyNmap         #\n");
    printf("#                       #\n");
    printf("#########################\n");
    printf("\n");
    ActiveDeviceList *devices = createNewActiveDeviceList();
    devices=ping(network, devices);
    debugActiveDeviceList(devices);
    
    int choice=0;
    int cont=1;
    while (cont == 1) {
        while (choice <1 || choice>6) {
            choice=1;
            printf("Which type of scan do you want to do ?\n");
            printf("1) TCP scan             2) TCP/SYN scan\n");
            printf("3) UDP scan             4) TCP IDLE scan\n");
            printf("5) Scan via FTP server  6) Exit \n");
            printf("\n");
            fflush(stdout);
            scanf("%d", &choice);
            printf("\n");
            fflush(stdout);
        }
        int addr=0;
        switch(choice) {
            case 6:
                cont=0;
                break;
            case 5:
                choice=0;
                break;
            case 4:
                printf("Which address do you want to scan ?\n");
                fflush(stdout);
                scanf("%d",&addr);
                int zomb=0;
                printf("Which device do you want to use as zombie?\n");
                fflush(stdout);
                scanf("%d",&zomb);
                int port=0;
                printf("Which port do you want to scan (should be closed on zombie)?\n");
                fflush(stdout);
                scanf("%d",&port);
                if (scanIDLE(devices->list[addr], devices->list[zomb], port)) {
                    printf("Port %d ouvert.\n", port);
                }
                else
                    printf("Port %d fermé.\n", port);
                choice=0;
                break;
            
            case 3:
                printf("Which address do you want to scan ?\n");
                fflush(stdout);
                scanf("%d",&addr);
                devices = scanUDPNetwork(devices, devices->list[addr]);
                printf("\n");
                fflush(stdout);
                debugActiveDevice(devices->list[addr]);
                choice=0;
                break;
            case 2:
                printf("Which address do you want to scan ?\n");
                fflush(stdout);
                scanf("%d",&addr);
                devices = scanTCPSYNNetwork(devices, devices->list[addr]);
                printf("\n");
                fflush(stdout);
                debugActiveDevice(devices->list[addr]);
                choice=0;
                break;
           case 1:
                printf("Which address do you want to scan ?\n");
                fflush(stdout);
                scanf("%d",&addr);
                devices = scanTCPNetwork(devices, devices->list[addr]);
                printf("\n");
                fflush(stdout);
                debugActiveDevice(devices->list[addr]);
                choice=0;
                break;
        }
    }
    free(network);
    freeActiveDeviceList(devices);
    return 0;
}

#endif