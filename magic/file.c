#include <glib.h>
#include <stdio.h>
#include <magic.h>
#include <gio/gio.h>

char* get_magic_file ();

int main (int argc, char* argv[])
{
    printf("magic version: %d\n", magic_version());
    if (argc <= 1) {
        printf("请输入要查看的文件路径\n");
        return -1;
    }

    magic_t magic = magic_open(0);
    if (NULL == magic) {
        printf("magic_open error\n");
        return -2;
    }

    g_autofree char* filePath = NULL;
    if (g_str_has_prefix (argv[1], "/")) {
        filePath = g_strdup(argv[1]);
    }
    else {
        g_autofree char* currDir = g_get_current_dir();
        filePath = g_canonicalize_filename(argv[1], currDir);
    }
    g_autoptr(GFile) file = g_file_new_for_path(filePath);
    if (!g_file_query_exists(file, NULL)) {
        printf("文件'%s'不存在\n", filePath);
        return -1;
    }

    int ret = magic_load(magic, NULL);

    const char* type = magic_file(magic, argv[1]);
    if (NULL == type) {
        printf("magic_file error: %s\n", magic_error(magic));
        return -3;
    }

    printf("==>Query file: %s\n", filePath);
    printf("         Type: %s\n", type);

    return 0;
}


