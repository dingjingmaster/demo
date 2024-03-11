/*************************************************************************
> FileName: ring-buffer1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 11 Mar 2024 02:53:08 PM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define RING_SIZE   10

/**
 * @brief
 *  无锁环形缓存区
 *  无锁环形区要求：
 *      1. 必须只有1个线程执行写入操作
 *      2. 必须只有1个线程执行读取操作
 */
typedef struct _RingNode RingNode;

struct _RingNode
{
    int                 idx;
    int                 data;
    RingNode*           next;
};

typedef struct _RingNoLocker
{
    int size;
    RingNode* ring; // 保存数据的环形缓存区
    struct Prod     // 生产者，写入者
    {
        RingNode* head;
        RingNode* tail;
    } prod;

    struct Cons     // 消费者，读取者
    {
        RingNode* head;
        RingNode* tail;
    } cons;

} RingNoLocker;

void* prod_func (void* data);
void* cons_func (void* data);

int main ()
{
    RingNoLocker rl;

    // 分配环形内存，初始化无锁环形区数据
    rl.size = RING_SIZE;
    rl.ring = NULL;

    RingNode* last = NULL;
    for (int i = 0; i < RING_SIZE; ++i) {
        RingNode* p = (RingNode*) malloc (sizeof (RingNode));
        p->data = 0;
        p->idx = i;
        if (0 == i) {
            last = p;
        }
        p->next = rl.ring;
        rl.ring = p;
    }
    last->next = rl.ring;

#if 0
    // 用来检测是否把单链表形成环
    for (RingNode* i = rl.ring; ; i = i->next) {
        printf ("idx: %-2d, data: %d\n", i->idx, i->data);
        sleep(1);
    }
#endif

    rl.prod.head = rl.ring;
    rl.prod.tail = rl.ring;

    rl.cons.head = rl.ring;
    rl.cons.tail = rl.ring;

#if 0
    // 生产者头部
    for (RingNode* i = rl.prod.head; ; i = i->next) {
        printf ("idx: %-2d, data: %d\n", i->idx, i->data);
        sleep(1);
    }
#endif

#if 0
    // 消费者头部
    for (RingNode* i = rl.cons.head; ; i = i->next) {
        printf ("idx: %-2d, data: %d\n", i->idx, i->data);
        sleep(1);
    }
#endif

    // 生产者线程
    pthread_t prodT;
    pthread_create(&prodT, NULL, prod_func, &rl);


    // 消费者线程
    pthread_t consT;
    pthread_create(&consT, NULL, cons_func, &rl);

    pthread_join(prodT, NULL);
    pthread_join(consT, NULL);

    return 0;
}

// 生产者 头入
// 消费者 尾出

// 生产者
void* prod_func (void* data)
{
    RingNoLocker* rl = (RingNoLocker*) data;

    printf("[生产者] IN\n");

    int i = 0;
    while (1) {
        // 缓存生产者的头索引和消费者的尾索引
        RingNode* pH = rl->prod.head;       // 生产者头部
        RingNode* cT = rl->cons.tail;       // 消费者尾部

        // 生产者next指针指向下一个对象
        RingNode* insertNext = pH->next;
        if (insertNext == cT) {
            //printf ("\033[1;31m[生产者] 队列已满，生产者无法继续\033[0m\n");
            sleep(1);
            continue;
        }
        pH->next = insertNext;

        // 入队
        insertNext->data = ++i;
        printf ("\033[1;31m[生产者] 插入数据 %d\033[0m\n", insertNext->data);

        rl->prod.head = insertNext;
        rl->prod.tail = insertNext;

        sleep(random() % 3 + 1);
    }

    return NULL;
}


void* cons_func (void* data)
{
    RingNoLocker* rl = (RingNoLocker*) data;

    printf ("[消费者] IN\n");
    while (1) {
        // 缓存消费者的头索引和生产者的尾部索引
        RingNode* cH = rl->cons.head;       // 消费者头部
        RingNode* pT = rl->prod.tail;       // 生产者尾部

        // 消费者next指向下一个位置
        RingNode* readNext = cH->next;
        if (readNext == pT) {
            //printf ("\033[1;32m[消费者] 队列为空，消费者无法继续\033[0m\n");
            sleep(1);
            continue;
        }

        cH->next = readNext;

        // 出队
        printf ("\033[1;32m[消费者] 读取数据 %d\033[0m\n", cH->data);

        rl->cons.head = readNext;
        rl->cons.tail = readNext;
    }

    return NULL;
}
