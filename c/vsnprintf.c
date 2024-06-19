/*************************************************************************
> FileName: vsnprintf.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 18 Jun 2024 02:25:50 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdarg.h>


int calculate_length(const char *format, va_list args) 
{
    char buffer[1];
    int length;

    // 使用 vsnprintf 来确定所需的长度
    length = vsnprintf(buffer, 1, format, args);

    // 由于 buffer 只有 1 个字符的空间，vsnprintf 会返回至少需要的字符数
    // 这包括了格式化字符串中所有的字符以及任何额外的参数
    return length;
}

void text (const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    int len = calculate_length(fmt, args);
    printf ("%d\n", len);

    va_end(args);
}

int main (void)
{
    text("asdasda");
    text("%s", "asdasda");

    return 0;
}
