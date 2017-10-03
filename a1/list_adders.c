/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "list.h"
#include "list_alloc.h"

bool memory_allocated = false;

/* list memory pool to create new lists */
LIST** list_memory = NULL;
LIST* curr_free_list;

/* node memory pool to create new nodes */
NODE** node_memory = NULL;
NODE* curr_free_node;

LIST* ListCreate(){
    LIST* new_list;

    new_list = request_list();
    if (new_list == NULL){
        return NULL; /* TODO: need to grow the memory when on bonus */
    }
    new_list->size = 0;
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->curr = NULL;
    return new_list;
}

int ListAdd(LIST* list, void* item){
    NODE* new_node;

    if (list == NULL){
        printf("Error in procedure ListAdd: Invalid parameter list\n");
        return -1;
    }
    
    new_node = request_node();
    if (new_node == NULL) {
        return -1; /* TODO: Expand node pool for bonus :) */
    }
    
    new_node->data = item;
    /* if list is empty */
    if (list->size == 0){
        list->head = new_node;
        list->tail = new_node;
        new_node->prev = NULL;
        new_node->next = NULL;
    } 

    /* if curr is at the end, we need to adjust list->tail */
    else if (list->curr == list->tail){
        list->tail = new_node;
        new_node->prev = list->curr;
        list->curr->next = new_node;
        new_node->next = NULL;
    } 

    /* otherwise, we're inserting in the middle and need to 
    adjust pointers. 
    */
    else {
        new_node->next = list->curr->next;
        new_node->prev = list->curr;
        new_node->next->prev = new_node;
        list->curr->next = new_node;
    }
    
    /* these need to be performed regardless of where we insert */
    list->curr = new_node;
    list->size++;
    return EXIT_SUCCESS;
}

int ListInsert(LIST* list, void* item){
    NODE* new_node;

    if (list == NULL){
        printf("Error in procedure ListInsert: Invalid parameter list\n");
        return -1;
    }

    new_node = request_node();
    if (new_node == NULL) {
        return -1; /* TODO: Expand node pool for bonus :) */
    }

    new_node->data = item;
    /* if list is empty */
    if (list->size == 0){
        list->head = new_node;
        list->tail = new_node;
        new_node->prev = NULL;
        new_node->next = NULL;
    }    
    /* If curr is at the beginning, we need to adjust list->head */
    else if (list->curr == list->head) {
        list->head = new_node;
        new_node->next = list->curr;
        list->curr->prev = new_node;
        new_node->prev = NULL;
    }
    /* otherwise, we're inserting in the middle and need to 
    adjust pointers. 
    */
    else {
        new_node->next = list->curr;
        new_node->prev = list->curr->prev;
        new_node->prev->next = new_node;
        list->curr->prev = new_node;
    }

    /* these need to be performed regardless of where we insert */
    list->curr = new_node;
    list->size++;
    return EXIT_SUCCESS;
}

int ListAppend(LIST* list, void* item){
    NODE* new_node;
    if (list == NULL){
        return -1;
    }

    new_node = request_node();
    new_node->data = item;

    /* If list is empty */
    if (list->size == 0) {
        list->head = new_node;
        new_node->prev = NULL;
    } else {
        list->tail->next = new_node;
        new_node->prev = list->tail;
    }

    list->tail = new_node;
    new_node->next = NULL;
    list->curr = new_node;
    list->size++;

    return EXIT_SUCCESS;
}

int ListPrepend(LIST* list, void* item){
    NODE* new_node;
    if (list == NULL){
        return -1;
    }

    new_node = request_node();
    new_node->data = item;

    /* If list is empty */
    if (list->size == 0) {
        list->tail = new_node;
        new_node->next = NULL;
    } else {
        list->head->prev = new_node;
        new_node->next = list->head;
    }

    list->head = new_node;
    new_node->prev = NULL;
    list->curr = new_node;
    list->size++;

    return EXIT_SUCCESS;
}

void ListConcat(LIST* list1, LIST* list2){
    if (list1 == NULL){
        printf("Error in procedure ListConcat: Invalid parameter list1\n");
        return;
    } else if (list2 == NULL){
        printf("Error in procedure ListConcat: Invalid parameter list2\n");
        return;
    }
    list1->tail->next = list2->head;
    list2->head->prev = list1->tail;
    list1->tail = list2->tail;

    list1->size += list2->size;

    release_list(list2);
}
