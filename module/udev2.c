/*************************************************************************
> FileName: udev2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月18日 星期二 11时12分37秒
 ************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <linux/skbuff.h>
#include <linux/init.h>

#define UEVENT_NETLINK 15  // NETLINK_KOBJECT_UEVENT

static struct sock *nl_sk = NULL;

// 收到 uevent 的回调
static void uevent_receive(struct sk_buff *skb)
{
    struct nlmsghdr *nlh;
    if (!skb)
        return;

    nlh = (struct nlmsghdr *)skb->data;
    pr_info("Uevent: %s\n", (char *)nlmsg_data(nlh));
}

static int __init uevent_monitor_init(void)
{
    struct netlink_kernel_cfg cfg = {
        .input = uevent_receive,
    };

    // 创建 netlink socket
    nl_sk = netlink_kernel_create(&init_net, UEVENT_NETLINK, &cfg);
    if (!nl_sk) {
        pr_err("Failed to create netlink socket for uevent\n");
        return -ENOMEM;
    }

    pr_info("Uevent monitor loaded\n");
    return 0;
}

static void __exit uevent_monitor_exit(void)
{
    if (nl_sk)
        netlink_kernel_release(nl_sk);

    pr_info("Uevent monitor unloaded\n");
}

module_init(uevent_monitor_init);
module_exit(uevent_monitor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("Monitor udev events via netlink in kernel module");
