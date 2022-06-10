/*************************************************************************
> FileName: read_write_file.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月30日 星期一 21时23分57秒
 ************************************************************************/
#include <gio/gio.h>


int main (int argc, char* argv[])
{
    GFile* file = NULL;
    char* path = NULL;
    GFileInfo* fileinfo = NULL;
    char** fileattr = NULL;
    GFileIOStream* fileio = NULL;
    GInputStream* inputio = NULL;       // 无须释放
    GInputStream* outputio = NULL;      // 无须释放
    char* str = "hello world 读取与写入字符串例子";
    char buf[1024] = {0};

    file = g_file_new_for_path ("./read_write.txt");
    path = g_file_get_path (file);
    printf ("写入的文件路径:%s\n", path);
    g_free (path);

    // 需要进行文件类型检查
    if (g_file_query_exists(file, NULL)) {
        fileio = g_file_open_readwrite (file, NULL, NULL);
    } else {
        fileio = g_file_create_readwrite (file, G_FILE_CREATE_NONE, NULL, NULL);
    }

    // 写入文件
    outputio = g_io_stream_get_output_stream(fileio);
    g_output_stream_write (outputio, str, strlen(str), NULL, NULL);

    // outputio/inputio继承seekable
    printf ("outputio seek:%d\n", g_seekable_tell(outputio));

    // 读取文件
    inputio = g_io_stream_get_input_stream(fileio);
    printf ("inputio seek:%d\n", g_seekable_tell(inputio));
    g_seekable_seek (inputio, 0, G_SEEK_SET, NULL, NULL);           // 重置文件指针
    g_input_stream_read (inputio, buf, sizeof (buf), NULL, NULL);
    printf ("读取到:%s\n", buf);

    // 关闭流
    g_io_stream_close (fileio, NULL, NULL);

    // 文件信息
    char* attrvalue = NULL;
    printf ("\n============================ 文件信息 =========================\n");
    fileinfo = g_file_query_info (file, "*", G_FILE_QUERY_INFO_NONE, NULL, NULL);
    fileattr = g_file_info_list_attributes (fileinfo, NULL);
    for (int i = 0; fileattr[i] != NULL; ++i) {
        attrvalue = g_file_info_get_attribute_as_string (fileinfo, fileattr[i]);
        printf ("%s --- %s\n", fileattr[i], attrvalue);
        g_free (fileattr[i]);
        g_free (attrvalue);
    }
    g_free (fileattr);


    // 关闭资源
    g_object_unref (fileio);
    g_object_unref (file);

    return 0;
}
