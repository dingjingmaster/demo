/*************************************************************************
> FileName: get-local-info.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 21 Jun 2023 02:28:41 PM CST
 ************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

int main (void)
{
    int fd = -1;
    int interfaceNum = 0;
    struct ifreq buf[16];
    struct ifconf ifc;
    struct ifreq ifrcopy;
    char mac[16] = {0};
    char ip[32] = {0};
    char broadAddr[32] = {0};
    char subnetMask[32] = {0};


    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        close(fd);
        return -1;
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t) buf;
    if (!ioctl(fd, SIOCGIFCONF, (char*)&ifc)) {
        interfaceNum = ifc.ifc_len / sizeof(struct ifreq);
        printf ("interface num = %d\n", interfaceNum);
        while (interfaceNum-- > 0) {
            printf ("ndevice name: %s\n", buf[interfaceNum].ifr_name);
            // ignore interface that not up or not running
            ifrcopy = buf[interfaceNum];
            if (ioctl (fd, SIOCGIFFLAGS ,&ifrcopy)) {
                printf ("ioctl: %s\n", strerror(errno));
                return -1;
            }
            if (!ioctl(fd, SIOCGIFHWADDR, (char*) (&buf[interfaceNum]))) {
                memset (mac, 0, sizeof(mac));
                snprintf (mac, sizeof(mac), "%02x%02x%02x%02x%02x%02x",
                        (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[0],
                        (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[1],
                        (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[2],
                        (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[3],
                        (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[4],
                        (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[5]
                        );
                printf ("mac: %s\n", mac);
            }
            else {
                printf ("ioctl: %s\n", strerror(errno));
                close(fd);
                return -1;
            }

            // get subnet mask of this interface
            if (!ioctl(fd, SIOCGIFNETMASK, &buf[interfaceNum])) {
                snprintf (subnetMask, sizeof(subnetMask), "%s",
                        (char*) inet_ntoa(((struct sockaddr_in*) & (buf[interfaceNum].ifr_netmask))->sin_addr)
                        );
                printf ("net mask: %s\n", subnetMask);
            }
            printf ("\n");
        }
    }
    close (fd);

    return 0;
}
