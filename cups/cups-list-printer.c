/*************************************************************************
> FileName: cups1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Sat 23 Dec 2023 09:59:52 AM CST
 ************************************************************************/
#include <stdio.h>
#include <cups/cups.h>

int print_dest(void *user_data, unsigned flags, cups_dest_t *dest)
{
  if (dest->instance)
    printf("%s/%s\n", dest->name, dest->instance);
  else
    puts(dest->name);

  return (1);
}

int main(void)
{
  cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, 0, 0, print_dest, NULL);

  return (0);
}
