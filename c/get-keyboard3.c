/*************************************************************************
> FileName: get-keyboard3.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月13日 星期五 11时55分35秒
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
    char name[256] = "Unknown";
    
    // 打开设备文件
    fd = open(device_path, O_RDONLY);
    if (fd < 0) {
        return 0;
    }
    
    // 获取设备名称
    if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) >= 0) {
        printf("检查设备: %s (%s)\n", device_path, name);
    }
    
    // 获取设备支持的事件类型
    if (ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), evbit) < 0) {
        close(fd);
        return 0;
    }
    
    // 检查是否支持 EV_KEY 事件
    if (!(evbit[0] & (1 << EV_KEY))) {
        printf("  不支持 EV_KEY 事件\n");
        close(fd);
        return 0;
    }
    
    // 获取支持的按键类型
    if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit) < 0) {
        close(fd);
        return 0;
    }
    
    // 检查是否支持字母键
    int has_letter_keys = 0;
    for (int i = KEY_A; i <= KEY_Z; i++) {
        if (keybit[i/8] & (1 << (i % 8))) {
            has_letter_keys++;
        }
    }
    
    // 检查是否支持数字键
    int has_number_keys = 0;
    for (int i = KEY_1; i <= KEY_0; i++) {
        if (keybit[i/8] & (1 << (i % 8))) {
            has_number_keys++;
        }
    }
    
    // 检查是否支持特殊键
    int has_special_keys = 0;
    if (keybit[KEY_ENTER/8] & (1 << (KEY_ENTER % 8))) has_special_keys++;
    if (keybit[KEY_SPACE/8] & (1 << (KEY_SPACE % 8))) has_special_keys++;
    if (keybit[KEY_BACKSPACE/8] & (1 << (KEY_BACKSPACE % 8))) has_special_keys++;
    
    printf("  字母键数量: %d\n", has_letter_keys);
    printf("  数字键数量: %d\n", has_number_keys);
    printf("  特殊键数量: %d\n", has_special_keys);
    
    close(fd);
    
    // 简化判断条件：只要支持 EV_KEY 并且有字母键或数字键就认为是键盘
    return (has_letter_keys > 0 || has_number_keys > 0);
}

int main() {
    int i;
    char device_path[256];
    
    // 遍历所有可能的输入设备
    for (i = 0; i < 32; i++) {
        snprintf(device_path, sizeof(device_path), "/dev/input/event%d", i);
        
        if (is_keyboard_device(device_path)) {
            printf("找到键盘设备: %s\n", device_path);
        }
    }
    
    return 0;
}
