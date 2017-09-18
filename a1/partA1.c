/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "common.h"

DWORD dwTlsIndex; /* enable thread local storage */

/* need CreateThread(), GetSystemTime(), Sleep() */

DWORD WINAPI Thread_Main( LPVOID lpParam ) {
    /* Get time that thread starts executing
     * Avoid use of GetSystemTime() because Microsoft said so.
     */
    /* TODO: More testing. Possibly use different method to measure time 
     * elapsed. See Windows documentation and ask Jarrod.
     */
    DWORD init_time = GetTickCount();

    /* set a counter for counting Square() recursive calls */
	size_t counter = 0;
	
	/* Do the following to prevent overwrite and compiler errors
	 * for attempting to dereference a LPVOID */
	size_t size = *(size_t*)lpParam;
	
	if(! TlsSetValue(dwTlsIndex, &counter))
        error_exit("TlsSetValue error in Thread_Main\n");
	
	for (size_t i = 1; i <= size && keepRunning; ++i){
		Square(i);
	}
	
	counter = *(size_t*)TlsGetValue(dwTlsIndex);
	printf("No. of Square calls: %d, Elapsed Time: %d\n",
            counter,
            GetTickCount() - init_time);
	return EXIT_SUCCESS;
}

void incr_func(){
	size_t* counter = TlsGetValue(dwTlsIndex);
	++(*counter);
}

/* Error function to return errors to stderr */
VOID error_exit (LPSTR lpszMessage) {
    fprintf(stderr, "%s\n", lpszMessage);
    ExitProcess(0);
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
        error_exit("thread_array malloc error\n");
	}
	
	/* allocating a TLS index */
	if ((dwTlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES) 
        error_exit("TlsAlloc error\n");
	
	/* create threads and stash them in thread array */
	printf("Creating threads\n");
	for (size_t i = 0; i < num_threads; ++i){
		thread_array[i] = CreateThread(
				NULL,                   /* security attributes */
				0,                      /* default stack size */
				Thread_Main,            /* thread function */
				&size,                  /* ptr to thread function argument */
				0,                      /* default creation flag */
				NULL);                  /* thread identifier */
	}
	Sleep(1000*deadline); /* 1000 for converting s to ms */
	keepRunning = false;

    /* Wait until threads complete before exiting main() */
    if (WaitForMultipleObjects(
            num_threads,        /* number of threads in array */
            thread_array,       /* pointer to array of object handles */
            TRUE,               /* bWaitAll: TRUE to wait for all threads */
            INFINITE)){         /* wait until all threads return */
        error_exit("WaitForMultipleObjects error\n");
    }
    
    /* free allocated memory */
    TlsFree(dwTlsIndex);        /* free allocated TLS memory */
	free(thread_array);         /* free thread array */  

	return EXIT_SUCCESS;
}
