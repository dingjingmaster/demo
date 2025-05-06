/*************************************************************************
> FileName: hook-cups-utils.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月06日 星期二 11时10分00秒
 ************************************************************************/
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdint.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>

#define LOGI

#ifdef LOGI
#include <syslog.h>
#define logi(...) syslog(LOG_ERR, __VA_ARGS__)
#else
#define logi(x)
#endif


typedef int (*CupsPrintFile) (const char* name, const char* filename, const char* title, int numOptions, void* ops);
int hook_cupsPrintFile (const char* name, const char* filename, const char* title, int numOptions, void* ops)
{
    syslog (LOG_ERR, "[Hook] cupsPrintFile\n name: %s\n filename: %s\n title: %s\n", (name ? name: ""), (filename ? filename : ""), (title ? title : ""));

    CupsPrintFile old = dlsym(RTLD_NEXT, "cupsPrintFile");
    if (NULL == old){
        syslog(LOG_ERR, "printer HOOK, dlsym Failed.");
        return 0;
    }

    logi("title:%s, len:%ld, num_options: %d", title ? title: "<null>", strlen(title), numOptions);

    if (!title) { goto pass; }

pass:
    return old(name, filename, title, numOptions, ops);

}

