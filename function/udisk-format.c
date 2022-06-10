/*************************************************************************
> FileName: udisk-format.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月22日 星期六 14时45分35秒
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <gio/gio.h>
#include <gdk/gdk.h>
#include <sys/stat.h>
#include <udisks/udisks.h>

typedef struct
{
    UDisksObject *object,*drive_object;
    UDisksBlock *block,*drive_block;
    UDisksClient *client;

    const gchar *format_type;
    const gchar *device_name;
    const gchar *erase_type;
    const gchar *filesystem_name;

    int *format_finish;
} CreateformatData;

gboolean is_iso(const gchar *device_path);
static void ensure_unused_cb(CreateformatData *data);                                                                                                                                                                    
static void createformatfree (CreateformatData *data);
static void ensure_format_cb (CreateformatData *data);
static void ensure_format_disk(CreateformatData *data);
static void ensure_create_partition_table(CreateformatData *data);
static void format_cb (GObject *source_object, GAsyncResult *res ,gpointer user_data);
static void format_disk (GObject *source_object, GAsyncResult *res ,gpointer user_data);
static UDisksObject* get_object_from_block_device (UDisksClient *client, const gchar *block_device);
void kdisk_format(const gchar * device_name,const gchar *format_type,const gchar * erase_type,const gchar * filesystem_name,int *format_finish);

int main (int argc, char* argv[])
{
    int format_finish = 0;
    gboolean found_format_dev = FALSE;
    gboolean is_erase = FALSE;
    char* format_type = "ntfs";
    char* name = "format-ntfs";
    GMount* mount = NULL;
    GVolume* volume = NULL;

    UDisksObject* object = NULL;
    UDisksObject* driver_object = NULL;
    UDisksBlock* block = NULL;
    UDisksBlock* driver_block;
    UDisksClient* client = NULL;

    gtk_init (&argc, &argv);
    GVolumeMonitor *monitor = g_volume_monitor_get ();
    GList* volumes = g_volume_monitor_get_volumes (monitor);
    if (NULL == volumes) {
        printf ("获取磁盘列表失败!\n\n");
        goto end;
    }

    for (GList* l = volumes; NULL != l; l = l->next) {
        volume = l->data;
        char* identifier = g_volume_get_identifier (volume, G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE);
        printf ("已连接的设备:%s\n", identifier);
        if (0 == g_strcmp0 (identifier, argv[1])) {
            found_format_dev = TRUE;
            g_free (identifier);
            break;
        }
        g_free (identifier);
    }

    if (argc <= 1) {
        printf ("请输入要格式化的设备:[/dev/sdxxx]\n\n");
        return -1;
    }

    if (NULL == mount) {
        kdisk_format(argv[1], format_type, is_erase, name, &format_finish);
        printf ("kdisk_format ok!\n");
        goto end;
    } else {
        printf ("mount not null!\n");
    }

    if (found_format_dev) {
        mount = g_volume_get_mount (volume);
        if (FALSE == g_mount_can_unmount(mount)) {
            goto end;
        }
    }

end:
    if (NULL != monitor) g_object_unref (monitor);
    if (NULL != volumes) g_list_foreach (volumes, (GFunc)g_object_unref, NULL);
    if (NULL != volumes) g_list_free (volumes);
    if (NULL != mount) g_object_unref (mount);

    gtk_main();

    return 0;
}


static void format_cb (GObject *source_object, GAsyncResult *res ,gpointer user_data)
{
    GError *error = NULL;
    CreateformatData *data = user_data;

    if (!udisks_block_call_format_finish (UDISKS_BLOCK (source_object), res,&error)) {   
        char *p = NULL;
        if(NULL != error && NULL != (p = strstr (error->message, "wipefs:"))) {
            printf ("U盘格式化失败! error:%s\n", error->message);
        }
        
        *(data->format_finish) =  -1;
    } else {
        *(data->format_finish) =  1;
        printf ("U盘格式化成功!\n");
    }

    createformatfree(data);
}

static void createformatfree (CreateformatData *data)
{
    g_object_unref(data->object);
    g_object_unref(data->block);

    if(data->drive_object!=NULL) {
        g_object_unref(data->drive_object);
    } 

    if(data->drive_block!=NULL) {
        g_object_unref(data->drive_block);
    }

    g_clear_object(&(data->client));
    g_free(data);
}

static UDisksObject* get_object_from_block_device (UDisksClient *client, const gchar *block_device)
{ 
    struct stat statbuf;
    const gchar *crypto_backing_device = NULL;
    UDisksObject *object = NULL, *crypto_backing_object = NULL;
    UDisksBlock *block = NULL;

    if (stat (block_device, &statbuf) != 0) {
        return object;
    }

    block = udisks_client_get_block_for_dev (client, statbuf.st_rdev);
    if (block == NULL) {
        return object;
    }

    object = UDISKS_OBJECT (g_dbus_interface_dup_object (G_DBUS_INTERFACE (block)));
    g_object_unref (block);

    crypto_backing_device = udisks_block_get_crypto_backing_device ((udisks_object_peek_block (object)));
    crypto_backing_object = udisks_client_get_object (client, crypto_backing_device);
    if (crypto_backing_object != NULL) {
        g_object_unref (object);
        object = crypto_backing_object;
    }

    return object;
} 

// 新建分区表
static void ensure_create_partition_table(CreateformatData *data)
{  
    GVariantBuilder options_builder;
    UDisksPartitionTable *table = NULL;

    g_variant_builder_init(&options_builder,G_VARIANT_TYPE_VARDICT);
    table = udisks_object_get_partition_table(data->drive_object);
    g_variant_builder_add (&options_builder,"{sv}", "partition-type", g_variant_new_string("primary"));
    udisks_partition_table_call_create_partition(table, 0, udisks_block_get_size(data->drive_block), "", "", g_variant_builder_end(&options_builder), NULL, ensure_format_cb, data);
} 

void kdisk_format(const gchar * device_name,const gchar *format_type,const gchar * erase_type,const gchar * filesystem_name,int *format_finish)
{  
    CreateformatData *data;
    data = g_new(CreateformatData,1);
    data->format_finish = 0;
    data->device_name = device_name;
    data->format_type = format_type;
    data->erase_type = erase_type;
    data->filesystem_name = filesystem_name;
    data->format_finish = format_finish;
    data->drive_object = NULL;
    data->drive_block = NULL;

    data->client =udisks_client_new_sync (NULL,NULL);
    data->object = get_object_from_block_device(data->client,data->device_name);
    data->block = udisks_object_get_block (data->object);

    ensure_unused_cb(data);
}

static void ensure_unused_cb(CreateformatData *data)                                                                                                                                                                         
{ 
    if(is_iso(data->device_name)==FALSE) {
        printf ("format cb\n");
        ensure_format_cb (data);
    } else {
        ensure_format_disk(data);
        printf ("format disk\n");
    }
}

static void ensure_format_cb (CreateformatData *data)
{
    GVariantBuilder options_builder;

    g_variant_builder_init (&options_builder, G_VARIANT_TYPE_VARDICT);
    if (g_strcmp0 (data->format_type, "empty") != 0) {
        g_variant_builder_add (&options_builder, "{sv}", "label", g_variant_new_string (data->filesystem_name));
    }

    if (g_strcmp0 (data->format_type, "vfat") != 0 && g_strcmp0 (data->format_type, "ntfs") != 0 && g_strcmp0 (data->format_type, "exfat") != 0) { 
        g_variant_builder_add (&options_builder, "{sv}", "take-ownership", g_variant_new_boolean (TRUE));
    } 

    if (data->erase_type != NULL){
        g_variant_builder_add (&options_builder, "{sv}", "erase", g_variant_new_string (data->erase_type));
    }

    g_variant_builder_add (&options_builder, "{sv}", "update-partition-type", g_variant_new_boolean (TRUE));
    printf ("call format, format type:%s\n", data->format_type);
    udisks_block_call_format (data->block, data->format_type, g_variant_builder_end (&options_builder), NULL, format_cb, data);
}

static void ensure_format_disk(CreateformatData *data)
{
    UDisksClient    *client;
    char ch[10] = {0};
    for(int i = 0; i <= 7; i++) {
        ch[i]=(data->device_name)[i];
    }
    
    data->drive_object = get_object_from_block_device(data->client,ch); 
    data->drive_block = udisks_object_get_block(data->drive_object);
    GVariantBuilder options_builder;
    g_variant_builder_init(&options_builder,G_VARIANT_TYPE_VARDICT);
    printf ("start call format...\n");
    udisks_block_call_format(data->drive_block, "dos", g_variant_builder_end(&options_builder), NULL, format_disk, data);
    printf ("call format ok!\n");
}
           
static void format_disk (GObject *source_object, GAsyncResult *res ,gpointer user_data)
{
    CreateformatData *data = user_data;
    if (!udisks_block_call_format_finish (UDISKS_BLOCK (source_object), res,NULL)) {
        *(data->format_finish) =  -1; //格式化失败
        createformatfree(data);
        return ;
    } else {
        ensure_create_partition_table(data);
    }
}  

gboolean is_iso(const gchar *device_path)
{
    UDisksObject *object;
    UDisksClient *client;
    UDisksBlock *block;

    client = udisks_client_new_sync(NULL, NULL);
    object = get_object_from_block_device(client, device_path);
    block = udisks_object_get_block(object);
                                                         
    if(g_strcmp0(udisks_block_get_id_type(block),"iso9660") == 0) {
        g_object_unref(object);
        g_object_unref(block);
        g_clear_object(&client);
        return TRUE;
    }

    g_object_unref(object);
    g_object_unref(block);
    g_clear_object(&client);
                                                                                         
    return FALSE;
}
