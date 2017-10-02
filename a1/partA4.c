/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "common.h"

#define MAX_COUNT 200

size_t square_counts = 0;

void incr_func(void){
    square_counts++;
}

int child_func(int size){
    
    for (size_t i = 0; i < size; ++i){
        Square(i);
    }
    
    printf("I am the child :'( WAAAA!!!\n");
    
    return EXIT_SUCCESS;
}

int timer(int deadline, int num_procs){
    printf("I am the timer!!\n");
    
    if (sleep(deadline) != 0){
        /* hopefully the parent has killed */
        return EXIT_SUCCESS;
    }
    for (size_t i = 0; i < num_procs; ++i){
        /* send signal to child process to exit */
        
    }
    
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[argc+1]){
    int num_procs, deadline, size;
    
    pid_t pid;
    
    if (parse_args(&num_procs, &deadline, &size, argc, argv) != EXIT_SUCCESS){
        arg_error();
        return EXIT_FAILURE;
    }
    
    size_t i = 0;
    do {
        pid = fork();
    } while (pid != 0 && ++i < num_procs);
    /* TODO: Consider changing parent's priority */
    if (pid == 0){
        return child_func(size);
    }
    
    /* create the timer process */
    pid = fork();
    if (pid == 0)
        return timer(deadline, num_procs);
    
    /* wait for all the children processes to exit */
    /* less than or equal because +1 for the timer */
    for (size_t i = 0; i <= num_procs; ++i)
        if (waitpid(-1, NULL, 0) == -1){
            printf("Parent wait error\n");
            return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
