/*************************************************************************
> FileName: cups-dinfo1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Sat 23 Dec 2023 10:58:57 AM CST
 ************************************************************************/
#include <stdio.h>
#include <cups/cups.h>


void print_dest_info (cups_dest_t* dest, cups_dinfo_t* info)
{
    if (!dest || !info) return;

    if (cupsCheckDestSupported(CUPS_HTTP_DEFAULT, dest, info, CUPS_FINISHINGS, NULL)) {
        ipp_attribute_t* finishings = cupsFindDestSupported(CUPS_HTTP_DEFAULT, dest, info, CUPS_FINISHINGS);
        int i, count = ippGetCount(finishings);

        puts("finishings supported:");
        for (i = 0; i < count; i ++) {
            printf("  %d\n", ippGetInteger(finishings, i));
        }
    }
    else {
        printf ("not supported!");
    }
}


int print_dest(void *user_data, unsigned flags, cups_dest_t *dest)
{
    if (dest->instance) {
        printf("%s/%s\n", dest->name, dest->instance);
    }
    else {
        puts(dest->name);
    }

    cups_dinfo_t* dinfo = cupsCopyDestInfo (CUPS_HTTP_DEFAULT, dest);
    print_dest_info(dest, dinfo);

    return (1);
}



int main(void)
{
  cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, 0, 0, print_dest, NULL);

  return (0);
}
