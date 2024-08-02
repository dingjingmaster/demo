/*************************************************************************
> FileName: get-gateway.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 02 Aug 2024 07:26:51 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

int get_gateway_address();

int main ()
{
    get_gateway_address();

    return 0;
}

int get_gateway_address()
{
#ifdef SIOCGNRTINFO
    int sock;
    struct rtentry rt;
    struct sockaddr_in* sin;

    sock = socket (AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    memset (&rt, 0, sizeof(rt));
    sin = (struct sockaddr_in*) &rt.rt_gateway;
    sin->sin_family = AF_INET;

    if (ioctl (sock, SIOCGNRTINFO, &rt) < 0) {
        perror ("ioctl");
        close (sock);
        return -1;
    }

    printf ("Gateway address: %s\n", inet_ntoa(sin->sin_addr));

    close(sock);
#else
    int sock, len, msgLen;
    struct nlmsghdr* nlh;
    struct rtmsg* rtm;
    char buf[4096] = {0};

    sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    memset (buf, 0, sizeof(buf));
    nlh = (struct nlmsghdr*) buf;
    nlh->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
    nlh->nlmsg_type = RTM_GETROUTE;
    nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    nlh->nlmsg_len = sizeof(nlh);
    nlh->nlmsg_pid = getpid();

    if (send (sock, nlh, nlh->nlmsg_len, 0) < 0) {
        perror("send");
        close(sock);
        return -1;
    }

    msgLen = recv (sock, buf, sizeof(buf), 0);
    if (msgLen < 0) {
        perror("recv");
        close(sock);
        return -1;
    }

    printf ("buf: %s\n", buf);

    for (; NLMSG_OK(nlh, msgLen); nlh = NLMSG_NEXT(nlh, msgLen)) {
        if (nlh->nlmsg_type == NLMSG_DONE) {
            printf ("done!\n");
            break;
        }
        if (nlh->nlmsg_type == RTM_NEWROUTE) {
            rtm = (struct rtmsg*) NLMSG_DATA(nlh);
            if (rtm->rtm_dst_len == 0 && rtm->rtm_table == RT_TABLE_MAIN) {
                printf ("RT_TABLE_MAIN\n");
                int attrLen = IFA_PAYLOAD(nlh);
                struct rtattr* attr = (struct rtattr*) RTM_RTA(rtm);
                while (attrLen && RTA_OK(attr, attrLen)) {
                    if (attr->rta_type == RTA_GATEWAY) {
                        printf ("gateway: %s\n", inet_ntoa(*(struct in_addr*)RTA_DATA(attr)));
                        break;
                    }
                    RTA_NEXT(attr, attrLen);
                }
            }
        }
    }
    close (sock);
#endif

    return 0;
}
