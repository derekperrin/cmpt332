/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int ListCount(LIST* list){
	printf("Got to procedure ListCount\n");
	if (list == NULL) 
		printf("Error in procedure ListCount: Invalid parameter list\n");
	return 0;
}

void* ListFirst(LIST* list){
	printf("Got to procedure ListFirst\n");
	if (list == NULL) 
		printf("Error in procedure ListFirst: Invalid parameter list\n");
	return NULL;
}

void* ListLast(LIST* list){
	printf("Got to procedure ListLast\n");
	if (list == NULL) 
		printf("Error in procedure ListLast: Invalid parameter list\n");
	return NULL;
}

void* ListNext(LIST* list){
	printf("Got to procedure ListNext\n");
	if (list == NULL) 
		printf("Error in procedure ListNext: Invalid parameter list\n");
	return NULL;
}

void* ListPrev(LIST* list){
	printf("Got to procedure ListPrev\n");
	if (list == NULL) 
		printf("Error in procedure ListPrev: Invalid parameter list\n");
	return NULL;
}

void* ListCurr(LIST* list){
	printf("Got to procedure ListCurr\n");
	if (list == NULL) 
		printf("Error in procedure ListCurr: Invalid parameter list\n");
	return NULL;
}

void* ListSearch(LIST* list, int (* comparator)(void* item1, void* item2), void* comparisonArg){
	printf("Got to procedure ListSearch\n");
	if (list == NULL){
		printf("Error in procedure ListSearch: Invalid parameter list\n");
		return NULL;
	}
	else if (comparator == NULL){
		printf("Error in procedure ListSearch: Invalid parameter comparator\n");
		return NULL;
	}
	else if (comparisonArg == NULL){
		printf("Error in procedure ListSearch: Invalid parameter comparisonArg\n");
		return NULL;
	}
	return NULL;
}