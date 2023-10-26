/*************************************************************************
> FileName: main.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 26 Oct 2023 08:49:13 AM CST
 ************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

#include "list-management.h"




int linux_api_init(void)
{
    dj_demo_list();

    return 0;
}

void linux_api_cleanup(void)
{
}

module_init(linux_api_init);
module_exit(linux_api_cleanup);

MODULE_LICENSE("GPL");
MODULE_VERSION("V0.0.0");
MODULE_AUTHOR("DingJing Demo");
MODULE_DESCRIPTION("linux API demo");
