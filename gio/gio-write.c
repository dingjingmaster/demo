/*************************************************************************
> FileName: gio-write.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年04月20日 星期二 15时01分10秒
 ************************************************************************/
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    GError*     error = NULL;

    GFile* writeFile = g_file_new_for_path ("./test.txt.run");
    if (NULL == writeFile) {
        return -1;
    }

    GFileOutputStream* fw = g_file_create (writeFile, G_FILE_CREATE_REPLACE_DESTINATION, NULL, &error);
    if (NULL != error) {
        printf ("error:%s\n", error->message);
        return -1;
    }

    gssize size = g_output_stream_write (fw, "123456789", 9, NULL, &error);
    if (NULL != error) {
        printf ("error:%s\n", error->message);
        return -1;
    }

    gboolean ret = g_output_stream_flush (fw, NULL, &error);
    if (NULL != error) {
        printf ("error:%s\n", error->message);
        return -1;
    }

    return 0;
}
