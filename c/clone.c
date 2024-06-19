/*************************************************************************
> FileName: clone.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 19 Jun 2024 01:11:34 PM CST
 ************************************************************************/

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
 

static int child_fn(void* arg) 
{
    printf("PID: %ld -- param: %s\n", (long)getpid(), (char*) arg);
    sleep (5);

    return 0;
}

 
int main() 
{
    int stackSize = 819200;
    char* stack = malloc (stackSize); //[1048576];
    char buf[] = "AAABBBCCCDDD";

    if (!stack) {
        printf ("malloc error\n");
        return -1;
    }

    pid_t child_pid = clone(child_fn, stack + stackSize, CLONE_VM | SIGCHLD, buf, NULL);

    printf("clone() = %ld\n", (long)child_pid);

    waitpid(child_pid, NULL, 0);

    printf("child terminated!\n");

    sleep (5);

    return 0;
}

#if 0

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/syscall.h>


int sub_process (void* udata)
{
    printf ("%s\n", (char*) udata);

    return 2;
}


int main (void)
{
    int ret = 0;
    int stackSize = 81920;
    char* stack = malloc (stackSize);
    char buf[] = "aaaaabbbbbbbbbbbcccccc";
    int flags = CLONE_NEWNS | CLONE_NEWPID;

    pid_t childPid = syscall(SYS_clone, sub_process, stack + stackSize, flags, buf);

    printf ("pid: %d\n", childPid);

    wait(&ret);

    printf ("child ret: %d\n", ret);

    return 0;
}
#endif
