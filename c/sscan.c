/*************************************************************************
> FileName: sscan.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri Jan 10 15:41:16 2025
 ************************************************************************/
#include <stdio.h>


int main() 
{
	char* sentence = "//aa/vv/dd/ee/";
	char str1[20] = {0};
	char str2[20] = {0};

	sscanf(sentence, "%s//%s", str1, str2);

	printf("%s -> %s\n", str1, str2);

	return 0;
}
