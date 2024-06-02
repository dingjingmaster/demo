/*************************************************************************
> FileName: pipe2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 19 Dec 2022 11:18:19 AM CST
 ************************************************************************/
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define F_LINUX_SPECIFIC_BASE               1024

#ifndef F_SETPIPE_SZ
#define F_SETPIPE_SZ (F_LINUX_SPECIFIC_BASE + 7)
#endif

int main (int argc, char* argv[])
{
    // array of 2 size a[0] is for reading 
    // and a[1] is for writing over a pipe 
    int a[2] = {0};

    // opening of pipe using pipe(a)   
    char buff[10] = {0};
    if (pipe2(a, O_CLOEXEC) == -1) {
        perror("pipe"); // error in pipe
        exit(1); // exit from the program
    }

    fcntl(a[1], F_SETPIPE_SZ, 1048576);

    // writing a string "code" in pipe
    write(a[1], "code", 5);
    printf("\n");

    // reading pipe now buff is equal to "code"
    read(a[0], buff, 5);

    // it will print "code"
    printf("%s", buff);

    return 0;
}
