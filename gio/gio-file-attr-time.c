/*************************************************************************
> FileName: gio-file-attr-time.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年06月15日 星期一 16时00分33秒
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <gio/gio.h>
#include <sys/stat.h>


int main (int argc, int argv[])
{
    GError* err = NULL;
    GFile* gfile = g_file_new_for_path ("./gio-file-attr-time.c");
    GFileInfo* fileInfo = g_file_query_info (gfile, "*", G_FILE_QUERY_INFO_NONE, NULL, &err);
    if (NULL != err) {
        printf ("error: %s", err->message);
    }

    guint64 acc = g_file_info_get_attribute_uint64 (fileInfo, G_FILE_ATTRIBUTE_TIME_ACCESS);
    guint64 mod = g_file_info_get_attribute_uint64 (fileInfo, G_FILE_ATTRIBUTE_TIME_MODIFIED);


    // 输出
    //printf ("最新访问<time::access>: %d\n", acc);
    //printf ("最新修改<time::modified>: %d\n", mod);

    // 系统调用获取文件信息
    //printf ("\n系统调用获取文件信息\n");
    struct stat statbuf;
    int fd = open ("./gio-file-attr-time.c", O_RDONLY);
    // ...
    if (0 == fstat(fd, &statbuf)) {
        struct timespec modifyTime = statbuf.st_mtim;
        struct timespec accessTime = statbuf.st_atim;

        printf ("最新访问: %d\n", accessTime.tv_sec);
        printf ("最新修改: %d\n", modifyTime.tv_sec);


    }



    return 0;
}




