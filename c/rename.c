/*************************************************************************
> FileName: rename.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年05月08日 星期五 10时24分09秒
 ************************************************************************/
#include <stdio.h>
#include <sys/stat.h>


int main (int argc, char* argv[])
{
    char*           oldName = "rename_dir";
    char*           newName = "rename_dir_new";

    mkdir(oldName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    rename (oldName, newName);


    return 0;
}
