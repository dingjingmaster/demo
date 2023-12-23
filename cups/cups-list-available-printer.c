/*************************************************************************
> FileName: cups-list-available-printer.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Sat 23 Dec 2023 10:11:43 AM CST
 ************************************************************************/
#include <stdio.h>
#include <cups/cups.h>


typedef struct
{
  int num_dests;
  cups_dest_t *dests;
} my_user_data_t;

int
my_dest_cb(my_user_data_t *user_data, unsigned flags, cups_dest_t *dest)
{
    if (flags & CUPS_DEST_FLAGS_REMOVED) {
        user_data->num_dests = cupsRemoveDest(dest->name, dest->instance, user_data->num_dests, &(user_data->dests));
    }
    else {
        user_data->num_dests = cupsCopyDest(dest, user_data->num_dests, &(user_data->dests));
    }

    return (1);
}

int
my_get_dests(cups_ptype_t type, cups_ptype_t mask, cups_dest_t **dests)
{
  my_user_data_t user_data = { 0, NULL };

  if (!cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, type, mask, (cups_dest_cb_t)my_dest_cb, &user_data)) {
    cupsFreeDests(user_data.num_dests, user_data.dests);

    *dests = NULL;

    return (0);
  }

  *dests = user_data.dests;

  return (user_data.num_dests);
}

int main(void)
{
  cups_dest_t *dests = NULL;

  int n = my_get_dests(0, 0, &dests);
  printf ("printer num: %d\n", n);

  if (dests) free (dests);

  return (0);
}
