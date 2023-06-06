.data
    arr: .int 3, 2, 1, 4, 5, 6
    arr_len: .int 6

.extern print_arr_int

.text
.global _start

_start:
    pushq %rbp
    movq %rsp, %rbp

    # 参数需要压栈
    subq $16, %rsp
    movl %edi, -4(%rbp)
    movq %rsi, -16(%rbp)

    # 参数传递顺序: RID  RSI  RDX  RCX  R8  R9  XMM0-7
    movl arr_len(%rip), %eax
    movl %eax, %esi
    leaq arr(%rip), %rax        # 需要注意: lea 表示把 内存地址直接给 AX 寄存器
    movq %rax, %rdi             # mov 表示把地址对应的内容给到 DI 寄存器

    call print_arr_int

    nop
    popq %rbp

    movq $60, %rax
    syscall
    
