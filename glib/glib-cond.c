/*************************************************************************
> FileName: glib-cond.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 15时23分41秒
 ************************************************************************/

#include <glib.h>

GMutex *mutex = NULL;
GCond* cond = NULL;
int storage = -1;

gboolean producer_thread_end = FALSE;
gboolean consumer_thread_end = FALSE;

// 生產者透過此函式設定產品
void produce_product(int product) 
{
    g_mutex_trylock(mutex);

    if(storage != -1) { 
        // 目前沒有空間收產品，請稍候！
        g_cond_wait(cond, mutex);
    } 

    storage = product; 
    g_print("生產者設定 %d\n", storage);

    // 喚醒消費者可以繼續工作了
    g_cond_signal(cond);

    g_mutex_unlock(mutex);
}

// 消費者透過此函式取走產品
int consume_product() 
{
    g_mutex_trylock(mutex);

    if(storage == -1) { 
        // 缺貨了，請稍候！
        g_cond_wait(cond, mutex);
    } 

    int p = storage; 
    g_print("消費者取走 %d\n", storage);

    storage = -1; 

    // 喚醒生產者可以繼續工作了
    g_cond_signal(cond);
    
    g_mutex_unlock(mutex);

    return p;
}

// 生產者執行緒會執行此函式
gpointer producer_thread(gpointer data) 
{
    int i;
    for(i = 1; i <= 10; i++) {
        g_usleep(rand());
        produce_product(i);
    }
    producer_thread_end = TRUE;
}

// 消費者執行緒會執行此函式
gpointer consumer_thread(gpointer data) 
{
    int i;
    for(i = 1; i <= 10; i++) {
        g_usleep(rand());
        consume_product();
    }
    consumer_thread_end = TRUE;
}

gpointer checking_thread(gpointer mloop) 
{
    while(TRUE) {
        if(producer_thread_end && consumer_thread_end) {
            g_main_loop_quit(mloop);
            break;
        }
        g_usleep(1000);
    }
}

int main(int argc, char *argv[]) 
{
    GMainLoop *mloop;

    if(!g_thread_supported()) {
        g_thread_init(NULL);
    }

    mloop = g_main_loop_new(NULL, FALSE);
    mutex = g_mutex_new();

    cond = g_cond_new();

    g_thread_create(producer_thread, NULL, FALSE, NULL);
    g_thread_create(consumer_thread, NULL, FALSE, NULL);
    g_thread_create(checking_thread, mloop, FALSE, NULL);

    g_main_loop_run(mloop);

    return 0;
}
