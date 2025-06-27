/*************************************************************************
> FileName: read-file-range.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月27日 星期五 10时58分20秒
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
	if (argc != 4) {
		printf ("输入要读取的文件, 起始位置， 结束位置\n");
		return 1;
	}

    int64_t s = strtol(argv[2], NULL, 10);
    int64_t e = strtol(argv[3], NULL, 10);
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

    if (e > fileStats.st_size) {
        e = fileStats.st_size;
    }

    int64_t len = e - s + 2;
    char* output = (char*) malloc (len);
    memset(output, 0, len);
    lseek(fd, s, SEEK_SET);
    const ssize_t size = read(fd, output, len - 2);

    printf ("%s\n", output);

    close(fd);

    if (output) { free (output); }

	return 0;
}
