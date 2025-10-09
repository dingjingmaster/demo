#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>

int is_remote_port_open(const char *ip, int port, int timeout_sec) 
{
    int sockfd;
    struct sockaddr_in addr;
    int ret;
    fd_set fdset;
    struct timeval tv;
    long arg;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    // 设置为非阻塞模式
    arg = fcntl(sockfd, F_GETFL, NULL);
    fcntl(sockfd, F_SETFL, arg | O_NONBLOCK);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        if (errno != EINPROGRESS) {
            close(sockfd);
            return 0;  // 连接立即失败 -> 不通或端口关闭
        }
    }

    // 使用 select 等待连接完成
    FD_ZERO(&fdset);
    FD_SET(sockfd, &fdset);
    tv.tv_sec = timeout_sec;
    tv.tv_usec = 0;

    ret = select(sockfd + 1, NULL, &fdset, NULL, &tv);
    if (ret > 0) {
        int so_error;
        socklen_t len = sizeof(so_error);
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len);
        if (so_error == 0) {
            close(sockfd);
            return 1;  // ✅ 连接成功 -> 端口开放
        } else {
            close(sockfd);
            return 0;  // ❌ 连接被拒绝或错误
        }
    } else if (ret == 0) {
        close(sockfd);
        return 0; // ❌ 超时
    } else {
        close(sockfd);
        return 0; // ❌ select 出错
    }
}

int main(int argc, char* argv[]) 
{
    if (argc != 3) {
        printf ("please input: %s <ip> <port>\n", argv[0]);
        return 1;
    }
    const char *server_ip = argv[1];
    int port = atol(argv[2]);
    int timeout = 2; // 2秒超时

    int result = is_remote_port_open(server_ip, port, timeout);
    if (result == 1) {
        printf("远程 %s:%d 端口开放（可连接）\n", server_ip, port);
    } 
    else if (result == 0) {
        printf("远程 %s:%d 不可用或被防火墙拦截\n", server_ip, port);
    } 
    else {
        printf("检测失败\n");
    }

    return 0;
}
