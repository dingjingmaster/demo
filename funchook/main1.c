/*************************************************************************
> FileName: main1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月22日 星期四 09时05分26秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "funchook/include/funchook.h"


typedef int (*Add) (int a, int b);

int add(int a, int b)
{
    return a + b;
}

int hook_add(int a, int b)
{
    return -1;
}

void* thread_handle(void* data)
{
    while (1) {
        printf ("%d + %d = %d\n", 1, 2, add(1, 2));
        sleep(1);
    }

    return NULL;
}

int main (int argc, char* argv[])
{
    Add t = add;
    pthread_t threadID;

    pthread_create(&threadID, NULL, thread_handle, NULL);

    sleep(5);

    printf("start Hook!\n");

    funchook_t* funchook = funchook_create();
    if (!funchook) {
        printf("funchook_create error!\n");
        return 1;
    }

    int ret = funchook_prepare(funchook, (void**) &t, hook_add);
    if (ret != 0) {
        printf("funchook_prepare error!\n");
        return 2;
    }
    // 其它 prepare

    ret = funchook_install(funchook, 0);
    if (0 != ret) {
        printf("funchook_install error!\n");
        return 3;
    }

    sleep(10);

    printf ("Stop Hook!\n");

    // uninstall
    ret = funchook_uninstall(funchook, 0);
    if (0 != ret) {
        printf("funchook_uninstall error!\n");
        return 4;
    }

    ret = funchook_destroy(funchook);
    if (0 != ret) {
        printf("funchook_destroy error!\n");
        return 5;
    }

    pthread_join(threadID, NULL);

    return 0;
}
