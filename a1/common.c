/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>

/***
 * Validates arguments
 * threads: number of threads to create
 * deadline: how long parent waits before killing thread in ms
 * size: n how many squares to compute in Square()
 * return EXIT_SUCCESS if valid, EXIT_FAILURE otherwise
***/
int parse_args(int* threads, int* deadline, int* size,
        int argc, char* argv[argc+1]){
    if (argc != 4) 
        return EXIT_FAILURE;
    if ( sscanf(argv[1], "%d", threads) != 1 ||
        sscanf(argv[2], "%d", deadline) != 1 ||
        sscanf(argv[3], "%d", size) != 1 ) {
        return EXIT_FAILURE;
    }
    if (*threads <= 0 || *deadline <= 0 || *size <= 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

/***
 * prints errors to stderr if any arguments are invalid
***/
void arg_error(){
    fprintf(stderr, "Invalid args: THREADS DEADLINE SIZE\n");
}

/***
 * Inefficient Square function provided by Makaroff
 * n: number of squares to compute
 * return: square of n
***/
void Square ( int n ) {
    printf("Got to procedure Square\n");
}
