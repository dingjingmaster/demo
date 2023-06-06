########################################################
# FileName     : common-calc.asm
# Author       : DingJing
# Mail         : dingjing@live.cn
# Created Time : 2022年01月06日 星期四 12时00分22秒
########################################################
.extern print_string
.extern print_double
.extern get_double
.extern get_int

.data
.balign 16                         # align data on 16-byte boundary
amountNeeded:   .double 0.0
interestRate:   .double 0.0
interest:       .double 0.0
monthlyPayment: .double 0.0
duration:       .quad   0          # integer data
const100:       .double 100.0
const12:        .double 12.0
const1:         .double 1.0

amountMessage:   .asciz  "       Enter Amount Needed:  $"
interestMessage: .asciz  "       Enter Interest Rate:  "
durationMessage: .asciz  "Enter Duration (in months):  "
paymentMessage:  .asciz  "    Monthly Payment Amount:  $"

.text
.globl _asmMain
_asmMain:

pushq	%rbp
movq	%rsp, %rbp                  # align stack pointer

leaq amountMessage(%rip), %rdi      # pass char* to print
callq print_string
callq get_double                    # get and store amountNeeded
movsd %xmm0, amountNeeded(%rip)

leaq interestMessage(%rip), %rdi    # pass char* to print
callq print_string
callq get_double                    # get interestRate as a %

movsd const100(%rip), %xmm1         # divide rate by 100
divsd %xmm1, %xmm0
movsd %xmm0, interestRate(%rip)     # store rate as decimal value

leaq durationMessage(%rip), %rdi    # pass char* to print
callq print_string
callq get_int                       # get and store duration
movq %rax, duration(%rip)

movsd const12(%rip), %xmm0          # interest = interestRate / const12
movsd interestRate(%rip), %xmm1
divsd %xmm0, %xmm1
movsd %xmm1, interest(%rip)

movsd interest(%rip), %xmm0         # investment formula (see Ch8 Supplement)
movsd amountNeeded(%rip), %xmm1
mulsd %xmm0, %xmm1                  # interest * amount needed, then store
movsd %xmm1, %xmm2

addsd const1(%rip), %xmm0           # add 1 to interest
movq duration(%rip), %rdi
callq _pow                          # pow(interest, duration)
subsd const1(%rip), %xmm0           # subtract 1 from result
vdivsd %xmm0, %xmm2, %xmm1          # complete formula with division
movsd %xmm1, monthlyPayment(%rip)

leaq paymentMessage(%rip), %rdi     # pass char* to print
callq print_string
movsd monthlyPayment(%rip), %xmm0
callq print_double                  # print monthlyPayment

popq    %rbp
retq                                # end _asmMain

_pow:                               # pow function
pushq	%rbp
movq %rsp, %rbp
movq %rdi, %rcx                     # use rcx for loop countdown
decq %rcx
movsd %xmm0, %xmm1

exp:                                # multiply by value duration times
mulsd %xmm1, %xmm0
loop exp

popq	%rbp
retq                                # end _pow

.end
