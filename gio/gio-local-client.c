/*************************************************************************
> FileName: gio-local-client.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 23 Mar 2023 05:21:23 PM CST
 ************************************************************************/

#include <glib.h>
#include <sys/un.h>
#include <stdbool.h>
#include <gio/gio.h>
#include <sys/socket.h>

int main (int argc, char *argv[])
{
    /* initialize glib */
    g_type_init ();

    GError * error = NULL;

    struct sockaddr_un addrT;
    memset (&addrT, 0, sizeof (addrT));
    addrT.sun_family = AF_LOCAL;
    strncpy (addrT.sun_path, "./a.sock", sizeof(addrT.sun_path) - 1);
    GSocketAddress* addr = g_socket_address_new_from_native(&addrT, sizeof (addrT));
    if (addr) {
        GSocket* sock = g_socket_new (G_SOCKET_FAMILY_UNIX, G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_DEFAULT, &error);
        if (sock) {
            if (g_socket_connect(sock, addr, NULL, &error)) {
                if (g_socket_condition_wait(sock, G_IO_OUT, NULL, &error)) {
                    const char* data = "Hello,World,This is local socket!";
                    if (g_socket_send_with_blocking(sock, data, strlen(data), true, NULL, &error)) {
                        printf ("send data to server OK!\n");
                    }
                    else {
                        printf("sendDataToServer error: %s\n", error->message);
                    }
                }
                else {
                    printf("g_socket_condition_wait error: %s\n", error->message);
                }
            }
            else {
                printf("g_socket_connect error: %s\n", error->message);
            }
            g_object_unref(sock);
        }
        else {
            printf("g_socket_new error: %s\n", error->message);
        }
        g_object_unref(addr);
    } while (0);

    if (error) { g_error_free(error); }

    return 0;
}
