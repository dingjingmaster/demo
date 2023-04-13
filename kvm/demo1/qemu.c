/*************************************************************************
> FileName: qemu.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 13 Apr 2023 04:02:07 PM CST
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>

int main (int argc, char* argv[])
{
    int ret = 0;
    struct kvm_sregs sregs;

    // 获取子文件系统的文件描述符
    int kvmFd = open ("/dev/kvm", O_RDWR);

    // 应用层检查版本是否支持
    ioctl(kvmFd, KVM_GET_API_VERSION, NULL);

    // 创建一个虚拟机，返回新建虚拟机的文件描述符
    int vmFd = ioctl(kvmFd, KVM_CREATE_VM, 0);

    // void* mmap(void* start, size_t length, int prot, int flags, int fd, off_t offset);
    unsigned char* ram = (unsigned char*) mmap (NULL, 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED | MAP_ANONYMOUS | MAP_EXECUTABLE, -1, 0);

    int kfd = open ("./test.bin", O_RDONLY);
    read (kfd, ram, 4096);

    struct kvm_userspace_memory_region mem = {
        .slot = 0,
        .guest_phys_addr = 0,
        .memory_size = 0x1000,
        .userspace_addr = (unsigned long) ram,
    };

    // 为虚拟机指定内存
    ret = ioctl (vmFd, KVM_SET_USER_MEMORY_REGION, &mem);

    // 为虚拟机创建vcpu
    int vcpufd = ioctl (vmFd, KVM_CREATE_VCPU, 0);
    int mmap_size = ioctl(kvmFd, KVM_GET_VCPU_MMAP_SIZE, NULL);
    struct kvm_run* run = (struct kvm_run*) mmap (NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, vcpufd, 0);

    // 获取虚拟机 CPU 寄存器
    ret = ioctl (vcpufd, KVM_GET_SREGS, &sregs);
    sregs.cs.base = 0;
    sregs.cs.selector = 0;

    // 设置虚拟机 CPU 寄存器
    ret = ioctl(vcpufd, KVM_SET_SREGS, &sregs);
    struct kvm_regs regs = {
        .rip = 0,
    };

    ret = ioctl (vcpufd, KVM_SET_REGS, &regs);

    while (1) {
        ret = ioctl (vcpufd, KVM_RUN, NULL);
        if (-1 == ret) {
            printf ("exit unknow\n");
            return -1;
        }

        switch (run->exit_reason) {
        case KVM_EXIT_HLT: {
            puts("KVM_EXIT_HLT");
            return 0;
        }
        case KVM_EXIT_IO: {
            putchar(*(((char*) run) + run->io.data_offset));
            break;
        }
        case KVM_EXIT_FAIL_ENTRY:{
            puts("entry error");
            return -1;
        }
        default: {
            puts ("other error");
            printf ("exit reason: %d\n", run->exit_reason);
            return -1;
        }
        }
    }

    return 0;
}
