/*************************************************************************
> FileName: strstr.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年08月20日 星期三 13时30分16秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>


int main (void)
{
    const char* keys = "|KEYFLAG_EDG|AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA|KEYFLAG_CDG8|8888888888888888|XXXX..12XXXX..12|XXXX1234|XXXX1234XXXX1234|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|";

    const char* keyFlags = "KEYFLAG_EDG|";

    char* subStr = strstr(keys, keyFlags);

    return 0;
}
