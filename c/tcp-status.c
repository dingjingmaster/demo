/*************************************************************************
> FileName: tcp-status.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年07月18日 星期五 16时50分58秒
 ************************************************************************/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/inet_diag.h>
#include <linux/sock_diag.h>
#include <linux/tcp.h>

#define TCP_ESTABLISHED 1
#define TCP_SYN_SENT    2
#define TCP_SYN_RECV    3
#define TCP_FIN_WAIT1   4
#define TCP_FIN_WAIT2   5
#define TCP_TIME_WAIT   6
#define TCP_CLOSE       7
#define TCP_CLOSE_WAIT  8
#define TCP_LAST_ACK    9
#define TCP_LISTEN      10
#define TCP_CLOSING     11
#define TCP_NEW_SYN_RECV 12

// TCP 状态掩码（对应 idiag_states）
#define TCPF_ESTABLISHED  (1 << TCP_ESTABLISHED)
#define TCPF_SYN_SENT     (1 << TCP_SYN_SENT)
#define TCPF_SYN_RECV     (1 << TCP_SYN_RECV)
#define TCPF_FIN_WAIT1    (1 << TCP_FIN_WAIT1)
#define TCPF_FIN_WAIT2    (1 << TCP_FIN_WAIT2)
#define TCPF_TIME_WAIT    (1 << TCP_TIME_WAIT)
#define TCPF_CLOSE        (1 << TCP_CLOSE)
#define TCPF_CLOSE_WAIT   (1 << TCP_CLOSE_WAIT)
#define TCPF_LAST_ACK     (1 << TCP_LAST_ACK)
#define TCPF_LISTEN       (1 << TCP_LISTEN)
#define TCPF_CLOSING      (1 << TCP_CLOSING)
#define TCPF_NEW_SYN_RECV (1 << TCP_NEW_SYN_RECV)

#define TCPF_ALL ( \
    TCPF_ESTABLISHED | TCPF_SYN_SENT | TCPF_SYN_RECV | \
    TCPF_FIN_WAIT1 | TCPF_FIN_WAIT2 | TCPF_TIME_WAIT | \
    TCPF_CLOSE | TCPF_CLOSE_WAIT | TCPF_LAST_ACK | \
    TCPF_LISTEN | TCPF_CLOSING | TCPF_NEW_SYN_RECV )

#define BUFFER_SIZE 8192

void parse_response(struct nlmsghdr *nlhT) 
{
    ssize_t remain = BUFFER_SIZE;
    struct nlmsghdr* nlh = nlhT;
    while (NLMSG_OK(nlh, BUFFER_SIZE)) {
        if (nlh->nlmsg_type == NLMSG_DONE) {
            printf ("finished!\n");
            break;
        }

        struct inet_diag_msg *diag_msg = NLMSG_DATA(nlh);
        struct sockaddr_in src, dst;

        memset(&src, 0, sizeof(src));
        memset(&dst, 0, sizeof(dst));

        src.sin_family = AF_INET;
        dst.sin_family = AF_INET;

        memcpy(&src.sin_addr, diag_msg->id.idiag_src, 4);
        memcpy(&dst.sin_addr, diag_msg->id.idiag_dst, 4);

        src.sin_port = ntohs(diag_msg->id.idiag_sport);
        dst.sin_port = ntohs(diag_msg->id.idiag_dport);

        char src_ip[INET_ADDRSTRLEN];
        char dst_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &src.sin_addr, src_ip, sizeof(src_ip));
        inet_ntop(AF_INET, &dst.sin_addr, dst_ip, sizeof(dst_ip));

        printf("TCP %s:%d -> %s:%d State: %u Inode: %u\n",
               src_ip, src.sin_port,
               dst_ip, dst.sin_port,
               diag_msg->idiag_state,
               diag_msg->idiag_inode);
        nlh = NLMSG_NEXT(nlh, remain);
    }
}

int main() {
    int sock = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_INET_DIAG);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct {
        struct nlmsghdr nlh;
        struct inet_diag_req_v2 req;
    } request;

    memset(&request, 0, sizeof(request));
    request.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(struct inet_diag_req_v2));
    request.nlh.nlmsg_type = SOCK_DIAG_BY_FAMILY;
    request.nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;

    request.req.sdiag_family = AF_INET;
    request.req.sdiag_protocol = IPPROTO_TCP;
    request.req.idiag_states = TCPF_ALL; // 所有 TCP 状态
    request.req.idiag_ext = 0;

    if (send(sock, &request, request.nlh.nlmsg_len, 0) < 0) {
        perror("send");
        close(sock);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t len;

    while ((len = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        struct nlmsghdr *nlh = (struct nlmsghdr*) buffer;
        parse_response(nlh);
    }

    if (len < 0) {
        perror("recv");
    }

    close(sock);

    return 0;
}
