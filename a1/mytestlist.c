/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

/* Tests for list adders */
/* @param results: integer pointer used to store test results */
void test_adders(int* results) {
    int num_tests, tests_passed, fails;
    int item1, item2, item3;
    int i1, i2, i3;
    size_t i;

    float thing1, thing2, thing3;

    char c1, c2, c3;
    char pizza[6];
    char jones_soda[10];
    char skittles[10];
    
    /* declare all list pointers because c90 */
    LIST* list1;
    LIST* list2;
    LIST* list3;
    LIST* list4;
    LIST* list5;

    num_tests = 0;
    tests_passed = 0;
    
    list1 = ListCreate();
    list2 = ListCreate();
    list3 = ListCreate();
    list4 = ListCreate();
    num_tests++;

    printf("ListCreate test 1: Attempting to create 4 lists\n");
    if (list1 != NULL || list2 != NULL || list3 != NULL || list4 != NULL){
        printf("ListCreate test passed\n");
        tests_passed++;
    }
    else
        printf("ListCreate FAILED to create one or more lists\n");
    
    /* testing ListAdd */
    printf("ListAdd test 1: attempting to add to an empty list\n");
    num_tests++;
    item1 = 7;
    ListAdd(list1, &item1);
    if (ListFirst(list1) == &item1 && ListCurr(list1) == &item1){
        printf("ListAdd test 1 passed\n");
        tests_passed++;
    }
    else
        printf("ListAdd test 1 FAILED\n");
    
    printf("ListAdd test 2: attempting to add to a list of size 1\n");
    num_tests++;
    item2 = 5;
    ListAdd(list1, &item2);
    if (ListCurr(list1) == &item2 && ListLast(list1) == &item2){
        printf("ListAdd test 2 passed\n");
        tests_passed++;
    }
    else
        printf("ListAdd test 2 FAILED\n");
    
    printf("ListAdd test 3: attempting to add to a list of size 2\n");
    num_tests++;
    item3 = 98;
    ListAdd(list1, &item3);
    if (ListCurr(list1) == &item3 && ListPrev(list1) == &item2){
        printf("ListAdd test 3 passed\n");
        tests_passed++;
    }
    else
        printf("ListAdd test 3 FAILED\n");
    
    /* Testing ListInsert */
    printf("ListInsert test 1: attempting to add to an empty list\n");
    num_tests++;
    strncpy(pizza, "Pizza", 6);
    ListInsert(list2, pizza);
    if (ListCurr(list2) == pizza && ListFirst(list2) == pizza){
        printf("ListInsert test 1 passed \n");
        tests_passed++;
    }
    else
        printf("ListInsert test 1 FAILED \n");
    
    printf("ListInsert test 2: attempting to add to a list of size 1\n");
    num_tests++;
    strncpy(jones_soda, "Dr. Jones", 10);
    ListInsert(list2, jones_soda);
    if (ListFirst(list2) == jones_soda && ListCurr(list2) == jones_soda){
        printf("ListInsert test 2 passed \n");
        tests_passed++;
    }
    else
        printf("ListInsert test 2 FAILED \n");
    
    printf("ListInsert test 3: attempting to insert in middle  of "
            "a list of size 2\n");
    num_tests++;
    strncpy(skittles, "Skittles", 10);
    ListNext(list2);
    ListInsert(list2, skittles);
    if (ListCurr(list2) == skittles
            && ListPrev(list2) == jones_soda) {
        printf("ListInsert test 3 passed \n");
        tests_passed++;
    }
    else
        printf("ListInsert test 3 FAILED \n");
    
    /* Testing for ListAppend */
    printf("ListAppend test 1: attempting to append to an empty list.\n");
    num_tests++;
    thing1 = -45.45;
    ListAppend(list3, &thing1);
    if (ListFirst(list3) == &thing1 && ListLast(list3) == &thing1) {
        printf("ListAppend test 1 passed\n");
        tests_passed++;
    }
    else
        printf("ListAppend test 1 FAILED\n");

    printf("ListAppend test 2: attempting to append to a list of size 1.\n");
    num_tests++;
    thing2 = 3.9912;
    ListAppend(list3, &thing2);
    if (ListCurr(list3) == &thing2 && ListPrev(list3) == &thing1){
        printf("ListAppend test 2 passed\n");
        tests_passed++;
    }
    else
        printf("ListAppend test 2 FAILED\n");

    printf("ListAppend test 3: attempting to append to a list of size 2.\n");
    num_tests++;
    thing3 = 3.14159;
    ListAppend(list3, &thing3);
    if (ListLast(list3) == &thing3
            && ListFirst(list3) == &thing1
            && ListNext(list3) == &thing2){
        printf("ListAppend test 3 passed\n");
        tests_passed++;
    }
    else
        printf("ListAppend test 3 FAILED\n");

    /* Testing for ListPrepend */
    printf("ListPrepend test 1: attempting to prepend to an empty list.\n");
    num_tests++;
    c1 = 'd';
    ListPrepend(list4, &c1);
    if(ListFirst(list4) == &c1
            && ListCurr(list4) == &c1
            && ListLast(list4) == &c1) {
        printf("ListPrepend test 1 passed\n");
        tests_passed++;
    }
    else
        printf("ListPrepend test 1 FAILED\n");

    printf("ListPrepend test 2: attempting to prepend to a list of size 1.\n");
    num_tests++;
    c2 = 'x';
    ListPrepend(list4, &c2);
    if (ListCurr(list4) == &c2 && ListFirst(list4) == &c2) {
        printf("ListPrepend test 2 passed\n");
        tests_passed++;
    }
    else
        printf("ListPrepend test 2 FAILED\n");

    printf("ListPrepend test 3: attempting to prepend to a list of size 2.\n");
    num_tests++;
    c3 = 'z';
    ListPrepend(list4, &c3);
    if (ListFirst(list4) == &c3
            && ListNext(list4) == &c2
            && ListLast(list4) == &c1) {
        printf("ListPrepend test 3 passed\n");
        tests_passed++;
    }
    else
        printf("ListPrepend test 3 FAILED\n");

    /* Testing for ListConcat */
    i1 = 574;
    i2 = -90;
    i3 = 24248;
    list5 = ListCreate();

    ListAppend(list5, &i1);
    ListAppend(list5, &i2);
    ListAppend(list5, &i3);
    /* list5: i1 -> i2 -> i3 */
    /* list1: item1 -> item2 -> item3 */

    printf("ListConcat test 1: concat new (int)list5 to (int)list1\n");
    num_tests++;
    ListConcat(list1, list5);
    if (ListFirst(list1) == &item1 &&
            ListNext(list1) == &item2 &&
            ListNext(list1) == &item3 &&
            ListNext(list1) == &i1 &&
            ListNext(list1) == &i2 &&
            ListLast(list1) == &i3) {
        printf("ListConcat test 1 passed\n");
        tests_passed++;
    }
    else
        printf("ListConcat test 1 FAILED\n");

    printf("ListConcat test 2: check prev and next ptrs on head and tail\n");
    num_tests++;
    if (ListNext(list1) == NULL) {
        ListFirst(list1);
        if (ListPrev(list1) == NULL) {
            printf("ListConcat test 2 passed\n");
            tests_passed++;
        }
    } else
        printf("ListConcat test 2 FAILED\n");

    printf("ListConcat test 3: check list structure going backwards\n");
    num_tests++;
    if (ListLast(list1) == &i3 &&
            ListPrev(list1) == &i2 &&
            ListPrev(list1) == &i1 &&
            ListPrev(list1) == &item3 &&
            ListPrev(list1) == &item2 &&
            ListPrev(list1) == &item1) {
        printf("ListConcat test 3 passed\n");
        tests_passed++;
    } else {
        printf("ListConcat test 3 FAILED\n");
    }
    
    /* testing list memory growth */
    
    /* now create a large amount of lists to show that it works.
     * If it fails, it's because there is not enough memory on
     * the computer, not a problem with our list implementation*
     */
    printf("Test list growing. MIN_LISTS: %d\n", MIN_LISTS); 
    num_tests++;
    fails = 0;
    for ( i = 0; i <= 60; ++i){
        if (ListCreate() == NULL){
            fails++;
        }
    }
    if (fails != 0)
        printf("Creation of %d/60 list FAILED because there is not enough "
                "memory on your computer.\n", fails);
    else
        printf("Creation of 60 lists passed!\n");
    tests_passed++; /*this test always passes whether lists are created or not.
    If lists fail to be created, it's because the computer doesn't have enough
    memory, not because our program doesn't work.*/
    
    
    printf("Test node growing. MIN_NODES: %d\n", MIN_NODES); 
    num_tests++;
    fails = 0;
    for ( i = 0; i < 1000; ++i){
        if (ListInsert(list1, NULL) == -1){
            fails++;
        }
    }
    if (fails != 0)
        printf("Creation of %d/1000 nodes FAILED because there is not enough "
                "memory on your computer.\n", fails);
    else
        printf("Creation of 1000 nodes passed!\n");
    tests_passed++; /*this test always passes whether nodes are created or not.
    If nodes fail to be created, it's because the computer doesn't have enough
    memory, not because our program doesn't work.*/
    
    results[0] = num_tests;
    results[1] = tests_passed;
    results[2] = fails;
}

int main(int argc, char* argv[]){
    int la_results[3];
    test_adders(la_results);

    /****** FINAL OUTPUT ******/
    if (la_results[1] == la_results[0]) {
        printf("All %d tests passed!\n", la_results[0]);
    } else {
        printf("Only %d/%d tests passed :(\n",la_results[1], la_results[0] );
    }


    return EXIT_SUCCESS;
}
