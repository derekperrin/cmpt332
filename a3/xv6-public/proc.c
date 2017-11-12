#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// Mutex structure
struct mutex {
	int owner;	// PID of the process which owns this mutex.
	int value;	// Value of the mutex. Either 0 or 1.
	struct spinlock mlock; // spinlock needed to adjust each mutex
};

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

static struct proc *initproc;

int nextpid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

void
pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
struct mutex mutexes[NOMUTEX];	// allocate mutexes from here
int freemtx = 0;	// next free mutex available to be created.


//PAGEBREAK: 32
// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize
// state required to run in the kernel.
// Otherwise return 0.
static struct proc*
allocproc(void)
{
  struct proc *p;
  char *sp;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == UNUSED)
      goto found;
  release(&ptable.lock);
  return 0;

found:
  p->state = EMBRYO;
  p->pid = nextpid++;
  /* CMPT 332 GROUP 23 Change, Fall 2017 */
  p->priority = 0;
  release(&ptable.lock);

  // Allocate kernel stack.
  if((p->kstack = kalloc()) == 0){
    p->state = UNUSED;
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;

  // Leave room for trap frame.
  sp -= sizeof *p->tf;
  p->tf = (struct trapframe*)sp;

  // Set up new context to start executing at forkret,
  // which returns to trapret.
  sp -= 4;
  *(uint*)sp = (uint)trapret;

  sp -= sizeof *p->context;
  p->context = (struct context*)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)forkret;

  return p;
}

