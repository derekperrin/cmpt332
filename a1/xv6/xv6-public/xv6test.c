#include "types.h"
#include "stat.h"
#include "user.h"

// used for testing Square
#define N 100

int child_func(int size) {
    if (size == 0 ){
        return 0;
    }
    return child_func(size -1) + size + size - 1;
}

int
main(void)
{
    int n, pid, cscount;

    printf(1, "Beginning xv6 test\n");

    for (n = 0; n < 10; ++n) {
        pid = fork();
        if (pid < 0)
            break;
        if (pid == 0){
            child_func(N);
            exit();
        }
    }

    for(n = 0; n < 10; ++n) {
        cscount = csinfo();
        printf(1, "cscount of parent: %d\n", cscount);
        sleep(1);
    }
    while(wait() > -1);
    exit();

}
