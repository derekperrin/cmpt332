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
  int owner;  // PID of the process which owns this mutex.
  int value;  // Value of the mutex. Either 0 or 1.
  struct spinlock mlock; // spinlock needed to adjust each mutex
};

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

/* CMPT 332 GROUP 23 Change, Fall 2017 */
struct qnode {
  struct proc *p;
  struct qnode *next;
  struct qnode *prev;
};

/* CMPT 332 GROUP 23 Change, Fall 2017 */
struct {
  struct spinlock qlock;  // used for mutual exclusion when accessing queues
  struct qnode *head;
  struct qnode *tail;
  int size;
} queue[NQUEUE];

struct qnode qnodes[NPROC];

struct qnode *freenode;

static struct proc *initproc;

int nextpid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

/* CMPT 332 GROUP 23 Change, Fall 2017 */
static void _queue_add(struct qnode *qn);
static void _queue_remove(struct qnode *qn);

void
pinit(void)
{
  int i;
  initlock(&ptable.lock, "ptable");
  /* CMPT 332 GROUP 23 Change, Fall 2017 */
  for (i = 0; i < NQUEUE; i++){
    initlock(&queue[i].qlock, "queue");
  }
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
struct mutex mutexes[NOMUTEX];  // allocate mutexes from here
int freemtx = 0;  // next free mutex available to be created.


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
    //p->state = UNUSED; // This is how xv6 originally did it.
    p->state = SLEEPING;
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
  struct qnode *qn;
  /* CMPT 332 GROUP 23 Change, Fall 2017 */
  int i;
  freenode = &qnodes[0];
  qnodes[0].prev = 0;
  for (i = 0; i < NPROC-1; i++){
    qnodes[i].next = &qnodes[i+1];
    qnodes[i+1].prev = &qnodes[i];
  }
  qnodes[NPROC-1].next = 0;

  for (i = 0; i < NQUEUE; i++)
    queue[i].size = 0;

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

  /* CMPT 332 GROUP 23 Change, Fall 2017 */
  qn = freenode;
  freenode = freenode->next;
  if(freenode != 0)
    freenode->prev = 0;
  qn->p = p;
  acquire(&queue[p->priority].qlock);
  _queue_add(qn);
  release(&queue[p->priority].qlock);

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
  struct qnode *qn;

  // Allocate process.
  if((np = allocproc()) == 0)
    return -1;

  /* CMPT 332 GROUP 23 Change, Fall 2017 */
  // Grab a node from the free node queue
  qn = freenode; //freenode should never initially be null here
  freenode = freenode->next; 
  if (freenode != 0)
    freenode->prev = 0;

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
  np->priority = proc->priority;

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;

  for(i = 0; i < NOFILE; i++)
    if(proc->ofile[i])
      np->ofile[i] = filedup(proc->ofile[i]);
  np->cwd = idup(proc->cwd);

  safestrcpy(np->name, proc->name, sizeof(proc->name));

  pid = np->pid;

  qn->p = np;
  // lock to force the compiler to emit the np->state write last.
  acquire(&ptable.lock);
  np->state = RUNNABLE;
  /* CMPT 332 GROUP 23 Change, Fall 2017 */
  _queue_add(qn);
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
  int i;
  struct proc *p;
  struct qnode *qn;

  for(;;){
    // Enable interrupts on this processor.
    sti();
    // Instead of looping over the process table looking for a process to run,
    // look over each queue to find a process to run.
    acquire(&ptable.lock);

    for (i = 0; i < NQUEUE; i++){
      acquire(&queue[i].qlock);
      if (queue[i].size == 0){
        release(&queue[i].qlock);
        continue;
      }
      qn = queue[i].head;
      _queue_remove(qn);
      p = qn->p;
      // release qn back to the system.
      qn->next = freenode;
      freenode = qn;proc->state = SLEEPING;
      if (qn->next != 0)
        qn->next->prev = qn;
      qn->prev = 0;
      qn->p = 0;
      release(&queue[i].qlock);

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
      break;
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
  int priority;
  struct qnode *qn;

  qn = freenode;
  freenode = freenode->next;
  if (freenode != 0){
    freenode->prev = 0;
  }
  acquire(&ptable.lock);  //DOC: yieldlock
  priority = proc->priority;
  qn->p = proc;

  acquire(&queue[priority].qlock);
  _queue_add(qn);
  release(&queue[priority].qlock);
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
  /* CMPT 332 GROUP 23 Change, Fall 2017 */
  struct qnode *np;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == SLEEPING && p->chan == chan){
      /* CMPT 332 GROUP 23 Change, Fall 2017 */
      np = freenode;
      freenode = freenode->next;
      if (freenode != 0)
        freenode->prev = 0;
      np->p = p;
      acquire(&queue[p->priority].qlock);
      _queue_add(np);
      release(&queue[p->priority].qlock);

      p->state = RUNNABLE;
    }
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
  /* CMPT 332 GROUP 23 Change, Fall 2017 */
  struct qnode *qn;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->killed = 1;
      // Wake process from sleep if necessary.
      if(p->state == SLEEPING){
        p->state = RUNNABLE;
        /* CMPT 332 GROUP 23 Change, Fall 2017 */
        qn = freenode;
        freenode = freenode->next;
        if (freenode != 0)
          freenode->prev = 0;
        qn->p = p;
        acquire(&queue[p->priority].qlock);
        _queue_add(qn);
        release(&queue[p->priority].qlock);
      }
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
// Search through the appropriate priority queue, remove item with given pid.
// Lock must be acquired on the appropriate priority queue.
void
_queue_remove(struct qnode *qn)
{
  int priority;
  priority = qn->p->priority;
  if (queue[priority].size == 1){
    queue[priority].head = 0;
    queue[priority].tail = 0;
  } else if (queue[priority].head == qn) {
    qn->next->prev = 0;
    queue[priority].head = qn->next;
  } else if (queue[priority].tail == qn) {
    qn->prev->next = 0;
    queue[priority].tail = qn->prev;
  } else {
    qn->prev->next = qn->next;
    qn->next = qn->prev;
  }
  queue[priority].size--;
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// Append a node onto the end of the queue given by the priority of its proc
// member. Must hold lock to the proper queue.
void
_queue_add(struct qnode *qn)
{
  int priority;
  priority = qn->p->priority;
  if (queue[priority].size == 0){
    queue[priority].head = qn;
    queue[priority].tail = qn;
  } else {
    queue[priority].tail->next = qn;
    qn->prev = queue[priority].tail;
    qn->next = 0;
    queue[priority].tail = qn;
  }
  queue[priority].size++;
}


/* CMPT 332 GROUP 23 Change, Fall 2017 */
// increase the priority of the current process by incr
int
nice(int incr)
{
  int new_priority;
  new_priority = proc->priority + incr;
  if (new_priority < 0 || new_priority > 4)
    return -1;
  proc->priority = new_priority;
  // don't move to another queue because current state is RUNNING
  return 0;
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// get the priority of the process with PID pid
int
getpriority(int pid)
{
  int priority;
  struct proc *p;
  priority = -1;
  acquire(&ptable.lock);
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if (p->pid == pid){
      priority = p->priority;
      break;
    }
  release(&ptable.lock);
  return priority;
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// set the prority of the process with PID pid to new_priority
int
setpriority(int pid, int new_priority)
{
  struct proc *p;
  struct qnode *qn;

  if (new_priority > 4 || new_priority < 0)
    return -1;
  acquire(&ptable.lock);
  // Find the process in the process table
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if (p->pid == pid){
      if (p->state == RUNNABLE){
        // remove from old queue
        acquire(&queue[p->priority].qlock);
        qn = queue[p->priority].head; // this should never be null
        while(qn != 0){
          if (qn->p->pid == pid){
            // remove process from its old queue
            _queue_remove(qn);
            release(&queue[p->priority].qlock);

            // add process to its new queue corresponding to its new priority
            qn->p->priority = new_priority;
            acquire(&queue[new_priority].qlock);
            _queue_add(qn);
            release(&queue[new_priority].qlock);
            release(&ptable.lock);
            return 0;
          }
        }
        release(&ptable.lock);
        return -1;  // some sort of error has occurred
      }
      break;
    }
  }

  p->priority = new_priority;
  release(&ptable.lock);
  return 0;
}

void
swapper(void){
  // this function is currently a test.
  // should be running in kernel mode... right????
  // make this function an infinite loop so it never returns.
  int i;
  
  cprintf("The swapper has been loaded.\n");
  
  for(;;){
    i += 1;
    cprintf("The swapper is running with i = %d :)\n", i);
  }
}

/* CMPT 332 GROUP 23 Change, Fall 2017 */
// TODO: remove all reads from global proc variable because it's null!
void
create_kernel_process(const char *name, void (*entrypoint) ()){
  struct proc *np/*, *temp*/;
  struct qnode *qn;
  
  // Allocate process
  if ((np = allocproc()) == 0)
    panic("Failed to allocate kernel process.");
  
  qn = freenode;
  freenode = freenode->next;
  if(freenode != 0)
    freenode->prev = 0;
  /*
  temp = proc;
  temp += 1;
  */
  if((np->pgdir = setupkvm()) == 0){
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    panic("Failed to setup pgdir for kernel process.");
  }
  np->sz = PGSIZE;
  np->parent = initproc; // parent is the first process.
  memset(np->tf, 0, sizeof(*np->tf));
  np->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  np->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  np->tf->es = np->tf->ds;
  np->tf->ss = np->tf->ds;
  np->tf->eflags = FL_IF;
  np->tf->esp = PGSIZE;
  np->tf->eip = 0;  // beginning of initcode.S
  
  // Clear %eax so that fork return 0 in the child
  np->tf->eax = 0;
  /*
  for(i = 0; i < NOFILE; i++)
    if(proc->ofile[i])                        // Change this. VERY BAD!
      np->ofile[i] = filedup(proc->ofile[i]); // Change this. VERY BAD!*/
  np->cwd = namei("/");
  
  safestrcpy(np->name, name, sizeof(name));
  
  qn->p = np;
  // lock to force the compiler to emit the np-state write last.
  acquire(&ptable.lock);
  np->context->eip = (uint)entrypoint;
  np->state = RUNNABLE;
  _queue_add(qn);
  release(&ptable.lock);
}


/*
void
create_kernel_process(const char *name, void (*entrypoint) ()){
  // create a process that never returns, like a system call but never returns.
  struct proc *p;
  char *sp;
  
  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == UNUSED)
      goto foundk;
  release(&ptable.lock);
  panic("Could not create kernel process because all processes are in use.");
  return;
  
foundk:
  p->state = EMBRYO;
  p->pid = nextpid++;
  p->priority = 0;
  release(&ptable.lock);
  
  // Allocate kernel stack.
  if((p->kstack = kalloc()) == 0){
    p->state = UNUSED;
    panic("Could not allocate kernel stack for kernel process.");
    return;
  }
  sp = p->kstack + KSTACKSIZE;
  
  // Leave room for trap frame.
  sp -= sizeof *p->tf;
  p->tf = (struct trapframe*)sp;
  
  // Set up new context to start executing at kernel_process()
  // which will be set to return to trapret but it won't actually return.
  sp -= 4;
  *(uint*)sp = (uint)trapret;
  
  sp -= sizeof *p->context;
  p->context = (struct context*)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)entrypoint;
  
  //return p;
}*/



























