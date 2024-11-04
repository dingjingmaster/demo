#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bpf/bpf.h>
#include <sys/types.h>
#include <bpf/libbpf.h>
#include <sys/resource.h>

int main(int argc, char **argv) 
{
    struct bpf_object *obj;
    int prog_fd, map_fd;
    uint32_t pid, mark;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <pid> <mark>\n", argv[0]);
        return 1;
    }

    pid = atoi(argv[1]);   // 获取目标进程 PID
    mark = atoi(argv[2]);  // 设置标签值

    // 打开编译好的 BPF 对象文件
    obj = bpf_object__open_file("net-mark.bpf.o", NULL);
    if (libbpf_get_error(obj)) {
        fprintf(stderr, "Failed to open BPF object file\n");
        return 1;
    }

    // 加载 BPF 对象文件
    if (bpf_object__load(obj)) {
        fprintf(stderr, "Failed to load BPF object\n");
        bpf_object__close(obj);
        return 1;
    }

    // 获取 BPF 程序的文件描述符
    struct bpf_program *prog = bpf_object__find_program_by_name(obj, "mark_traffic");
    prog_fd = bpf_program__fd(prog);

    // 获取 pid_to_mark_map 的 map fd
    map_fd = bpf_object__find_map_fd_by_name(obj, "gPidToMarkMap");

    // 将 PID 和标签插入到 map 中
    if (bpf_map_update_elem(map_fd, &pid, &mark, BPF_ANY) != 0) {
        perror("Failed to update pid_to_mark_map");
        bpf_object__close(obj);
        return 1;
    }

    // 将 eBPF 程序附加到 cgroup 的 ingress 网络钩子上
    // 假设已经创建并挂载了一个 cgroup
    int cgroup_fd = open("/sys/fs/cgroup/my_cgroup", O_RDONLY);
    if (cgroup_fd < 0) {
        perror("Failed to open cgroup");
        return 1;
    }

    if (bpf_prog_attach(prog_fd, cgroup_fd, BPF_CGROUP_INET_INGRESS, 0) != 0) {
        perror("Failed to attach BPF program to cgroup");
        close(cgroup_fd);
        bpf_object__close(obj);
        return 1;
    }

    printf("eBPF program loaded and PID %u is marked with %u\n", pid, mark);

    // 挂载后保持程序运行，直到手动终止
    getchar();

    // 卸载 BPF 程序
    bpf_prog_detach(cgroup_fd, BPF_CGROUP_INET_INGRESS);
    close(cgroup_fd);
    bpf_object__close(obj);

    return 0;
}

