/*************************************************************************
> FileName: macro.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 28 Aug 2023 08:49:23 AM CST
 ************************************************************************/
#include <stdio.h>

#define EMBED_STR(name, path)                       \
    extern const char name[];                       \
    asm(".section .rodata, \"a\", @progbits\n"      \
        #name ":\n"                                 \
        ".incbin \"" path "\"\n"                    \
        ".byte 0\n"                                 \
        ".previous\n"                               \
    );

#define STR "Hello World"
#define MACRO_STR_LEN(x) (sizeof(x) - 1)

int main (int argc, char* argv[])
{
    EMBED_STR(srcfile, "macro.c");
    printf ("\n%s\n\n", srcfile);

    printf ("%s: %d", STR, MACRO_STR_LEN(STR));
    return 0;
}
