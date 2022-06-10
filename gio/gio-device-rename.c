/*************************************************************************
> FileName: gio-device-rename.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年11月05日 星期五 09时50分35秒
 ************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <gio/gio.h>
#include <sys/stat.h>
#include <udisks/udisks.h>
#include <gio/gunixmounts.h>

typedef struct _DeviceRenameData        DeviceRenameData;

static int device_rename (const char* devName, const char* name);
static void udisk_umounted (GMount* mount, GAsyncResult *res, gpointer udata);
static UDisksObject* getObjectFromBlockDevice (UDisksClient* client, const gchar* bdevice);

struct _DeviceRenameData
{
    char*           devName;
    char*           rename;
};

int main (int argc, char* argv[])
{
    if (argc < 3) {
        printf ("请输入设备  挂载点路径  和  名字 \n");
        return -1;
    }

    char* path = NULL;

    if (g_str_has_prefix (argv[1], "file://")) {
        path = g_strdup (argv[1]);
    } else if (g_str_has_prefix (argv[1], "/")) {
        path = g_strdup_printf ("file://%s", argv[1]);
    } else {
        path = g_strdup_printf ("file://%s/%s", g_get_current_dir(), argv[1]);
    }

    g_autoptr (GError) error = NULL;
    g_autoptr (GFile) file = g_file_new_for_uri (path);
    g_return_val_if_fail (G_IS_FILE (file), -1);

    // check is mount point 
    g_autoptr (GFileInfo) fileInfo = g_file_query_info (file, G_FILE_ATTRIBUTE_UNIX_IS_MOUNTPOINT, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);
    if (error) printf ("%s\n", error->message);
    g_return_val_if_fail (G_IS_FILE_INFO (fileInfo) && !error && g_file_info_get_attribute_boolean (fileInfo, G_FILE_ATTRIBUTE_UNIX_IS_MOUNTPOINT), -1);

    // unmount device
    g_autoptr (GMount) mount = g_file_find_enclosing_mount (file, NULL, &error);
    if (error) printf ("%s\n", error->message);
    g_return_val_if_fail (G_IS_MOUNT (mount), -1);

    g_autoptr (GVolume) volume = g_mount_get_volume (mount);
    g_return_val_if_fail (G_IS_VOLUME (volume), -1);

    g_autofree char* devName = g_volume_get_identifier (volume, G_DRIVE_IDENTIFIER_KIND_UNIX_DEVICE);
    g_return_val_if_fail (devName, -1);

    DeviceRenameData* data = (DeviceRenameData*) g_malloc0 (sizeof (DeviceRenameData));
    data->devName = g_strndup (devName, strlen(devName));
    data->rename = g_strndup (argv[2], strlen(argv[2]));

    g_mount_unmount_with_operation (mount, G_MOUNT_UNMOUNT_NONE, NULL, NULL, (GAsyncReadyCallback) udisk_umounted, data);

    //

    g_main_loop_run (g_main_loop_new (NULL, TRUE));

    return 0;
}

int device_rename (const char* devName, const char* name)
{
    //判断参数个数是否合法
    g_return_val_if_fail(devName && name, -1);

    UDisksClient* client = udisks_client_new_sync(NULL, NULL);
    g_return_val_if_fail(client, -1);

    UDisksObject* udiskObj = getObjectFromBlockDevice(client, devName);
    g_return_val_if_fail(udiskObj, -1);

    //从设备名获取文件系统类型
    UDisksFilesystem* diskFilesystem = udisks_object_get_filesystem(udiskObj);
    g_return_val_if_fail(diskFilesystem, -1);

    GVariantBuilder optionsBuilder;
    g_variant_builder_init(&optionsBuilder, G_VARIANT_TYPE_VARDICT);
    g_variant_builder_add (&optionsBuilder, "{sv}", "label", g_variant_new_string (devName));
    g_variant_builder_add (&optionsBuilder, "{sv}", "take-ownership", g_variant_new_boolean (TRUE));

    g_autoptr (GError) error = NULL;
    gboolean ret = udisks_filesystem_call_set_label_sync (diskFilesystem, name, g_variant_builder_end(&optionsBuilder), NULL, &error);
    if (error) printf ("%s\n", error->message);

    return ret ? 0 : -1;
}

UDisksObject* getObjectFromBlockDevice (UDisksClient* client, const gchar* bdevice)
{
    struct stat statbuf;
    UDisksBlock* block = NULL;
    UDisksObject* object = NULL;
    UDisksObject* cryptoBackingObject = NULL;
    g_autofree const gchar* cryptoBackingDevice = NULL;

    g_return_val_if_fail(stat(bdevice, &statbuf) == 0, object);

    block = udisks_client_get_block_for_dev (client, statbuf.st_rdev);
    g_return_val_if_fail(block != NULL, object);

    object = UDISKS_OBJECT (g_dbus_interface_dup_object (G_DBUS_INTERFACE (block)));

    cryptoBackingDevice = udisks_block_get_crypto_backing_device ((udisks_object_peek_block (object)));
    cryptoBackingObject = udisks_client_get_object (client, cryptoBackingDevice);
    if (cryptoBackingObject != NULL) {
        g_object_unref (object);
        object = cryptoBackingObject;
    }

    g_object_unref (block);

    return object;
}

static void udisk_umounted (GMount* mount, GAsyncResult *res, gpointer udata)
{
    g_return_if_fail (udata);

    g_autoptr (GError) error = NULL;

    if (g_mount_unmount_with_operation_finish (G_MOUNT (mount), res, &error)) {
        g_return_if_fail (device_rename (((DeviceRenameData*) udata)->devName, ((DeviceRenameData*) udata)->rename) == 0);
        // 挂载
        printf ("挂载!!!\n");
    } else {
        if (error) printf ("%s  --> %d\n", error->message, error->code);
    }
}
