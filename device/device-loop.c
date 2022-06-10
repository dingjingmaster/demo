/*************************************************************************
> FileName: device-loop.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2022年02月28日 星期一 13时33分54秒
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <gio/gio.h>
#include <sys/stat.h>
#include <udisks/udisks.h>

UDisksObject* get_object_from_block_device (UDisksClient* client, const gchar* bdevice);

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要挂载的 CD 设备名!\n");
        return -1;
    }

    const char*     uri = argv[1];

    GFile* file = g_file_new_for_uri (uri);
    g_return_val_if_fail (G_IS_FILE(file), -1);

    GError* error = NULL;
    GFileInfo* fileInfo = g_file_query_info (file, "mountable::*", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);
    if (error) {
        printf ("query '%s' info error: %s\n", uri, error->message);
        return -1;
    }

    if (!g_file_info_has_attribute (fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_UNIX_DEVICE_FILE)) {
        printf ("There is no '%s' attribute\n", G_FILE_ATTRIBUTE_MOUNTABLE_UNIX_DEVICE_FILE);
        return -1;
    }

    const char* devName = g_file_info_get_attribute_string (fileInfo, G_FILE_ATTRIBUTE_MOUNTABLE_UNIX_DEVICE_FILE);
    if (NULL == devName) {
        printf ("devName is null!");
        return -1;
    }

    UDisksClient* udiskClient = udisks_client_new_sync (NULL, NULL);
    if (NULL == udiskClient) {
        printf ("get udisks client error!\n");
        return -1;
    }

    UDisksObject* udisksObj = get_object_from_block_device (udiskClient, devName);
    if (NULL == udisksObj) {
        printf ("get UDisksObject error!\n");
        return -1;
    }

    UDisksBlock* udisksBlock = udisks_object_get_block (udisksObj);
    if (NULL == udisksBlock) {
        printf ("get UDisksBlock error!\n");
        return -1;
    }

    gboolean ret = udisks_block_call_open_device_sync (udisksBlock, "rw", NULL, NULL, NULL, NULL, NULL, &error);
    if (NULL != error) {
        printf ("open device error: %s\n", error->message);
        return -1;
    }
    printf ("%s\n", ret ? "true" : "false");




    return 0;
}

UDisksObject* get_object_from_block_device (UDisksClient* client, const gchar* bdevice)
{
    struct stat statbuf;
    UDisksObject* object = NULL;
    UDisksObject* cryptoBackingObject = NULL;
    g_autofree const gchar* cryptoBackingDevice = NULL;

    g_return_val_if_fail(stat(bdevice, &statbuf) == 0, object);

    UDisksBlock* block = udisks_client_get_block_for_dev (client, statbuf.st_rdev);
    g_return_val_if_fail(block != NULL, object);

    object = UDISKS_OBJECT (g_dbus_interface_dup_object (G_DBUS_INTERFACE (block)));

    cryptoBackingDevice = udisks_block_get_crypto_backing_device ((udisks_object_peek_block (object)));
    cryptoBackingObject = udisks_client_get_object (client, cryptoBackingDevice);
    if (cryptoBackingObject != NULL) {
        g_object_unref (object);
        object = cryptoBackingObject;
    }

    return object;
}

















