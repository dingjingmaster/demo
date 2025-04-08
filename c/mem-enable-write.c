/*************************************************************************
> FileName: mem-enable-write.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月08日 星期二 16时23分07秒
 ************************************************************************/
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>


void set_mem_write (void* mem);
void set_mem_readonly (void* mem);

int main (int argc, char* argv[])
{
	const char* str = "aaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	const char* str1 = "ssss";

	set_mem_write((void*)str);
	strcpy(str, str1);
	set_mem_readonly((void*)str);

	printf("%s\n", str);
}

void set_mem_write (void* mem)
{
    if (!mem) { return; }

    size_t psize = sysconf(_SC_PAGESIZE);
    uintptr_t addr = (uintptr_t) mem;
    uintptr_t pstart = addr & ~(psize - 1);
    if (0 != mprotect((void*) pstart, psize, PROT_READ | PROT_WRITE)) {
		perror("mprotect");
    }
}

void set_mem_readonly (void* mem)
{
    size_t psize = sysconf(_SC_PAGESIZE);
    uintptr_t addr = (uintptr_t) mem;
    uintptr_t pstart = addr & ~(psize - 1);
    if (0 != mprotect((void*) pstart, psize, PROT_READ)) {
		perror("mprotect");
    }
}

