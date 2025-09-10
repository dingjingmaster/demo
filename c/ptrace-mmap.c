/*************************************************************************
> FileName: ptrace-mmap.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年09月10日 星期三 19时18分27秒
 ************************************************************************/

#define _GNU_SOURCE
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <syscall.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <inttypes.h>

#ifndef SYS_mmap
// x86_64 syscall number for mmap is normally 9
#define SYS_mmap 9
#endif

// Read a word (long) from target using PTRACE_PEEKTEXT
long ptrace_read_word(pid_t pid, void *addr) 
{
    errno = 0;
    long val = ptrace(PTRACE_PEEKTEXT, pid, addr, NULL);
    if (val == -1 && errno) {
        perror("ptrace_peektext");
    }
    return val;
}

// Write a word (long) to target using PTRACE_POKETEXT
int ptrace_write_word(pid_t pid, void *addr, long data) 
{
    if (ptrace(PTRACE_POKETEXT, pid, addr, (void*)data) == -1) {
        perror("ptrace_poketext");
        return -1;
    }
    return 0;
}

// write arbitrary bytes to the target process memory (word by word)
int write_data(pid_t pid, uint8_t *dest, const uint8_t *src, size_t size, uint8_t *backup) 
{
    size_t i = 0;
    // write in sizeof(long) chunks
    const size_t W = sizeof(long);
    while (i < size) {
        long word = 0;
        memcpy(&word, src + i, (size - i >= W) ? W : (size - i));
        // backup existing
        long old = ptrace_read_word(pid, dest + i);
        if (old == -1 && errno) return -1;
        if (backup) memcpy(backup + i, &old, (size_t)W);
        if (ptrace_write_word(pid, dest + i, word) == -1) return -1;
        i += W;
    }
    return 0;
}

// read arbitrary bytes from target into buf (word by word)
int read_data(pid_t pid, uint8_t *src, uint8_t *buf, size_t size) 
{
    size_t i = 0;
    const size_t W = sizeof(long);
    while (i < size) {
        long word = ptrace_read_word(pid, src + i);
        if (word == -1 && errno) return -1;
        memcpy(buf + i, &word, W);
        i += W;
    }
    return 0;
}

int remote_mmap(pid_t pid, size_t length, int prot, int flags, int fd, off_t offset, unsigned long *remote_addr_out) 
{
    int status;
    // 1. Attach
    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {
        perror("ptrace_attach");
        return -1;
    }
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return -1;
    }
    if (!WIFSTOPPED(status)) {
        fprintf(stderr, "Target did not stop\n");
        return -1;
    }

    // 2. Backup regs
    struct user_regs_struct regs, saved_regs;
    if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1) {
        perror("ptrace_getregs");
        goto detach_fail;
    }
    memcpy(&saved_regs, &regs, sizeof(regs));

    // 3. choose remote code address on the stack
    // Put code at (rsp - 0x200), align to 8
    uint8_t backup_space[0x300];
    memset(backup_space, 0, sizeof(backup_space));
    unsigned long remote_code_addr = (regs.rsp - 0x300) & ~0x7UL;
    // small shellcode: syscall; int3
    uint8_t code[] = { 0x0f, 0x05, /* syscall */
                       0xcc /* int3 */ };
    size_t code_sz = sizeof(code);

    // 4. backup existing memory at remote_code_addr and write shellcode
    if (read_data(pid, (uint8_t *)remote_code_addr, backup_space, sizeof(backup_space)) == -1) {
        fprintf(stderr, "Failed to backup target memory\n");
        goto restore_regs;
    }
    if (write_data(pid, (uint8_t *)remote_code_addr, code, code_sz, NULL) == -1) {
        fprintf(stderr, "Failed to write shellcode\n");
        goto restore_mem_from_backup;
    }

    // 5. Prepare registers for syscall
    regs.rax = SYS_mmap;      // syscall number
    regs.rdi = 0;             // addr = NULL
    regs.rsi = length;        // length
    regs.rdx = prot;          // prot
    regs.r10 = flags;         // flags (note: syscall uses r10)
    regs.r8  = fd;            // fd
    regs.r9  = (unsigned long)offset; // offset
    // set rip to injected code; adjust rsp to keep alignment
    unsigned long old_rsp = regs.rsp;
    regs.rsp = remote_code_addr; // move stack pointer to where code resides (safe)
    regs.rip = remote_code_addr;
    // ensure alignment: RSP should be 16-byte aligned before syscall (call doesn't happen here,
    // but keep alignment for safety)
    regs.rsp &= ~0xFUL;

    if (ptrace(PTRACE_SETREGS, pid, NULL, &regs) == -1) {
        perror("ptrace_setregs");
        goto restore_mem_from_backup;
    }

    // 6. Continue and wait for the int3 (SIGTRAP)
    if (ptrace(PTRACE_CONT, pid, NULL, NULL) == -1) {
        perror("ptrace_cont");
        goto restore_regs;
    }
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid2");
        goto restore_regs;
    }
    if (!WIFSTOPPED(status)) {
        fprintf(stderr, "Target did not stop after syscall\n");
        goto restore_regs;
    }
    // Typically stopped by SIGTRAP due to int3
    int sig = WSTOPSIG(status);
    if (sig != SIGTRAP && sig != SIGSTOP) {
        // Could be other signals; still try to continue
        fprintf(stderr, "Stopped by signal %d\n", sig);
    }

    // 7. Read registers to get return value
    if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1) {
        perror("ptrace_getregs_after");
        goto restore_regs;
    }
    unsigned long result = regs.rax;
    // if negative signed -> errno
    if ((long)result >= (unsigned long)(-4095) && (long)result < 0) {
        long err = -(long)result;
        fprintf(stderr, "remote mmap failed: errno=%ld (%s)\n", err, strerror(err));
        // still restore and return error
        goto restore_regs;
    }

    if (remote_addr_out) *remote_addr_out = result;
    printf ("result addr: 0x%lx\n", result);

restore_regs:
    // restore original registers
    if (ptrace(PTRACE_SETREGS, pid, NULL, &saved_regs) == -1) {
        perror("ptrace_setregs_restore");
    }

restore_mem_from_backup:
    // restore original memory area
    // write back backup_space to remote_code_addr
    // We will write the same sized area we backed up
    if (write_data(pid, (uint8_t *)remote_code_addr, backup_space, sizeof(backup_space), NULL) == -1) {
        fprintf(stderr, "Failed to restore remote memory\n");
    }

detach_fail:
    // detach
    if (ptrace(PTRACE_DETACH, pid, NULL, NULL) == -1) {
        perror("ptrace_detach");
        return -1;
    }
    // success if we set remote_addr_out
    return (remote_addr_out && *remote_addr_out) ? 0 : 0;
}

int main(int argc, char **argv) 
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <pid> <length>\n", argv[0]);
        return 1;
    }
    pid_t pid = (pid_t)atoi(argv[1]);
    size_t len = (size_t)atoll(argv[2]);

    unsigned long remote_addr = 0;
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    int fd = -1;
    off_t offset = 0;

    if (remote_mmap(pid, len, prot, flags, fd, offset, &remote_addr) == -1) {
        fprintf(stderr, "remote_mmap failed\n");
        return 1;
    }
    printf("remote mmap returned addr = 0x%lx\n", remote_addr);
    return 0;
}
