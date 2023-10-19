/*************************************************************************
> FileName: main.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 17 Oct 2023 09:48:45 AM CST
 ************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>




int scullc_init(void)
{
    return 0;
}

void scullc_cleanup(void)
{
}

module_init(scullc_init);
module_exit(scullc_cleanup);
MODULE_AUTHOR("DingJing Demo");
MODULE_LICENSE("MIT");
