/*************************************************************************
> FileName: huge-pages.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 31 Jan 2024 03:23:14 PM CST
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define HUGE_PAGES_SIZE         (2 * 1024 * 1024)   // 2MB
#define HUGE_PAGES_COUNT        (1000)

int main ()
{
    //const char* filePath = "hugepages.data";
    int fileSize = HUGE_PAGES_SIZE * HUGE_PAGES_COUNT;

    // 打开 Huge Pages 文件
    //int fd = open (filePath, O_CREAT | O_RDWR, 0644);
    //if (fd < 0) {
    //    perror("open");
    //    return -1;
    //}

    // 调整大小
    //if (ftruncate(fd, fileSize) < 0) {
    //    perror("ftruncate");
    //    return -1;
    //}

    // 映射 Huge Pages 到内存
    void* addr = mmap (NULL, fileSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (MAP_FAILED == addr) {
        perror ("mmap");
        return -1;
    }

    // 使用 Huge Pages 进行内存操作
    // 这里使用 addr 指针来访问Huge Pages

    
    int i = 0;
    while (fileSize > ++i) strncpy (&(addr[i]), "a", 1);

    //while (1);

    // 解除映射关系并关闭文件
    munmap(addr, fileSize);

    //close(fd);

    //unlink(filePath);

    return 0;
}
