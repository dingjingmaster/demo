#include <linux/ip.h>
#include <linux/in.h>
#include <linux/bpf.h>
#include <linux/types.h>
#include <linux/if_ether.h>
#include <bpf/bpf_helpers.h>

typedef __u8    u8;
typedef __u16   u16;
typedef __u32   u32;
typedef __u64   u64;

/**
 * @brief 需要打标签的 PID
 */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u32);                       // 进程ID
    __type(value, u32);                     // 标签值
    __uint(max_entries, 1024);             // 最多支持 10240 个进程
} gPidToMarkMap SEC(".maps");

SEC("cgroup/skb")
int mark_traffic(struct __sk_buff* skb)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;     // 获取当前进程的 PID
    u32* mark = bpf_map_lookup_elem(&gPidToMarkMap, &pid);
    if (mark) {
        skb->mark = 42; //*mark;
        bpf_printk("Marking mark[%d] %d\n", pid, *mark);
    }
    else {
        bpf_printk("No mark %d\n", pid);
        return 0;
    }

    return 1;
}

char _license[] SEC("license") = "GPL";
