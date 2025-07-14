/*************************************************************************
> FileName: nf1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年07月14日 星期一 09时40分16秒
 ************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/string.h>
#include <linux/byteorder/generic.h>

static struct nf_hook_ops *nf_tracer_ops = NULL;
static struct nf_hook_ops *nf_tracer_out_ops = NULL;

static unsigned int nf_tracer_handler(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    if(skb==NULL) {
        return NF_ACCEPT;
    }

    struct iphdr * iph;
    iph = ip_hdr(skb);

    if(iph && iph->protocol == IPPROTO_TCP) {
        struct tcphdr *tcph = tcp_hdr(skb);

        pr_info("source : %pI4:%hu | dest : %pI4:%hu | seq : %u | ack_seq : %u | window : %hu | csum : 0x%hx | urg_ptr %hu\n", &(iph->saddr),ntohs(tcph->source),&(iph->saddr),ntohs(tcph->dest), ntohl(tcph->seq), ntohl(tcph->ack_seq), ntohs(tcph->window), ntohs(tcph->check), ntohs(tcph->urg_ptr));
    }

    return NF_ACCEPT;
}


static int __init nf_tracer_init(void) {

    nf_tracer_ops = (struct nf_hook_ops*)kcalloc(1,  sizeof(struct nf_hook_ops), GFP_KERNEL);

    if(nf_tracer_ops!=NULL) {
        nf_tracer_ops->hook = (nf_hookfn*)nf_tracer_handler;
        nf_tracer_ops->hooknum = NF_INET_PRE_ROUTING;
        nf_tracer_ops->pf = NFPROTO_IPV4;
        nf_tracer_ops->priority = NF_IP_PRI_FIRST;

        nf_register_net_hook(&init_net, nf_tracer_ops);
    }

    nf_tracer_out_ops = (struct nf_hook_ops*)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);

    if(nf_tracer_out_ops!=NULL) {
        nf_tracer_out_ops->hook = (nf_hookfn*)nf_tracer_handler;
        nf_tracer_out_ops->hooknum = NF_INET_LOCAL_OUT;
        nf_tracer_out_ops->pf = NFPROTO_IPV4;
        nf_tracer_out_ops->priority = NF_IP_PRI_FIRST;

        nf_register_net_hook(&init_net, nf_tracer_out_ops);
    }

    return 0;
}

static void __exit nf_tracer_exit(void) {

    if(nf_tracer_ops != NULL) {
        nf_unregister_net_hook(&init_net, nf_tracer_ops);
        kfree(nf_tracer_ops);
    }

    if(nf_tracer_out_ops != NULL) {
        nf_unregister_net_hook(&init_net, nf_tracer_out_ops);
        kfree(nf_tracer_out_ops);
    }
}

module_init(nf_tracer_init);
module_exit(nf_tracer_exit);

MODULE_LICENSE("GPL");
