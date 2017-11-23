/*
 * CMPT 332 -- Fall 2017
 * Assignment 3
 * Derek Perrin 		dmp450 11050915
 * Dominic McKeith 	dom258 11184543
 */

#include "types.h"
#include "stat.h"
#include "user.h"

void
child_proc(void)
{
  int *memcell;
  // int i;
  for(;;){
    memcell = malloc(4096);
    if (memcell == 0){
      printf(1, "Out of memory. Exiting\n");
      exit();
    }
  }
  memcell[0] = 1;
  printf(1, "%d\n", memcell[0]);
  exit();
}

int
main(int argc, char* argv[])
{
  int i, pid;
  int pids[20];

  // As per the assignment spec, fork 20 children.
  for (i = 0; i < 20; i++) {
    pid = fork();
    if (pid == 0){
      child_proc();
    }
    pids[i] = pid;
  }

  printf(1, "first child pid: %d\n", pids[0]);

  while(wait() >= 0);
  exit();
}
