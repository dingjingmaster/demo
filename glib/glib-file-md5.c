/*************************************************************************
> FileName: glib-file-md5.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 21 Dec 2023 09:50:58 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("输入格式：<命令> file\n");
        return 1;
    }

    if (!g_str_has_prefix (argv[1], "/")) {
        printf ("请使用绝对路径\n");
        return 1;
    }

    char buf[4096] = {0};
    gsize readCount = 0;
    const char* res = NULL;
    GChecksum* md5sum = NULL;
    GFileInputStream* fRead = NULL;
    GFile* file = g_file_new_for_path(argv[1]);
    if (!G_IS_FILE(file) || !g_file_query_exists(file, NULL)) {
        printf ("文件不存在\n");
        goto error;
    }

    fRead = g_file_read(file, NULL, NULL);
    if (!fRead) {goto error;}

    md5sum = g_checksum_new (G_CHECKSUM_MD5);
    while ((readCount = g_input_stream_read(G_INPUT_STREAM(fRead), buf, sizeof(buf), NULL, NULL)) > 0) {
        g_checksum_update(md5sum, buf, readCount);
    }


    if (readCount < 0) {
        goto error;
    }

    res = g_checksum_get_string (md5sum);
    if (res) {
        printf ("%s\n", res);
    }

out:
    if (fRead) {g_object_unref(fRead);}
    if (file)  {g_object_unref(file);}
    if (md5sum) { g_checksum_free (md5sum); }
    return 0;

error:
    if (fRead) {g_object_unref(fRead);}
    if (file)  {g_object_unref(file);}
    if (md5sum) { g_checksum_free (md5sum); }
    return -1;
}
