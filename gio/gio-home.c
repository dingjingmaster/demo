/*************************************************************************
> FileName: gio-home.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年01月19日 星期二 19时57分35秒
 ************************************************************************/
#include <pwd.h>
#include <stdio.h>
#include <gio/gio.h>


int main (int argc, char* argv)
{
    struct passwd* pw = NULL;

    printf ("home: %s\n", g_get_home_dir());

    pw = getpwuid (getuid ());
    if (pw) {
        printf ("shell:%s | gid:%d | name: %s | passwd:%s | uid:%d | home dir:%s\n", pw->pw_shell, pw->pw_gid, pw->pw_name, pw->pw_passwd, pw->pw_uid, pw->pw_dir);
    }

    return 0;
}


