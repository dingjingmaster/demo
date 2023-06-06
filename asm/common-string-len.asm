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

.data
string: .ascii "This string has\na line break in it.\0"
.equ len, (. - string)

.text
.global _start

_start:
    # 打印字符串长度应该是: 35
    # FIXME:// 暂时结果错误 .......
    pushaq
    movq $0, %rdi
    movq len(%rip), %rdi
    call print_int
    popaq

    mov $60, %rax
    xor %rdi, %rdi
    syscall
.end
