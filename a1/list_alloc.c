/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_alloc.h"

size_t list_mem_blocks = 0;
size_t list_mem_size = 1;

size_t node_mem_blocks = 0;
size_t node_mem_size = 1;

/* return 0 on success, -1 on fail */
int add_node_memory(void){
    NODE** new_node_memory;
    size_t i;

    /* realloc acts as malloc when allocating the first memory block
     * because node_memory is initially NULL
     */
    new_node_memory = /* on next line */
        realloc(node_memory, sizeof(NODE*) * (node_mem_blocks+1));
    if (new_node_memory == NULL)
        return -1;
    node_memory = new_node_memory;
    node_memory[node_mem_blocks] = /* on next line */
        malloc(sizeof(NODE) * MIN_NODES * node_mem_size);
    if (node_memory[node_mem_blocks] == NULL)
        return -1;
    
    /* set up the list of free nodes */
    curr_free_node = node_memory[node_mem_blocks];
    
    /* make each free node point to the next free node */
    for ( i = 0; i < MIN_NODES*node_mem_size - 1; ++i) {
        curr_free_node[i].next = &curr_free_node[i+1];
    }
    
    /* set the last free node to point to NULL to represent no more free 
     * nodes after that node
     */
    curr_free_node[MIN_NODES*node_mem_size - 1].next = NULL;
    
    node_mem_blocks++;
    node_mem_size <<= 1;
    return 0;
}

/* return 0 on success, -1 on fail */
int add_list_memory(void){
    LIST** new_list_memory;
    size_t i;
    
    /* realloc acts as malloc when allocating the first memory block
     * because list_memory is initially NULL
     */
    new_list_memory = /* on next line */
        realloc(list_memory, sizeof(LIST*) * (list_mem_blocks+1));
    if (new_list_memory == NULL)
        return -1;
    list_memory = new_list_memory;
    list_memory[list_mem_blocks] = /* on next line */
        malloc(sizeof(LIST) * MIN_LISTS * list_mem_size);
    if (list_memory[list_mem_blocks] == NULL)
        return -1;
    
    /* Set up the list of free lists */
    curr_free_list = list_memory[list_mem_blocks];
    
    /* make each free list point to the next free list */
    for ( i = 0; i < MIN_LISTS*list_mem_size - 1; ++i){
        curr_free_list[i].next_free = &curr_free_list[i+1];
    }
    
    /* set the last free list to point to NULL to represent no more free 
     * lists after that list
     */
    curr_free_list[MIN_LISTS*list_mem_size - 1].next_free = NULL;
    
    list_mem_blocks++;
    list_mem_size <<= 1;
    return 0;
}

LIST* request_list(void) {
    LIST* new_list;

    if (curr_free_list == NULL){
        if (add_list_memory() == -1) /* add_list_memory returns -1 on fail */
            return NULL;
    }
    new_list = curr_free_list;
    curr_free_list = curr_free_list->next_free;
    new_list->next_free = NULL;
    return new_list;
}

NODE* request_node(void) {
    NODE* new_node;
    if (curr_free_node == NULL){
        if (add_node_memory() == -1) /* add_node_memory returns -1 on fail */
            return NULL;
    }
    new_node = curr_free_node;
    curr_free_node = curr_free_node->next;
    return new_node;
}

void release_node(NODE* node) {
    /* TODO: complete this later */
    node->next = curr_free_node;
    curr_free_node = node;
}

void release_list(LIST* list) {
    list->next_free = curr_free_list;
    curr_free_list = list;
}

