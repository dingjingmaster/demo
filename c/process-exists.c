/*************************************************************************
> FileName: process-exists.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 14 Oct 2022 09:05:13 AM CST
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief 检测进程是否存在
 * @return 0 表示存在，其它表示不存在
 */
int check_exist (const char* process)
{
    FILE*           fp = NULL;
    char            command[120] = {0};
    char            pidBuf[12] = {0};

    snprintf (command, sizeof(command) - 1, "pidof %s", process);

    fp = popen(command, "r");
    if (fp) {
        fgets (pidBuf, sizeof(pidBuf) - 1, fp);
        printf ("popen: %s\n", pidBuf);
        pclose(fp);

        if (strlen(pidBuf) > 1) {
            return 0;
        }
    }

    return -1;
}

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("请输入进程名字\n");
        return 0;
    }

    printf ("process: '%s' %s\n", argv[1], (!check_exist(argv[1]) ? "exist" : "not exists"));

    return 0;
}
