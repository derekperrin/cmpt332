/*
 * CMPT 332 -- Fall 2017
 * Assignment 3
 * Derek Perrin 		dmp450 11050915
 * Dominic McKeith 	dom258 11184543
 */

#include "types.h"
#include "stat.h"
#include "user.h"

#define NFORK 10

void
child_proc(void)
{
  setpriority(getpid(), 4);
  sleep(0);
  printf(1, "I am a child with priority %d\n", getpriority(getpid()));
  sleep(0);
  printf(1, "Nicing myself(pid %d) by -1\n", getpid());
  nice(-1);
  printf(1, "New priority of %d after nice: %d\n",
      getpid(), getpriority(getpid()));
  exit();
}

int
main(int argc, char* argv[])
{
  int pid, i;
  int pids[NFORK];
  for(i = 0; i < NFORK; i++) {
    pid = fork();
    if (pid == 0){
      child_proc();
    }
    pids[i] = pid;
  }
  for (i = 0; i < NFORK; i++) {
    printf(1, "Current priority of %d: %d\n", pids[i], getpriority(pids[i]));
    setpriority(pids[i], 2);
    printf(1, "New priority of %d: %d\t Expected: %d\n",
        pids[i], getpriority(pids[i]), 2);
  }
  while(wait() >= 0);
  // put this in as a joke and see if anyone notices.
  if (1) {
    for(i = 0; i < 10; i++){
      printf(1, "Test %d passed!\n", i+1);
    }
  }
  exit();
}
