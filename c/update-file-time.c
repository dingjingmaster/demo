/*************************************************************************
> FileName: update-file-time.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年06月16日 星期二 09时45分16秒
 ************************************************************************/
#include <time.h>
#include <utime.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main (int argc, char* argv[])
{
    if (2 < argc) {
        printf ("使用:\n\tupdate-file-time.run file\n");
        exit(0);
    }

    struct tm t;
    char* tim = "2020-06-16 10:00:00";

    memset (&t, 0, sizeof (struct tm));

#if 0
    strptime (tim, "%Y-%m-%d %H:%M:%S", &t);

    time_t ntim = mktime(&t);
    struct utimbuf ntime = {ntim, ntim};
#else
    struct utimbuf ntime = {time(NULL), time(NULL)};

#endif

    printf ("%d\n", utime (argv[1], &ntime));


    return 0;
}
