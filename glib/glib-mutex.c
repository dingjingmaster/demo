/*************************************************************************
> FileName: glib-mutex.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 15时04分50秒
 ************************************************************************/

#include <glib.h>

#define MUTEX 1

struct _User 
{
GString *name;
GString *id;
glong count;    
};
typedef struct _User User;

#if MUTEX
GMutex mutex;
#endif

void user_set_name_id(User *user, GString *name, GString *id) 
{
#if MUTEX
    if(g_mutex_trylock(&mutex)) {
        user->name = name;
        user->id = id;
        if(!user_check_name_id(user)) {
            g_print("%d: illegal name or id....\n", user->count);
        }
        user->count++;
        g_mutex_unlock(&mutex);
    } else {
        g_usleep(1000);
    }
#else
    user->name = name;
    user->id = id;
    if(!user_check_name_id(user)) {
        g_print("%d: illegal name or id....\n", user->count);
    }
    user->count++;
#endif
}

gboolean user_check_name_id(User *user) 
{
    return user->name->str[0] == user->id->str[0];
}

gpointer thread1(gpointer user) 
{
    GString *name = g_string_new("Justin Lin");
    GString *id = g_string_new("J.L.");
    while(TRUE) {
        user_set_name_id(user, name, id);
    }
}

gpointer thread2(gpointer user) 
{
    GString *name = g_string_new("Shang Hwang");
    GString *id = g_string_new("S.H.");
    while(TRUE) {
        user_set_name_id(user, name, id);
    }
}


int main(int argc, char *argv[]) 
{
    GMainLoop *mloop;
            
    if(!g_thread_supported()) {
        g_thread_init(NULL);
    }
                
    User user;

    mloop = g_main_loop_new(NULL, FALSE);

#if MUTEX
    g_mutex_init (&mutex);
#endif

    g_thread_create(thread1, &user, FALSE, NULL);
    g_thread_create(thread2, &user, FALSE, NULL);
                                    
    g_main_loop_run(mloop);
                                        
    return 0;
}
