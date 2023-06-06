########################################################
# FileName     : common-mmx.asm
# Author       : DingJing
# Mail         : dingjing@live.cn
# Created Time : 2022年01月06日 星期四 11时56分53秒
########################################################
.data
w1: .word 1, 2, 3, 4
w5: .word 5, 6, 7, 8
result: .quad 0

.text
.global _main
_main:

movq w1(%rip), %mm0     # move quadword w1-w4 to MM0
movq w5(%rip), %mm1     # move quadword w5-w8 to MM1

paddsw %mm1, %mm0       # add packed signed words
movq %mm0, result(%rip) # move data to result

movq $60, %rax
xorq %rdi, %rdi
syscall
.end
