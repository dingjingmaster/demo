/*************************************************************************
> FileName: get-keyboard1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月13日 星期五 11时52分37秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>

int is_keyboard_device(const char *device_path) {
    int fd;
    unsigned long evbit[2] = {0};
    unsigned long keybit[KEY_MAX/8 + 1] = {0};

    // 打开设备文件
    fd = open(device_path, O_RDONLY);
    if (fd < 0) {
        return 0;
    }

    // 获取设备支持的事件类型
    if (ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), evbit) < 0) {
        close(fd);
        return 0;
    }

    // 检查是否支持 EV_KEY 事件
    if (!(evbit[0] & (1 << EV_KEY))) {
        close(fd);
        return 0;
    }

    // 获取支持的按键类型
    if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit) < 0) {
        close(fd);
        return 0;
    }

    // 检查是否支持任何字母键
    int has_letter_keys = 0;
    for (int i = KEY_A; i <= KEY_Z; i++) {
        if (keybit[i/8] & (1 << (i % 8))) {
            has_letter_keys = 1;
            break;
        }
    }

    // 检查是否支持任何数字键
    int has_number_keys = 0;
    for (int i = KEY_1; i <= KEY_0; i++) {
        if (keybit[i/8] & (1 << (i % 8))) {
            has_number_keys = 1;
            break;
        }
    }

    // 检查是否支持任何功能键
    int has_function_keys = 0;
    for (int i = KEY_F1; i <= KEY_F12; i++) {
        if (keybit[i/8] & (1 << (i % 8))) {
            has_function_keys = 1;
            break;
        }
    }

    close(fd);

    // 如果支持 EV_KEY 并且至少支持字母键或数字键，则认为是键盘
    return (has_letter_keys || has_number_keys || has_function_keys);
}

int main() {
    int i;
    char device_path[256];
    char name[256] = "Unknown";

    // 遍历所有可能的输入设备
    for (i = 0; i < 32; i++) {
        snprintf(device_path, sizeof(device_path), "/dev/input/event%d", i);

        if (is_keyboard_device(device_path)) {
            // 获取设备名称
            int fd = open(device_path, O_RDONLY);
            if (fd >= 0) {
                if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) >= 0) {
                    printf("找到键盘设备: %s (%s)\n", device_path, name);
                }
                close(fd);
            }
        }
    }

    return 0;
}
