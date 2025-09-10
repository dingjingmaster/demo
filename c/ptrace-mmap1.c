/*************************************************************************
> FileName: ./ptrace-mmap1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年09月10日 星期三 19时28分33秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <pid>\n", argv[0]);
        return -1;
    }

    pid_t pid = atoi(argv[1]);
    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {
        perror("PTRACE_ATTACH");
        return -1;
    }
    waitpid(pid, NULL, 0);

    struct user_regs_struct regs, saved_regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    saved_regs = regs;

    // 准备系统调用参数
    regs.rax = SYS_mmap;
    regs.rdi = 0;                       // addr
    regs.rsi = 0x1000;                  // length
    regs.rdx = PROT_READ | PROT_WRITE;  // prot
    regs.r10 = MAP_ANONYMOUS | MAP_PRIVATE; // flags
    regs.r8  = -1;                      // fd
    regs.r9  = 0;                       // offset

    // x86_64 调用 syscall: 设置 rip 指向 syscall
    unsigned long syscall_insn = 0x050f; // syscall 指令 opcode: 0x0f05
    unsigned long orig_code = ptrace(PTRACE_PEEKTEXT, pid, regs.rip, NULL);
    ptrace(PTRACE_POKETEXT, pid, regs.rip, (orig_code & ~0xffff) | syscall_insn);

    ptrace(PTRACE_SETREGS, pid, NULL, &regs);
    ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
    waitpid(pid, NULL, 0);

    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    unsigned long mmap_addr = regs.rax;
    printf("mmap returned: 0x%lx\n", mmap_addr);

    // 恢复原来的指令
    ptrace(PTRACE_POKETEXT, pid, saved_regs.rip, orig_code);

    // 写入 AAABBB
    const char *data = "AAABBB";
    for (size_t i = 0; i < strlen(data); i += sizeof(long)) {
        long val = 0;
        memcpy(&val, data + i, sizeof(long));
        ptrace(PTRACE_POKETEXT, pid, mmap_addr + i, val);
    }

    ptrace(PTRACE_SETREGS, pid, NULL, &saved_regs);
    ptrace(PTRACE_DETACH, pid, NULL, NULL);

    return 0;
}
