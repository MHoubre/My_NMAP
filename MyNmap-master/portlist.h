#ifndef _PORTLIST_H
#define _PORTLIST_H

/*
 * Imports
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * Structs
 */
typedef struct PortList {
    int port;
    struct PortList *next;
    int isLast;
} PortList;

/*
 * Functions
 */

/*
 * Create a new PortList
 * @param{value} is the port the new PortList is initialized with
 * @return a new PortList initialized with @param{value}
 */
struct PortList *createNewPortList(int value);

/*
 * Add an port entry to the specified Portlist
 * @param{list} is the PortList to add an entry. If 
 * @param{value} is the port added 
 * @return the updated PortList
 */
struct PortList *addToList(struct PortList *list, int value);

/*
 * Free a PortList
 * @param{list} is freed
 */
void freePortList(struct PortList *list);

/*
 * Extract all saved ports from the specified PortList
 * @param{list} is the PortList we want to get ports from
 * @return an int array which contains all saved ports
 */
int *getPorts(PortList *list);

/*
 * Return the size of the specified PortList
 * @param{list} is the PortList we want to get its size
 * @return its size
 */
int getListSize(PortList *list);

#endif