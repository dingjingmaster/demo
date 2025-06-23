/*************************************************************************
> FileName: ipc.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月18日 星期三 17时33分49秒
 ************************************************************************/
#include "ipc.h"

#include <stdio.h>
#include <sys/un.h>
#include <string.h>
#include <gio/gio.h>
#include <stdbool.h>
#include <sys/socket.h>

static void sendDataTo(const char* sockPath, const char* data, size_t dataLen)
{
    GError* error = NULL;

    do {
        GSocketFamily s_family = G_SOCKET_FAMILY_UNIX;
        struct sockaddr_un addrT;
        memset (&addrT, 0, sizeof (addrT));
        addrT.sun_family = AF_LOCAL;
        strncpy (addrT.sun_path, sockPath, strlen(sockPath));
        GSocketAddress* addr = g_socket_address_new_from_native(&addrT, sizeof (addrT));
        if (addr) {
            GSocket* sock = g_socket_new (s_family, G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_DEFAULT, &error);
            if (sock) {
                if (g_socket_connect(sock, addr, NULL, &error)) {
                    if (g_socket_condition_wait(sock, G_IO_OUT, NULL, &error)) {
                        if (g_socket_send_with_blocking(sock, data, dataLen, true, NULL, &error)) {
                            printf ("send data to client OK!\n");
                        }
                        else {
                            printf ("sendDataToClient error: %s\n", error->message);
                        }
                    }
                    else {
                        printf ("g_socket_condition_wait error: %s\n", error->message);
                    }
                }
                else {
                    printf ("g_socket_connect error: %s, sock path: %s\n", error->message, sockPath);
                }
                g_object_unref(sock);
            }
            else {
                printf ("g_socket_new error: %s\n", error->message);
            }
            g_object_unref(addr);
        }
    } while (0);

    if (error) { g_error_free(error); }
}


void sendDataToClient(const char* data, size_t dataLen)
{
    sendDataTo(IPC_CLIENT_SOCKET_PATH, data, dataLen);
}

void sendDataToDaemon(const char* data, size_t dataLen)
{
    sendDataTo(IPC_DAEMON_SOCKET_PATH, data, dataLen);
}
