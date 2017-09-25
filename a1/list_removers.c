/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void* ListRemove(LIST* list){
    printf("Got to procedure ListPrepend\n");
    if (list == NULL){
        printf("Error in procedure ListRemove: Invalid parameter list\n");
        return NULL;
    }
    return NULL;
}

void ListFree(LIST* list, void (* itemFree)(void* itemToBeFreed)){
    printf("Got to procedure ListFree\n");
    if (list == NULL){
        printf("Error in procedure ListFree: Invalid parameter list\n");
    } else if (itemFree == NULL) {
        printf("Error in procedure ListFree: Invalid parameter itemFree\n");
    }
}

void* ListTrim(LIST* list){
    printf("Got to procedure ListTrim\n");
    if (list == NULL){
        printf("Error in procedure ListTrim: Invalid parameter list\n");
        return NULL;
    }
    return NULL;
}