.text

.global _start

_start:
    movq $60, %rax      # 60 表示 exit
    syscall

