/*************************************************************************
> FileName: pthread-lock.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 09 Jan 2023 02:37:12 PM CST
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>


int main (int argc, char* argv[])
{

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    for (int i = 0; i < 1000; ++i) {
        //pthread_mutex_lock(&mutex);
        while (pthread_mutex_trylock(&mutex));

        printf ("%d\n", i);

        pthread_mutex_unlock (&mutex);
    }

    pthread_mutex_destroy (&mutex);

    return 0;
}
