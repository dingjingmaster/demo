/*************************************************************************
> FileName: param-num.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年03月29日 星期六 11时11分25秒
 ************************************************************************/

#define __syscall_arg_t					long int
#define __SSC(__x)						((__syscall_arg_t) (__x))
#define __SYSCALL_CANCEL7_ARG
#define SYSCALL_CANCEL(...)				__SYSCALL_CANCEL_CALL(__VA_ARGS__)
#define __SYSCALL_CANCEL_CALL(...)		__SYSCALL_CANCEL_DISP (__SYSCALL_CANCEL, __VA_ARGS__)
#define __SYSCALL_CANCEL_DISP(b,...)	__SYSCALL_CANCEL_CONCAT (b,__SYSCALL_CANCEL_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define __SYSCALL_CANCEL_NARGS(...)		__SYSCALL_CANCEL_NARGS_X (__VA_ARGS__,7,6,5,4,3,2,1,0,)
#define __SYSCALL_CANCEL_NARGS_X(a,b,c,d,e,f,g,h,n,...) n
#define __SYSCALL_CANCEL_CONCAT(a,b)    __SYSCALL_CANCEL_CONCAT_X (a, b)
#define __SYSCALL_CANCEL_CONCAT_X(a,b)  a##b
#define INTERNAL_SYSCALL_NCS_CALL(...)	__INTERNAL_SYSCALL_DISP (__INTERNAL_SYSCALL_NCS, __VA_ARGS__)
#define __INTERNAL_SYSCALL_DISP(b,...)	__SYSCALL_CONCAT (b,__INTERNAL_SYSCALL_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define __SYSCALL_CONCAT_X(a,b)			a##b
#    define __SYSCALL_CONCAT_X(a, b) a##b
#    define __SYSCALL_CONCAT(a, b) __SYSCALL_CONCAT_X (a, b)

#    define __INTERNAL_SYSCALL0(name) INTERNAL_SYSCALL (name, 0)
#    define __INTERNAL_SYSCALL1(name, a1) INTERNAL_SYSCALL (name, 1, a1)
#    define __INTERNAL_SYSCALL2(name, a1, a2) INTERNAL_SYSCALL (name, 2, a1, a2)
#    define __INTERNAL_SYSCALL3(name, a1, a2, a3) INTERNAL_SYSCALL (name, 3, a1, a2, a3)
#    define __INTERNAL_SYSCALL4(name, a1, a2, a3, a4) INTERNAL_SYSCALL (name, 4, a1, a2, a3, a4)
#    define __INTERNAL_SYSCALL5(name, a1, a2, a3, a4, a5) \
        INTERNAL_SYSCALL (name, 5, a1, a2, a3, a4, a5)
#    define __INTERNAL_SYSCALL6(name, a1, a2, a3, a4, a5, a6) \
        INTERNAL_SYSCALL (name, 6, a1, a2, a3, a4, a5, a6)
#    define __INTERNAL_SYSCALL7(name, a1, a2, a3, a4, a5, a6, a7) \
        INTERNAL_SYSCALL (name, 7, a1, a2, a3, a4, a5, a6, a7)

#    define __INTERNAL_SYSCALL_NARGS_X(a, b, c, d, e, f, g, h, n, ...) n
#    define __INTERNAL_SYSCALL_NARGS(...) \
        __INTERNAL_SYSCALL_NARGS_X (__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0, )
#    define __INTERNAL_SYSCALL_DISP(b, ...) \
        __SYSCALL_CONCAT (b, __INTERNAL_SYSCALL_NARGS (__VA_ARGS__)) (__VA_ARGS__)

/* Issue a syscall defined by syscall number plus any other argument required.
   It is similar to INTERNAL_SYSCALL macro, but without the need to pass the
   expected argument number as second parameter.  */
#    define INTERNAL_SYSCALL_CALL(...) __INTERNAL_SYSCALL_DISP (__INTERNAL_SYSCALL, __VA_ARGS__)

#    define __INTERNAL_SYSCALL_NCS0(name) INTERNAL_SYSCALL_NCS (name, 0)
#    define __INTERNAL_SYSCALL_NCS1(name, a1) INTERNAL_SYSCALL_NCS (name, 1, a1)
#    define __INTERNAL_SYSCALL_NCS2(name, a1, a2) INTERNAL_SYSCALL_NCS (name, 2, a1, a2)
#    define __INTERNAL_SYSCALL_NCS3(name, a1, a2, a3) INTERNAL_SYSCALL_NCS (name, 3, a1, a2, a3)
#    define __INTERNAL_SYSCALL_NCS4(name, a1, a2, a3, a4) \
        INTERNAL_SYSCALL_NCS (name, 4, a1, a2, a3, a4)
