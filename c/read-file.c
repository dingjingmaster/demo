/*************************************************************************
> FileName: read-file.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 20 Feb 2025 09:26:04 AM CST
 ************************************************************************/
#include <enca.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main (int argc, char* argv[])
{
	if (argc < 2) {
		printf ("输入要读取的文件\n");
		return 1;
	}

    struct stat fileStats;
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("File %s not found!\n", argv[1]);
		return 2;
    }
    if (0 != fstat(fd, &fileStats)) {
        printf("fstat failed\n");
        return 3;
    }

	//EncaAnalyser an = enca_analyser_alloc("zh_CN");
	//if (!an) {
	//	printf ("an error\n");
	//	return 3;
	//}

	//enca_set_threshold(an, 1.38);
	//enca_set_multibyte(an, 1);
	//enca_set_ambiguity(an, 1);
	//enca_set_garbage_test(an, 1);

    unsigned long fileSize = fileStats.st_size;
	unsigned long length = (fileSize / sizeof(uint32_t)) + 2;
    printf("File size: %ld\n", length * sizeof(uint32_t));
    char* output = (char*) malloc (length * sizeof(uint32_t) + 2);
    memset(output, 0, length * sizeof(uint32_t) + 2);
    const ssize_t size = read(fd, output, length * sizeof(uint32_t));

	//enca_analyser_free(an);
    close(fd);

    write(STDOUT_FILENO, output, size);

	return 0;
}
