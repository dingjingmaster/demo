#define _GNU_SOURCE
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdarg.h>
#include <unistd.h>
#include <dlfcn.h>

#include <stdio.h>


ssize_t write(int s, const void *data, size_t size)
{
	printf("before real write!\n");
	ssize_t (*sys_write)(int, const void *, size_t) = (ssize_t(*)(int, const void *, size_t))dlsym(RTLD_NEXT, "write");
	ssize_t i = sys_write(s, data, size);
	printf("after real write!\n");
	return i;
}
