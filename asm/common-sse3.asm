########################################################
# FileName     : common-sse3.asm
# Author       : DingJing
# Mail         : dingjing@live.cn
# Created Time : 2022年01月06日 星期四 11时59分01秒
########################################################
.data
.balign 16
vectorA: .float 1.2, 3.4, 5.6, 7.8
vectorB: .float 7.8, 5.6, 3.4, 1.2

.text
.global _main
_main:

movaps vectorA(%rip), %xmm0 # move vectorA to XMM0
movaps vectorB(%rip), %xmm1 # move vectorB to XMM1
haddps %xmm1, %xmm0         # horizontal add packed SP vectorB to vectorA

movq $60, %rax
xorq %rdi, %rdi
syscall
.end
