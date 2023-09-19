#include <stdio.h>

int main (void)
{
    for (char i = 0; i != 127; ++i) {
        if ((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z') || (i >= '0' && i <= '9')) {
            printf("'%c' -- 0x%2X -- %-3d\n", i, i, i);
        }
    }

    return 0;
}
