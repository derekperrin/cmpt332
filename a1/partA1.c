/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "common.h"

/* need CreateThread(), GetSystemTime(), Sleep() */

DWORD WINAPI Thread_Main( LPVOID lpParam ) {
    printf("Got to procedure Thread_Main\n");
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[argc+1]){

    HANDLE* thread_array;
    int num_threads, size, deadline;

    if ( parse_args(&num_threads, &deadline, &size, argc, argv) != 0 ) {
        arg_error();
        return EXIT_FAILURE;
    }

    /* create space for the threads */
    thread_array = malloc(sizeof(HANDLE) * num_threads);
    if (thread_array == NULL){
        return EXIT_FAILURE;
    }

    /* create threads and stash them in thread array */
    printf("Creating threads\n");
    for (size_t i = 0; i < num_threads; i++){
        thread_array[i] = CreateThread(
                NULL,                   /* security attributes */
                0,                      /* default stack size */
                Thread_Main,            /* thread function */
                &size,                  /* ptr to thread function argument */
                0,                      /* default creation flag */
                NULL);                  /* thread identifier */
    }

    free(thread_array);
	return EXIT_SUCCESS;
}
