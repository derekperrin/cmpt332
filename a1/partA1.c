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
	size_t counter = 0;
	
	/* Do the following to prevent overwrite and compiler errors
	 * for attempting to dereference a LPVOID */
	size_t size = *(size_t*)lpParam;
	
	/* TODO: get the time thread runs for */
	if(! TlsSetValue(dwTlsIndex, &counter))
		return EXIT_FAILURE;
	
	for (size_t i = 1; i <= size && keepRunning; ++i){
		Square(i);
	}
	
	counter = *(size_t*)TlsGetValue(dwTlsIndex);
	printf("Invocations of Square function: %d\n", counter);
	return EXIT_SUCCESS;
}

void incr_func(){
	size_t* counter = TlsGetValue(dwTlsIndex);
	++(*counter);
}

int main(int argc, char* argv[argc+1]){
	/* TODO: free all thread local storage */
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
	
	/* allocating a TLS index */
	if ((dwTlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES) 
		return EXIT_FAILURE;
	
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
	/* TODO: WaitForSingleObject */
	free(thread_array);
	return EXIT_SUCCESS;
}
