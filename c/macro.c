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

int main (int argc, char* argv[])
{
    EMBED_STR(srcfile, "macro.c");
    printf ("\n%s\n\n", srcfile);

    return 0;
}
