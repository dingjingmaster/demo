/*************************************************************************
> FileName: cups-dinfo.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Sat 23 Dec 2023 10:52:14 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <cups/cups.h>

static inline cups_dest_t* copy_cups_dest_t (cups_dest_t* d)
{
    if (!d) return NULL;

    cups_dest_t* t1 = (cups_dest_t*) g_malloc0 (sizeof (cups_dest_t));

    memcpy(t1, d, sizeof(cups_dest_t));

    t1->name = (d->name ? g_strdup(d->name) : NULL);
    t1->instance = (d->instance ? g_strdup(d->instance) : NULL);
    t1->options = (d->options ? (cups_option_t*) g_malloc0(sizeof(cups_option_t)) : NULL);
    if (t1->options) {
        if (d->options->name) {
            t1->options->name = g_strdup(d->options->name);
        }

        if (d->options->value) {
            t1->options->value = g_strdup(d->options->value);
        }
    }

    return t1;
}

int get_dests(void *udata, unsigned flags, cups_dest_t *dest)
{
    cups_dest_t*** dests = (cups_dest_t***)udata;

    int n = 2;
    // FIXME:// 计算数量，或者用结构体计算数量

    if (!(*dests)) {
        *dests = (cups_dest_t**) g_malloc0(sizeof(cups_dest_t*) * n);
        (*dests)[0] = copy_cups_dest_t(dest);
    }
    else {
        // FIXME://
    }

  return (1);
}

void print_dest_info (cups_dest_t* dest, cups_dinfo_t* info)
{
    if (!dest || !info) return;

    printf ("in\n");
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

int main(void)
{
    cups_dest_t** dests = NULL;

    cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, 0, 0, get_dests, &dests);

    for (int i = 0; dests[i]; ++i) {
        printf ("1\n");
        cups_dinfo_t* dinfo = cupsCopyDestInfo (CUPS_HTTP_DEFAULT, dests[i]);
        printf ("2\n");
        print_dest_info(dests[i], dinfo);
    }

    // FIXME:// free
    return (0);
}
