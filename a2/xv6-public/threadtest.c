#include "types.h"
#include "stat.h"
#include "user.h"

int 
Square(int N)
{
	return 0;
}

void tmain(void *args)
{
	printf(1, "I'm a child thread\n");
	exit();
}

int
main(void)
{
	int i;
	void *stack1, *stack2, *stack3, *stack4, *stack5;
	void* dummy;
	dummy = 0;

	stack1 = malloc(256);
	stack2 = malloc(256);
	stack3 = malloc(256);
	stack4 = malloc(256);
	stack5 = malloc(256);

	thread_create(tmain, stack1 + 255, 0);
	thread_create(tmain, stack2 + 255, 0);
	thread_create(tmain, stack3 + 255, 0);
	thread_create(tmain, stack4 + 255, 0);
	thread_create(tmain, stack5 + 255, 0);

	for (i = 0; i < 5; i++) {
		thread_join(&dummy);
		free(dummy);
	}
	exit();
}
