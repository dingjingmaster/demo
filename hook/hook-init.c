/*************************************************************************
> FileName: hook-init.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月17日 星期四 18时14分01秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>

void hook_init() __attribute__((constructor()));

void hook_init()
{
	printf("hook init\n");

	setenv("TEST", "111111", 1);
}
