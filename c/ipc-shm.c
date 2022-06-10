/*************************************************************************
> FileName: ipc-shm.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年03月16日 星期二 15时59分36秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<string.h>
#include<errno.h>

typedef struct _Teacher
{
    char name[64];
    int age;
} Teacher;

int main(int argc, char *argv[])
{
    int ret = 0;
    int    shmid;
    //创建共享内存 ，相当于打开文件，文件不存在则创建
    shmid = shmget(0x2234, sizeof(Teacher), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget err");
        return errno;
    }
    printf("shmid:%d \n", shmid);
    Teacher *p = NULL;
    //将共享内存段连接到进程地址空间
    p = shmat(shmid, NULL, 0);//第二个参数shmaddr为NULL，核心自动选择一个地址
    if (p == (void *) - 1) {
        perror("shmget err");
        return errno;
    }
    strcpy(p->name, "aaaa");
    p->age = 33;
    //将共享内存段与当前进程脱离
    shmdt(p);

    printf("键入1 删除共享内存，其他不删除\n");

    int num;
    scanf("%d", &num);
    if (num == 1) {
        //用于控制共享内存
        ret = shmctl(shmid, IPC_RMID, NULL);//IPC_RMID为删除内存段
        if (ret < 0) {
            perror("rmerrr\n");
        }
    }

    return 0;
}
