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
        fprintf(stderr, "Error in ListRemove: list is NULL\n");
        return NULL;
    } else if ( list->curr == NULL) {
        fprintf(stderr, "Error in ListRemove: list->curr is NULL\n");
        return NULL;
    }
    node_to_remove = list->curr;

    /* 4 scenarios:
     * 1) list only has 1 node
     * 2) curr is at tail
     * 3) curr is at head
     * 4) everything else
     */

    if (list->size == 1) {
        list->curr = NULL;
        list->head = NULL;
        list->tail = NULL;
    } else if ( list->curr == list->tail) {
        node_to_remove->prev->next = NULL;
        list->curr = list->curr->prev;
        list->tail = list->curr;
    } else if ( list->curr == list->head) {
        node_to_remove->next->prev = NULL;
        list->curr = list->curr->next;
        list->head = list->curr;
    } else {
        node_to_remove->prev->next = node_to_remove->next;
        node_to_remove->next->prev = node_to_remove->prev;
        list->curr = list->curr->next;
    }
    
    list->size--;
    release_node(node_to_remove);   /* method inside our list_alloc lib */
    return node_to_remove->data;
}

void ListFree(LIST* list, void (* itemFree)(void* itemToBeFreed)){
    NODE* temp_node;
    if (list == NULL) {
        fprintf(stderr, "Error in ListFree: list is NULL\n");
        return;
    }
    list->curr = list->head;
    for(;;){
        if (list->curr == NULL)
            break;
        temp_node = list->curr;
        itemFree(list->curr->data);
        list->curr = list->curr->next;
        release_node(temp_node);
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    release_list(list);
}

void* ListTrim(LIST* list){
    NODE* node_to_remove;
    if (list == NULL) {
        fprintf(stderr, "Error in ListTrim: list is NULL\n");
        return NULL;
    }

    /* 3 cases to consider:
     * 1) list is empty. Return NULL.
     * 2) list only has 1 element. Nullify everything.
     * 3) general case
     */
    if (list->size == 0) {
        return NULL;
    } else if (list->size == 1) {
        node_to_remove = list->tail;
        list->curr = NULL;
        list->tail = NULL;
        list->head = NULL;
    } else {
        node_to_remove = list->tail;
        node_to_remove->prev->next = NULL;
        list->tail = node_to_remove->prev;
        list->curr = list->tail;
    }
    list->size--;
    release_node(node_to_remove);
    return(node_to_remove->data);
}
