/*************************************************************************
> FileName: ptrace.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年09月10日 星期三 12时39分49秒
 ************************************************************************/
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#ifndef _GNU_SOURCE
#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>


int main (int argc, char* argv[])
{
    return 0;
}

