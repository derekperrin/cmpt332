/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main(int argc, char* argv[]){
    printf("Got to procedure main\n");
	
	ListCreate();
	ListCount(NULL);
	ListFirst(NULL);
	ListLast(NULL);
	ListNext(NULL);
	ListPrev(NULL);
	ListCurr(NULL);
	ListAdd(NULL, 0);
	ListInsert(NULL, 0);
	ListPrepend(NULL, 0);
	ListRemove(NULL);
	ListConcat(NULL, NULL);
	ListFree(NULL, NULL);
	ListTrim(NULL);
	ListSearch(NULL, NULL, 0);
	
    return EXIT_SUCCESS;
}
