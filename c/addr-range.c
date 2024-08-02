/*************************************************************************
> FileName: addr-range.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 02 Aug 2024 02:14:06 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main (int argc, char* argv[])
{
    struct in_addr startIp, endIp;
    int startIpInt, endIpInt;

    const char* ip1 = "192.168.0.1";
    const char* ip2 = "192.168.0.254";

    if (0 == inet_aton(ip1, &startIp)) {
        printf ("Invalid start ip: %s\n", ip1);
        return 1;
    }

    if (0 == inet_aton(ip2, &endIp)) {
        printf ("Invalid end ip: %s\n", ip2);
        return 1;
    }

    // ip 转为整数
    startIpInt = ntohl (startIp.s_addr);
    endIpInt = ntohl (endIp.s_addr);

    // 遍历 IP
    for (int i = startIpInt; i <= endIpInt; ++i) {
        struct in_addr ip;
        ip.s_addr = htonl(i);
        printf ("%s\n", inet_ntoa(ip));
    }

    return 0;
}
