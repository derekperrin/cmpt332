/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/
#ifndef LIST_ALLOC_H
#define LIST_ALLOC_H

#include <stdio.h>
#include <stdlib.h>

NODE* request_node(void);
LIST* request_list(void);
void free_node(NODE*);

#endif /* LIST_ALLOC_H */