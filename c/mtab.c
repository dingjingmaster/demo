/*************************************************************************
> FileName: mtab.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 08 Jul 2024 04:27:03 PM CST
 ************************************************************************/
#include <stdio.h>
#include <mntent.h>

int main (void)
{
    struct mntent* ent = NULL;

    FILE* mtab = setmntent("/etc/mtab", "r");
    if (NULL == mtab) {
        printf ("error1\n");
        return -1;
    }

    while ((ent = getmntent(mtab)) != NULL) {
        printf ("================>\n"
                "mnt_fsname: %s\n"
                "mnt_dir: %s\n"
                "mnt_type: %s\n"
                "mnt_opts: %s\n\n\n", ent->mnt_fsname, ent->mnt_dir, ent->mnt_type, ent->mnt_opts);
    }
    endmntent(mtab);

    return 0;
}
