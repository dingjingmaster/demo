/*************************************************************************
> FileName: strtok.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 31 Oct 2022 03:07:37 PM CST
 ************************************************************************/
#include <stdio.h>
#include <string.h>

int main (int argc, char* argv[])
{
    char a[] = "_usb=/home/1;_usb=/home/2;_usb=/home/3;";
    char t[] = "/home/2/3";

    for (char* b = strtok(a, ";"); b != NULL; b = strtok(NULL, ";")) {
        if (NULL != strtok(t, b)) {
            printf ("find!\n");
            break;
        }
        printf ("%s\n", b);
    }

    return 0;
}
