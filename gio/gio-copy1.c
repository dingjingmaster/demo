/*************************************************************************
> FileName: gio-copy1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年09月18日 星期五 17时27分52秒
 ************************************************************************/

#include <glib.h>
#include <stdio.h>
#include <gio/gio.h>


int main (int argc, char* argv[])
{
    if (argc < 3) {
        printf ("请输入要复制的文件和目的地， 比如: gio-copy.run src-path dest-path(目标文件名字)\n");
        return -1;
    }

    g_autofree char* srcF = NULL;
    g_autofree char* destF = NULL;

    if (g_str_has_prefix(argv[1], "/")) {
        srcF = g_strdup (argv[1]);
    } else {
        srcF = g_canonicalize_filename(argv[1], NULL);
    }

    if (g_str_has_prefix(argv[2], "/")) {
        destF = g_strdup (argv[2]);
    } else {
        destF = g_canonicalize_filename(argv[2], NULL);
    }

    printf ("src: %s\ndest: %s\n", srcF, destF);

    GError* error = NULL;
    GFile* fileSrc = g_file_new_for_path (srcF);
    char* dstFile = g_strdup(destF);
    if (g_file_test(destF, G_FILE_TEST_IS_DIR)) {
        g_autofree char* name = g_file_get_basename(fileSrc);
        if (name) {
            if (dstFile) { g_free (dstFile); }
            dstFile = g_strdup_printf("%s/%s", destF, name);
            printf("dst path: %s\nname: %s\n", dstFile, name);
        }
    }
    GFile* fileDest = g_file_new_for_path (dstFile);

    if (!G_IS_FILE(fileSrc) || !G_IS_FILE(fileDest)) {
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

