/*************************************************************************
> FileName: hello.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月17日 星期四 18时16分02秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[])
{
	printf("main\n");

	const char* str = getenv("TEST");
	printf ("ENV: TEST=%s\n", str ? str : "<null>");

	return 0;
}
