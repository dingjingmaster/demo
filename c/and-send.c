/*************************************************************************
> FileName: and-send.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 20 Aug 2024 07:42:32 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/socket.h>


#define STATUS_SOCKET_IPC                       "/usr/local/andsec/start/andsec_status.sock"

typedef struct MsgRequest
{
    int type;
    int len;
    char data[1024];
}MsgRequestType;


void send_message(const char* msg);

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要发送信息\n");
        return -1;
    }
    send_message(argv[1]);
    return 0;
}

void send_message(const char* msg)
{
    int val = 1;
    int sockFd = 0;
    int timeout = 2000;
    int revtimeout = 31 * 1000;
    struct sockaddr_un address;

    if ((sockFd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed\n");
        return;
    }

    setsockopt(sockFd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, &revtimeout, sizeof(revtimeout));
    setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path, 108, STATUS_SOCKET_IPC);
    int len = sizeof(address);

    if (0 != connect(sockFd, (struct sockaddr*)&address, len)) {
        printf("connect error\n");
        return;
    }

    MsgRequestType msgBuf;

    memset(&msgBuf, 0, sizeof(msgBuf));

    msgBuf.type = 2;
    msgBuf.len = strlen(msg);
    memcpy(&(msgBuf.data), msg, strlen(msg));

    send(sockFd, (void*) &msgBuf, strlen(msg) + sizeof(MsgRequestType), 0);
    close(sockFd);

    return;
}
