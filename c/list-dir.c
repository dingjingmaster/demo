/*************************************************************************
> FileName: list-dir.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed Jan 22 10:53:09 2025
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>


int main (int argc, char* argv[])
{
	if (argc < 2) {
		printf ("Usage: <dir path>\n");
		return -1;
	}

	const char* dirPath = argv[1];

	DIR* dir = opendir(dirPath);
	if (NULL == dir) {
		perror("opendir");
		return 1;
	}

	struct dirent* entry = NULL;
	printf ("Content of directory: %s\n", dirPath);
	while (NULL != (entry = readdir(dir))) {
		printf ("%s\n", entry->d_name);
	}
	closedir(dir);

	return 0;
}
