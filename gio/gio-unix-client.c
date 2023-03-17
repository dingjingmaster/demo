/*************************************************************************
> FileName: gio-unix-client.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 16 Mar 2023 10:58:06 AM CST
 ************************************************************************/
#include <gio/gio.h>


int main (int argc, char* argv[])
{
    const char* sockPath = argv[1];

    g_autoptr(GError) error = NULL;

    g_autoptr(GSocketClient) client = g_socket_client_new();

    g_autoptr(GSocketAddress) sock = g_unix_socket_address_new(sockPath);

    g_autoptr(GSocketConnection) conn = g_socket_client_connect(client, sock, NULL, &error);
    if (error) {
        printf ("error: %s\n", error->message);
        return -1;
    }

    gchar* buffer = "hello";

    GOutputStream* out = g_io_stream_get_output_stream (conn);
    g_output_stream_write(out, buffer, strlen(buffer), NULL, &error);
    if (error) {
        printf ("error: %s\n", error->message);
        return -1;
    }



    return 0;
}
