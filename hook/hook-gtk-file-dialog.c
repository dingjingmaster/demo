/*************************************************************************
> FileName: hook-gtk-file-dialog.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 07 Jun 2023 01:43:18 PM CST
 ************************************************************************/
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "c-log.h"

#ifndef RTLD_NEXT
#define RTLD_NEXT (void*)-1
#endif

typedef void* (*GtkFileChooserGetFileNames) (void* chooser);

#if 0
void* gtk_file_chooser_get_filenames (void* chooser)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    GtkFileChooserGetFileNames old = (GtkFileChooserGetFileNames) dlsym (RTLD_NEXT, "gtk_file_chooser_get_filenames");
    if (old == NULL) {
        return NULL;
    }
    loge("OK1 files");

    sleep (10);
    return old(chooser);
}

void* gtk_file_chooser_get_filename (void* chooser)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    GtkFileChooserGetFileNames old = (GtkFileChooserGetFileNames) dlsym (RTLD_NEXT, "gtk_file_chooser_get_filename");
    if (old == NULL) {
        return NULL;
    }
    loge("OK1 file");
    sleep (10);
    return old(chooser);
}
#endif 

// no
void gtk_target_list_add (void* list, unsigned long atom, unsigned int flags, unsigned int info)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    loge("OK1 file");
}

// no
char** gtk_selection_data_get_uris(void* s)
{
    return NULL;
}

void* gtk_drag_begin_with_coordinates (void* widget, void* targets, long actions, int button, void* event, int x, int y)
{
    return NULL;
}

int gtk_drag_check_threshold (void* widget, int sx, int sy, int tx, int ty)
{
    return 0;
}

void gtk_drag_dest_add_image_targets (void* widget)
{
}

void gtk_drag_dest_add_uri_targets(void* w)
{
}

void gtk_drag_dest_add_text_targets(void* w)
{
}

long gtk_drag_dest_find_target (void* widget, void* context, void* targetLists)
{
    return 0;
}

void* gtk_drag_dest_get_target_list (void* widget)
{
    return NULL;
}

void gtk_drag_dest_set_target_list (void* widget, void* list)
{
}

void gtk_drag_finish (void* context, bool success, bool del, unsigned int tim)
{
}

void gtk_drag_get_data (void* w, void* i, long t, unsigned int tim)
{
}

void* gtk_drag_get_source_widget (void* context)
{
    return NULL;
}

void gtk_drag_source_add_uri_targets(void* w)
{
}

void gtk_drag_source_add_text_targets(void* w)
{
}

void* gtk_drag_source_get_drag (void* s)
{
    return NULL;
}

void* gtk_drag_source_new (void)
{
    return NULL;
}

void gtk_drag_source_set (void* w, unsigned long button, const void* targets, int ntargets, unsigned long actions)
{
}

void gtk_drag_source_set_content (void* s, void* c)
{
}

void gtk_drag_source_set_target_list (void* w, void* tl)
{
}

void* gtk_drop_down_new (void* model, void* exp)
{
    return NULL;
}

void* gtk_drop_down_new_from_strings (const char* const * strings)
{
    return NULL;
}

void* gtk_drop_target_get_current_drop (void* s)
{
    return NULL;
}

const void* gtk_drop_target_get_value (void* s)
{
    return NULL;
}

void* gtk_drop_target_get_drop (void* s)
{
    return NULL;
}

