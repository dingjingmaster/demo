.data
mval: .quad 664751
dval: .quad 8

.text
.global _start
_start:

    # MUL 1-op
    mov mval(%rip), %rax
    mov $8, %rbx
    mul %rbx

    # IMUL 1-op
    mov mval(%rip), %rax
    mov $8, %rbx
    imul %rbx

    # IMUL 2-op
    mov $8, %rax
    imul mval(%rip), %rax

    # IMUL 3-op
    imul $8, mval(%rip), %rax

    # DIV 1-op
    mov $0, %rdx
    mov $5318008, %rax
    mov dval(%rip), %rcx
    div %rcx

    # IDIV 1-op
    mov $0, %rdx
    mov $5318008, %rax
    mov dval(%rip), %rcx
    idiv %rcx

    mov $60, %rax
    xor %rdi, %rdi
    syscall
.end
