/*************************************************************************
> FileName: mmap-malloc.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年07月22日 星期二 18时25分26秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define PAGE_SIZE 4096

// 自定义的 malloc 函数，使用 mmap
void* my_malloc(size_t size) 
{
    // 计算需要分配的内存页数
    size_t pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    // 使用 mmap 分配内存
    void* ptr = mmap(NULL, pages * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        return NULL; // 分配失败
    }
    return ptr;
}

// 自定义的 free 函数，使用 munmap
void my_free(void* ptr, size_t size) 
{
    // 计算需要释放的内存页数
    size_t pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    munmap(ptr, pages * PAGE_SIZE);
}

int main() 
{
    size_t size = 100; // 要分配的内存大小
    // 使用自定义的 malloc 分配内存
    void* mem = my_malloc(size);
    if (mem == NULL) {
        perror("mmap failed");
        return EXIT_FAILURE;
    }

    // 使用分配的内存
    strcpy(mem, "Hello, mmap!");
    printf("%s\n", (char*)mem);

    // 释放内存
    my_free(mem, size);
    return EXIT_SUCCESS;
}
