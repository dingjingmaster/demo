/*************************************************************************
> FileName: gio-is-remote.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年09月18日 星期五 10时16分09秒
 ************************************************************************/

#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[]) 
{
    if (argc < 2) {
        printf ("please input a path\n");
        return -1;
    }

    GFile*          file = NULL;
    GError*         error = NULL;
    GMount*         mount = NULL;
    GDrive*         drive = NULL;
    GVolume*        volume = NULL;
    GFileInfo*      fileInfo = NULL;

    file = g_file_new_for_path (argv[1]);
    if (NULL == file) {
        goto end;
    }

#if 0
    // it's not work ...
    // query information about the filesystem the file is on
    //fileInfo = g_file_query_filesystem_info (file, "filesystem::*", NULL, &error);

    // query information about the file
    fileInfo = g_file_query_info (file, "filesystem::*", G_FILE_QUERY_INFO_NONE, NULL, &error);
    if (NULL != error) {
        printf ("query error: %s\n", error->message);
        goto end;
    }

    if (NULL != fileInfo) {
        printf ("file '%s' is remote: '%s'\n", argv[1],
                g_file_info_get_attribute_boolean(fileInfo,
                    G_FILE_ATTRIBUTE_FILESYSTEM_REMOTE) ? "true" : "false");
    }
#else
    mount = g_file_find_enclosing_mount (file, NULL, &error);
    if (NULL != error) {
        printf ("get mount point error: %s\n", error->message);
        goto end;
    }

    if (NULL != mount) {
        volume = g_mount_get_volume (mount);
        if (NULL != volume) {
            printf ("volume identifier:%s\n", g_volume_get_identifier (volume, G_VOLUME_IDENTIFIER_KIND_NFS_MOUNT));
        } else {
            printf ("volume is null\n");
        } 
        
        drive = g_mount_get_drive (mount);
        if (NULL != drive) {
            printf ("drive identifier:%s\n", g_drive_get_identifier(drive, G_DRIVE_IDENTIFIER_KIND_UNIX_DEVICE));
        } else {
            printf ("drive is null\n");
        }
    }

#endif

end:
    if (NULL != error) {
        g_object_unref (error);
    }

    if (NULL != fileInfo) {
        g_object_unref (fileInfo);
    }

    if (NULL != file) {
        g_object_unref (file);
    }

    if (NULL != mount) {
        g_object_unref (mount);
    }

    if (NULL != volume) {
        g_object_unref (volume);
    }
}
