/*************************************************************************
> FileName: netfilter1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年02月03日 星期二 13时32分39秒
 ************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/skbuff.h>
#include <linux/inet.h>
#include <net/tcp.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("you");
MODULE_DESCRIPTION("Minimal Netfilter DNAT example");

static struct nf_hook_ops nfho;

static unsigned int dnat_hook(void *priv,
                              struct sk_buff *skb,
                              const struct nf_hook_state *state)
{
    struct iphdr *iph;
    struct tcphdr *tcph;

    if (!skb)
        return NF_ACCEPT;

    iph = ip_hdr(skb);
    if (!iph || iph->protocol != IPPROTO_TCP)
        return NF_ACCEPT;

    /* 确保 TCP 头可写 */
    if (!pskb_may_pull(skb, iph->ihl * 4 + sizeof(struct tcphdr)))
        return NF_ACCEPT;

    tcph = (struct tcphdr *)((__u8 *)iph + iph->ihl * 4);

    /* 匹配 dport = 80 */
    if (ntohs(tcph->dest) != 80)
        return NF_ACCEPT;

    printk(KERN_INFO "[dnat] TCP %pI4:%u -> %pI4:%u\n",
           &iph->saddr, ntohs(tcph->source),
           &iph->daddr, ntohs(tcph->dest));

    /* === 开始 DNAT === */

    /* 修改目的 IP 为 127.0.0.1 */
    iph->daddr = htonl(0x7F000001); /* 127.0.0.1 */

    /* 修改目的端口为 8080 */
    tcph->dest = htons(8080);

    /* 重算 TCP 校验和 */
    tcph->check = 0;
    tcph->check = tcp_v4_check(
        skb->len - iph->ihl * 4,
        iph->saddr,
        iph->daddr,
        csum_partial((char *)tcph,
                     skb->len - iph->ihl * 4,
                     0)
    );

    /* 重算 IP 校验和 */
    iph->check = 0;
    iph->check = ip_fast_csum((u8 *)iph, iph->ihl);

    return NF_ACCEPT;
}

static int __init dnat_init(void)
{
    nfho.hook = dnat_hook;
    nfho.hooknum = NF_INET_PRE_ROUTING;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_NAT_DST;

    nf_register_net_hook(&init_net, &nfho);

    printk(KERN_INFO "mini_dnat loaded\n");
    return 0;
}

static void __exit dnat_exit(void)
{
    nf_unregister_net_hook(&init_net, &nfho);
    printk(KERN_INFO "mini_dnat unloaded\n");
}

module_init(dnat_init);
module_exit(dnat_exit);

