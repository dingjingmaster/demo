/*************************************************************************
> FileName: get-keyboard7.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月13日 星期五 12时41分27秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>

int is_keyboard_device(const char *device_path) {
    char sysfs_path[256];
    char value[256];
    FILE *fp;
    
    // 从 /dev/input/eventX 路径提取设备号
    int event_num;
    if (sscanf(device_path, "/dev/input/event%d", &event_num) != 1) {
        return 0;
    }
    
    // 构建 sysfs 路径
    snprintf(sysfs_path, sizeof(sysfs_path), 
             "/sys/class/input/event%d/device/properties", event_num);
    
    // 读取设备属性
    fp = fopen(sysfs_path, "r");
    if (!fp) {
        return 0;
    }
    
    // 检查 ID_INPUT_KEYBOARD 属性
    while (fgets(value, sizeof(value), fp)) {
        if (strstr(value, "ID_INPUT_KEYBOARD=1")) {
            fclose(fp);
            return 1;
        }
    }
    
    fclose(fp);
    return 0;
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
