.data
num1: .quad 2
num2: .quad 4

.text
.global _main, _sum
_main:

movq $10, %rax
decq %rax
movq $5, %rbx

leaq num1(%rip), %rdi
leaq num2(%rip), %rsi
callq _sum

addq %rbx, %rax
decq %rax

movq $60, %rax
xorq %rdi, %rdi
syscall

_sum:
pushq %rbp
movq %rsp, %rbp
pushq %rbx
movq (%rdi), %rax
addq (%rsi), %rax
popq %rbx
popq %rbp
retq

.end
