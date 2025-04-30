/*************************************************************************
> FileName: lsm-hook-open.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月30日 星期三 16时08分06秒
 ************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/security.h>
#include <linux/path.h>
#include <linux/dcache.h>
#include <linux/namei.h>
#include <linux/errno.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Name");
MODULE_DESCRIPTION("LSM redierct file");

static int hook_open (struct file* file)
{
	struct path* path = &file->f_path;
	char* origPath = path->dentry->d_name.name;

	pr_info("origPath: %s\n", origPath);

	return 0;
}

static int __init lsm_init(void)
{
	pr_info("LSM file start\n");
	security_add_hooks((struct security_hook_list[]) {{
		.hook = hook_open,
		.hooknum = LSM_FILE_OPS_OPEN,
	}}, 1, NULL);

	return 0;
}

static void __exit lsm_exit(void)
{
	pr_info("exit\n");

	security_remove_hooks((struct security_hook_list[]) {{
		.hook = hook_open,
		.hooknum = LSM_FILE_OPS_OPEN,
	}}, 1);
}

module_init(lsm_init);
module_exit(lsm_exit);
