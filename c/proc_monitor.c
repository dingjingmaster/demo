#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/connector.h>
#include <linux/cn_proc.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main() {
    struct sockaddr_nl addr;
    struct nlmsghdr *nlh = NULL;
    struct cn_msg *cn_msg;
    int sock;
    char buf[BUFFER_SIZE];
    int ret;

    // 创建 netlink socket
    sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
    if (sock == -1) {
        perror("socket");
        return -1;
    }

    // 设置本地地址
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_groups = CN_IDX_PROC;
    addr.nl_pid = getpid();

    // 绑定 socket
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(sock);
        return -1;
    }

    // 订阅进程事件
    nlh = (struct nlmsghdr *)buf;
    cn_msg = (struct cn_msg *)NLMSG_DATA(nlh);
    nlh->nlmsg_len = NLMSG_LENGTH(sizeof(struct cn_msg) + sizeof(enum proc_cn_mcast_op));
    nlh->nlmsg_type = NLMSG_DONE;
    nlh->nlmsg_flags = 0;
    nlh->nlmsg_seq = 0;
    nlh->nlmsg_pid = getpid();

    cn_msg->id.idx = CN_IDX_PROC;
    cn_msg->id.val = CN_VAL_PROC;
    cn_msg->seq = 0;
    cn_msg->ack = 0;
    cn_msg->len = sizeof(enum proc_cn_mcast_op);

    // 发送订阅消息
    ret = send(sock, nlh, nlh->nlmsg_len, 0);
    if (ret == -1) {
        perror("send");
        close(sock);
        return -1;
    }

    printf("开始监听进程事件...\n");

    // 接收并处理事件
    while (1) {
        ret = recv(sock, buf, sizeof(buf), 0);
        if (ret == -1) {
            if (errno == EINTR)
                continue;
            perror("recv");
            break;
        }

        nlh = (struct nlmsghdr *)buf;
        cn_msg = (struct cn_msg *)NLMSG_DATA(nlh);
        struct proc_event *ev = (struct proc_event *)cn_msg->data;

        switch (ev->what) {
            case PROC_EVENT_FORK:
                printf("进程创建: 父进程 PID=%d (TGID=%d), 子进程 PID=%d (TGID=%d)\n",
                       ev->event_data.fork.parent_pid,
                       ev->event_data.fork.parent_tgid,
                       ev->event_data.fork.child_pid,
                       ev->event_data.fork.child_tgid);
                break;
            case PROC_EVENT_EXIT:
                // 只有当 PID 等于 TGID 时，才认为是真正的进程退出
                if (ev->event_data.exit.process_pid == ev->event_data.exit.process_tgid) {
                    printf("进程退出: PID=%d (TGID=%d), 退出码=%d\n",
                           ev->event_data.exit.process_pid,
                           ev->event_data.exit.process_tgid,
                           ev->event_data.exit.exit_code);
                } else {
                    printf("线程退出: PID=%d (TGID=%d), 退出码=%d\n",
                           ev->event_data.exit.process_pid,
                           ev->event_data.exit.process_tgid,
                           ev->event_data.exit.exit_code);
                }
                break;
        }
    }

    close(sock);
    return 0;
} 