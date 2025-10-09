#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int host_to_ip (const char* hostname, char* ipBuf, int ipBufLen)
{
    struct addrinfo hints, *res, *p;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // AF_INET 表示仅 IPv4，AF_UNSPEC 表示 IPv4+IPv6 都行
    hints.ai_socktype = SOCK_STREAM;  // 任意类型都可

    int status = getaddrinfo(hostname, NULL, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    printf("域名 %s 对应的 IP 地址:\n", hostname);

    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;

        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // 转成字符串
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("  %s: %s\n", ipver, ipstr);
        if (ipBuf) {
            memset(ipBuf, 0, ipBufLen);
            strncpy(ipBuf, ipstr, ipBufLen - 1);
        }
    }

    freeaddrinfo(res); // 释放内存

    return 0;

}

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
    char ipBuf[128] = {0};

    if (0 != host_to_ip (ip, ipBuf, sizeof(ipBuf))) {
        printf ("host to ip error!\n");
        return 1;
    }
    inet_pton(AF_INET, ipBuf, &addr.sin_addr);

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
