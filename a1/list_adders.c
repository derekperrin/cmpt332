/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

bool memory_allocated = false;

LIST* ListCreate(){
    /* allocated memory the first time a list is created */
    if (!memory_allocated) {
        list_memory = malloc(sizeof(LIST) * MAX_LISTS);
        node_memory = malloc(sizeof(NODE) * MAX_NODES);
        for (size_t i = 0; i < MAX_NODES - 1; ++i) {
            node_memory[i].next = &node_memory[i+1];
        }
        for (size_t i = 0; i < MAX_LISTS - 1; ++i) {
            list_memory[i].next = &list_memory[i+1];
        }
        node_memory[MAX_NODES-1].next = NULL;
        list_memory[MAX_LISTS-1].next = NULL;

        /* point to the free lists and nodes at the head */
        curr_free_list = list_memory;
        curr_free_node = node_memory;

        /* set memory_allocated to true to prevent entering this again */
        memory_allocated = true;
    }
    /* TODO: check if there is memory left to give a list out */
    
    LIST* newList = curr_free_list;
    curr_free_list = curr_free_list->next;

    newList->size = 0;
    newList->head = NULL;
    newList->tail = NULL;
    newList->curr = NULL;
	printf("Got to procedure ListCreate\n");
	return newList;
}

int ListAdd(LIST* list, void* item){
	printf("Got to procedure ListAdd\n");
	if (list == NULL){
		printf("Error in procedure ListAdd: Invalid parameter list\n");
		return EXIT_FAILURE;
	} else if (item == NULL){
		printf("Error in procedure ListAdd: Invalid parameter item\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int ListInsert(LIST* list, void* item){
	printf("Got to procedure ListInsert\n");
	if (list == NULL){
		printf("Error in procedure ListInsert: Invalid parameter list\n");
		return EXIT_FAILURE;
	} else if (item == NULL){
		printf("Error in procedure ListInsert: Invalid parameter item\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int ListAppend(LIST* list, void* item){
	printf("Got to procedure ListAppend\n");
	if (list == NULL){
		printf("Error in procedure ListAppend: Invalid parameter list\n");
		return EXIT_FAILURE;
	} else if (item == NULL){
		printf("Error in procedure ListAppend: Invalid parameter item\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int ListPrepend(LIST* list, void* item){
	printf("Got to procedure ListPrepend\n");
	if (list == NULL){
		printf("Error in procedure ListPrepend: Invalid parameter list\n");
		return EXIT_FAILURE;
	} else if (item == NULL){
		printf("Error in procedure ListPrepend: Invalid parameter item\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void ListConcat(LIST* list1, LIST* list2){
	printf("Got to procedure ListConcat\n");
	if (list1 == NULL){
		printf("Error in procedure ListConcat: Invalid parameter list1\n");
		return;
	} else if (list2 == NULL){
		printf("Error in procedure ListConcat: Invalid parameter list2\n");
		return;
	}
}
