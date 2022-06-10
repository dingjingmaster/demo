/*************************************************************************
> FileName: gsettings-demo2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月30日 星期一 23时21分00秒
 ************************************************************************/
#include <gio/gio.h>

int main (int argc, char* argv)
{
    char** k1 = NULL;
    char** k2 = NULL;
    GSettingsSchemaSource* gs = NULL;

    gs = g_settings_schema_source_get_default();

    g_settings_schema_source_list_schemas (gs, TRUE, &k1, &k2);
    for (int i = 0; k1[i] != NULL; ++i) {
        printf ("%s\n", k1[i]);
    }

    printf ("===============\n");

    for (int i = 0; k2[i] != NULL; ++i) {
        printf ("%s\n", k2[i]);
    }

    return 0;
}
