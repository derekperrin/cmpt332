/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

#define MAX_LISTS 64    /* we picked these because they're computer numbers */
#define MAX_NODES 1024

/* NODE structure: Used to store data and are used in the linked list. */
typedef struct NODE {
	void* data;
	struct NODE* next;
	struct NODE* prev;
} NODE;

/* LIST structure: A standard linked list, but a union is used so we can
 * link the list to other free lists in our memory pool when the list is
 * available.
 * Note to graders: Sorry for the complexity
 */
typedef struct LIST{
	NODE* head;
	NODE* tail;
	union {NODE* curr; struct LIST* next;};
	int size;
} LIST;

/* list memory pool to create new lists */
LIST* list_memory;
LIST* curr_free_list;

/* node memory pool to create new nodes */
NODE* node_memory;
NODE* curr_free_node;

LIST* ListCreate();
int ListCount(LIST*);
void* ListFirst(LIST*);
void* ListLast(LIST*);
void* ListNext(LIST*);
void* ListPrev(LIST*);
void* ListCurr(LIST*);
int ListAdd(LIST*, void*);
int ListInsert(LIST*, void*);
int ListPrepend(LIST*, void*);
void* ListRemove(LIST*);
void ListConcat(LIST*, LIST*);
void ListFree(LIST*, void (*)(void*));
void* ListTrim(LIST*);
void* ListSearch(LIST*, int (*)(void*, void*), void*);
#endif /* LIST_H */
