/*************************************************************************
> FileName: glib-internationalization.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 15 Mar 2023 11:01:02 AM CST
 ************************************************************************/
#include <glib.h>
#include <glib/gi18n.h>

int main (int argc, char* argv[])
{
    printf ("_()    -- 标记要翻译的字符串，在运行时候替换为翻译后的字符串\n");
    printf ("Q_()   -- 作用\n");
    printf ("N_()   -- 作用\n");
    printf ("NC_()  -- 作用\n");
    printf ("C_()   -- 作用同_()，可以标出上下文：C_(\"Navigation\", \"Back\"); C_(\"b\", \"Back\")，这两个翻译可以不一样\n");

    return 0;
}
