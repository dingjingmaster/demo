/*************************************************************************
> FileName: opencc-1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月23日 星期一 19时30分07秒
 ************************************************************************/
#include "opencc.h"

#include <stdio.h>
#include <string.h>


int main() 
{
    opencc_t opencc = opencc_open("s2t.json");
    const char* input = "汉字";
    char* converted = opencc_convert_utf8(opencc, input, strlen(input));  // 漢字

    printf("input: %s, result: %s\n", input, converted);

    opencc_convert_utf8_free(converted);
    opencc_close(opencc);
  
    return 0;
}
