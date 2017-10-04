/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int ListCount(LIST* list){
    if (list == NULL) {
        fprintf(stderr, "Error is ListCount: list is NULL\n");
        return EXIT_FAILURE;
    }
    return list->size;
}

void* ListFirst(LIST* list){
    if (list == NULL) {
        fprintf(stderr, "Error in ListFirst: list is NULL\n");
        return NULL;
    }
    if (list->size == 0)
        return NULL;
    list->curr = list->head;
    return list->head->data;
}

void* ListLast(LIST* list){
    if (list == NULL) {
        fprintf(stderr, "Error in ListLast: list is NULL\n");
        return NULL;
    }
    if (list->size == 0)
        return NULL;
    list->curr = list->tail;
    return list->tail->data;
}

void* ListNext(LIST* list){
    if (list == NULL) {
        fprintf(stderr, "Error in ListNext: list is NULL\n");
        return NULL;
    } else if ( list->curr == NULL ) {
        return NULL;
    } else if (list->curr->next == NULL) {
        return NULL;
    }
    list->curr = list->curr->next;
    return list->curr->data;
}

void* ListPrev(LIST* list){
    if (list == NULL) {
        fprintf(stderr, "Error in ListPrev: list is NULL\n");
        return NULL;
    } else if ( list->curr == NULL ){
        return NULL;
    } else if ( list->curr->prev == NULL ){
        return NULL;
    }
    list->curr = list->curr->prev;
    return list->curr->data;
}

void* ListCurr(LIST* list){
    if (list == NULL) {
        fprintf(stderr, "Error in ListCur: list is NULL\n");
        return NULL;
    } if (list->curr == NULL) {
        return NULL;
    }
    return list->curr->data;
}

void* ListSearch(LIST* list, int (* comparator)(void* item1, void* item2),
        void* comparisonArg){
    if (list == NULL) {
        fprintf(stderr, "Error in ListSearch: list is NULL\n");
        return NULL;
    }
    else if (list->size == 0) {
        return NULL;
    } else if (comparator == NULL){
        fprintf(stderr, "Error in ListSearch: list comparator is NULL\n");
        return NULL;
    } else if (comparisonArg == NULL){
        fprintf(stderr, "Error in ListSearch: list comparisonArg is NULL\n");
        return NULL;
    } else if (list->curr == NULL) {
        return NULL;
    }
    while (comparator(comparisonArg, list->curr->data) != 1){
        if (list->curr == list->tail)
            return NULL;
        list->curr = list->curr->next;
    }
    return list->curr->data;
}
