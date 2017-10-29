#include "types.h"
#include "stat.h"
#include "user.h"

void tmain(void *args)
{
	printf(1, "I'm a child thread\n");
	exit();
}

int
main(void)
{
	int tid;
	void *stack, *args;
	stack = malloc(4096);
	args = malloc(2048);
	tid = thread_create(tmain, stack, args);
	printf(1, "thread id: %d\n", tid);
	sleep(5);
	printf(1, "hopefully child finished.\n");
	free(stack);
	free(args);
	exit();
}
