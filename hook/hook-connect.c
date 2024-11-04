#include <stdio.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

typedef int(*ConnectPtr)(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

ConnectPtr connectPtr = NULL;

void hook_connect_init() __attribute__((constructor()));
void hook_connect_cleanup() __attribute__((destructor()));

int connect (int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
    if (!connectPtr) {
        printf("connectPtr is null!\n");
        return -1;
    }

#define BUF_SIZE 2048
    int ret = 0;
    int port = 0;
    sa_family_t family = addr->sa_family;

    printf ("family: '%u'\n", family);

    char* buf1 = (char*) malloc (BUF_SIZE);
    if (!buf1) {
        ret = -1;
        goto err;
    }

    char* buf2 = (char*) malloc (BUF_SIZE);
    if (!buf2) {
        ret = -1;
        goto err;
    }

    memset(buf1, 0, BUF_SIZE);
    memset(buf2, 0, BUF_SIZE);

    // process
    if (0 >= readlink("/proc/self/exe", buf1, BUF_SIZE - 1)) {
        ret = -1;
        printf("readlink error!\n");
        goto err;
    }

    printf ("process[%d]: '%s'\n", getpid(), buf1);

    // ip && port
    memset(buf1, 0, BUF_SIZE);
    if (AF_INET != family) {
        goto out;
    }

    struct sockaddr_in* addrIn = (struct sockaddr_in*) addr;
    inet_ntop(AF_INET, &(addrIn->sin_addr), buf1, BUF_SIZE);
    port = ntohs(addrIn->sin_port);

    printf("ip: %s:%d\n", buf1, port);






out:
    ret = connectPtr(sockfd, addr, addrlen);

err:
    if (buf1) {
        free (buf1);
    }

    if (buf2) {
        free (buf2);
    }

    return ret;
}


void hook_connect_init()
{
    printf("constructor\n");

    connectPtr = (ConnectPtr) dlsym(RTLD_NEXT, "connect");
}

void hook_connect_cleanup()
{
    printf("clean up\n");
    connectPtr = NULL;
}
