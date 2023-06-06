.text
.global _main
_main:
xor %rax, %rax
mov $5, %rcx

myLoop:
   inc %rax
   loop myLoop

mov $60, %rax
xor %rdi, %rdi
syscall
.end
