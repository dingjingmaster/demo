/*************************************************************************
> FileName: format-path.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu Jan  9 18:21:29 2025
 ************************************************************************/
#include <stdio.h>
#include <string.h>

void format_path (char* filePath);

int main (void)
{
	char path1[] = "/////////a/////////d//////////f/////////c////////f//////";
	char path2[] = "////////////a/////////b//////c/////////d////////////e/////f.txt";
	char path3[] = "";
	char path4[] = "/a/b/c/d";
	char path5[] = "/a/b/c/d/";
	char path6[] = "/a/b/c/d/a.txt";

	printf ("path1: %s\n", path1);
	format_path(path1);
	printf ("format-path1: %s\n", path1);

	printf ("path2: %s\n", path2);
	format_path(path2);
	printf ("format-path2: %s\n", path2);

	printf ("path3: %s\n", path3);
	format_path(path3);
	printf ("format-path3: %s\n", path3);

	printf ("path4: %s\n", path4);
	format_path(path4);
	printf ("format-path4: %s\n", path4);

	printf ("path5: %s\n", path5);
	format_path(path5);
	printf ("format-path5: %s\n", path5);

	printf ("path6: %s\n", path6);
	format_path(path6);
	printf ("format-path6: %s\n", path6);

	return 0;
}

void format_path (char* filePath)
{
	if (!filePath) { return; }

	int i = 0;
	int fLen = (int) strlen (filePath);
	for (i = 0; filePath[i]; ++i) {
		while (filePath[i] && '/' == filePath[i] && '/' == filePath[i + 1]) {
			for (int j = i; filePath[j] || j < fLen; filePath[j] = filePath[j + 1], ++j);
		}
	}

	if ((i - 1 >= 0) && filePath[i - 1] == '/') {
		filePath[i - 1] = '\0';
	}
}
