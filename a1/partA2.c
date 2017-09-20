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

void child_main(int* n) {
    int size = *n;
    for (size_t i = 1; i <= size; ++i) {
        Square(i);
    }
    printf("Child info: %d\n", MyPid());
    Pexit();
}

void parent_main(int* args){
    /* declare necessary argument variables */
    int num_threads = args[0];
    int deadline = args[1];

    PID* thread_array;
    thread_array = malloc(sizeof(PID) * num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        /* create thread */
        thread_array[i] = Create(
                child_main, /* pointer to child thread function */
                2 << 22,    /* stack size */
                "child",    /* look at string cat, or if we need unique */
                args + 2,       /* pointer to arguments for child_main */
                NORM,       /* NORM privilege level */
                USR        /* user level thread */
          );
        if (thread_array[i] == PNUL){
            error_exit("Create child thread error\n");
        }
    }
    /* Sleep parent thread until deadline */
    Sleep(deadline * 100);  /* TICKINTERVAL = 10000 micro-s per tick */
    
    /* TODO: kill threads */
    /*
    for (size_t i = 0; i < num_threads; ++i) {
        if (PExists(i)){
            if (Kill(i) == PNUL)
                error_exit("Kill thread error\n");
            printf("Thread_info\n");
        }
    }
    */

    free(thread_array);
    free(args);

    /* exit */
    Pexit();
}

void error_exit(char* error_message){
    fprintf(stderr, "%s", error_message);
    exit(EXIT_FAILURE);
}

int mainp(int argc, char* argv[argc+1]){
    int* args;
    args = malloc(sizeof(int) * argc - 1);

    /* parse arguments. I'm abusing pointer notation a bit */
    if (parse_args(args, args+1, args+2, argc, argv) != 0 ){
        arg_error();
        exit(EXIT_FAILURE);
    }

    if(Create(
            parent_main,            /* function pointer to thread func */
            2<<22,                  /* stack size magic number */
            "parent_thread",        /* name identifier of thread */
            args,                   /* argument pointer */
            HIGH,                   /* HIGH because it's the parent */
            USR)                    /* user level threads */ 
        == PNUL) {
        error_exit("Create parent error\n");
    }

	printf("Got to procedure main A2\n");
	return EXIT_SUCCESS;
}
