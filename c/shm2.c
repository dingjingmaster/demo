/*************************************************************************
> FileName: shm2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年01月29日 星期五 14时33分34秒
 ************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
//#include <xenctrl.h>
//#include <xen/domctl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <signal.h>
//#include <xen/event_channel.h>
//#include <mysql/mysql.h>
//#include <libdis.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <xs.h>
#include <pthread.h>
#include <sys/sem.h>
pthread_mutex_t thread_over_mutex=PTHREAD_MUTEX_INITIALIZER;

static int set_semvalue(int sem_id, int val);
static void del_semvalue(int sem_id);
static int semaphore_p(int sem_id, int used);
static int semaphore_v(int sem_id);
static int get_semvalue(int sem_id);
union semun
{
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

static int sem_id_1;
static int sem_id_2;
struct sembuf sem_b;

static int set_semvalue(int sem_id, int val)
{
    union semun sem_union;

    sem_union.val = val;//-(THREAD_NUM-1);
    return semctl(sem_id, 0, SETVAL, sem_union);
}

static int get_semvalue(int sem_id)
{
    union semun sem_union;

    //sem_union.val = val;//-(THREAD_NUM-1);

    return semctl(sem_id, 0, GETVAL, sem_union);
}

static void del_semvalue(int sem_id)
{
    union semun sem_union;

    if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "Failed to delete semaphore/n");
}

static int semaphore_p(int sem_id, int used)
{

    sem_b.sem_num = 0;
    sem_b.sem_op = -1;
    sem_b.sem_flg = SEM_UNDO;
    printf("semaphore_p [%d %d]is begin\n", get_semvalue(sem_id),used);
    if(semop(sem_id, &sem_b, used) == -1)
    {
        fprintf(stderr, "semaphore_p failed/n");
        return 0;
    }
    printf("semaphore_p [%d]is over\n", get_semvalue(sem_id));

    return 1;
}


static int semaphore_v(int sem_id)
{
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;
    sem_b.sem_flg = SEM_UNDO;
    printf("semaphore_v [%d]is begin\n", get_semvalue(sem_id));
    if(semop(sem_id, &sem_b, 1) == -1)
    {
        fprintf(stderr, "semaphore_v failed/n");
        return 0;
    }
    printf("semaphore_v [%d]is over\n", get_semvalue(sem_id));
    return 1;
}

static int semaphore_wait(int sem_id)
{
    sem_b.sem_num = 0;
    sem_b.sem_op = 0;
    sem_b.sem_flg = SEM_UNDO;
    printf("I am waiting\n");
    printf("SEM is %d\n",get_semvalue(sem_id));
    if(semop(sem_id, &sem_b, 1) == -1)
    {
        fprintf(stderr, "semaphore_v failed/n");
        return 0;
    }
    printf("I am coming\n");
    return 1;
}

void * fun(void)
{

        semaphore_v(sem_id_2);
        semaphore_v(sem_id_2);

}

int main()
{
    sem_id_2 = semget((key_t)4321, 1, 0666 | IPC_CREAT);
    int r;
    if((r=set_semvalue(sem_id_2,0))<0 )
    {
        fprintf(stderr, "Failed to initialize semaphore%d\n",r);
    }

    printf("SEM2 is %d\n",get_semvalue(sem_id_2));
    pthread_t consume_tid;

    pthread_create(&consume_tid, NULL, fun, NULL);

    printf("main is waiting\n");
    semaphore_p(sem_id_2,2);
    printf("ALL is over\n");
    pthread_join(consume_tid, NULL);
    return 1;
}

