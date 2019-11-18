/*************************************************************************
> FileName: demo_character.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月18日 星期一 10时03分18秒
 ************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

int is_chinese(char c) 
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c == 95))
        return 1;
    return 0;
}

int main(void) 
{
    char str1[] = "中文";
    char str2[] = "english";
    char str3[] = "中文qeqweq中文";

    for (int i = 0; i < strlen(str1); ++i) {
        printf ("1: %d\n", is_chinese(str1[i]));
    }

    for (int i = 0; i < strlen(str2); ++i) {
        printf ("0: %d\n", is_chinese(str2[i]));
    }

    puts("---------------------------------------------\n");
    for (int i = 0; i < strlen(str3); ++i) {
        printf ("3: %d\n", is_chinese(str3[i]));
    }

    return 0;
}
