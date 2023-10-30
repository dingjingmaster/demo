/*************************************************************************
> FileName: file-list.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 30 Oct 2023 02:56:55 PM CST
 ************************************************************************/

#include <stdio.h>
#include <magic.h>

int main (int argc, char* argv[])
{
    struct magic_set* magic = magic_open(MAGIC_CHECK);
    if (NULL == magic) {
        printf ("magic open error\n");
        return -1;
    }

    //magic_load (magic, NULL);
    magic_list (magic, NULL);

    if (magic) {
        magic_close(magic);
    }

    (void) argc;
    (void) argv;

    return 0;
}
