/*************************************************************************
> FileName: sem1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年01月29日 星期五 17时33分58秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <sys/sem.h>//包含信号量定义的头文件  
  
//联合类型semun定义  
union semun
{  
    int val;  
    struct semid_ds *buf;  
    unsigned short *array;  
};  
  
//函数声明  
//函数：设置信号量的值  
static int set_semvalue(void);  
//函数：删除信号量  
static void del_semvalue(void);  
//函数：信号量P操作  
static int semaphore_p(void);  
//函数：信号量V操作  
static int semaphore_v(void);  
  
static int sem_id;  //信号量ID  
  
  
int main(int argc,char *argv[])  
{  
    int i;  
    int pause_time;  
    char op_char = 'O';  
  
    srand( (unsigned int)getpid() );  
  
    // 创建一个新的信号量或者是取得一个已有信号量的键  
    sem_id = semget( (key_t)1234, 1, 0666 | IPC_CREAT );  
  
    // 如果参数数量大于1，则这个程序负责创建信号和删除信号量  
    if( argc > 1 )  
    {
        if( !set_semvalue() )  
        {  
            fprintf( stderr, "failed to initialize semaphore\n" );
            exit( EXIT_FAILURE );
        }
  
        op_char = 'X';  //对进程进行标记
        sleep(5);  
    }  
  
    //循环：访问临界区  
    for( i = 0; i < 10; ++i )
    {  
        // P操作，尝试进入临界区
        if( !semaphore_p() )
            exit( EXIT_FAILURE );
        printf( "%c", op_char );  
        // 刷新标准输出缓冲区，把输出缓冲区里的东西打印到标准输出设备上
        fflush( stdout );
        
        pause_time = rand() % 3;
        sleep( pause_time );
  
        printf( "%c", op_char );
        fflush( stdout );
        
        // V操作，尝试离开临界区
        if( !semaphore_v() )  
            exit( EXIT_FAILURE );  
        pause_time = rand() % 2;  
        sleep( pause_time );
    }  
  
    printf( "\n %d - finished \n", getpid() );  
  
    if ( argc > 1 )  
    {  
        sleep( 10 );  
        del_semvalue();//删除信号量  
    }
    return 0;
}  
  
  
//函数：设置信号量的值  
static int set_semvalue(void)  
{  
    union semun sem_union;  
    sem_union.val = 1;  
  
    if ( semctl( sem_id, 0, SETVAL, sem_union ) )  
        return 0;
  
    return 1;
}  
  
//函数：删除信号量  
static void del_semvalue(void)  
{  
    union semun sem_union;  
  
    if( semctl( sem_id, 0, IPC_RMID, sem_union ) )
        fprintf( stderr, "Failed to delete semaphore\n" );
}
  
//函数：信号量P操作：对信号量进行减一操作  
static int semaphore_p(void)  
{  
    struct sembuf sem_b;  
  
    sem_b.sem_num = 0;  //信号量编号  
    sem_b.sem_op = -1;  //P操作   
    sem_b.sem_flg = SEM_UNDO;  
  
    if( semop( sem_id, &sem_b, 1 ) == -1 )  
    {  
        fprintf( stderr, "semaphore_p failed\n" );  
        return 0;
    }
  
    return 1;  
}
  
//函数：信号量V操作：对信号量进行加一操作  
static int semaphore_v(void)  
{  
    struct sembuf sem_b;  
  
    sem_b.sem_num = 0;  //信号量编号  
    sem_b.sem_op = 1;   //V操作    
    sem_b.sem_flg = SEM_UNDO;  
  
    if( semop( sem_id, &sem_b, 1 ) == -1 )  
    {  
        fprintf( stderr, "semaphore_v failed\n" );  
        return 0;
    }
  
    return 1;
}  
