#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if_arp.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>

void getMacAdr (const char* ip,char* mac)
{
    struct arpreq arp;
    struct sockaddr_in *sin;
    int s1;

    //init socket
    s1 = socket(AF_INET, SOCK_DGRAM, 0);
    if(s1 == -1) {
        perror("socket");
        exit(-1);
    }
    
    //set ip addr
    sin = (struct sockaddr_in*) &arp.arp_pa;
    memset (sin, 0, sizeof(*sin));
    sin->sin_family = AF_INET;
    if (inet_pton(AF_INET, ip, &sin->sin_addr) < 0) {
        perror("inet_pton");
        close(s1);
        exit(EXIT_FAILURE);   
    }

    printf("ip is %d\n",&sin->sin_addr);
    //send arp
    //strncpy(arp.arp_dev,"enp61s0",sizeof(arp.arp_dev)-1);
    strncpy(arp.arp_dev, "enp0s20f0u4c4i2", sizeof(arp.arp_dev) - 1);
    printf("arp.apr_dev is %s\n", arp.arp_dev);


    if (ioctl(s1, SIOCGARP, (char*) &arp) == -1) {
        perror("ioctl");
        close(s1);
        exit(EXIT_FAILURE);
    }
    
    sprintf (mac,"%02x:%02x:%02x:%02x:%02x:%02x",
            (unsigned char)arp.arp_ha.sa_data[0],
            (unsigned char)arp.arp_ha.sa_data[1],
            (unsigned char)arp.arp_ha.sa_data[2],
            (unsigned char)arp.arp_ha.sa_data[3],
            (unsigned char)arp.arp_ha.sa_data[4],
            (unsigned char)arp.arp_ha.sa_data[5]);
   
    close(s1);
};

int main (int argc,char* argv[])
{
    if (argc != 2) {
        fprintf (stderr, "Usage: %s <ip_address>\n", argv[0]);
        exit (EXIT_FAILURE);
    }
    
    char mac_address [18];
    getMacAdr (argv[1], mac_address);

    printf ("MAC Address for %s: %s\n", argv[1], mac_address);

    return 0;
}

