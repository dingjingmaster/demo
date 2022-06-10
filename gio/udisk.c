#include <gio/gio.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <sys/stat.h>
#include <udisks/udisks.h>


UDisksObject* getObjectFromBlockDevice(UDisksClient* client, const gchar* bdevice);

int main (int argc, char* argv[])
{
	char* devName = "/dev/sda";
	char* name = "dd";

	UDisksClient* client = udisks_client_new_sync(NULL, NULL);
	g_return_val_if_fail(client, -1);

	UDisksObject* udiskObj = getObjectFromBlockDevice(client, devName);
    g_return_val_if_fail(udiskObj, -1);

	UDisksFilesystem* diskFilesystem = udisks_object_get_filesystem(udiskObj);
	g_return_val_if_fail(diskFilesystem, -1);

	GVariantBuilder optionsBuilder;
    g_variant_builder_init(&optionsBuilder, G_VARIANT_TYPE_VARDICT);
	g_variant_builder_add (&optionsBuilder, "{sv}", "label", g_variant_new_string (devName));
	g_variant_builder_add (&optionsBuilder, "{sv}", "take-ownership", g_variant_new_boolean (TRUE));


	udisks_filesystem_call_set_label_sync (diskFilesystem, name, g_variant_builder_end(&optionsBuilder), NULL, NULL);

	return 0;
}

UDisksObject* getObjectFromBlockDevice(UDisksClient* client, const gchar* bdevice)
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
