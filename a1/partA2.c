/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <os.h>

#include "common.h"

void incr_func(void){
}

void parent_main(int* args){
    /* TODO: declare vars and stuff */

    /* TODO: create threads */

    /* TODO: kill threads */

    /* TODO: free memory */
}

void error_exit(char* error_message){
    fprintf(stderr, "%s", error_message);
    exit(EXIT_FAILURE);
}

int mainp(int argc, char* argv[argc+1]){
    int *args;
    args = malloc(sizeof(int) * argc - 1);

    /* parse arguments. I'm abusing pointer notation a bit */
    if (parse_args(args, args+1, args+2, argc, argv) != 0 ){
        arg_error();
        exit(EXIT_FAILURE);
    }

    if(Create(
            parent_main,          /* function pointer to thread func */
            2<<22,              /* stack size magic number */
            "parent_thread",    /* name identifier of thread */
            args,               /* argument pointer */
            HIGH,               /* HIGH because it's the parent */
            USR)                /* user level threads */ 
        == PNUL) {
        error_exit("Create parent error\n");
    }

	printf("Got to procedure main A2\n");
	return EXIT_SUCCESS;
}
