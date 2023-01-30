/*************************************************************************
> FileName: locker.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 17 Jan 2023 09:20:08 AM CST
 ************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/file.h>

#define RAM_DISK_DB_FILE_LOCK           "/dev/shm/.est.db.lock"
#define DISK_DB_FILE_LOCK               "/usr/local/ultrasec/dat/.est.db.lock"

const char* lockFile = NULL;

void init_lock_file ()
{
    if (!access (RAM_DISK_DB_FILE_LOCK, F_OK)) {
        lockFile = RAM_DISK_DB_FILE_LOCK;
    } else if (!access(DISK_DB_FILE_LOCK, F_OK)) {
        lockFile = DISK_DB_FILE_LOCK;
    } else {
        printf("lock file not exists!");
        exit (-1);
    }
}

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("参数不对!输入1则上锁，输入0则解锁\n");
        return -1;
    }

    bool flag = (1 == atoi(argv[1]));

    init_lock_file();

    FILE* f = NULL;

    if (!lockFile) {
        printf ("lock file not exists\n");
        return -1;
    }

    if (!f) {
        f = fopen (lockFile, "a+");
        if (!f) {
            printf ("lock file: '%s' error\n", lockFile);
            return -1;
        }
    }

    if (flag && (flock (f->_fileno, LOCK_EX | LOCK_NB) == 0)) {
        printf ("'%s' locked!\n", lockFile);
        return 0;
    } else if (!flag && (flock (f->_fileno, LOCK_UN) == 0)) {
        printf ("'%s' unlocked!\n", lockFile);
        return 0;
    }

    return 0;
}
