.extern print_hello

.text
.global _start

_start:
    pushq %rbp
    movq %rsp, %rbp

    call print_hello

    movq $60, %rax
    syscall
    
