/*************************************************************************
> FileName: timerfd1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 26 Sep 2022 08:57:38 PM CST
 ************************************************************************/

#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>
#include <sys/timerfd.h>

int main() 
{
    struct itimerspec timebuf;
    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    struct timespec s = {1, 1};
    timebuf.it_interval = s; // period timeout value = 1s
    timebuf.it_value = s;  // initial timeout value = 5s100ns
    timerfd_settime(timerfd, 0, &timebuf, NULL);

    struct pollfd fds[1];
    int len = sizeof(fds) / sizeof(fds[0]);
    fds[0].fd = timerfd;
    fds[0].events = POLLIN | POLLERR | POLLHUP;

    while (1) {
        int n = poll(fds, len, -1);
        for (int i = 0; i < len && n-- > 0; ++i) {
            if (fds[i].revents & POLLIN) {
                uint64_t val;
                int ret = read(timerfd, &val, sizeof(val));
                if (ret != sizeof(val)) {
                    break;
                }
                printf ("timerfd = %d timeout!\n", timerfd);
            }
        }
    }
    close(timerfd);
    return 0;
}