#    define __INTERNAL_SYSCALL_NCS5(name, a1, a2, a3, a4, a5) \
        INTERNAL_SYSCALL_NCS (name, 5, a1, a2, a3, a4, a5)
#    define __INTERNAL_SYSCALL_NCS6(name, a1, a2, a3, a4, a5, a6) \
        INTERNAL_SYSCALL_NCS (name, 6, a1, a2, a3, a4, a5, a6)
#    define __INTERNAL_SYSCALL_NCS7(name, a1, a2, a3, a4, a5, a6, a7) \
        INTERNAL_SYSCALL_NCS (name, 7, a1, a2, a3, a4, a5, a6, a7)

#    define INTERNAL_SYSCALL_NCS_CALL(...) \
        __INTERNAL_SYSCALL_DISP (__INTERNAL_SYSCALL_NCS, __VA_ARGS__)

#    define __INLINE_SYSCALL0(name) INLINE_SYSCALL (name, 0)
#    define __INLINE_SYSCALL1(name, a1) INLINE_SYSCALL (name, 1, a1)
#    define __INLINE_SYSCALL2(name, a1, a2) INLINE_SYSCALL (name, 2, a1, a2)
#    define __INLINE_SYSCALL3(name, a1, a2, a3) INLINE_SYSCALL (name, 3, a1, a2, a3)
#    define __INLINE_SYSCALL4(name, a1, a2, a3, a4) INLINE_SYSCALL (name, 4, a1, a2, a3, a4)
#    define __INLINE_SYSCALL5(name, a1, a2, a3, a4, a5) INLINE_SYSCALL (name, 5, a1, a2, a3, a4, a5)
#    define __INLINE_SYSCALL6(name, a1, a2, a3, a4, a5, a6) \
        INLINE_SYSCALL (name, 6, a1, a2, a3, a4, a5, a6)
#    define __INLINE_SYSCALL7(name, a1, a2, a3, a4, a5, a6, a7) \
        INLINE_SYSCALL (name, 7, a1, a2, a3, a4, a5, a6, a7)

#    define __INLINE_SYSCALL_NARGS_X(a, b, c, d, e, f, g, h, n, ...) n
#    define __INLINE_SYSCALL_NARGS(...) \
        __INLINE_SYSCALL_NARGS_X (__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0, )
#    define __INLINE_SYSCALL_DISP(b, ...) \
        __SYSCALL_CONCAT (b, __INLINE_SYSCALL_NARGS (__VA_ARGS__)) (__VA_ARGS__)

/* Issue a syscall defined by syscall number plus any other argument
   required.  Any error will be handled using arch defined macros and errno
   will be set accordingly.
   It is similar to INLINE_SYSCALL macro, but without the need to pass the
   expected argument number as second parameter.  */
#    define INLINE_SYSCALL_CALL(...) __INLINE_SYSCALL_DISP (__INLINE_SYSCALL, __VA_ARGS__)

#    define __INTERNAL_SYSCALL_NCS0(name) INTERNAL_SYSCALL_NCS (name, 0)
#    define __INTERNAL_SYSCALL_NCS1(name, a1) INTERNAL_SYSCALL_NCS (name, 1, a1)
#    define __INTERNAL_SYSCALL_NCS2(name, a1, a2) INTERNAL_SYSCALL_NCS (name, 2, a1, a2)
#    define __INTERNAL_SYSCALL_NCS3(name, a1, a2, a3) INTERNAL_SYSCALL_NCS (name, 3, a1, a2, a3)
#    define __INTERNAL_SYSCALL_NCS4(name, a1, a2, a3, a4) \
        INTERNAL_SYSCALL_NCS (name, 4, a1, a2, a3, a4)
#    define __INTERNAL_SYSCALL_NCS5(name, a1, a2, a3, a4, a5) \
        INTERNAL_SYSCALL_NCS (name, 5, a1, a2, a3, a4, a5)
#    define __INTERNAL_SYSCALL_NCS6(name, a1, a2, a3, a4, a5, a6) \
        INTERNAL_SYSCALL_NCS (name, 6, a1, a2, a3, a4, a5, a6)
#    define __INTERNAL_SYSCALL_NCS7(name, a1, a2, a3, a4, a5, a6, a7) \
        INTERNAL_SYSCALL_NCS (name, 7, a1, a2, a3, a4, a5, a6, a7)

