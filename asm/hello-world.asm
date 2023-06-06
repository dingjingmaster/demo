.data           
    msg: .ascii "Hello World\n"

.text            

.global _start

_start:
    movq $1, %rax           # 写 系统调用
    movq $1, %rdi           # 标准输出文件描述符
    movq $msg, %rsi         # 字符串地址
    movq $12, %rdx
    syscall                 # 调用 写操作

    movq $60, %rax          # exit 调用
    movq $0, %rdi           # 
    syscall                 # 调用
