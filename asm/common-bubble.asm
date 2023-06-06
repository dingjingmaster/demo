.data
array: .long 3, 2, 6, 4, 1, 9, 6, 5, 0, 10
count: .long 10

.extern print_arr_int

.text
.global _start

####################################################
# int main() {
#   
#   int  size = 10;
#   int  value[5] = {3, 2, 6, 4, 1, 9, 6, 5, 0, 10};
#   int  i, tmp;
#   bool swapped;
#   
#   do {
#      swapped = false;
#      for (i = 0; i < size - 1; i++) {
#         if (value[i + 1] < value[i]) {
#            tmp = value[i];
#            value[i] = value[i + 1];
#            value[i + 1] = tmp;
#            swapped = true;
#         }
#      }
#   } while (swapped);
#
#   return 0;
#}
####################################################

_start:
    # 打印原始数组
    pushq %rbp
    pushq %rsp
    pushq %rsi
    pushq %rdi
    pushq %rax
    pushq %rcx

    movq %rsp, %rbp

    subq $16, %rsp
    movl %edi, -4(%rbp)
    movq %rsi, -16(%rbp)

    movl count(%rip), %eax              # 相对寻址，找数组长度变量
    movl %eax, %esi
    leaq array(%rip), %rax
    movq %rax, %rdi

    call print_arr_int

    popq %rcx
    popq %rax
    popq %rdi
    popq %rsi
    popq %rsp
    popq %rbp

    movslq count(%rip), %rcx            # 类似 MOV 指令，兼容 32 位程序在 64 位跑
                                        # count(%rip) 表示全局变量 count
    dec %rcx                            # 减一，比 sub 快

    outerLoop:
        push %rcx                       # 保存计数器
        leaq array(%rip), %rsi          # rsi 存放数组首地址

        innerLoop:
            movl (%rsi), %eax           # 首个元素给 AX 寄存器
            cmpl %eax, 4(%rsi)          # AX 与 SI 四字节比较 (根据所占内存)
            jg nextStep                 # AX 值大于 SI 跳到 nextStep 标签
            xchgl 4(%rsi), %eax         # 交换两者值
            movl %eax, (%rsi)

        nextStep:
            addq $4, %rsi               # 下一个元素
            loop innerLoop
        popq %rcx
        loop outerLoop                  # CX 寄存器不为 0 时候循环

    # 打印排序后数组
    pushq %rbp
    pushq %rsp
    pushq %rax
    pushq %rsi
    pushq %rdi

    movq %rsp, %rbp

    subq $16, %rsp
    movl %edi, -4(%rbp)
    movq %rsi, -16(%rbp)

    movl count(%rip), %eax              # 相对寻址，找数组长度变量
    movl %eax, %esi
    leaq array(%rip), %rax
    movq %rax, %rdi

    call print_arr_int

    popq %rdi
    popq %rsi
    popq %rax
    popq %rsp
    popq %rbp

    movq $60, %rax
    xorq %rdi, %rdi
    syscall
.end
