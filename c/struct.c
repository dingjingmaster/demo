/*************************************************************************
> FileName: struct.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月11日 星期五 14时59分19秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    bool                            isReplaced;
	int a;
    const char*                     name;           // 函数名称
    unsigned long                   replaceFunc;    // 替换函数
    unsigned long                   origAddr;       // 被替换的函数

    unsigned long                   uPrivData1;     // 用户数据1
    unsigned long                   uPrivData2;     // 用户数据2
    unsigned long                   uPrivData3;     // 用户数据2
    unsigned long                   uPrivData4;     // 用户数据2
} KHookInfo;

int main (void)
{
	KHookInfo kh;

	printf ("%d\n", sizeof(KHookInfo));
}
