/*************************************************************************
> FileName: watch-dog.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 20 Feb 2024 03:24:27 PM CST
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>

int main (void)
{
    // 打开看门狗
    int fd = open ("/dev/watchdog", O_WRONLY);
    if (-1 == fd) {
        perror("无法打开看门狗!");
        exit(-1);
    }

    int timeout = 10;   // 定时器超时时间(以秒为单位)
    if (0 != ioctl(fd, WDIOC_SETTIMEOUT, &timeout)) {
        perror("无法打开看门狗定时器");
        close(fd);
        exit(-1);
    }

    // 喂狗
    while (1) {
        if (write(fd, "\0", 1) != 1) {
            perror("喂狗失败!");
            close(fd);
            exit(-1);
        }
        sleep(100);   // 每隔一秒喂狗
    }

    // 关闭看门狗设备
    close(fd);

    return 0;
}
