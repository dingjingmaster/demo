/*************************************************************************
> FileName: madvise.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2022年02月22日 星期二 10时01分59秒
 ************************************************************************/
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <time.h>
#include <sys/time.h>

#define FILE_LEGNTH (1000*1024*1024) /*200MB的文件*/

#define TIMER(val) do{\
    struct timeval tm;\
    gettimeofday(&tm, NULL);\
    val = tm.tv_sec * 1000 + tm.tv_usec/1000;\
} while(0)

//返回耗时
int doprocess(char *p)        
{
    //下面开始逐个字符读取
    long starttime, endtime;
    TIMER(starttime);
    int nSum = 0;
    int i;
    for (i=0; i<FILE_LEGNTH; i++) {
        nSum += *p;
        p++;
    }

    TIMER(endtime);
    return (endtime - starttime);
}
            
/// 创建一个大的空文件
void CreateFile(const char *filepath)
{
    int fd = open(filepath, O_CREAT | O_RDWR);
    if (fd<1) {
        perror("open error");
        return;
    }

    lseek(fd, FILE_LEGNTH-1, SEEK_SET);
    write(fd, "\0", 1);
    close(fd);
    fd = -1;
}

/// 顺序读
void SequenceRead(const char *filepath)
{
    int fd = open(filepath, O_RDWR | O_EXCL);
    if (-1==fd) { 
        perror("open error");
        return;
    }

    void* p = mmap(NULL, FILE_LEGNTH, PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED==p) {
        perror("map error");
        return;
    }

    close(fd);
    fd = -1;
        
    //内存使用建议
    if (-1==madvise(p, FILE_LEGNTH, MADV_WILLNEED | MADV_SEQUENTIAL)) {
        perror("madvise error");
        return;
    }
        
    //下面开始逐个字符读取
    int interval = doprocess((char *)p);
    printf("sequence read time: %d\n", interval);

    if (-1==munmap(p, FILE_LEGNTH)) {
        perror("unmap error");
        return;
    }
}

/// 随机读
void RandomRead(const char *filepath)
{
    int fd = open(filepath, O_RDWR);
    if (-1==fd) {
        perror("open error");
        return;
    }
        
    // 建立匿名映射
    void* p = mmap(NULL, FILE_LEGNTH, PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED==p) {
        perror("map error");
        return;
    }
    close(fd);
    fd = -1;
        
    //内存使用建议
    if (-1==madvise(p, FILE_LEGNTH, MADV_DONTNEED)) { //MADV_RANDOM)) {
        perror("madvise error");
        return;
    }
      
    //下面开始逐个字符读取
    int interval = doprocess((char *)p);
    printf("random read time: %d\n", interval);
    if (-1==munmap(p, FILE_LEGNTH)) {
        perror("unmap error");
        return;
    }
}

/// without madvise
void WithMadviseRead(const char *filepath)
{
    int fd = open(filepath, O_RDWR);
    if (-1==fd) {
        perror("open error");
        return;
    }
        
    // 建立匿名映射
    void* p = mmap(NULL, FILE_LEGNTH, PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED==p) {
        perror("map error");
        return;
    }
    close(fd);
    fd = -1;
         
    //下面开始逐个字符读取
    int interval = doprocess((char *)p);
    printf("without madvise read time: %d\n", interval);
    if (-1==munmap(p, FILE_LEGNTH)) {
        perror("unmap error");
        return;
    }
}

int main()
{
    const char path[100] = "./aa";
    CreateFile(path);
    RandomRead(path);
    SequenceRead(path);
    WithMadviseRead(path);
    
    return 1;
}
