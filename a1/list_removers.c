/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_alloc.h"

void* ListRemove(LIST* list){
    NODE* node_to_remove;

    if (list == NULL) {
        fprintf(stderr, "list is NULL\n");
        return NULL;
    }
    node_to_remove = list->curr;
    
    list->curr->prev->next = list->curr->next;
    
    release_node(node_to_remove);   /* method inside our list_alloc lib */
    return node_to_remove->data;
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
