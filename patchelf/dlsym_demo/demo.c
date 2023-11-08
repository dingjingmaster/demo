#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <string.h>  
  
#define BUF_SIZE 1024  
  
int main() {  
    int fd;  
    char buf[BUF_SIZE];  
    ssize_t bytes_written;  
  
    // 打开文件  
    fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);  
    if (fd == -1) {  
        perror("open");  
        exit(EXIT_FAILURE);  
    }  
  
    // 写入数据  
    strcpy(buf, "Hello, world!\n");  
    bytes_written = write(fd, buf, strlen(buf));  
    if (bytes_written == -1) {  
        perror("write");  
        exit(EXIT_FAILURE);  
    }  
  
    // 关闭文件  
    close(fd);  
  
    return 0;  
}
