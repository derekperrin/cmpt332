/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.h"

void incr_func(void) {

}

void error_exit(char* error_message){
    fprintf(stderr, "%s", error_message);
    exit(EXIT_FAILURE);
}

void* thread_func(void* args) {
    return 0;
}

int main(int argc, char* argv[argc+1]){
    int error_code;
    int num_threads, deadline, size;

    if (parse_args(&num_threads, &deadline, &size, argc, argv) != EXIT_SUCCESS){
        arg_error();
    }

    pthread_t* thread_array;

    if ((thread_array = malloc(sizeof(pthread_t) * num_threads)) == NULL)
        error_exit("malloc error partA3.c\n");

    for (size_t i = 0; i < num_threads; ++i) {

        error_code = pthread_create(
                        thread_array + i,    /* array of threads */
                        NULL,               /* default attributes */
                        thread_func,        /* thread routine */
                        &size);              /* args */
        if (error_code != 0) {
            printf("pthread_create error code %d\n", error_code);
            error_exit("pthread_create error partA3.c\n");
        }
    }



    return EXIT_SUCCESS;
}
