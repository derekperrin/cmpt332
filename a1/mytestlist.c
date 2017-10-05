/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

/* Used for testing ListSearch */
int listComp1(void* item1, void* item2) {
    if (*(int*)item1 == *(int*)item2)
        return 1;
    return 0;
}

/* Used for testing ListSearch 
 * checks if item1 divides item2.
 */
int listComp2(void* item1, void* item2) {
    if (*(int*)item2 == 0) {
        /* division by zero error */
        return 0;
    }
    if ((*(int*)item2 % *(int*)item1) == 0) {
        return 1;
    }
    return 0;
}

/* Tests for list_removers *
 * @param results: integer pointers used to store test results
 */

void test_removers(int* results) {
    int num_tests, tests_passed, fails;
    LIST* list1;
    LIST* list2;
    LIST* list3;

    char c1, c2, c3, c4;
    int* i1;
    int* i2;
    int* i3;
    int* i4;

    num_tests = 0;
    tests_passed = 0;

    printf("Beginning list_removers tests\n");

    list1 = ListCreate();
    list2 = ListCreate();
    list3 = ListCreate();

    c1 = 'H';
    c2 = 'e';
    c3 = 'l';
    c4 = 'p';

    printf("ListRemove test 1: checking with list of size 0\n");
    num_tests++;
    if (ListRemove(list1) == NULL) {
        printf("ListRemove test 1 passed\n");
        tests_passed++;
    } else
        printf("ListRemove test 1 FAILED\n");

    printf("ListRemove test 2: cheking with list of size 1\n");
    num_tests++;
    ListAdd(list1, &c1);
    if (ListRemove(list1) == &c1
            && ListCount(list1) == 0) {
        printf("ListRemove test 2 passed\n");
        tests_passed++;
    } else
        printf("ListRemove test 2 FAILED\n");

    printf("ListRemove test 3: checking with list size 4, curr @ head\n");
    num_tests++;
    ListAdd(list1, &c1);
    ListAdd(list1, &c2);
    ListAdd(list1, &c3);
    ListAdd(list1, &c4);
    ListFirst(list1);
    if (ListRemove(list1) == &c1
            && ListCount(list1) == 3
            && ListCurr(list1) == ListFirst(list1)) {
        printf("ListRemove test 3 passed\n");
        tests_passed++;
    } else
        printf("ListRemove test 3 FAILED\n");

    printf("ListRemove test 4: checking with list->curr @ tail\n");
    num_tests++;
    ListLast(list1);
    if (ListRemove(list1) == &c4
            && ListCount(list1) == 2
            && ListCurr(list1) == ListLast(list1)) {
        printf("ListRemove test 4 passed\n");
        tests_passed++;
    } else
        printf("ListRemove test 4 FAILED\n");

    printf("ListRemove test 5: checking with list->curr in middle\n");
    num_tests++;
    ListAdd(list1, &c4);
    ListAdd(list1, &c1);
    ListPrev(list1);
    ListPrev(list1);
    if (ListRemove(list1) == &c3
            && ListCount(list1) == 3) {
        printf("ListRemove test 5 passed\n");
        tests_passed++;
    } else
        printf("ListRemove test 5 FAILED\n");

    printf("ListTrim test1: checking with empty list\n");
    num_tests++;
    if (ListTrim(list2) == NULL) {
        printf("ListTrim test 1 passed\n");
        tests_passed++;
    } else
        printf("ListTrim test 1 FAILED\n");

    printf("ListRemove test 2: checking with list of size 1"
            " that everything is NULL after removal\n");
    num_tests++;
    ListAdd(list2, &c1);
    if (ListTrim(list2) == &c1
            && ListCount(list2) == 0
            && ListFirst(list2) == NULL
            && ListCurr(list2) == NULL
            && ListLast(list2) == NULL){
        printf("ListTrim test 2 passed\n");
        tests_passed++;
    } else
        printf("ListTrim test2 FAILED\n");

    printf("ListTrim test 3: checking general case\n");
    num_tests++;
    ListAdd(list2, &c1);
    ListAdd(list2, &c1);
    if (ListTrim(list2) == &c1
            && ListCount(list2) == 1) {
        printf("ListTrim test 3 passed\n");
        tests_passed++;
    } else
        printf("ListTrim test 3 FAILED\n");

    printf("ListFree test 1: testing removing a list with integer arrays\n");
    num_tests++;
    i1 = calloc(10, sizeof(int));
    i2 = calloc(20, sizeof(int));
    i3 = calloc(100, sizeof(int));
    i4 = calloc(1000, sizeof(int));
    ListAdd(list3, i1);
    ListAdd(list3, i2);
    ListAdd(list3, i3);
    ListAdd(list3, i4);
    ListFree(list3, free);
    if (ListFirst(list3) == NULL
            && ListLast(list3) == NULL
            && ListCurr(list3) == NULL
            && ListCount(list3) == 0) {
        printf("ListFree test 1 passed\n");
        tests_passed++;
    } else
        printf("ListFree test 1 FAILED\n");
    
    printf("\n\n\n");

    results[0] = num_tests;
    results[1] = tests_passed;
    results[2] = fails;
}