/* Issue a syscall defined by syscall number plus any other argument required.
   It is similar to INTERNAL_SYSCALL_NCS macro, but without the need to pass
   the expected argument number as third parameter.  */
#    define INTERNAL_SYSCALL_NCS_CALL(...) \
        __INTERNAL_SYSCALL_DISP (__INTERNAL_SYSCALL_NCS, __VA_ARGS__)

/* Cancellation macros.  */
/* Adjust both the __syscall_cancel and the SYSCALL_CANCEL macro to support
   7 arguments instead of default 6 (curently only mip32).  It avoid add
   the requirement to each architecture to support 7 argument macros
   {INTERNAL,INLINE}_SYSCALL.  */
#    ifdef HAVE_CANCELABLE_SYSCALL_WITH_7_ARGS
#        define __SYSCALL_CANCEL7_ARG_DEF __syscall_arg_t a7,
#        define __SYSCALL_CANCEL7_ARCH_ARG_DEF , __syscall_arg_t a7
#        define __SYSCALL_CANCEL7_ARG 0,
#        define __SYSCALL_CANCEL7_ARG7 a7,
#        define __SYSCALL_CANCEL7_ARCH_ARG7 , a7
#    else
#        define __SYSCALL_CANCEL7_ARG_DEF
#        define __SYSCALL_CANCEL7_ARCH_ARG_DEF
#        define __SYSCALL_CANCEL7_ARG
#        define __SYSCALL_CANCEL7_ARG7
#        define __SYSCALL_CANCEL7_ARCH_ARG7
#    endif
#define __SYSCALL_CONCAT(a,b)			__SYSCALL_CONCAT_X (a, b)
#    define TYPEFY(X, name) __typeof__ (ARGIFY (X)) name
#    define TYPEFY1(X) __typeof__ ((X) - (X))
#    define ARGIFY(X) ((TYPEFY1 (X)) (X))
#    define REGISTERS_CLOBBERED_BY_SYSCALL "cc", "r11", "cx"

#if 1
#define __SYSCALL_CANCEL4(name, a1, a2, a3, a4) \
  __syscall_cancel (__SSC (a1), __SSC (a2), __SSC (a3),	\
  __SSC(a4), 0, 0, __SYSCALL_CANCEL7_ARG __NR_##name)
#endif

#define INTERNAL_SYSCALL_NCS(number, nr, args...) internal_syscall##nr (number, args)

#define internal_syscall6(number, arg1, arg2, arg3, arg4, arg5, arg6)                  \
        ({                                                                                 \
            unsigned long int resultvar;                                                   \
            TYPEFY (arg6, __arg6)                   = ARGIFY (arg6);                       \
            TYPEFY (arg5, __arg5)                   = ARGIFY (arg5);                       \
            TYPEFY (arg4, __arg4)                   = ARGIFY (arg4);                       \
            TYPEFY (arg3, __arg3)                   = ARGIFY (arg3);                       \
            TYPEFY (arg2, __arg2)                   = ARGIFY (arg2);                       \
            TYPEFY (arg1, __arg1)                   = ARGIFY (arg1);                       \
            register TYPEFY (arg6, _a6) asm ("r9")  = __arg6;                              \
            register TYPEFY (arg5, _a5) asm ("r8")  = __arg5;                              \
            register TYPEFY (arg4, _a4) asm ("r10") = __arg4;                              \
            register TYPEFY (arg3, _a3) asm ("rdx") = __arg3;                              \
            register TYPEFY (arg2, _a2) asm ("rsi") = __arg2;                              \
            register TYPEFY (arg1, _a1) asm ("rdi") = __arg1;                              \
            asm volatile ("syscall\n\t"                                                    \
                          : "=a"(resultvar)                                                \
                          : "0"(number), "r"(_a1), "r"(_a2), "r"(_a3), "r"(_a4), "r"(_a5), \
                            "r"(_a6)                                                       \
                          : "memory", REGISTERS_CLOBBERED_BY_SYSCALL);                     \
            (long int)resultvar;                                                           \
        })


extern __SYSCALL_CANCEL_CALL(openat, AT_FDCWD, file, oflag, mode);

extern INTERNAL_SYSCALL_NCS_CALL(__SSC (AT_FDCWD), __SSC (file), __SSC (oflag), __SSC(mode), 0, 0, __SYSCALL_CANCEL7_ARG __NR_openat);

int main (int argc, char* argv[])
{
	return 0;
}
