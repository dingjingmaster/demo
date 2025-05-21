/*************************************************************************
> FileName: pthread-self.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月21日 星期三 15时31分21秒
 ************************************************************************/
#include <stdio.h>
#include <pthread.h>

int main ()
{
    printf ("pthread_self: %lu\n", pthread_self());
}
