/*************************************************************************
> FileName: gio-gfile.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月13日 星期五 15时03分32秒
 ************************************************************************/
#include <gio/gio.h>
#include <stdio.h>

int main (void)
{
    GFile* file = NULL;
    GFile* file1 = NULL;
    GFile* file2 = NULL;
    GFile* file3 = NULL;
    char* file4 = NULL;
    GFileOutputStream* file_output = NULL;

    // 创建文件 g_file_new_for_uri /comdline/tmp/
    file = g_file_new_for_path ("test.test");

    file1 = g_file_parse_name ("/data/code/a/b/a.txt");
    file2 = g_file_parse_name ("/data/code/a/哈哈/a.txt");

    // 检查两个 GFile 是否是一个引用
    g_file_equal (file, file1);

    // g_file_get_basename  == a.txt
    printf ("file1's (/data/code/a/b/a.txt) base name:%s\n", g_file_get_basename(file1));

    // 输出完整路径/绝对路径
    printf ("file(test.test):%s\n", g_file_get_path(file));

    // 同上，但是结果会缓存
    //printf ("%s\n", g_file_peek_path(file)); 低版本不支持

    // uri格式的，本地路径是 file://+绝对路径
    printf ("%s\n", g_file_get_uri(file));

    // 解析utf8格式的路径
    printf ("%s\n", g_file_get_parse_name(file1));

    // 父路径，文件的父路径就是本文件夹内
    file3 = g_file_get_parent(file);
    printf ("%s\n", g_file_get_uri(file3));

    // 路径、父路径
    printf ("%d\n", g_file_has_parent (file, file3));
    printf ("%d\n", g_file_has_parent (file3, file));

    // g_file_get_child g_file_get_child 父路径、文件
    printf ("%s\n", g_file_get_uri(g_file_get_child(file1, "a")));

    // g_file_get_child_for_display_name 同上

    // g_file_has_prefix
    printf ("%d\n", g_file_has_prefix(file1, file1)); // FALSE
    printf ("%d\n", g_file_has_prefix(file, file3));  // TRUE file3是file的父路径

    // 子路径相对与父路径的路径，其实就是父路径有，子路径没有的那部分  ---  父, 子
    printf ("%s\n", g_file_get_relative_path(file3, file)); 
    //printf ("%s\n", g_file_get_relative_path(file, file3)); // 段错误

    // 将文件相对路径解析为绝对路径 低版本不支持
    //printf ("%s\n", g_file_peek_path(g_file_resolve_relative_path(file3, "bbbxxx")));

    // 检查文件是否是本地原生的，windows和linux
    printf ("%d\n", g_file_is_native(file1));

    // 检查是否某种 uri， http、file 需要去掉://
    printf ("%d\n", g_file_has_uri_scheme (file1, "file"));
    file4 = g_file_get_uri (file1);
    printf ("%d\n", g_file_has_uri_scheme (g_file_parse_name(file4), "file"));

    // 输出 file
    printf ("%s\n", g_file_get_uri_scheme(file3));

    // 创建文件 文件必须不存在，默认每个人都可以读取， GCancellable 不为NULL，可在其它进程使其退出写状态，返回错误
    file_output = g_file_create (file1, G_FILE_CREATE_PRIVATE, NULL, NULL);

    // g_file_replace 覆盖文件，返回流

    // g_file_append_to_async 异步打开要追加的文件 g_file_append_to 同步调用版本
    // 异步调用之后，调用 g_file_append_to_finish 返回操作结果
    // 异步调用，在请求满足的时候调用，回调的数据，比如要输出的数据

    // g_file_query_info
    // 获取有关指定文件的请求信息，结果是键值对GFileInfo，attributes是以逗号分割的属性或属性通配符列表

    // g_file_query_exists 
    // 检查文件是否存在，一定要检查

    // g_file_query_file_type
    // 检查文件类型，普通文件、文件夹、管道

    // g_file_query_default_handler
    // 返回注册为默认应用程序的GAppInfo，以处理由file指定的文件

    // g_file_measure_disk_usage
    // 递归测量文件的磁盘使用情况 类似 du 命令，progress回调更新进度

    
    g_object_unref (file_output);
    g_free (file4);
    g_object_unref (file3);
    g_object_unref (file2);
    g_object_unref (file1);
    g_object_unref (file);
}
