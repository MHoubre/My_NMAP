#include <stdlib.h>
#include <stdio.h>

#include "portlist.h"

int main(int argc, char** argv) {
    printf("Test n°1 :\n");
    int i=2222;
    PortList *list = createNewPortList(i);
    fflush(stdout);
    int j=80;
    list = addToList(list, j);
    int *ports = getPorts(list);
    for (int i=0; i<getListSize(list); i++) {
        printf("Port: %d\n",ports[i]);
    }
    freePortList(list);
    free(ports);
    printf("Test n°2 :\n");
    PortList *l2 = createNewPortList(0);
    l2 = addToList(l2, 1);
    int *p2 = getPorts(l2);
    for (int i=0; i<getListSize(l2); i++) {
        printf("Port: %d\n",p2[i]);
    }
    free(p2);
    freePortList(l2);
    return 0;
}