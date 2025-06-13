/*************************************************************************
> FileName: get-keyboard2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月13日 星期五 11时54分38秒
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
    
    // 检查是否支持功能键
    int has_function_keys = 0;
    for (int i = KEY_F1; i <= KEY_F12; i++) {
        if (keybit[i/8] & (1 << (i % 8))) {
            has_function_keys++;
        }
    }
    
    // 检查是否支持特殊键
    int has_special_keys = 0;
    if (keybit[KEY_ENTER/8] & (1 << (KEY_ENTER % 8))) has_special_keys++;
    if (keybit[KEY_SPACE/8] & (1 << (KEY_SPACE % 8))) has_special_keys++;
    if (keybit[KEY_BACKSPACE/8] & (1 << (KEY_BACKSPACE % 8))) has_special_keys++;
    if (keybit[KEY_TAB/8] & (1 << (KEY_TAB % 8))) has_special_keys++;
    if (keybit[KEY_ESC/8] & (1 << (KEY_ESC % 8))) has_special_keys++;
    
    // 检查是否支持鼠标按键
    int has_mouse_keys = 0;
    if (keybit[BTN_LEFT/8] & (1 << (BTN_LEFT % 8))) has_mouse_keys++;
    if (keybit[BTN_RIGHT/8] & (1 << (BTN_RIGHT % 8))) has_mouse_keys++;
    if (keybit[BTN_MIDDLE/8] & (1 << (BTN_MIDDLE % 8))) has_mouse_keys++;
    
    close(fd);
    
    // 判断条件：
    // 1. 必须支持足够多的字母键（至少10个）
    // 2. 必须支持足够多的数字键（至少5个）
    // 3. 必须支持足够多的特殊键（至少3个）
    // 4. 不能支持太多鼠标按键（最多1个）
    return (has_letter_keys >= 10 && 
            has_number_keys >= 5 && 
            has_special_keys >= 3 && 
            has_mouse_keys <= 1);
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
