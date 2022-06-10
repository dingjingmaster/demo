/*************************************************************************
> FileName: gio-file-info.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年11月18日 星期三 14时01分40秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    if (argc <= 1) {
        printf ("請輸入要查询的uri!!!\n");
        exit(1);
    }

    GFile*          file = NULL;
    GFile*          file1 = NULL;
    GError*         error = NULL;
    GFileInfo*      fileInfo = NULL;

    file = g_file_new_for_uri (argv[1]);
    if (NULL == file) {
        printf ("GFile error!!!\n");
        return -1;
    }

    file1 = g_file_new_for_uri ("file:///");
    fileInfo = g_file_query_info(file, "*", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);
    if (NULL != error || NULL == fileInfo) {
        printf ("Query file info error:%s !!!\n", error->message);
        return -1;
    }

    printf ("\n========================== All of attributes ========================================\n");
    printf ("g_file_get_basename():                                 %30s\n", g_file_get_basename(file));
    printf ("g_file_info_get_display_name():                        %30s\n", g_file_info_get_display_name(fileInfo));
    printf ("get_relative_path('file:///', file):                   %30s\n", g_file_get_relative_path(file1, file));
    printf ("G_FILE_ATTRIBUTE_STANDARD_TYPE:                        %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_TYPE));
    printf ("G_FILE_ATTRIBUTE_STANDARD_IS_HIDDEN:                   %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_STANDARD_IS_HIDDEN)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_STANDARD_IS_BACKUP:                   %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_STANDARD_IS_BACKUP)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_STANDARD_IS_SYMLINK:                  %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_STANDARD_IS_SYMLINK)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_STANDARD_IS_VIRTUAL:                  %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_STANDARD_IS_VIRTUAL)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_STANDARD_IS_VOLATILE:                 %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_STANDARD_IS_VOLATILE)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_STANDARD_NAME:                        %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_NAME));
    printf ("G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME:                %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME));
    printf ("G_FILE_ATTRIBUTE_STANDARD_EDIT_NAME:                   %30s\n", g_file_info_get_attribute_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_EDIT_NAME));
    printf ("G_FILE_ATTRIBUTE_STANDARD_COPY_NAME:                   %30s\n", g_file_info_get_attribute_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_COPY_NAME));
    printf ("G_FILE_ATTRIBUTE_STANDARD_DESCRIPTION:                 %30s\n", g_file_info_get_attribute_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_DESCRIPTION));
    //printf ("G_FILE_ATTRIBUTE_STANDARD_ICON:                      %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_ICON));
    //printf ("G_FILE_ATTRIBUTE_STANDARD_SYMBOLIC_ICON:             %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_SYMBOLIC_ICON));
    printf ("G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE:                %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE));
    printf ("G_FILE_ATTRIBUTE_STANDARD_FAST_CONTENT_TYPE:           %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_FAST_CONTENT_TYPE));
    printf ("G_FILE_ATTRIBUTE_STANDARD_SIZE:                        %30u\n", g_file_info_get_attribute_uint64(fileInfo, G_FILE_ATTRIBUTE_STANDARD_SIZE));
    printf ("G_FILE_ATTRIBUTE_STANDARD_ALLOCATED_SIZE:              %30u\n", g_file_info_get_attribute_uint64(fileInfo, G_FILE_ATTRIBUTE_STANDARD_ALLOCATED_SIZE));
    printf ("G_FILE_ATTRIBUTE_STANDARD_SYMLINK_TARGET:              %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_SYMLINK_TARGET));
    printf ("G_FILE_ATTRIBUTE_STANDARD_TARGET_URI:                  %30s\n", g_file_info_get_attribute_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_TARGET_URI));
    printf ("G_FILE_ATTRIBUTE_STANDARD_SORT_ORDER:                  %30d\n", g_file_info_get_attribute_int32(fileInfo, G_FILE_ATTRIBUTE_STANDARD_SORT_ORDER));
    printf ("G_FILE_ATTRIBUTE_ETAG_VALUE:                           %30s\n", g_file_info_get_attribute_string(fileInfo, G_FILE_ATTRIBUTE_ETAG_VALUE));
    printf ("G_FILE_ATTRIBUTE_ID_FILE:                              %30s\n", g_file_info_get_attribute_string(fileInfo, G_FILE_ATTRIBUTE_ID_FILE));
    printf ("G_FILE_ATTRIBUTE_ID_FILESYSTEM:                        %30s\n", g_file_info_get_attribute_string(fileInfo, G_FILE_ATTRIBUTE_ID_FILESYSTEM));
    printf ("G_FILE_ATTRIBUTE_ACCESS_CAN_READ:                      %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_ACCESS_CAN_READ)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE:                     %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE:                   %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_ACCESS_CAN_DELETE:                    %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_ACCESS_CAN_DELETE)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_ACCESS_CAN_TRASH:                     %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_ACCESS_CAN_TRASH)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_ACCESS_CAN_RENAME:                    %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_ACCESS_CAN_RENAME)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_CAN_MOUNT:                  %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_CAN_MOUNT)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_CAN_UNMOUNT:                %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_CAN_UNMOUNT)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_CAN_EJECT:                  %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_CAN_EJECT)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_UNIX_DEVICE:                %30s\n", g_file_info_get_attribute_string(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_UNIX_DEVICE));
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_UNIX_DEVICE_FILE:           %30s\n", g_file_info_get_attribute_string(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_UNIX_DEVICE_FILE));
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_HAL_UDI:                    %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_HAL_UDI));
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_CAN_START:                  %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_CAN_START)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_CAN_START_DEGRADED:         %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_CAN_START_DEGRADED)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_CAN_STOP:                   %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_CAN_STOP)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_START_STOP_TYPE:            %30u\n", g_file_info_get_attribute_uint32(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_START_STOP_TYPE));
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_CAN_POLL:                   %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_CAN_POLL)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_MOUNTABLE_IS_MEDIA_CHECK_AUTOMATIC:   %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_IS_MEDIA_CHECK_AUTOMATIC)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_TIME_MODIFIED:                        %30u\n", g_file_info_get_attribute_uint64(fileInfo, G_FILE_ATTRIBUTE_TIME_MODIFIED));
    printf ("G_FILE_ATTRIBUTE_TIME_MODIFIED_USEC:                   %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_TIME_MODIFIED_USEC));
    printf ("G_FILE_ATTRIBUTE_TIME_ACCESS:                          %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_TIME_ACCESS));
    printf ("G_FILE_ATTRIBUTE_TIME_ACCESS_USEC:                     %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_TIME_ACCESS_USEC));
    printf ("G_FILE_ATTRIBUTE_TIME_CHANGED:                         %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_TIME_CHANGED));
    printf ("G_FILE_ATTRIBUTE_TIME_CHANGED_USEC:                    %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_TIME_CHANGED_USEC));
    printf ("G_FILE_ATTRIBUTE_TIME_CREATED:                         %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_TIME_CREATED));
    printf ("G_FILE_ATTRIBUTE_TIME_CREATED_USEC:                    %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_TIME_CREATED_USEC));
    printf ("G_FILE_ATTRIBUTE_UNIX_DEVICE:                          %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_UNIX_DEVICE));
    printf ("G_FILE_ATTRIBUTE_UNIX_INODE:                           %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_UNIX_INODE));
    printf ("G_FILE_ATTRIBUTE_UNIX_MODE:                            %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_UNIX_MODE));
    printf ("G_FILE_ATTRIBUTE_UNIX_NLINK:                           %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_UNIX_NLINK));
    printf ("G_FILE_ATTRIBUTE_UNIX_UID:                             %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_UNIX_UID));
    printf ("G_FILE_ATTRIBUTE_UNIX_GID:                             %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_UNIX_GID));
    printf ("G_FILE_ATTRIBUTE_UNIX_RDEV:                            %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_UNIX_RDEV));
    printf ("G_FILE_ATTRIBUTE_UNIX_BLOCK_SIZE:                      %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_UNIX_BLOCK_SIZE));
    printf ("G_FILE_ATTRIBUTE_UNIX_BLOCKS:                          %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_UNIX_BLOCKS));
    printf ("G_FILE_ATTRIBUTE_UNIX_IS_MOUNTPOINT:                   %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_UNIX_IS_MOUNTPOINT)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_DOS_IS_ARCHIVE:                       %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_DOS_IS_ARCHIVE)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_DOS_IS_MOUNTPOINT:                    %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_DOS_IS_MOUNTPOINT)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_DOS_IS_SYSTEM:                        %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_DOS_IS_SYSTEM)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_DOS_REPARSE_POINT_TAG:                %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_DOS_REPARSE_POINT_TAG));
    printf ("G_FILE_ATTRIBUTE_OWNER_USER:                           %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_OWNER_USER));
    printf ("G_FILE_ATTRIBUTE_OWNER_USER_REAL:                      %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_OWNER_USER_REAL));
    printf ("G_FILE_ATTRIBUTE_OWNER_GROUP:                          %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_OWNER_GROUP));
    printf ("G_FILE_ATTRIBUTE_THUMBNAIL_PATH:                       %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_THUMBNAIL_PATH));
    printf ("G_FILE_ATTRIBUTE_THUMBNAILING_FAILED:                  %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_THUMBNAILING_FAILED)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_THUMBNAIL_IS_VALID:                   %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_THUMBNAIL_IS_VALID)?"true":"false");
    //printf ("G_FILE_ATTRIBUTE_PREVIEW_ICON:                       %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_PREVIEW_ICON));
    printf ("G_FILE_ATTRIBUTE_FILESYSTEM_SIZE:                      %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_FILESYSTEM_SIZE));
    printf ("G_FILE_ATTRIBUTE_FILESYSTEM_FREE:                      %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_FILESYSTEM_FREE));
    printf ("G_FILE_ATTRIBUTE_FILESYSTEM_USED:                      %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_FILESYSTEM_USED));
    printf ("G_FILE_ATTRIBUTE_FILESYSTEM_TYPE:                      %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_FILESYSTEM_TYPE));
    printf ("G_FILE_ATTRIBUTE_FILESYSTEM_READONLY:                  %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_FILESYSTEM_READONLY)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_FILESYSTEM_USE_PREVIEW:               %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_FILESYSTEM_USE_PREVIEW));
    printf ("G_FILE_ATTRIBUTE_FILESYSTEM_REMOTE:                    %30s\n", g_file_info_get_attribute_boolean(fileInfo, G_FILE_ATTRIBUTE_FILESYSTEM_REMOTE)?"true":"false");
    printf ("G_FILE_ATTRIBUTE_GVFS_BACKEND:                         %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_GVFS_BACKEND));
    printf ("G_FILE_ATTRIBUTE_SELINUX_CONTEXT:                      %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_SELINUX_CONTEXT));
    printf ("G_FILE_ATTRIBUTE_TRASH_ITEM_COUNT:                     %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_TRASH_ITEM_COUNT));
    printf ("G_FILE_ATTRIBUTE_TRASH_ORIG_PATH:                      %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_TRASH_ORIG_PATH));
    printf ("G_FILE_ATTRIBUTE_TRASH_DELETION_DATE:                  %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_TRASH_DELETION_DATE));
    printf ("G_FILE_ATTRIBUTE_RECENT_MODIFIED:                      %30s\n", g_file_info_get_attribute_as_string(fileInfo, G_FILE_ATTRIBUTE_RECENT_MODIFIED));
    printf ("\n=====================================================================================\n");
}


