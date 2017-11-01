/* CMPT 332 -- Fall 2017
* Assignment 2
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <Monitor.h>
#include <list.h>
#include <stdlib.h>
#include <stdio.h>

/* UBC Pthreads */
#include <os.h>
#include <standards.h>

typedef struct CV {
    int sem;
    LIST* wait_queue;
}CV;


CV cond_vars[NO_CVS];
int enter_mtx, urgent_sem, enter_queue_sem;
LIST* urgent_queue;
LIST* enter_queue;

void MonInit(void) {
    int i;

    /* Initialize queues used for debugging purpposes */
    urgent_queue = ListCreate();
    enter_queue = ListCreate();

    /* Initialize semaphores for entering monitor and getting on queues */
    urgent_sem = NewSem(0);
    enter_mtx = NewSem(1);  
    enter_queue_sem = NewSem(1);

    for (i = 0; i < NO_CVS; ++i) {
        cond_vars[i].wait_queue = ListCreate();
        cond_vars[i].sem = NewSem(0);   /* shrugs */
    }
}

void MonEnter(void) {
    PID* my_pid;

    my_pid = malloc(sizeof(PID));
    *my_pid = MyPid();

    P(enter_queue_sem);
    ListPrepend(enter_queue, (void*)my_pid);
    P(enter_mtx);
    ListTrim(enter_queue);
    V(enter_queue_sem);
    return;
}

void MonLeave(void) {
    if (ListCount(urgent_queue) > 0) {
        V(urgent_sem);
    } else if (ListCount(enter_queue) > 0) {
        V(enter_mtx);
    }
    return;
}

void MonWait(int cv) {
    PID *my_pid;

    my_pid = malloc(sizeof(PID));
    *my_pid = MyPid();
    ListPrepend(cond_vars[cv].wait_queue,(void*)my_pid);
    P(cond_vars[cv].sem);

    free(ListTrim(cond_vars[cv].wait_queue));
    return;
}

void MonSignal(int cv) {
    PID* my_pid;
    
    my_pid = malloc(sizeof(PID));
    *my_pid = MyPid();

    ListPrepend(urgent_queue, (void*)my_pid);
    V(cond_vars[cv].sem);
    P(urgent_sem);
    free(ListTrim(urgent_queue));
    return;
}
