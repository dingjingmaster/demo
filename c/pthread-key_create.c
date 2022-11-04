/*************************************************************************
> FileName: pthread-key_create.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 04 Nov 2022 01:53:15 PM CST
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_key_t key;

struct test_struct
{
    int     i;
    float   k;
};

void* child1 (void* arg)
{
    struct test_struct d1 = {
        .i = 10,
        .k = 3.1415,
    };

    pthread_setspecific(key, &d1);

    printf ("结构体的地址: 0X%p\n", &d1);
    printf ("child1 中 pthread_getspecific(key)返回指针为: 0X%p\n", (struct test_struct*)pthread_getspecific(key));

    return NULL;
}

void* child2 (void* arg)
{
    int tmp = 20;

    sleep(2);

    pthread_setspecific(key, &tmp);

    printf ("结构体的地址: 0X%p\n", &tmp);
    printf ("child2 中 pthread_getspecific(key)返回指针为: 0X%p\n", (struct test_struct*)pthread_getspecific(key));

    return NULL;
}

int main (void)
{
    pthread_t t1, t2;

    pthread_key_create (&key, NULL);

    pthread_create(&t1, NULL, child1, NULL);
    pthread_create(&t2, NULL, child2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_key_delete(key);

    return 0;
}
