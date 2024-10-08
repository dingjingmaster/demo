#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>

#define IPC_CLIENT_SOCKET_PATH                      "/usr/local/andsec/start/andsec_status.sock"


typedef enum _IpcServerType
{
    IPC_TYPE_FM_MENU_DECRYPT_FILE = 6
} IpcServerType;

struct __attribute__((packed)) IpcMessage
{
    unsigned int        type;                       // 处理类型：IpcServerType、IpcClientType
    unsigned long       dataLen;
    char                data[];
};

struct __attribute__((packed)) DataInfo
{
    unsigned long       dataLen;                    // 具体数据大小
    char                data[];                     // 具体数据
};

void send_data_to_daemon(IpcServerType type, char* files, unsigned int dataLen);

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要发送的文件绝对路径\n");
        return -1;
    }

    send_data_to_daemon(IPC_TYPE_FM_MENU_DECRYPT_FILE, argv[1], strlen(argv[1]));

    return 0;
}


void send_data_to_daemon(IpcServerType type, char* files, unsigned int dataLen)
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
    snprintf(address.sun_path, 108, IPC_CLIENT_SOCKET_PATH);
    int len = sizeof(address);

    if (0 != connect(sockFd, (struct sockaddr*)&address, len)) {
        printf("connect error\n");
        return;
    }

    struct IpcMessage msgBuf;
    memset(&msgBuf, 0, sizeof(msgBuf));

    struct DataInfo data;
    memset(&data, 0, sizeof(data));

    data.dataLen = dataLen;

    msgBuf.type = type;
    msgBuf.dataLen = sizeof(data) + dataLen;

    int allLen = dataLen + sizeof(data) + sizeof(msgBuf);
    char* sendBuf = (char*) malloc (allLen);
    if (sendBuf) {
        memset(sendBuf, 0, allLen);
        memcpy(sendBuf, &msgBuf, sizeof(msgBuf));
        memcpy(sendBuf + sizeof(msgBuf), &data, sizeof(data));
        memcpy(sendBuf + sizeof(msgBuf) + sizeof(data), files, dataLen);
        send(sockFd, (void*) sendBuf, allLen, 0);
        free(sendBuf);
    }

    close(sockFd);

    return;
}

