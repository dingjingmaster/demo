/*************************************************************************
> FileName: getpeername.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 20 Feb 2024 10:02:52 AM CST
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 10000

int main (int argc, char* argv[])
{
    int serverFd = socket (AF_INET, SOCK_STREAM, 0);
    if (serverFd  <= 0) {
        perror("socket failed!");
        exit (-1);
    }

    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind (serverFd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("bind error");
        exit (-1);
    }

    if (listen(serverFd, 3) < 0) {
        perror("bind error");
        exit(-1);
    }

    printf ("server listening on port %d\n", PORT);

    int sockLen = sizeof (addr);
    int newSock = 0;
    while ((newSock = accept(serverFd, (struct sockaddr*) &addr, (socklen_t*) &sockLen))) {
        if (newSock < 0) {
            printf ("connect error!\n");
            continue;
        }

        int sockSize;
        int readByte = 0;
        char tmp[256] = {0};
        int clientBuf[1024] = {0};
        struct sockaddr_in addrT;

        if (getpeername(newSock, (struct sockaddr*) &addrT, &sockSize) < 0) {
            printf ("getpeername error!\n");
            close(newSock);
            continue;
        }

        inet_ntop(AF_INET, &addrT.sin_addr, tmp, sizeof(tmp));
        printf ("client: %s:%d\n", tmp, addrT.sin_port);

        int flags = fcntl (newSock, F_GETFL, 0);
        if (-1 == flags) {
            perror("fcntl get failed!");
            close(newSock);
            continue;
        }
        if (-1 == fcntl(newSock, F_SETFL, flags | O_NONBLOCK)) {
            perror("fcntl set error!");
            close(newSock);
            continue;
        }

        while (1) {
            readByte = read(newSock, clientBuf, sizeof(clientBuf) - 1);
            if ((readByte == 0) || (0 == strcmp(clientBuf, "exit\n"))) {
                printf("\e[31mclose!\e[0m\n");
                close(newSock);
                break;
            }
            else if (readByte < 0) {
                if (EAGAIN == errno) { continue; }
                printf ("error[%2d]: %s\n", errno, strerror(errno));
                close(newSock);
                break;
            }

            printf ("%s", clientBuf);
            memset (clientBuf, 0, sizeof(clientBuf));
        }
    }

    return 0;
}
