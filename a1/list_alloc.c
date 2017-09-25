/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_alloc.h"

NODE* request_node(void) {
    if (curr_free_node == NULL){
        return NULL;
    }
    NODE* new_node = curr_free_node;
    curr_free_node = curr_free_node->next;
    return new_node;
}

LIST* request_list(void) {
    if (curr_free_list == NULL){
        return NULL;
    }
    LIST* new_list = curr_free_list;
    curr_free_list = curr_free_list->next_free;
    new_list->next_free = NULL;
    return new_list;
}

void release_node(NODE* node) {
    /* TODO: complete this later */
    node->next = curr_free_node;
    curr_free_node = node;
}

void release_list(LIST* list) {
    list->head = NULL;

    list->next_free = curr_free_list;
    curr_free_list = list;
}
