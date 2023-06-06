.data
sum: .quad 0                # .quad 用于定义 64 位数值

.macro pushaq
    push %rax
    push %rcx
    push %rdx
    push %rbx
    push %rbp
    push %rsi
    push %rdi
.endm # pushaq

.macro popaq
    pop %rdi
    pop %rsi
    pop %rbp
    pop %rbx
    pop %rdx
    pop %rcx
    pop %rax
.endm # popaq

.extern print_int

.text

.global _start

####################################
# 计算 25 + 50 的值, 使用 add 指令 #
####################################
_start:
    mov $25, %rax
    mov $50, %rbx
    add %rbx, %rax
    mov %rax, sum(%rip)

    pushaq                          # 少于 7 个时候的参数传递 rdi, rsi, rdx, rcx, r8, r9
                                    # 多于 7 个，前六个不变，后几个从右向左压栈
    movq sum(%rip), %rdi
    call print_int
    popaq

    mov $60, %rax
    xor %rdi, %rdi
    syscall
.end
