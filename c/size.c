/*************************************************************************
> FileName: size.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月27日 星期日 17时41分35秒
 ************************************************************************/
#include <stdio.h>

int main ()
{
	printf("33 << 20: %lu(%lu M)\n", 33 << 20, (33 << 20) / (1024 * 1024));

	return 0;
}
