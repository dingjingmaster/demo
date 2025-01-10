/*************************************************************************
> FileName: time.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年05月08日 星期五 11时20分52秒
 ************************************************************************/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[])
{
    time_t          curtime;
    struct tm*      curt;
 
    time(&curtime);
    curt = localtime(&curtime);

    printf("%2d%02d%02d%02d%02d\n", 1900 + curt->tm_year, curt->tm_mon + 1, curt->tm_mday, curt->tm_hour, curt->tm_min);

    return 0;
}
