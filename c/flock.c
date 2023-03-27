/*************************************************************************
> FileName: flock.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 27 Mar 2023 09:28:30 AM CST
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/file.h>

static FILE* fl = NULL;
static const char* lockFile1 = "lock-file.lock";

bool sqlite_lock()
{
    if (!fl) {
        fl = fopen (lockFile1, "a+");
        if (!fl) {
            printf("fopen error!\n");
            exit(-1);
        }
    }

    if (fl) {
        if (flock (fl->_fileno, LOCK_EX | LOCK_NB) == 0) {
            printf("locked\n");
            return true;
        }
    }

    return false;
}


bool sqlite_unlock()
{
    if (fl) {
        if (flock (fl->_fileno, LOCK_UN) == 0) {
            printf("unlocked\n");
            return true;
        }
    } else {
        return true;
    }

    return false;
}

int main (int argc, char* argv[]) 
{
    while (1) {
        printf("start lock!\n");
        while(!sqlite_lock());
        printf ("加锁成功！！！！\n");
        sleep (10);
        printf ("加锁成功！！！！\n");
        while(!sqlite_unlock());
        printf("stop lock!\n");
        sleep(5);
    }

    return 0;
}
