/*************************************************************************
> FileName: hook-cups.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月29日 星期二 14时14分06秒
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

static void* handle = NULL;

void hook_init() __attribute__((constructor()));

void hook_init()
{
    handle = dlopen("/usr/local/andsec/lib/libhook-cups-utils.so", RTLD_NOW | RTLD_LOCAL);
}

#define get_function_ptr(funcName) dlsym(handle, funcName);

#if 0
typedef int (*newCupsCreateJob)(void* http, const char* name, const char* title, int num_options, void* options);
int cupsCreateJob(void* http, const char* name, const char* title, int num_options, void* options)
{
    logi("create print job...");
    newCupsCreateJob oldCupsCreateJob = NULL; // 保存成old_open以备调用

    oldCupsCreateJob = get_function_ptr("hook_cupsCreateJob");
    if (NULL == oldCupsCreateJob){
        syslog(LOG_ERR, "printer HOOK, dlsym Failed.");
        return 0;
    }

    logi("name:%s, title:%s, len:%ld, num_options: %d", name, title, strlen(title), num_options);

    if (!title) { goto pass; }

pass:
    return oldCupsCreateJob(http, name, title, num_options, options);
}

typedef int(*CupsCreateDestJob) (void* http, void* dest, int* jobID, const char* title, int numOptions, void* options);
int cupsCreateDestJob (void* http, void* dest, int* jobID, const char* title, int numOptions, void* options)
{
    logi("cups create dest job...");
    CupsCreateDestJob oldCupsCreateJob = NULL; // 保存成old_open以备调用

    oldCupsCreateJob = get_function_ptr("hook_cupsCreateDestJob");
    if (NULL == oldCupsCreateJob){
        syslog(LOG_ERR, "printer HOOK, dlsym Failed.");
        return 0;
    }

    logi("title:%s, len:%ld, num_options: %d", title, strlen(title), numOptions);

    if (!title) { goto pass; }

pass:
    return oldCupsCreateJob(http, dest, jobID, title, numOptions, options);
}
#endif

typedef int (*CupsPrintFile) (const char* name, const char* filename, const char* title, int numOptions, void* ops);
int cupsPrintFile (const char* name, const char* filename, const char* title, int numOptions, void* ops)
{
    syslog (LOG_ERR, "[Hook] cupsPrintFile\n name: %s\n filename: %s\n title: %s\n", (name ? name: ""), (filename ? filename : ""), (title ? title : ""));
    CupsPrintFile oldCupsCreateJob = NULL; // 保存成old_open以备调用

    oldCupsCreateJob = get_function_ptr("hook_cupsPrintFile");
    if (NULL == oldCupsCreateJob){
        syslog(LOG_ERR, "printer HOOK, dlsym Failed.");
        return 0;
    }

    logi("title:%s, len:%ld, num_options: %d", title ? title: "<null>", strlen(title), numOptions);

    if (!title) { goto pass; }

pass:
    return oldCupsCreateJob(name, filename, title, numOptions, ops);
}

