/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

int main(int argc, char* argv[]){
	
	LIST* list1 = ListCreate();
	LIST* list2 = ListCreate();
	LIST* list3 = ListCreate();
	LIST* list4 = ListCreate();
	
	/* testing ListAdd */
	int item1 = 7;
	ListAdd(list1, &item1);
	if (list1->head->data == &item1 && list1->curr->data == &item1)
		printf("ListAdd test 1 passed\n");
	else
		printf("ListAdd test 1 failed\n");
	
	int item2 = 5;
	ListAdd(list1, &item2);
	if (list1->head->next->data == &item2)
		printf("ListAdd test 2 passed\n");
	else
		printf("ListAdd test 2 failed\n");
	
	int item3 = 98;
	ListAdd(list1, &item3);
	if (list1->curr->data == &item3)
		printf("ListAdd test 3 passed\n");
	else
		printf("ListAdd test 3 failed\n");
	
	/* Testing ListInsert */
	char pizza[6];
	strncpy(pizza, "Pizza", 6);
	ListInsert(list2, pizza);
	if (list2->head->data == pizza)
		printf("ListInsert test 1 passed \n");
	else
		printf("ListInsert test 1 failed \n");
	
	char jones_soda[10];
	strncpy(jones_soda, "Dr. Jones", 10);
	ListInsert(list2, jones_soda);
	if (list2->curr->data == jones_soda)
		printf("ListInsert test 2 passed \n");
	else
		printf("ListInsert test 2 failed \n");
	
	char skittles[10];
	strncpy(skittles, "Skittles", 10);
	ListInsert(list2, skittles);
	if (list2->head->data == skittles)
		printf("ListInsert test 3 passed \n");
	else
		printf("ListInsert test 3 failed \n");
	
	/* Testing for ListAppend
	
	/*
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
	*/
    return EXIT_SUCCESS;
}
