#include "types.h"
#include "stat.h"
#include "user.h"

#define STACKSZ 1024

int mtx_id, buffindex;

char BUFFER[1000];

void
producer(void *arg)
{
  int pid, i, j;
  char items[9] = "Producer";

  j = 0;
  pid = getpid();

  while(j < 10){
    
    sleep(1); // used to demonstrate concurrency actually exists
    // acquire the mutex lock
    /*
    if(mtx_lock(mtx_id) < 0) {
      printf(2, "Error in producer obtaining lock\n");
      exit();
    }
    */
    printf(1, "Mutex lock acquired by producer pid %d\n", pid);
    // Critical Section
    for (i = 0; i < 8; i++) {
      BUFFER[i + buffindex] = items[i];
      printf(1, "%s\n", BUFFER);
    }
    buffindex += 8;

    // unlock the mutex
    /*
    if(mtx_unlock(mtx_id) < 0) {
      printf(2, "Error in producer releasing lock\n");
      exit();
    }
    */

    j++;
  }

  exit();

}

void
consumer(void *arg)
{
  int pid, j;

  pid = getpid();
  j = 0;

  while(j < 10) {
    
    sleep(1); // used to demonstrate concurrency actually exists.
    // acquire the mutex lock
    if(mtx_lock(mtx_id) < 0) {
      printf(2, "Error in consumer obtaining lock\n");
      exit();
    }

    // Critical Section
    printf(1, "Lock acquired by consumer pid %d\n", pid);

    printf(1, "Consuming BUFFER\n");
    printf(1, "BUFFER: %s\n", BUFFER);
    memset(BUFFER, 0, sizeof(BUFFER));
    buffindex = 0;
    printf(1, "pid %d cleared BUFFER\n", pid);

    // unlock the mutex
    if(mtx_unlock(mtx_id) < 0) {
      printf(2, "Error in consumer releasing lock\n");
      exit();
    }

    j++;
  }

  exit();

}

int
main(void)
{
  int i;// , p1, p2, p3, c1, c2, c3;
  void *prodstack1, *prodstack2, *prodstack3;
  void *consstack1, *consstack2, *consstack3;
  void *stackret;
  stackret = 0;

  mtx_id = mtx_create(0);
  buffindex = 0;

  prodstack1 = malloc(STACKSZ);
  prodstack2 = malloc(STACKSZ);
  prodstack3 = malloc(STACKSZ);
  consstack1 = malloc(STACKSZ);
  consstack2 = malloc(STACKSZ);
  consstack3 = malloc(STACKSZ);
  
  thread_create(producer, (prodstack1 + STACKSZ - 1), 0);
  thread_create(producer, (prodstack2 + STACKSZ - 1), 0);
  thread_create(consumer, (consstack1 + STACKSZ - 1), 0);
  thread_create(consumer, (consstack2 + STACKSZ - 1), 0);
  thread_create(producer, (prodstack3 + STACKSZ - 1), 0);
  thread_create(consumer, (consstack3 + STACKSZ - 1), 0);

  /*
  if (p1 < 0 || p2 < 0 || p3 < 0 || c1 < 0 || c2 < 0 || c3 < 0){
    printf(2, "Error in main creating threads\n");
    exit();
  }
  */

  for (i = 0; i < 6; i++) {
    if (thread_join(&stackret) < 0) {
      printf(2, "Error in thread_join\n");
      exit();
    }
    free(stackret - STACKSZ + 1);
  }

  printf(1, "Final contents of BUFFER: %s\n", BUFFER);
  exit();
}
