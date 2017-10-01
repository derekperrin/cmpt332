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

pthread_t* thread_array;
pthread_key_t key;          /* thread specific data key for custom thread id */
int* square_count;
int* completed_threads;

/* Used to increment the square counter for each thread */
void incr_func(void) {
    int id = *(int*)(pthread_getspecific(key));
    square_count[id]++;
}

void error_exit(char* error_message){
    fprintf(stderr, "%s", error_message);
    exit(EXIT_FAILURE);
}

void* thread_func(void* args) {
    struct timespec start_time, end_time;
    
    int size = *(int*) args;
    int id = *((int*)(args) + 1);
    free(args);                 /* give memory back asap */
    pthread_setspecific(key, &id);

    if (clock_gettime(CLOCK_MONOTONIC, &start_time) != 0){
        error_exit("partA3.c: Could not get start time\n");
    }
    
    for (size_t i = 1; i <= size; ++i){
        Square(i);
    }
    
    if (clock_gettime(CLOCK_MONOTONIC, &end_time) != 0){
        error_exit("partA3.c: Could not get end time\n");
    }
    
    /* Measure the time difference since the thread started and print it */
    int elapsed_time = (int)(end_time.tv_sec - start_time.tv_sec)*1000 +
        (int)(end_time.tv_nsec - start_time.tv_nsec)/1000000;
    
    unsigned long my_pid = (unsigned long)pthread_self();

    printf("Thread %lu: No. of Square calls: %d, Elapsed time: %d ms\n",
        my_pid, square_count[id], elapsed_time);

    completed_threads[id] = 1;
    pthread_exit(0);
}

int main(int argc, char* argv[argc+1]){
    int error_code;
    int num_threads, deadline, size;
    int* args;

    if (parse_args(&num_threads, &deadline, &size, argc, argv) != EXIT_SUCCESS){
        arg_error();
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for stuff up above */
    if ((thread_array = malloc(sizeof(pthread_t) * num_threads)) == NULL)
        error_exit("malloc error thread_array partA3.c\n");

    if ((square_count = calloc(num_threads, sizeof(int))) == NULL)
        error_exit("calloc error square_count partA3.c\n");

    if ((completed_threads = calloc(num_threads, sizeof(int))) == NULL)
        error_exit("calloc error completed_threads partA3.c\n");

    /* set up the thread specific data to store the id later on */
    pthread_key_create(&key, NULL);

    for (size_t i = 0; i < num_threads; ++i) {
        args = (int*)calloc(2, sizeof(int));
        if (args == NULL) 
            error_exit("calloc error args partA3.c\n");
        args[0] = size;
        args[1] = i;
        error_code = pthread_create(
                        thread_array + i,           /* array of threads */
                        NULL,                       /* default attributes */
                        thread_func,                /* thread routine */
                        args);                      /* args size and id */
        if (error_code != 0) {
            printf("pthread_create error code %d\n", error_code);
            error_exit("pthread_create error partA3.c\n");
        }
    }
    
    if (sleep(deadline) != 0){
        error_exit("Parent could not sleep\n");
    }

    /* ignore this comment */
    /* and this one below it */

    for (size_t i = 0; i < num_threads; ++i) {
        if (completed_threads[i])
            continue;
        if ((pthread_cancel(thread_array[i]) == 0)) {
            unsigned long thread_id = (unsigned long)(thread_array[i]);
            printf("Thread %lu: No. of Square calls: %d, Elapsed time: %d ms\n",
                thread_id, square_count[i], deadline * 1000);
        } else {
            error_exit("pthread_cancel error partA3.c\n");
        }
    }

    /* free all memory */
    /* nevermind, not necessary according to blackboard. OS takes care of it */
    /*
    pthread_key_delete(key);
    free(square_count);
    free(thread_array);
    */

    return EXIT_SUCCESS;
}
