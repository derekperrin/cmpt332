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
#include <string.h>

#define MAX_MESSAGE_SIZE 512

typedef struct thread_communication{
    char* message;
    int from; 
    /* from symantics:
        0: server
        1: inputMessage
        2: receiveMessage
        3: printMessage
        4: sendMessage
     */
} thread_comm;

/* Read in a message fromt he keyboard and send it to the server */
void inputMessage(void){
    /* TODO: Implement this for waiting for keyboard input */
    printf("I am the message inputter\n");
    
    /* constantly read in from the console */
    /*for(;;){
        
    }*/
    
    RttExit();
}

/* Recieve a message from the network and send it to the server */
void receiveMessage(void){
    /* TODO: Implement whatever this does */
    printf("I am the message reciever\n");
    RttExit();
}

/* grab a message from the server that was placed there by receiveMessage and
 * print it to the console
 */
void printMessage(void){
    /* TODO: Implement this to print stuff to screen */
    printf("I am the message printer\n");
    RttExit();
}

/* Grab a message from the server that was put there by inputMessage and send
 * it over the network to the destination address
 */
void sendMessage(void){
    /* TODO: Implement whatever this does too */
    printf("I am the message sender\n");
    RttExit();
}

void server(void){
    LIST* incoming,* outgoing;
    RttThreadId* from;
    thread_comm* received_message;
    u_int* message_size;
    /* TODO: Implement server to handle everything */
    printf("I am the server\n");
    /* First, setup the queues for incomming and outgoing messages */
    /* setup the queue for incomming messages */
    
    incoming = ListCreate();
    outgoing = ListCreate();
    received_message = malloc(sizeof(thread_comm));
    message_size = malloc(sizeof(u_int));
    *message_size = sizeof(thread_comm);
    from = NULL;
    
    for(;;){
        /* If a thread has sent a message to the server */
        if (RttMsgWaits() == 1){
            /* recieve the message */
            /* TODO: check return value of RttReceive */
            RttReceive(from, received_message, message_size);
            switch(received_message->from){
                case 0: /* Message is from server */
                    printf("Error: Server received message from itself!\n");
                    break;
                case 1: /* Message is from inputMessage */
                    printf("Server received message from inputMessage.\n");
                    ListAdd(outgoing, received_message->message);
                    /* TODO: check return value of ListAdd */
                    break;
                case 2: /* Message is from receiveMessage */
                    printf("Server received message from receiveMessage.\n");
                    ListAdd(incoming, received_message->message);
                    break;
                case 3: /* Message is from printMessage */
                    printf("Error: server should not be receiving messages "
                           "from printMessage.\n");
                    break;
                case 4: /* Message is from sendMessage */
                    printf("Server received message from sendMessage.\n");
                    break;
                default:
                    printf("Error: server received message from unknown source"
                           ".\n");
                    break;
            }
        }
        /* If the server is not receiving a message, then send a message to a 
           local thread if there is one in the queue */
        if (ListCount(outgoing) > 0){
            /* send a message to printMessage and remove node from Queue/List*/
        }
        /* Else, send a message over the network. */
    }
    
    RttExit();
}



/* Set up the threads */
int mainp(int argc, char* argv[]){
    int my_port, their_port;
    char* pc_name;
    my_port = atoi(argv[1]);
    pc_name = argv[2];
    their_port = atoi(argv[3]);

    printf("Arguments: %d %s %d\n", my_port, pc_name, their_port);
    /* use seperate scopes to get around c90 conventions 
       and to make things more organized. */ /* LOL LOL LOL -Derek */
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
        sm_thread_name = malloc(sizeof(char)*12);
        strncpy(sm_thread_name, "sendMessage\0", 12);

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

        /*sm_pid = */RttCreate(sm_id, sendMessage, 2<<20 /* one megabyte */, 
                               sm_thread_name, 
                               sm_args /* have no args for now */,
                               *sm_priority, 
                               RTTUSR /* make it a user level thread */);
    }
    {
        /* set up the printMessage thread */
        RttThreadId* pm_id;
        char* pm_thread_name;
        RttSchAttr* pm_priority;
        int* pm_args;

        pm_id = malloc(sizeof(RttThreadId));
        pm_thread_name = malloc(sizeof(char)*12);
        strncpy(pm_thread_name, "printMessage\0", 12);

        pm_priority = malloc(sizeof(RttSchAttr));

        pm_priority->startingtime.seconds = 0; /* Start time of 0? */
        pm_priority->startingtime.microseconds = 0; /* Start time of 0? */
        pm_priority->deadline.seconds = LONG_MAX; /*Deadline of the max 
                                    because we don't want the thread to exit */
        pm_priority->deadline.microseconds = LONG_MAX; /* "" */
        pm_priority->priority = 20; /* choose a value halfway between the 
                                           recommended values (10 to 30) */

        pm_args = malloc(sizeof(int));
        *pm_args = 10;

        /*sm_pid = */RttCreate(pm_id, printMessage, 2<<20 /* one megabyte */, 
                               pm_thread_name, 
                               pm_args /* have no args for now */,
                               *pm_priority, 
                               RTTUSR /* make it a user level thread */);
    }
    {
        /* set up the receiveMessage thread */
        RttThreadId* rm_id;
        char* rm_thread_name;
        RttSchAttr* rm_priority;
        int* rm_args;

        rm_id = malloc(sizeof(RttThreadId));
        rm_thread_name = malloc(sizeof(char)*15);
        strncpy(rm_thread_name, "receiveMessage\0", 15);

        rm_priority = malloc(sizeof(RttSchAttr));

        rm_priority->startingtime.seconds = 0; /* Start time of 0? */
        rm_priority->startingtime.microseconds = 0; /* Start time of 0? */
        rm_priority->deadline.seconds = LONG_MAX; /*Deadline of the max 
                                    because we don't want the thread to exit */
        rm_priority->deadline.microseconds = LONG_MAX; /* "" */
        rm_priority->priority = 20; /* choose a value halfway between the 
                                           recommended values (10 to 30) */

        rm_args = malloc(sizeof(int));
        *rm_args = 10;

        /*rm_pid = */RttCreate(rm_id, receiveMessage, 2<<20 /* one megabyte */,
                               rm_thread_name, 
                               rm_args /* have no args for now */,
                               *rm_priority, 
                               RTTUSR /* make it a user level thread */);
    }
    {
        /* set up the inputMessage thread */
        RttThreadId* im_id;
        char* im_thread_name;
        RttSchAttr* im_priority;
        int* im_args;

        im_id = malloc(sizeof(RttThreadId));
        im_thread_name = malloc(sizeof(char)*15);
        strncpy(im_thread_name, "receiveMessage\0", 15);

        im_priority = malloc(sizeof(RttSchAttr));

        im_priority->startingtime.seconds = 0; /* Start time of 0? */
        im_priority->startingtime.microseconds = 0; /* Start time of 0? */
        im_priority->deadline.seconds = LONG_MAX; /*Deadline of the max 
                                    because we don't want the thread to exit */
        im_priority->deadline.microseconds = LONG_MAX; /* "" */
        im_priority->priority = 20; /* choose a value halfway between the 
                                           recommended values (10 to 30) */

        im_args = malloc(sizeof(int));
        *im_args = 10;

        /*im_pid = */RttCreate(im_id, inputMessage, 2<<20 /* one megabyte */,
                               im_thread_name, 
                               im_args /* have no args for now */,
                               *im_priority, 
                               RTTUSR /* make it a user level thread */);
    }
    return 0;
}



