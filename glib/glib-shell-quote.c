/*************************************************************************
> FileName: glib-shell-quote.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 17 Feb 2025 09:47:43 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
	if (argc < 2) {
		printf ("输入文件\n");
			return 1;
	}

	char* shellPath = g_shell_quote(argv[1]);
	if (shellPath) {
		printf ("g_shell_quote: %s\n", shellPath);
		g_free (shellPath);
	}

	return 0;
}
