/*************************************************************************
> FileName: mmap.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2022年02月22日 星期二 09时28分47秒
 ************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

int main ()
{
    int fd = -1;

    if ((fd = open("./file.txt", O_RDWR|O_CREAT, S_IRWXU)) < 0) {
        printf ("open file wrong!\n");
        exit(1);
    }
 
    struct stat file_stat;
    if (fstat (fd, &file_stat) < 0) {
        printf (" fstat wrong\n");
        exit(1);
    }

    void *start_fp;
    if ((start_fp = mmap (NULL, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        printf ("mmap wrong\n");
        exit(0);
    }

    snprintf ((char *)start_fp, 4, "test");
    msync (start_fp, file_stat.st_size, MS_ASYNC);

    if (munmap(start_fp, file_stat.st_size ) < 0 ) {
        printf("munmap wrong\n");
        exit(1);
    }

    return 0;
}
