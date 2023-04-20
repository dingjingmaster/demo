/*************************************************************************
> FileName: getpwname.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 20 Apr 2023 03:45:59 PM CST
 ************************************************************************/
#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>

int main (int argc, char* argv[])
{
    struct passwd* pwd = getpwuid (getuid());

    printf("name    : %s\n", pwd->pw_name);
    printf("passwd  : %s\n", pwd->pw_passwd);

    return 0;
}
