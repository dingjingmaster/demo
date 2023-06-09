/*************************************************************************
> FileName: hook-gtk-file-dialog.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 07 Jun 2023 01:43:18 PM CST
 ************************************************************************/
#include <syslog.h>
#include <stdlib.h>

typedef void* (*GtkFileChooserGetFileNames) (void* chooser);

void* gtk_file_chooser_get_filenames (void* chooser)
{
    return NULL;
}

void* gtk_file_chooser_get_filename (void* chooser)
{

    return NULL;
}
