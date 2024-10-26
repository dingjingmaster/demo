/*************************************************************************
> FileName: gio-local-server.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 23 Mar 2023 05:21:38 PM CST
 ************************************************************************/

#include <glib.h>
#include <sys/un.h>
#include <stdbool.h>
#include <gio/gio.h>
#include <sys/socket.h>

/* this function will get called everytime a client attempts to connect */
gboolean incoming_callback (GSocketService *service, GSocketConnection *connection, GObject *source_object, gpointer user_data)
{
    g_print("Received Connection from client!\n");
    GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
    gchar message[1024];
    g_input_stream_read (istream, message, 1024, NULL, NULL);
    g_print("Message was: \"%s\"\n", message);
    
    return FALSE;
}

int main (int argc, char **argv)
{
    /* initialize glib */
    g_type_init();

    GError * error = NULL;

    GSocket* mServerSocket = g_socket_new (G_SOCKET_FAMILY_UNIX, G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_DEFAULT, &error);
    if (error) {
        printf("g_socket_new error: %s\n", error->message);
        g_abort();
        return -1;
    }

    g_socket_set_blocking (mServerSocket, true);

    g_remove ("./a.sock");

    struct sockaddr_un addrT;
    memset (&addrT, 0, sizeof (addrT));
    addrT.sun_family = AF_LOCAL;
    strncpy (addrT.sun_path, "./a.sock", sizeof(addrT.sun_path) - 1);
    GSocketAddress* mServerAddr = g_socket_address_new_from_native(&addrT, sizeof (addrT));
    if (!g_socket_bind (mServerSocket, mServerAddr, false, &error)) {
        printf("bind error: %s\n", error->message);
        g_abort();
        return -1;
    }

    if (!g_socket_listen (mServerSocket, &error)) {
        printf("listen error: %s\n", error->message);
        g_abort();
        return -1;
    }

    GSocketService* mServer = g_socket_service_new();
    g_socket_listener_add_socket (G_SOCKET_LISTENER(mServer), mServerSocket, NULL, &error);
    if (error) {
        printf("g_socket_listener_add_socket error: %s\n", error->message);
        g_abort();
        return -1;
    }

    /* listen to the 'incoming' signal */
    g_signal_connect (mServer, "incoming", G_CALLBACK (incoming_callback), NULL);

    /* start the socket service */
    g_socket_service_start (mServer);

    /* enter mainloop */
    g_print ("Waiting for client!\n");

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
  
    return 0;
}
