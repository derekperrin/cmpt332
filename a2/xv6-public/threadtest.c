#include "types.h"
#include "stat.h"
#include "user.h"

int j;

int 
Square(int N)
{
	return 0;
}

void tmain(void *args)
{
	int pid, *dArgs;
	dArgs = (int*) args;
	pid = getpid();
	j = (j+1)%4;
	printf(1, "PID: %d\tArg: %d\n", pid, dArgs[j]);
	exit();
}

int
main(void)
{
	int i, *args;
	void *stack1, *stack2, *stack3, *stack4, *stack5;
	void *dummy;
	dummy = 0;
	j = 0;

	args = malloc(4*sizeof(int));
	args[0] = 2;
	args[1] = 3;
	args[2] = 5;
	args[3] = 7;
	stack1 = malloc(256);
	stack2 = malloc(256);
	stack3 = malloc(256);
	stack4 = malloc(256);
	stack5 = malloc(256);

	thread_create(tmain, stack1 + 255, (void*)args);
	thread_create(tmain, stack2 + 255, (void*)args);
	thread_create(tmain, stack3 + 255, (void*)args);
	thread_create(tmain, stack4 + 255, (void*)args);
	thread_create(tmain, stack5 + 255, (void*)args);

	for (i = 0; i < 5; i++) {
		thread_join(&dummy);
		free(dummy - 255);
	}
	free(args);
	exit();
}
