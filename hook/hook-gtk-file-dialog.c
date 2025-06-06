/*************************************************************************
> FileName: hook-gtk-file-dialog.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 07 Jun 2023 01:43:18 PM CST
 ************************************************************************/
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef RTLD_NEXT
#define RTLD_NEXT (void*)-1
#endif

typedef void* (*GtkFileChooserGetFileNames) (void* chooser);

void* gtk_file_chooser_get_filenames (void* chooser)
{
    GtkFileChooserGetFileNames old = (GtkFileChooserGetFileNames) dlsym (RTLD_NEXT, "gtk_file_chooser_get_filenames");
    if (old == NULL) {
        return NULL;
    }
    syslog(LOG_ERR, "OK1 files");

    sleep (10);
    return old(chooser);
}

void* gtk_file_chooser_get_filename (void* chooser)
{
    GtkFileChooserGetFileNames old = (GtkFileChooserGetFileNames) dlsym (RTLD_NEXT, "gtk_file_chooser_get_filename");
    if (old == NULL) {
        return NULL;
    }
    syslog(LOG_ERR, "OK1 file");
    sleep (10);
    return old(chooser);
}

void* gtk_file_chooser_get_file (void* chooser)
{
    GtkFileChooserGetFileNames old = (GtkFileChooserGetFileNames) dlsym (RTLD_NEXT, "gtk_file_chooser_get_file");
    if (old == NULL) {
        return NULL;
    }
    syslog(LOG_ERR, "OK1 file 000");

    return old(chooser);
}

