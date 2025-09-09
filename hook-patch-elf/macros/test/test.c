#include "macros.h"

#include <stdio.h>


int main (void)
{
    {
        const char* str = "192.168.2.22:4433";
        char buf[1024] = {0};

        //c_is_ipv4_addr (str, buf, sizeof(buf) - 1);
        printf ("%s --> %s\n", str, buf);
    }
    return 0;
}
