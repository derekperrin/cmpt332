/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/
#define _POSIX_C_SOURCE 199309L
/* The POSIX clock is not enabled by default in C99, enable it with this. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#include "common.h"

void incr_func(void) {
    
}

void error_exit(char* error_message){
    fprintf(stderr, "%s", error_message);
    exit(EXIT_FAILURE);
}

void* thread_func(void* args) {
    struct timespec start_time, end_time;
    
    int size = *(int*) args;
    
    if (clock_gettime(CLOCK_MONOTONIC, &start_time) != 0){
        error_exit("partA3.c: Could not get start time\n");
    }
    
    for (size_t i = 1; i <= size; ++i){
        Square(i);
    }
    
    if (clock_gettime(CLOCK_MONOTONIC, &end_time) != 0){
        error_exit("partA3.c: Could not get end time\n");
    }
    
    int elapsed_time = (int)(end_time.tv_sec - start_time.tv_sec)*1000 +
        (int)(end_time.tv_nsec - start_time.tv_nsec)/1000000;
    printf("%d\n", elapsed_time);
    
    return 0;
}

int main(int argc, char* argv[argc+1]){
    int error_code;
    int num_threads, deadline, size;

    if (parse_args(&num_threads, &deadline, &size, argc, argv) != EXIT_SUCCESS){
        arg_error();
        exit(EXIT_FAILURE);
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
    
    if (sleep(deadline) != 0){
        error_exit("Parent could not sleep\n");
    }
    
    return EXIT_SUCCESS;
}
