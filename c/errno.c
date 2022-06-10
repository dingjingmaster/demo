/*************************************************************************
> FileName: errno.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年05月08日 星期五 10时24分09秒
 ************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main (int argc, char* argv[])
{
    errno = 0;

    FILE* fp = fopen ("newName", "r");
    if (0 != errno) {
        char* err = strerror(errno);
        printf ("errno: %d\n", errno);
        if (err) printf ("error p: %p\nerror: %s\n", err, err);
    }

    return 0;
}
