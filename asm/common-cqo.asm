.text
.global _main
_main:

mov $-534, %rax
cqo
mov $15, %rbx
idiv %rbx

mov $60, %rax
xor %rdi, %rdi
syscall
.end
