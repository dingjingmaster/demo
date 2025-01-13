/*************************************************************************
> FileName: test-ipc.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon Jan 13 15:21:02 2025
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/un.h>
#include <sys/socket.h>


#define STR_FREE(x) \
    do { \
        if (x) { \
            free (x); \
            x = NULL; \
        } \
    } while (0) \

#define IPC_SERVER_SOCKET_PATH              "/usr/local/andsec/start/sec_daemon.sock"


struct __attribute__((packed)) IpcMessage
{
    unsigned int        type;                       // 处理类型：IpcServerType、IpcClientType
    unsigned long       dataLen;
    char                data[];
};

int send_data_to_daemon(unsigned int type, const char* sendData, int sendDataLen, char* recvData, int recvDataBufLen);


int main (int argc, char* argv[])
{
	{
		// backup
		const char* file = "/tmp/file.b";

		send_data_to_daemon(22, file, strlen(file), NULL, 0);
	}

	return 0;
}


int send_data_to_daemon(unsigned int type, const char* sendData, int sendDataLen, char* recvData, int recvDataBufLen)
{
    int res = 0;
    int val = 1;
    int sockFd = 0;
    int timeout = 2000;
    int revtimeout = 31 * 1000;
    struct sockaddr_un address;

    char* sendBuf = NULL;
    char recvBuf[1024] = {0};

    if (sendDataLen < 0 || recvDataBufLen < 0) {
        syslog(LOG_ERR, "sendData: %p, recvData: %p, sendDataLen: %d, recvDataBufLen: %d", sendData, recvData, sendDataLen, recvDataBufLen);
        return -1;
    }

    do {
        if ((sockFd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
            syslog(LOG_ERR, "[IPC] socket() failed!");
            res = -1;
            break;
        }

        setsockopt(sockFd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, &revtimeout, sizeof(revtimeout));
        setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

        memset(&address, 0, sizeof(struct sockaddr_un));
        address.sun_family = AF_UNIX;
        snprintf(address.sun_path, 108, IPC_SERVER_SOCKET_PATH);
        int len = sizeof(address);

        if (0 != connect(sockFd, (struct sockaddr*)&address, len)) {
            syslog(LOG_ERR, "[IPC] connect error!");
            res = -1;
            break;
        }

        struct IpcMessage msgBuf;
        memset(&msgBuf, 0, sizeof(msgBuf));

        msgBuf.type = type;
        msgBuf.dataLen = sendDataLen;

        // 开始发送
        int allLen = (int) sizeof(struct IpcMessage) + sendDataLen;
        sendBuf = (char*) malloc (sizeof(char) * allLen);
        if (!sendBuf) {
            syslog(LOG_ERR, "[IPC] malloc is null!");
            res = -1;
            break;
        }
        memset(sendBuf, 0, allLen);
        memcpy(sendBuf, &msgBuf, sizeof(msgBuf));
        if (sendDataLen > 0) {
            memcpy(sendBuf + sizeof(msgBuf), sendData, sendDataLen);
        }
        write(sockFd, sendBuf, allLen);
        STR_FREE(sendBuf);

        // 开始接收
        if (recvData) {
            int ri = 0;

            ri = read(sockFd, recvBuf, sizeof(recvBuf));
            if (ri < sizeof(struct IpcMessage)) {
                res = -1;
                syslog(LOG_ERR, "[IPC] recvBuf len too small.");
            }
            else {
                struct IpcMessage* im = (struct IpcMessage*) recvBuf;
                memset(recvData, 0, recvDataBufLen);
                memcpy(recvData, im->data, im->dataLen);
            }
        }
    } while (0);

    STR_FREE(sendBuf);

    return res;
}
