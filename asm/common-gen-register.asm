.data                   # .ascii  .byte  .short  .int  .long  .word  .quad 分别定义
                        # 字符串 一个字节 短整形 整形 长整形 一个字  8字节
# .abort 停止汇编
# .align 以某种对齐方式，在未使用的存储区填充值，第一个值表示对齐方式(4, 8, 16, 32) 第二个值表示填充的值
# .if  .else  .endif  条件预编译
# .include "file"  包含指定头文件
# .common
# .equ 把一个符号定义成某一个值. 该指令并不分配空间，相当于 C 中的 #define
# .macro  .endm 定义一段宏代码
# .req 为寄存器定义一个别名
# .code[16|32] 指定指令代码产生的长度，16表示 thumb 指令，32表示arm指令
# .ltorg 表示当前往下的定义在归于当前段，并为之分配空间
# _start 汇编语言缺省入口，可以该，到链接脚本区使用 ENTRY 指明入口就行

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
.extern print_short
.extern print_enter

.bss
.lcomm letter, 1                    # .comm  声明未初始化数据的通用内存区域
.lcomm r, 4                         # .lcomm 声明位初始化数据的本地通用内存区域; lcomm 符号, 字节长度
.lcomm s, 4
.lcomm t, 4
.lcomm x, 2
.lcomm y, 2
.lcomm z, 2

.text
.global _start                      # .global 是全局符号，常供 ld 使用

_start:
    pushaq                          # 少于 7 个时候的参数传递 rdi, rsi, rdx, rcx, r8, r9
                                    # 多于 7 个，前六个不变，后几个从右向左压栈
    movq letter(%rip), %rdi
    call print_int

    movq r(%rip), %rdi
    call print_int

    movq s(%rip), %rdi
    call print_int

    movq t(%rip), %rdi
    call print_int

    movq x(%rip), %rdi
    call print_int

    movq y(%rip), %rdi
    call print_int

    movq z(%rip), %rdi
    call print_int

    call print_enter

    popaq

    movb $0x77, letter(%rip)        # letter = 119
    movl $0x5, r(%rip)              # r = 5
    movl $0x2, s(%rip)              # s = 2
    movw $0xa, x(%rip)              # x = 10
    movw $0x4, y(%rip)              # y = 4

    movw x(%rip), %ax               # x = 10
    addw y(%rip), %ax               # y = 4, ax = 14
    movw %ax, z(%rip)               # z = 14

    movw x(%rip), %ax               # x = 10
    subw y(%rip), %ax               # y = 4
    movw %ax, z(%rip)               # z = 6

    movl $0x0, %edx
    movl r(%rip), %eax              # r = 5
    movl s(%rip), %ecx              # s = 2
    divl %ecx                       # AX / CX = 2
    movl %eax, t(%rip)              # t = 2

    movl $0x0, %edx                 # DX = 0 

    pushaq
    movl %edx, %edi
    call print_int                  # t = 1 ?
    popaq

    movl r(%rip), %eax              # r = 5
    movl s(%rip), %ecx              # s = 2

    pushaq
    movl %edx, %edi
    call print_int                  # t = 1 
    popaq

    divl %ecx                       # 需要注意: AX mod CX  == 5 % 2  ==  1  结果 在 DX 里保存
                                    # FIXME:// 需要重新梳理 寄存器功能，以及各个指令如何与寄存器配合使用

    pushaq
    movl %edx, %edi
    call print_int                  # t = 1 
    popaq

    movl %edx, t(%rip)              # t = 0

    pushaq
    movl %edx, %edi
    call print_int                  # t = 1 
    popaq


    pushaq
    movq letter(%rip), %rdi         
    call print_int                  # letter = 119

    movq r(%rip), %rdi
    call print_int                  # r = 5

    movq s(%rip), %rdi
    call print_int                  # s = 2

    movq t(%rip), %rdi
    call print_int                  # t = 1 

    movq x(%rip), %rdi
    call print_short                # x = 10

    movq y(%rip), %rdi
    call print_short                # y = 4

    movq z(%rip), %rdi
    call print_int                  # z = 6

    popaq

    movq $60, %rax
    xorq %rdi, %rdi
    syscall
.end

