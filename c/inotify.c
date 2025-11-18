/*************************************************************************
> FileName: inotify.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月18日 星期二 11时33分44秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

int main() {
    int fd = inotify_init1(IN_NONBLOCK);
    if (fd < 0) { perror("inotify_init"); return 1; }

    int wd = inotify_add_watch(fd, "/dev", IN_CREATE | IN_DELETE);
    if (wd < 0) { perror("inotify_add_watch"); return 1; }

    char buf[BUF_LEN];

    printf("Listening for /dev device changes...\n");

    while (1) {
        int len = read(fd, buf, BUF_LEN);
        if (len < 0) { usleep(100000); continue; }

        int i = 0;
        while (i < len) {
            struct inotify_event *event = (struct inotify_event *)&buf[i];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    struct stat st;
                    char path[128] = {0};
                    snprintf(path, sizeof(path) - 1, "/dev/%s", event->name);
                    if (0 == lstat(path, &st) && (S_IFBLK == (S_IFMT & st.st_mode))) {
                        printf("Device created: %s\n", event->name);
                    }
                }
                if (event->mask & IN_DELETE) {
                    struct stat st;
                    char path[128] = {0};
                    snprintf(path, sizeof(path) - 1, "/dev/%s", event->name);
                    printf("Device removed: %s\n", event->name);
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
    return 0;
}
