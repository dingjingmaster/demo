/*************************************************************************
> FileName: icmp-ping2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 13 Aug 2024 01:58:19 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/capability.h>
#include <sys/prctl.h>

#define PACKET_SIZE 64
#define TIMEOUT_SEC 1

volatile int received_reply = 0;

unsigned short in_cksum(unsigned short *ptr, int nbytes);


void signal_handler(int signum) 
{
    if (signum == SIGALRM) {
        // Timeout occurred, no reply received
        return;
    }
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <host_or_ip>\n", argv[0]);
        return 1;
    }

    const char *target = argv[1];
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(target);

    // 获取 cap_net_raw 权限
    cap_t caps = cap_init();
    int f = CAP_NET_RAW;
    cap_set_flag(caps, CAP_EFFECTIVE, 1, &f, CAP_SET);
    cap_set_proc(caps);
    cap_free(caps);

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    unsigned char packet[PACKET_SIZE];
    struct icmp *icmp_hdr = (struct icmp *)packet;

    icmp_hdr->icmp_type = ICMP_ECHO;
    icmp_hdr->icmp_code = 0;
    icmp_hdr->icmp_id = getpid();
    icmp_hdr->icmp_seq = 1;
    icmp_hdr->icmp_cksum = 0;
    icmp_hdr->icmp_cksum = in_cksum((unsigned short *)icmp_hdr, sizeof(struct icmp));

    struct timeval tv;
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt");
        close(sock);
        return 1;
    }

    signal(SIGALRM, signal_handler);
    alarm(TIMEOUT_SEC);

    if (sendto(sock, packet, sizeof(struct icmp), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("sendto");
        close(sock);
        return 1;
    }

    unsigned char reply_packet[PACKET_SIZE];
    struct sockaddr_in reply_addr;
    socklen_t reply_addr_len = sizeof(reply_addr);

    ssize_t reply_len = recvfrom(sock, reply_packet, PACKET_SIZE, 0, (struct sockaddr *)&reply_addr, &reply_addr_len);
    if (reply_len < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("Request timed out.\n");
        } else {
            perror("recvfrom");
        }
    } else {
        received_reply = 1;
        printf("Reply received from %s\n", inet_ntoa(reply_addr.sin_addr));
    }

    close(sock);
    return 0;
}

unsigned short in_cksum(unsigned short *ptr, int nbytes) 
{
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }

    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char *)&oddbyte) = *(u_char *)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = (short)~sum;
    return (answer);
}

