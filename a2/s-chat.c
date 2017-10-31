/* CMPT 332 -- Fall 2017
* Assignment 2
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>
#include <stdlib.h>
#include <list.h>
#include <rtthreads.h>
#include <limits.h>


void inputMessage(void){
    /* TODO: Implement this for waiting for keyboard input */
    printf("I am the message inputter\n");
    RttExit();
}

void recieveMessage(void){
    /* TODO: Implement whatever this does */
    printf("I am the message reciever\n");
    RttExit();
}

void printMessage(void){
    /* TODO: Implement this to print stuff to screen */
    printf("I am the message printer\n");
    RttExit();
}

void sendMessage(void){
    /* TODO: Implement whatever this does too */
    printf("I am the message sender\n");
    RttExit();
}

void server(void){
    /* TODO: Implement server to handle everything */
    printf("I am the server\n");
    RttExit();
}



/* Set up the threads */
int mainp(int argc, char* argv[]){
    int my_port, their_port;
    char* pc_name;
    my_port = atoi(argv[1]);
    pc_name = argv[2];
    their_port = atoi(argv[3]);

    printf("%d %s %d\n", my_port, pc_name, their_port);
    /* use seperate scopes to get around c90 conventions 
       to make things more organized. */
    {
        /* set up the server thread */
        /*int server_pid;*/
        RttThreadId* server_id;
        char* server_thread_name;
        RttSchAttr* server_priority;
        int* server_args;



        server_id = malloc(sizeof(RttThreadId));
        server_thread_name = malloc(sizeof(char)*7);
        server_thread_name[0] = 's'; 
        server_thread_name[1] = server_thread_name[4] = 'e'; 
        server_thread_name[2] = server_thread_name[6] = 'r'; 
        server_thread_name[4] = 'v';
        server_thread_name[7] = '\0';

        server_priority = malloc(sizeof(RttSchAttr));

        server_priority->startingtime.seconds = 0; /* Start time of 0? */
        server_priority->startingtime.microseconds = 0; /* Start time of 0? */
        server_priority->deadline.seconds = LONG_MAX; /*Deadline of the max 
                                        because we don't want the thread to exit */
        server_priority->deadline.microseconds = LONG_MAX; /* "" */
        server_priority->priority = 20; /* choose a value halfway between the 
                                           recommended values (10 to 30) */

        server_args = malloc(sizeof(int));
        *server_args = 10;

        /*server_pid = */RttCreate(server_id, server, 2<<20 /* one megabyte */, 
                               server_thread_name, 
                               server_args /* have no args for now */,
                               *server_priority, 
                               RTTUSR /* make it a user level thread */);
    }
    {
    /* set up the sendMessage thread */
        RttThreadId* sm_id;
        char* sm_thread_name;
        RttSchAttr* sm_priority;
        int* sm_args;



        sm_id = malloc(sizeof(RttThreadId));
        sm_thread_name = malloc(sizeof(char)*7);
        sm_thread_name[0] = 's'; 
        sm_thread_name[1] = sm_thread_name[4] = 'e'; 
        sm_thread_name[2] = sm_thread_name[6] = 'r'; 
        sm_thread_name[4] = 'v';
        sm_thread_name[7] = '\0';

        sm_priority = malloc(sizeof(RttSchAttr));

        sm_priority->startingtime.seconds = 0; /* Start time of 0? */
        sm_priority->startingtime.microseconds = 0; /* Start time of 0? */
        sm_priority->deadline.seconds = LONG_MAX; /*Deadline of the max 
                                        because we don't want the thread to exit */
        sm_priority->deadline.microseconds = LONG_MAX; /* "" */
        sm_priority->priority = 20; /* choose a value halfway between the 
                                           recommended values (10 to 30) */

        sm_args = malloc(sizeof(int));
        *sm_args = 10;

        /*server_pid = */RttCreate(sm_id, sendMessage, 2<<20 /* one megabyte */, 
                               sm_thread_name, 
                               sm_args /* have no args for now */,
                               *sm_priority, 
                               RTTUSR /* make it a user level thread */);
    
    }
    return 0;
}



