/*************************************************************************
> FileName: stat.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 20 Oct 2022 03:43:19 PM CST
 ************************************************************************/
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("请输入文件名\n");
        return -1;
    }

    struct stat sb;

    if (stat(argv[1], &sb) == -1) {
        return -1;
    }

    printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));

    struct tm* mtim = localtime(&sb.st_mtime);
    char buf[32] = {0};

    snprintf (buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d", 1900 + mtim->tm_year, 1 + mtim->tm_mon, mtim->tm_mday, mtim->tm_hour, mtim->tm_min, mtim->tm_sec);

    printf("Last file modification:   %s\n", buf);
}
