.data
src: .ascii "Test sentence!\n\0"
.equ lenSrc, (. - src)
dst: .ascii "Test sentence!\n\0"
.equ lenDst, (. - dst)

.text
.global _main
_main:

# compare string lengths
movq $lenSrc, %rax
cmpq $lenDst, %rax
jne notequal

# when lengths are equal execute CMPS
leaq src(%rip), %rsi
leaq dst(%rip), %rdi
movq $lenSrc, %rcx
cld
repe cmpsb

# after CMPS, see if it ended successfully
jnz notequal

# equality things
jmp done

notequal:
# non-equality things

done:
movq $60, %rax
xorq %rdi, %rdi
syscall
.end
