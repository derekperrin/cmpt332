#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "common.h"

/* need CreateThread(), GetSystemTime(), Sleep() */

int main(int argc, char* argv[argc+1]){

    HANDLE* thread_array;
    int num_threads, size, deadline;

    parse_args(&num_threads, &deadline, &size, argc, argv);

    /* create space for the threads */
    thread_array = malloc(sizeof(HANDLE) * num_threads);
    if (thread_array == NULL) return EXIT_FAILURE;

    /* create threads and stash them in thread array */
    for (size_t i = 0; i < threads; i++){
        thread_array[i] = CreateThread(
                NULL,                   /* security attributes */
                0,                      /* default stack size */
                thread_main,       /* thread function */
                &size,                   /* ptr to thread function argument */
                0,                      /* default creation flag */
                &dwThreadIdArray[i]);   /* thread identifier */
    }

    free(thread_array);
	return EXIT_SUCCESS;
}
