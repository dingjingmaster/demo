#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>

#define MAX_PATH 1024
#define MAX_ENV_SIZE 4096

// 检查环境变量中是否包含图形界面相关的变量
int is_gui_process(const char *environ_data, size_t size) {
    const char *gui_indicators[] = {
        "DISPLAY=",
        "WAYLAND_DISPLAY=",
        "XDG_SESSION_TYPE=",
        "GDMSESSION=",
        "GNOME_DESKTOP_SESSION_ID=",
        "KDE_FULL_SESSION=",
        "DESKTOP_SESSION=",
        NULL
    };

    // 遍历所有环境变量
    const char *env = environ_data;
    while (env < environ_data + size) {
        for (int i = 0; gui_indicators[i] != NULL; i++) {
            if (strncmp(env, gui_indicators[i], strlen(gui_indicators[i])) == 0) {
                return 1;
            }
        }
        env += strlen(env) + 1;  // 移动到下一个环境变量
    }
    return 0;
}

// 读取进程的命令行
void get_process_cmdline(pid_t pid, char *cmdline, size_t size) {
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
    
    FILE *fp = fopen(path, "r");
    if (fp) {
        size_t read = fread(cmdline, 1, size - 1, fp);
        cmdline[read] = '\0';
        // 替换命令行中的 null 字符为空格
        for (size_t i = 0; i < read; i++) {
            if (cmdline[i] == '\0') {
                cmdline[i] = ' ';
            }
        }
        fclose(fp);
    } else {
        strncpy(cmdline, "unknown", size);
    }
}

int main() {
    DIR *proc_dir;
    struct dirent *entry;
    struct stat st;
    char environ_path[MAX_PATH];
    char cmdline[MAX_PATH];
    char environ_data[MAX_ENV_SIZE];
    FILE *fp;
    size_t read_size;

    // 打开 /proc 目录
    proc_dir = opendir("/proc");
    if (proc_dir == NULL) {
        perror("无法打开 /proc 目录");
        return 1;
    }

    printf("发现以下图形界面相关进程：\n");
    printf("PID\t命令行\n");
    printf("----------------------------\n");

    // 遍历 /proc 目录
    while ((entry = readdir(proc_dir)) != NULL) {
        char proc_path[MAX_PATH];
        snprintf(proc_path, sizeof(proc_path), "/proc/%s", entry->d_name);
        
        // 检查是否为数字目录（进程ID）且是目录
        if (strspn(entry->d_name, "0123456789") == strlen(entry->d_name) && 
            stat(proc_path, &st) == 0 && S_ISDIR(st.st_mode)) {
            pid_t pid = atoi(entry->d_name);
            
            // 构建 environ 文件路径
            snprintf(environ_path, sizeof(environ_path), "/proc/%d/environ", pid);
            
            // 读取进程的环境变量
            fp = fopen(environ_path, "r");
            if (fp) {
                read_size = fread(environ_data, 1, sizeof(environ_data) - 1, fp);
                environ_data[read_size] = '\0';
                fclose(fp);

                // 检查是否为图形界面进程
                if (is_gui_process(environ_data, read_size)) {
                    get_process_cmdline(pid, cmdline, sizeof(cmdline));
                    printf("%d\t%s\n", pid, cmdline);
                }
            }
        }
    }

    closedir(proc_dir);
    return 0;
} 