//PAGEBREAK: 32
// Set up first user process.
void
userinit(void)
{
  struct proc *p;
  extern char _binary_initcode_start[], _binary_initcode_size[];

  p = allocproc();
  initproc = p;
  if((p->pgdir = setupkvm()) == 0)
    panic("userinit: out of memory?");
  inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = FL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0;  // beginning of initcode.S

  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");

  p->state = RUNNABLE;
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
  uint sz;

  sz = proc->sz;
  if(n > 0){
    if((sz = allocuvm(proc->pgdir, sz, sz + n)) == 0)
      return -1;
  } else if(n < 0){
    if((sz = deallocuvm(proc->pgdir, sz, sz + n)) == 0)
      return -1;
  }
  proc->sz = sz;
  switchuvm(proc);
  return 0;
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int
fork(void)
{
  int i, pid;
  struct proc *np;

  // Allocate process.
  if((np = allocproc()) == 0)
    return -1;

  // Copy process state from p.
  if((np->pgdir = copyuvm(proc->pgdir, proc->sz)) == 0){
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    return -1;
  }
  np->sz = proc->sz;
  np->parent = proc;
  *np->tf = *proc->tf;
  /* CMPT 332 GROUP 23 Change, Fall 2017 */
  np->priority = proc->priority

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;

  for(i = 0; i < NOFILE; i++)
    if(proc->ofile[i])
      np->ofile[i] = filedup(proc->ofile[i]);
  np->cwd = idup(proc->cwd);

  safestrcpy(np->name, proc->name, sizeof(proc->name));

  pid = np->pid;

  // lock to force the compiler to emit the np->state write last.
  acquire(&ptable.lock);
  np->state = RUNNABLE;
  release(&ptable.lock);

  return pid;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void
exit(void)
{
  struct proc *p;
  int fd;

  if(proc == initproc)
    panic("init exiting");

  // Close all open files.
  for(fd = 0; fd < NOFILE; fd++){
    if(proc->ofile[fd]){
      fileclose(proc->ofile[fd]);
      proc->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(proc->cwd);
  end_op();
  proc->cwd = 0;

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  wakeup1(proc->parent);

  // Pass abandoned children to init.
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->parent == proc){
      p->parent = initproc;
      if(p->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  proc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(void)
{
  struct proc *p;
  int havekids, pid;

  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for zombie children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    /* CMPT 332 GROUP 23 Change, Fall 2017 */
      if(p->parent != proc || p->pgdir == proc->pgdir)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->state = UNUSED;
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || proc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(proc, &ptable.lock);  //DOC: wait-sleep
  }
}

//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.
void
scheduler(void)
{
  struct proc *p;

  for(;;){
    // Enable interrupts on this processor.
    sti();

    // Loop over process table looking for process to run.
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;

      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      proc = p;
      switchuvm(p);
      p->state = RUNNING;
      swtch(&cpu->scheduler, proc->context);
      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
      proc = 0;
    }
    release(&ptable.lock);

  }
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state.
void
sched(void)
{
  int intena;

  if(!holding(&ptable.lock))
    panic("sched ptable.lock");
  if(cpu->ncli != 1)
    panic("sched locks");
  if(proc->state == RUNNING)
    panic("sched running");
  if(readeflags()&FL_IF)
    panic("sched interruptible");
  intena = cpu->intena;
  swtch(&proc->context, cpu->scheduler);
  cpu->intena = intena;
}

// Give up the CPU for one scheduling round.
void
yield(void)
{
  acquire(&ptable.lock);  //DOC: yieldlock
  proc->state = RUNNABLE;
  sched();
  release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void
forkret(void)
{
  static int first = 1;
  // Still holding ptable.lock from scheduler.
  release(&ptable.lock);

  if (first) {
    // Some initialization functions must be run in the context
    // of a regular process (e.g., they call sleep), and thus cannot
    // be run from main().
    first = 0;
    iinit(ROOTDEV);
    initlog(ROOTDEV);
  }

  // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void
sleep(void *chan, struct spinlock *lk)
{
  if(proc == 0)
    panic("sleep");

  if(lk == 0)
    panic("sleep without lk");

  // Must acquire ptable.lock in order to
  // change p->state and then call sched.
  // Once we hold ptable.lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup runs with ptable.lock locked),
  // so it's okay to release lk.
  if(lk != &ptable.lock){  //DOC: sleeplock0
    acquire(&ptable.lock);  //DOC: sleeplock1
    release(lk);
  }

  // Go to sleep.
  proc->chan = chan;
  proc->state = SLEEPING;
  sched();

  // Tidy up.
  proc->chan = 0;

  // Reacquire original lock.
  if(lk != &ptable.lock){  //DOC: sleeplock2
    release(&ptable.lock);
    acquire(lk);
  }
}

//PAGEBREAK!
// Wake up all processes sleeping on chan.
// The ptable lock must be held.
static void
wakeup1(void *chan)
{
  struct proc *p;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == SLEEPING && p->chan == chan)
      p->state = RUNNABLE;
}

// Wake up all processes sleeping on chan.
void
wakeup(void *chan)
{
  acquire(&ptable.lock);
  wakeup1(chan);
  release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int
kill(int pid)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->killed = 1;
      // Wake process from sleep if necessary.
      if(p->state == SLEEPING)
        p->state = RUNNABLE;
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}

//PAGEBREAK: 36
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  int i;
  struct proc *p;
  char *state;
  uint pc[10];

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    cprintf("%d %s %s", p->pid, state, p->name);
    if(p->state == SLEEPING){
      getcallerpcs((uint*)p->context->ebp+2, pc);
      for(i=0; i<10 && pc[i] != 0; i++)
        cprintf(" %p", pc[i]);
    }
    cprintf("\n");
  }
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// Creates a new thread of execution. The new thread is the same as a process,
// but it shares the address space with the parent process. New thread starts
// execution at tmain and takes a pointer to arguments in arg. The user is
// responsible for providing a stack for the newly provided thread.
int
thread_create(void (*tmain)(void *), void *stack, void *arg)
{
  int tid, i;
  uint sp, ustack[2];
  struct proc *nt;

  // Allocate thread like a process.
  if ((nt = allocproc()) == 0)
    return -1;

  // Copy process state and page table from p.
  nt->pgdir = proc->pgdir;
  nt->sz = proc->sz;
  nt->parent = proc;
  *(nt->tf) = *(proc->tf);
  nt->tf->eip = (uint)tmain; // Set instruction pointer to tmain.

  // EXPERIMENTAL
  nt->tf->eax = 0;
  nt->tstack = (char*)stack;  // lets us give stack back in thread_join()

  // Set stack pointer to point to new stack.
  // Decrement it by 2 to move it below the return PC.
  // See Figure 2-3 in xv6 book for memory layout. Also, see code for exec()
  sp = (uint)stack;
  sp -= 2*sizeof(uint);
  ustack[0] = 0xffffffff; // fake return PC
  ustack[1] = (uint)arg;  // argument
  if (copyout(nt->pgdir, sp, ustack, 2*sizeof(uint)) < 0)
    return -1;

  nt->tf->esp = sp;

  // Copy open files.
  for(i = 0; i < NOFILE; i++)
    if(proc->ofile[i])
      nt->ofile[i] = filedup(proc->ofile[i]);

  nt->cwd = idup(proc->cwd);

  // Don't actually need this. Just for debugging.
  safestrcpy(nt->name, proc->name, sizeof(proc->name));

  tid = nt->pid;

  // Change thread state.
  acquire(&ptable.lock);
  nt->state = RUNNABLE;
  release(&ptable.lock);

  return tid;
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// Calling process will block until a child thread exits.
// Return pid of child thread on success. Store the stack pointer of the child
// thread in the stack argument provided so the user can free the stack.
int
thread_join(void **stack)
{
  struct proc *p;
  int havekids, pid;

  acquire(&ptable.lock);
  for(;;){/* CMPT 332 GROUP 23 Change, Fall 2017 */
    // Scan through table looking for zombie children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != proc)
        continue;
      if(proc->pgdir != p->pgdir)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        p->state = UNUSED;
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        *stack = (void*)p->tstack;
        p->tstack = 0;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || proc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(proc, &ptable.lock);  //DOC: wait-sleep
  }
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// Creates a new mutex if one is available.
// Return the id of the mutex if one is created, -1 otherwise.
// if "locked" is 0, mutex is initialized as unlocked. Otherwise it's locked.
int
mtx_create(int locked)
{
	int mtx_id;
	mtx_id = 0;
	// check if any more mutexes are available to be created.
	if (freemtx >= NOMUTEX)
		return -1;

	// Initialize the spinlock so processes can use it.
	initlock(&mutexes[mtx_id].mlock, "mutex");

	mtx_id = freemtx;
	freemtx++;

	// Set lock state based on argument
  acquire(&mutexes[mtx_id].mlock);
	if(locked){
		mutexes[mtx_id].owner = proc->pid;
		mutexes[mtx_id].value = 1;
	} else {
		mutexes[mtx_id].owner = 0;
		mutexes[mtx_id].value = 0;
	}
  release(&mutexes[mtx_id].mlock);

	return mtx_id;
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// Requests ownership of the mutex. Calling process blocks if mutex is not
// available. Returns 0 on success, -1 otherwise(invalid parameter)
int
mtx_lock(int lock_id)
{
	void *chan;
	chan = 0;
	// make sure a mutex was allocated before allowing a lock request to be made.
	if (lock_id >= freemtx || lock_id < 0)
		return -1;
	chan = (void*)(mutexes + lock_id);
	acquire(&(mutexes[lock_id].mlock));

	// sleep until a lock is acquired on the mutex
	while (mutexes[lock_id].value > 0 && mutexes[lock_id].owner != proc->pid) {
		sleep(chan, &(mutexes[lock_id].mlock));
	}

	// after the lock has been acquired
	mutexes[lock_id].owner = proc->pid;
	mutexes[lock_id].value = 1;
	release(&(mutexes[lock_id].mlock));
  return 0;
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// Releases ownership of the mutex. Calling process must own mutex.
// Returns 0 on success, -1 otherwise.
int
mtx_unlock(int lock_id)
{
  void *chan;
  chan = 0;
	// check lock_id is valid and if proc owns mutex
	if (lock_id >= freemtx || lock_id < 0)
		return -1;
	if (mutexes[lock_id].owner != proc->pid)
		return -1;

  chan = (void*)(mutexes + lock_id);
	acquire(&(mutexes[lock_id].mlock));
	mutexes[lock_id].value = 0;
	mutexes[lock_id].owner = -1;
	release(&(mutexes[lock_id].mlock));
  wakeup(chan);
  return 0;
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// increase the priority of the current process by incr
int
nice(int incr)
{
  return 0;
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// get the priority of the process with PID pid
int
getpriority(int pid)
{
  return 0;
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// set the prority of the process with PID pid to new_priority

int
setpriority(int pid, int new_priority)
{
  return 0;
}
