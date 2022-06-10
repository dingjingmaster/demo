/*************************************************************************
> FileName: gio-copy.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年09月18日 星期五 17时27分52秒
 ************************************************************************/

#include <stdio.h>
#include <gio/gio.h>


int main (int argc, char* argv)
{
    GError* error = NULL;
    GFile* fileSrc = g_file_new_for_path ("./aa");
    GFile* fileDest = g_file_new_for_path ("./a");
    if (NULL == fileSrc || NULL == fileDest) {
        printf ("error!\n");
        return 0;
    }

    g_file_copy (fileSrc, fileDest,
            G_FILE_COPY_NOFOLLOW_SYMLINKS | G_FILE_COPY_ALL_METADATA,
                NULL, NULL, NULL, &error);
    if (NULL != error) {
        printf ("error:%s\n", error->message);
    }

    printf ("ok!\n");

    return 0;
}

