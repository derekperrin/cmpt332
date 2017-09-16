/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/
#ifndef LIST_H
#define LIST_H
typedef struct NODE {
	void* data;
	struct NODE* next;
	struct NODE* prev;
} NODE;

typedef struct LIST{
	NODE* head;
	NODE* tail;
	NODE* curr;
	int size;
} LIST;

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
