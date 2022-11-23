/*************************************************************************
> FileName: gio-dir-file-num-limit.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 08 Nov 2022 11:13:20 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <stdbool.h>

bool dir_num_limit (const char* parent, int limitSize);

int main (int argc, char* argv[])
{
    if (argc < 3) {
        printf ("命令行格式:\ngio-dir-file-num-limit.run <限制数量> <路径1> <路径2> ...");
        return -1;
    }

    bool ret = false;
    for (int i = 2; i < argc; ++i) {
        //dir_num_limit(argv[i], atoi(argv[1]));
    }

    return 0;
}

bool dir_num_limit (const char* parent, int limitSize)
{
}
