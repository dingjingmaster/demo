/*************************************************************************
> FileName: tcp-status-proc.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年07月18日 星期五 17时06分37秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define LINE_LEN 512

void hex_to_ip(char *hex, char *out) {
    unsigned int ip;
    sscanf(hex, "%8X", &ip);
    ip = htonl(ip);  // 转为网络序
    inet_ntop(AF_INET, &ip, out, INET_ADDRSTRLEN);
}

int main() {
    FILE *fp = fopen("/proc/net/tcp", "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    char line[LINE_LEN];
    fgets(line, sizeof(line), fp); // 跳过标题行

    while (fgets(line, sizeof(line), fp)) {
        char local_addr[64], rem_addr[64], state[8];
        int local_port, rem_port;
        unsigned int st;
        unsigned long inode;

        sscanf(line,
               "%*d: %8[0-9A-Fa-f]:%X %8[0-9A-Fa-f]:%X %2X %*s %*s %*s %*s %*s %*u %lu",
               local_addr, &local_port, rem_addr, &rem_port, &st, &inode);

        char local_ip[INET_ADDRSTRLEN];
        char rem_ip[INET_ADDRSTRLEN];

        hex_to_ip(local_addr, local_ip);
        hex_to_ip(rem_addr, rem_ip);

        printf("TCP %s:%d -> %s:%d  state: %02X inode: %lu\n",
               local_ip, local_port, rem_ip, rem_port, st, inode);
    }

    fclose(fp);
    return 0;
}