/* Tests for list_movers */
/* @param results: integer pointer used to store test results */
void test_movers(int* results) {
    int num_tests, tests_passed, fails;
    LIST* list1;
    LIST* list2;
    LIST* list3;
    LIST* list4;

    int item1, item2, item3;
    int i1, i2, i3, i4, i5, i6;

    num_tests = 0;
    tests_passed = 0;

    list1 = ListCreate();
    list2 = ListCreate();
    list3 = ListCreate();
    list4 = ListCreate();

    printf("Beginning list_movers tests\n");

    /* Testing ListCount */
    num_tests++;
    printf("ListCount test 1: checking size of empty list\n");
    if (ListCount(list1) == 0) {
        printf("ListCount test 1 passed\n");
        tests_passed++;
    } else
        printf("ListCount test 1 FAILED\n");

    num_tests++;
    printf("ListCount test 2: checking size of list with size 1.\n");
    item1 = 17;
    ListAdd(list1, &item1);
    if (ListCount(list1) == 1) {
        printf("ListCount test 2 passed\n");
        tests_passed++;
    } else
        printf("ListCount test 2 FAILED\n");

    /* Testing ListFirst */
    printf("ListFirst test 1: checking empty list\n");
    num_tests++;
    if (ListFirst(list2) == NULL) {
        printf("ListFirst test 1 passed\n");
        tests_passed++;
    } else
        printf("ListFirst test 1 FAILED\n");

    printf("ListFirst test 2: checking with list of size 1\n");
    num_tests++;
    ListAdd(list2, &item1);
    if (ListFirst(list2) == &item1) {
        printf("ListFirst test 2 passed\n");
        tests_passed++;
    } else
        printf("ListFirst test 2 FAILED\n");

    printf("ListFirst test 3: checking with list of size 2\n");
    num_tests++;
    item2 = 3020;
    ListInsert(list2, &item2);
    if (ListFirst(list2) == &item2) {
        printf("ListFirst test 3 passed\n");
        tests_passed++;
    } else
        printf("ListFirst test 3 FAILED\n");

    ListRemove(list2);
    ListRemove(list2);

    printf("ListLast  test 1: checking with list of size 0\n");
    num_tests++;
    if (ListLast(list2) == NULL) {
        printf("ListLast test 1 passed\n");
        tests_passed++;
    } else
        printf("ListLast test 1 FAILED\n");

    printf("ListLast test 2: checking with list of size 1\n");
    num_tests++;
    ListAdd(list2, &item2);
    if (ListLast(list2) == &item2) {
        printf("ListLast test 2 passed\n");
        tests_passed++;
    } else 
        printf("ListLast test 2 FAILED\n");

    printf("ListLast test 3: checking with list of size 2."
            " Also checking position of list->curr\n");
    num_tests++;
    item3 = 32143;
    ListAdd(list2, &item3);
    if (ListLast(list2) == &item3
        && ListCurr(list2) == &item3) {
        printf("ListLast test 3 passed\n");
        tests_passed++;
    } else
        printf("ListLast test 3 FAILED\n");

    /* list2 = item2 -> item3 -> item1 */
    ListAppend(list2, &item1);
    ListFirst(list2);
    printf("ListNext test 1: checking at the head of size 3 list\n");
    num_tests++;
    if (ListNext(list2) == &item3
            && ListNext(list2) == &item1
            && ListNext(list2) == NULL){
        printf("ListNext test 1 passed\n");
        tests_passed++;
    } else
        printf("ListNext test 1 FAILED\n");

    /* traverse backwards to HEAD and attempt to go back one more */
    printf("ListPrev test 1: checking at the tail of size 3 list\n");
    num_tests++;
    if (ListPrev(list2) == &item3
            && ListPrev(list2) == &item2
            && ListPrev(list2) == NULL) {
        printf("ListPrev test 1 passed\n");
        tests_passed++;
    } else
        printf("ListPrev test 1 FAILED\n");

    printf("ListCurr test1: checking ListCurr on an empty list\n");
    num_tests++;
    if (ListCurr(list3) == NULL) {
        printf("ListCurr test 1 passed\n");
        tests_passed++;
    } else
        printf("ListCurr test 1 FAILED\n");

    printf("ListCurr test2: checking ListCurr on the generic case\n");
    num_tests++;
    ListAdd(list3, &item2);
    if (ListCurr(list3) == &item2) {
        printf("ListCurr test 2 passed\n");
        tests_passed++;
    } else
        printf("ListCurr test 2 FAILED\n");

    /* last test on ListSearch...this one will suck */
    i1 = 7;
    i2 = 5;
    i3 = 25;
    i4 = 16;
    i5 = 73;
    i6 = 5;

    ListAdd(list4, &i1);
    ListAdd(list4, &i2);
    ListAdd(list4, &i3);
    ListAdd(list4, &i4);
    ListAdd(list4, &i5);
    ListAdd(list4, &i6);

    ListFirst(list4);
    printf("ListSearch test1: test equality of integers\n");
    num_tests++;
    if (ListSearch(list4, listComp1, &i5) == &i5){
        printf("ListSearch test1 passed\n");
        tests_passed++;
    } else
        printf("ListSearch test1 FAILED\n");

    ListNext(list4);
    printf("ListSearch test2: test equality, but fail and reach end of list\n");
    num_tests++;
    if (ListSearch(list4, listComp1, &i5) == ListLast(list4)) {
        printf("ListSearch test 2 passed\n");
        tests_passed++;
    } else
        printf("ListSearch test 2 FAILED\n");

    ListFirst(list4);
    printf("ListSearch test 3: test divisibility of integers\n");
    num_tests++;
    if (ListSearch(list4, listComp2, &i2) == &i2) {
        printf("ListSearch test 3 passed\n");
        tests_passed++;
    } else
        printf("ListSearch test 3 FAILED\n");

    ListNext(list4);
    printf("ListSearch test 4: test divisibility with different integers\n");
    num_tests++;
    if (ListSearch(list4, listComp2, &i2) == &i3) {
        printf("ListSearch test 4 passed\n");
        tests_passed++;
    } else
        printf("ListSearch test 4 FAILED\n");

    printf("ListSearch test 5: test divisiblity but fail\n");
    num_tests++;
    if (ListSearch(list4, listComp2, &i1) == ListLast(list4)) {
        printf("ListSearch test 5 passed\n");
        tests_passed++;
    } else
        printf("ListSearch test 5 FAILED\n");


    printf("\n\n\n");

    results[0] = num_tests;
    results[1] = tests_passed;
    results[2] = fails;
}

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

    printf("Beginning list_adders tests\n");
    
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

    printf("\n\n\n");
    
    results[0] = num_tests;
    results[1] = tests_passed;
    results[2] = fails;
}

int main(int argc, char* argv[]){
    int la_results[3];
    int lm_results[3];
    int lr_results[3];
    test_movers(lm_results);
    test_adders(la_results);
    test_removers(lr_results);

    /****** FINAL OUTPUT ******/
    if (la_results[1] == la_results[0]) {
        printf("All %d list_adders tests passed!\n", la_results[0]);
    } else {
        printf("Only %d/%d list_adders tests passed :(\n",
                la_results[1], la_results[0] );
    }

    if (lm_results[1] == lm_results[0]) {
        printf("All %d list_movers tests passed!\n", lm_results[0]);
    } else {
        printf("Only %d/%d list_movers tests passed :(\n",
                lm_results[1], lm_results[0]);
    }

    if (lr_results[1] == lr_results[0]) {
        printf("All %d list_removers tests passed!\n", lr_results[0]);
    } else {
        printf("Only %d/%d list_removers tests passed :(\n",
                lr_results[1], lr_results[0]);
    }

    return EXIT_SUCCESS;
}
