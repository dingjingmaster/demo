/*************************************************************************
> FileName: namespace2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 20 Jun 2024 03:36:23 PM CST
 ************************************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mount.h>

#define STACK_SIZE (1024 * 1024)
static char container_stack[STACK_SIZE];

char* const container_args[] = {
    "/bin/bash",
    NULL
};

int container_main(void* arg) 
{
    printf("Container [%5d] - inside the container!\n", getpid());
    sethostname("container_dawn", 15);
    execv(container_args[0], container_args);
    printf("Something's wrong!\n");
    sleep(5);
    return 1;
}

int main() 
{
    printf("Parent [%5d] - start a container!\n", getpid());
    errno = 0;
    int container_id = clone(container_main, container_stack + STACK_SIZE, 
            CLONE_NEWUTS | CLONE_NEWNS | CLONE_NEWPID,
            NULL);
    if (container_id < 0) {
        printf ("%d: %s\n", container_id, strerror(errno));
        return -1;
    }
    waitpid(container_id, NULL, 0);
    printf("Parent - container stopped!\n");
    return 0;
}
