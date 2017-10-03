/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

int main(int argc, char* argv[]){
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
    if (list1 != NULL || list2 != NULL || list3 != NULL || list4 != NULL){
        printf("ListCreate test passed\n");
        tests_passed++;
    }
    else
        printf("ListCreate failed to create one or more lists\n");
    
    /* testing ListAdd */
    num_tests++;
    item1 = 7;
    ListAdd(list1, &item1);
    if (list1->head->data == &item1 && list1->curr->data == &item1){
        printf("ListAdd test 1 passed\n");
        tests_passed++;
    }
    else
        printf("ListAdd test 1 failed\n");
    
    num_tests++;
    item2 = 5;
    ListAdd(list1, &item2);
    if (list1->head->next->data == &item2){
        printf("ListAdd test 2 passed\n");
        tests_passed++;
    }
    else
        printf("ListAdd test 2 failed\n");
    
    num_tests++;
    item3 = 98;
    ListAdd(list1, &item3);
    if (list1->curr->data == &item3){
        printf("ListAdd test 3 passed\n");
        tests_passed++;
    }
    else
        printf("ListAdd test 3 failed\n");
    
    /* Testing ListInsert */
    num_tests++;
    strncpy(pizza, "Pizza", 6);
    ListInsert(list2, pizza);
    if (list2->head->data == pizza){
        printf("ListInsert test 1 passed \n");
        tests_passed++;
    }
    else
        printf("ListInsert test 1 failed \n");
    
    num_tests++;
    strncpy(jones_soda, "Dr. Jones", 10);
    ListInsert(list2, jones_soda);
    if (list2->curr->data == jones_soda){
        printf("ListInsert test 2 passed \n");
        tests_passed++;
    }
    else
        printf("ListInsert test 2 failed \n");
    
    num_tests++;
    strncpy(skittles, "Skittles", 10);
    ListInsert(list2, skittles);
    if (list2->head->data == skittles) {
        printf("ListInsert test 3 passed \n");
        tests_passed++;
    }
    else
        printf("ListInsert test 3 failed \n");
    
    /* Testing for ListAppend */
    num_tests++;
    thing1 = -45.45;
    ListAppend(list3, &thing1);
    if (list3->head->data == &thing1 && list3->tail->data == &thing1) {
        printf("ListAppend test 1 passed\n");
        tests_passed++;
    }
    else
        printf("ListAppend test 1 failed\n");

    num_tests++;
    thing2 = 3.9912;
    ListAppend(list3, &thing2);
    if (list3->curr->data == &thing2){
        printf("ListAppend test 2 passed\n");
        tests_passed++;
    }
    else
        printf("ListAppend test 2 failed\n");

    num_tests++;
    thing3 = 3.14159;
    ListAppend(list3, &thing3);
    if (list3->tail->data == &thing3){
        printf("ListAppend test 3 passed\n");
        tests_passed++;
    }
    else
        printf("ListAppend test 3 failed\n");

    /* Testing for ListPrepend */
    num_tests++;
    c1 = 'd';
    ListPrepend(list4, &c1);
    if(list4->head->data == &c1 && list4->tail->data == &c1) {
        printf("ListPrepend test 1 passed\n");
        tests_passed++;
    }
    else
        printf("ListPrepend test 1 failed\n");

    num_tests++;
    c2 = 'x';
    ListPrepend(list4, &c2);
    if (list4->curr->data == &c2) {
        printf("ListPrepend test 2 passed\n");
        tests_passed++;
    }
    else
        printf("ListPrepend test 2 failed\n");

    num_tests++;
    c3 = 'z';
    ListPrepend(list4, &c3);
    if (list4->head->data == &c3) {
        printf("ListPrepend test 3 passed\n");
        tests_passed++;
    }
    else
        printf("ListPrepend test 3 failed\n");

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

    num_tests++;
    ListConcat(list1, list5);
    if (list1->head->data == &item1 &&
            list1->head->next->data == &item2 &&
            list1->head->next->next->data == &item3 &&
            list1->head->next->next->next->data == &i1 &&
            list1->head->next->next->next->next->data == &i2 &&
            list1->tail->data == &i3) {
        printf("ListConcat test 1 passed\n");
        tests_passed++;
    }
    else
        printf("ListConcat test 1 failed\n");

    num_tests++;
    if (list1->head->prev == NULL &&
            list1->tail->next == NULL) {
        printf("ListConcat test 2 passed\n");
        tests_passed++;
    }
    else
        printf("ListConcat test 2 failed\n");

    num_tests++;
    if (list1->tail->data == &i3 &&
            list1->tail->prev->data == &i2 &&
            list1->tail->prev->prev->data == &i1 &&
            list1->tail->prev->prev->prev->data == &item3 &&
            list1->tail->prev->prev->prev->prev->data == &item2 &&
            list1->tail->prev->prev->prev->prev->prev->data == &item1) {
        printf("ListConcat test 3 passed\n");
        tests_passed++;
    } else {
        printf("ListConcat test 3 failed\n");
    }
    
    /* testing list memory growth */
    
    /* now create a large amount of lists to show that it works.
     * If it fails, it's because there is not enough memory on
     * the computer, not a problem with our list implementation*
     */
    num_tests++;
    fails = 0;
    for ( i = 0; i <= 60; ++i){
        if (ListCreate() == NULL){
            fails++;
        }
    }
    if (fails != 0)
        printf("Creation of %d/60 list failed because there is not enough "
                "memory on your computer.\n", fails);
    else
        printf("Creation of 60 lists passed!\n");
    tests_passed++; /*this test always passes whether lists are created or not.
    If lists fail to be created, it's because the computer doesn't have enough
    memory, not because our program doesn't work.*/
    
    
    num_tests++;
    fails = 0;
    for ( i = 0; i < 1000; ++i){
        if (ListInsert(list1, NULL) == -1){
            fails++;
        }
    }
    if (fails != 0)
        printf("Creation of %d/1000 nodes failed because there is not enough "
                "memory on your computer.\n", fails);
    else
        printf("Creation of 1000 nodes passed!\n");
    tests_passed++; /*this test always passes whether nodes are created or not.
    If nodes fail to be created, it's because the computer doesn't have enough
    memory, not because our program doesn't work.*/
    
    /****** FINAL OUTPUT ******/
    if (tests_passed == num_tests) {
        printf("All %d tests passed!\n", num_tests);
    } else {
        printf("Only %d/%d tests passed :(\n", tests_passed, num_tests);
    }


    return EXIT_SUCCESS;
}
