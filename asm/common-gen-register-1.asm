.data
sum: .quad 0
val: .quad 25

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

.extern print_long

.text
.global _start

_start:
    mov $0, %rax                # 0
    inc %rax                    # AX = 1
    add $200, %rax              # AX = 201

    pushaq
    # print sum
    movq %rax, %rdi
    call print_long             # AX = 201
    popaq

    sub val(%rip), %rax         # val = 25 - 201 = -176 X  --> 正确 201 - 25 = 176 值保存在 %rax

    pushaq
    # print sum
    movq %rax, %rdi
    call print_long             # AX = 176
    popaq

    mov %rax, sum(%rip)         # sum = 176
    decq sum(%rip)              # sum = 175
    negq sum(%rip)              # sum = -175

    pushaq
    # print sum
    movq sum(%rip), %rdi
    call print_long             # sum = -175

    # print val
    movq val(%rip), %rdi
    call print_long             # val = 25 --> 值 未变过
    popaq

    movq $60, %rax
    xorq %rdi, %rdi
    syscall
.end
