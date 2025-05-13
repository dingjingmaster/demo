#include <linux/fs.h>
#include <linux/bpf.h>
#include <asm/barrier.h>
#include <linux/sched.h>
#include <linux/dcache.h>
#include <linux/ptrace.h>
#include <linux/compiler.h>
#include <bpf/bpf_helpers.h>

char LICENSE[] SEC("license") = "Dual BSD/GPL";

// 允许的进程 PID（可改）
const volatile u32 allowed_pid = 0;

// Hook 到 LSM 的 file_open 事件
SEC("lsm/file_open")
int BPF_PROG(restrict_open, struct file *file)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    if (pid == allowed_pid) {
        return 0;
    }

    struct dentry *dentry = file->f_path.dentry;
    struct qstr d_name = dentry->d_name;

    // 拒绝访问路径为 "secure-dir" 的目录
    if (d_name.len == 10 &&
        d_name.name[0] == 's' &&
        d_name.name[1] == 'e' &&
        d_name.name[2] == 'c' &&
        d_name.name[3] == 'u' &&
        d_name.name[4] == 'r' &&
        d_name.name[5] == 'e' &&
        d_name.name[6] == '-' &&
        d_name.name[7] == 'd' &&
        d_name.name[8] == 'i' &&
        d_name.name[9] == 'r') {
        return -13; // EACCES
    }

    return 0;
}
