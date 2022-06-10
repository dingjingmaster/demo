/*************************************************************************
> FileName: demo8.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月20日 星期五 09时49分25秒
 ************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

typedef void (*p) (char*);
// 基于可变参的回调实现信号原理

// 写入事件
void file_write (int fd, const char* buffer, ...)
{
    if (NULL == buffer) return;

    int index = 0;
    void* ptr = NULL;
    va_list args;
    p func = NULL;

    write (fd, buffer, strlen(buffer));

    va_start(args, buffer);
    while (1) {
        ptr = va_arg (args, void*);
        if (NULL == ptr) break;
        switch (index % 2) {
        case 0:
            func = ptr;
            break;
        case 1:
            func ((char*)ptr);
            break;
        }

        ++ index;
    }
    va_end (args);
}

void file_print1 (char* buffer)
{
    printf ("======\n%s\n======\n", buffer);
}

void file_print2 (char* buffer)
{
    printf ("======\n%s\n======\n", buffer);
}

int main (void)
{
    file_write (1, "测试file_write\n", file_print1, "file_print1", file_print2, "file_print2", NULL);
}
