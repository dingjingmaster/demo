/*************************************************************************
> FileName: comandline.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年11月04日 星期四 20时06分51秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>

void ff (const char* aa)
{
    printf ("%s\n", aa);
}

int main (int argc, char* argv[]) 
{
    ff ("aaa" "bbb" "ccc");

    return 0;
}
