#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
int
sys_thread_create(void)
{
  void (*tmain) (void *);
  int stack;
  void *arg;
  if (argptr(0, (void*) &tmain, sizeof(tmain)) < 0)
    return -1;
  if (argint(1, (void*) &stack) < 0)
    return -1;
  if (argptr(2, (void*) &arg, sizeof(arg)) < 0)
    return -1;
  return thread_create(tmain, (void*)stack, arg);
/*
	int tmain, stack, arg;
	if(argint(0, &tmain) < 0)
			return -1;
	if(argint(1, &stack) < 0)
			return -1;
	if(argint(2, &arg) < 0)
			return -1;
  return thread_create((void*)tmain, (void*)stack, (void*)arg);
  */
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
int
sys_thread_join(void)
{
  void **stack;
  if(argptr(0, (void*) &stack, sizeof(stack) < 0))
     return -1;
  
  return thread_join(stack);
}
