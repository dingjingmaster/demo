/*************************************************************************
> FileName: cups-submit-print-job.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Sat 23 Dec 2023 11:02:58 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <stdbool.h>
#include <cups/cups.h>

int print_dest(void *user_data, unsigned flags, cups_dest_t *dest)
{
    if (dest->instance)
        printf("%s/%s\n", dest->name, dest->instance);
    else
        puts(dest->name);

    int job_id = 0;
    int num_options = 0;
    cups_option_t *options = NULL;

    num_options = cupsAddOption(CUPS_COPIES, "42", num_options, &options);
    num_options = cupsAddOption(CUPS_MEDIA, CUPS_MEDIA_LETTER, num_options, &options);
    num_options = cupsAddOption(CUPS_SIDES, CUPS_SIDES_TWO_SIDED_PORTRAIT, num_options, &options);

    cups_dinfo_t* info = cupsCopyDestInfo (CUPS_HTTP_DEFAULT, dest);

    bool success = false;
    if (cupsCreateDestJob(CUPS_HTTP_DEFAULT, dest, info, &job_id, "My Document", num_options, options) == IPP_STATUS_OK) {
        success = true;
        printf("Created job: %d\n", job_id);
    }
    else {
        printf("Unable to create job: %s\n", cupsLastErrorString());
    }

    if (success) {
        if (IPP_STATUS_OK == cupsFinishDestDocument(CUPS_HTTP_DEFAULT, dest, info)) {
            printf ("finished!\n");
        }
        else {
            printf("Unable to finished job: %s\n", cupsLastErrorString());
        }
    }

    return (1);
}

int main(void)
{
    cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, 0, 0, print_dest, NULL);
    return (0);
}
