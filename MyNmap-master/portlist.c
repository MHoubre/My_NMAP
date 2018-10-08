#include "portlist.h"

PortList *createNewPortList(int value) {
    PortList *newList = (PortList *)malloc(sizeof(PortList));
    newList->port = value;
    newList->next=NULL;
    newList->isLast=1;
    return newList;
}

PortList *addToList(PortList *list, int value) {
    if (list->port == 0) { // si on a créé une liste vide on modifie juste le premier champ
        list->port=value;
        return list;
    }
    int exist=0;
    int end=0;
    PortList *tmp = list;
    while (!tmp->isLast) {
        if (tmp->port == value) {
            exist++;
            end++;
            break;
        }
        tmp = tmp->next;
    }
    if (!end) {
        if (tmp->port == value)
            exist++;
    }
    if (exist)
        return list;
    
    tmp = list;
    while (!tmp->isLast) {
        tmp = tmp->next;
    }
    tmp->isLast = 0;
    PortList *newLast = (PortList *)malloc(sizeof(PortList));
    newLast->port = value;
    newLast->next = NULL;
    newLast->isLast = 1;
    tmp->next = newLast;
    return list;
}

void freePortList(PortList *list) {
    PortList *tmp=list;
    PortList *next=list->next;
    while(!tmp->isLast) {
        //free(&(tmp->isLast));
        next=tmp->next;
        free(tmp);
        tmp = next;
    }
    //free(&(tmp->isLast));
    free(tmp);
}

int *getPorts(PortList *list) {
    int size=1;
    PortList *tmp=list;
    while (!tmp->isLast) {
        size++;
        tmp=tmp->next;
    }
    tmp=list; // on le replace au début de la chaine
    int *ports = (int *)malloc(sizeof(int)*size);
    for (int i=0; i<size; i++) {
        *(ports+i) = tmp->port;
        tmp = tmp->next;
    }
    return ports;
}

int getListSize(PortList *list) {
    int size=1;
    PortList *tmp=list;
    while (!tmp->isLast) {
        size++;
        tmp = tmp->next;
    }
    return size;
}