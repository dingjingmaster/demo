/*************************************************************************
> FileName: gio-appinfo.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月18日 星期二 20时21分15秒
 ************************************************************************/

#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    GList* applist = g_app_info_get_all();

    printf ("所有的 .desktop 对应的 GAppInfo 信息\n");
    for (GList* l = applist; NULL != l; l = l->next) {
        GAppInfo* app = l->data;
        printf ("name:%s\n", g_app_info_get_name(app));
    }


    return 0;
}
