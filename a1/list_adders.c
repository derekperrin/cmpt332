/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

LIST* ListCreate(){
	printf("Got to procedure ListCreate\n");
	return NULL;
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