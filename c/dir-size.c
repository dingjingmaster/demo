/*************************************************************************
> FileName: dir-size.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月27日 星期四 11时25分00秒
 ************************************************************************/
#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>

int print_info (const char* fpath, const struct stat* sb, int tflag, struct FTW *ftwbuf)
{

    printf("%-3s %2d ",
                  (tflag == FTW_D) ?   "d"   : (tflag == FTW_DNR) ? "dnr" :
                  (tflag == FTW_DP) ?  "dp"  : (tflag == FTW_F) ?   "f" :
                  (tflag == FTW_NS) ?  "ns"  : (tflag == FTW_SL) ?  "sl" :
                  (tflag == FTW_SLN) ? "sln" : "???",
                  ftwbuf->level);

    if (FTW_F == tflag) {
        printf("[FILE] %s size = %ld bytes\n", fpath, sb->st_size);
    }
    else if (FTW_D == tflag) {
        printf("[DIR ] %s\n", fpath);
    }

    return 0;
}

int main (int argc, char* argv[])
{
    const char* path = (argc > 1) ? argv[1] : ".";

    nftw(path, print_info, 20, 0);

    return 0;
}
