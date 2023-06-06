.text

.global _start

_start:
    movl $1, %eax       # 1 表示 32 位 系统 的 exit
    int $0x80

