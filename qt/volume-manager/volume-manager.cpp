#include "volume-manager.h"

#include <QMap>
#include <QDebug>
#include <QMutex>
#include <QDir>

#include <glib.h>
#include <gudev/gudev.h>
#include <udisks/udisks.h>

// 块设备相关
static void uevent (GUdevClient* client, const char* action, GUdevDevice* device, gpointer udata);
static void monitor_added (GDBusObjectManager* manager, GDBusObject* object, gpointer udata);
static void monitor_removed (GDBusObjectManager* manager, GDBusObject* object, gpointer udata);
static void monitor_property_changed (GDBusObjectManagerClient* manager, GDBusObjectProxy* objProxy, GDBusProxy* interfaceProxy, GVariant* changeProperties, const char* const invalidProterty, gpointer udata);


// 光盘设备相关
static QStringList getSubdirList (const QString& path);


VolumeManager* VolumeManager::gInstance = nullptr;

static const char* subsystems[] =
{
    "block",
    "iscsi_connection",
    "scsi",
    nullptr
};

class VolumeManagerPrivate
{
public:
    VolumeManagerPrivate (VolumeManager* ptr);
    QStringList getAllBlockMountPoints();

public:
    UDisksClient*                       mUdisksClient;
    GDBusObjectManager*                 mUdisksManager;

    GUdevClient*                        mUdevClient;

    VolumeManager*                      q_ptr;

    // FIXME://目前仅支持单线程，无须加锁也可
    QMutex                              mLocker;
    QSet<QString>                       mBurnDevice;
    QMap<QString, QSet<QString>>        mBlockDevice;           // 块设备
};

VolumeManagerPrivate::VolumeManagerPrivate(VolumeManager *ptr)
    : q_ptr(ptr)
{
    // 块设备相关
    mUdevClient = g_udev_client_new(subsystems);

    mUdisksClient = udisks_client_new_sync(nullptr, nullptr);
    mUdisksManager = udisks_client_get_object_manager(mUdisksClient);

    //g_signal_connect(mUdevClient, "uevent", (GCallback)uevent, static_cast<gpointer>(this));
    g_signal_connect(mUdisksManager, "object-added", G_CALLBACK(monitor_added), this);
    g_signal_connect(mUdisksManager, "object-removed", G_CALLBACK(monitor_removed), this);
    g_signal_connect(mUdisksManager, "interface-proxy-properties-changed", G_CALLBACK(monitor_property_changed), this);

    // 初始化猎取当前已连接到设备的文件
    g_autoptr(GUdevEnumerator) enumerator = g_udev_enumerator_new(mUdevClient);

    enumerator = g_udev_enumerator_add_match_property (enumerator, "ID_BUS", "usb");
    GList* devices = g_udev_enumerator_execute (enumerator);
    for (GList* l = devices; nullptr != l; l = g_list_next(l)) {
        GUdevDevice* d = static_cast<GUdevDevice*> (l->data);
        const char* device = g_udev_device_get_device_file(d);
        QSet<QString> mountPoints;

        QString devicePath = QString(device).replace("/dev/", "/org/freedesktop/UDisks2/block_devices/");
        GDBusObject* obj = g_dbus_object_manager_get_object(mUdisksManager, devicePath.toUtf8().constData());
        if (UDISKS_OBJECT(obj)) {
            GList* ifp = g_dbus_object_get_interfaces (G_DBUS_OBJECT(obj));
            for (GList* l = ifp; nullptr != l; l = l->next) {
                const char* name = g_dbus_proxy_get_interface_name(G_DBUS_PROXY(l->data));
                if (!g_strcmp0("org.freedesktop.UDisks2.Filesystem", name)) {
                    char** cacheProperties = g_dbus_proxy_get_cached_property_names(G_DBUS_PROXY(l->data));
                    for (int n = 0; cacheProperties && cacheProperties[n]; ++n) {
                        if (!strcasecmp("mountpoints", cacheProperties[n])) {
                            GVariant* value = g_dbus_proxy_get_cached_property(G_DBUS_PROXY(l->data), cacheProperties[n]);
                            if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING)) {
                                mountPoints += g_variant_dup_string (value, nullptr);
                            } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_BYTESTRING)) {
                                mountPoints += g_variant_dup_bytestring(value, NULL);
                            } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING_ARRAY)
                                       || g_variant_is_of_type(value, G_VARIANT_TYPE_BYTESTRING_ARRAY)
                                       || g_variant_is_of_type(value, G_VARIANT_TYPE_OBJECT_PATH_ARRAY)) {
                                const gchar** strv = nullptr;
                                if (g_variant_is_of_type(value, G_VARIANT_TYPE_BYTESTRING_ARRAY)) {
                                    strv = g_variant_get_bytestring_array (value, nullptr);
                                } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_OBJECT_PATH_ARRAY)) {
                                    strv = g_variant_get_objv(value, nullptr);
                                } else {
                                    strv = g_variant_get_strv(value, nullptr);
                                }
                                for (int m = 0; nullptr != strv && strv[m]; ++m) {
                                    mountPoints += strv[m];
                                }
                            } else {
                                qDebug() << "unKnow";
                            }
                        }
                    }
                }
            }
        }
        mBlockDevice[device] = mountPoints;
    }




    // 光盘设备相关

}

QStringList VolumeManagerPrivate::getAllBlockMountPoints()
{
    QSet<QString> mp;

    // 块设备
    for (auto& it : mBlockDevice) {
        mp += it;
    }

    // 光盘设备

    //updateBlockDeviceMountPoints(QStringList& mountPoints);

    return mp.values();
}


VolumeManager *VolumeManager::instance()
{
    static QMutex locker;

    if (!gInstance) {
        locker.lock();
        if (!gInstance) {
            gInstance = new VolumeManager;
        }
        locker.unlock();
    }

    return gInstance;
}

VolumeManager::VolumeManager(QObject *parent)
    : QObject{parent}, d_ptr(new VolumeManagerPrivate(this))
{

}


static void uevent (GUdevClient* client, const char* action, GUdevDevice* device, gpointer udata)
{
    if (!client || !action || !device || !g_udev_device_has_property(device, "DEVNAME")) return;

    VolumeManagerPrivate* vmp = static_cast<VolumeManagerPrivate*>(udata);

    const char* devName = g_udev_device_get_property (device, "DEVNAME");
    const char* idbus = g_udev_device_get_property (device, "ID_BUS");
    const char* uuid = g_udev_device_get_property (device, "ID_FS_UUID");

    qDebug() << devName << "[" << action << "] " << idbus << " -- " << uuid;

    if (vmp->mBlockDevice.contains(devName)) {
        //QString mountPoint =
    }

    qDebug() << devName;

}

static void monitor_added (GDBusObjectManager* manager, GDBusObject* object, gpointer udata)
{
    auto obj = static_cast<VolumeManagerPrivate*>(udata);
    QString path = g_dbus_object_get_object_path(object);

    if (path.startsWith("/org/freedesktop/UDisks2/block_devices/")) {
        QString devName = path.replace("/org/freedesktop/UDisks2/block_devices/", "/dev/");
        if (!obj->mBlockDevice.contains(devName)) {
            obj->mBlockDevice[devName] = QSet<QString>();
            qDebug() << "[ADD] Block device:" << devName;
        }
    }
}

static void monitor_removed (GDBusObjectManager* manager, GDBusObject* object, gpointer udata)
{
    auto obj = static_cast<VolumeManagerPrivate*>(udata);
    QString path = g_dbus_object_get_object_path(object);

    if (path.startsWith("/org/freedesktop/UDisks2/block_devices/")) {
        QString devName = path.replace("/org/freedesktop/UDisks2/block_devices/", "/dev/");
        QStringList ls1 = obj->getAllBlockMountPoints();
        if (obj->mBlockDevice.contains(devName)) {
            obj->mBlockDevice.remove(devName);
        }

        QStringList ls2 = obj->getAllBlockMountPoints();
        if (ls1.size() != ls2.size()) {
            Q_EMIT obj->q_ptr->updateBlockDeviceMountPoints(ls2);
            qDebug() << "[REMOVE] Block device:" << devName;
        }
    }
}


static void monitor_property_changed (GDBusObjectManagerClient* manager, GDBusObjectProxy* objProxy, GDBusProxy* interfaceProxy, GVariant* changeProperties, const char* const invalidProperty, gpointer udata)
{
    //if (g_strv_length((gchar**) invalidProperty) == 0)      return;
    auto obj = static_cast<VolumeManagerPrivate*>(udata);

    GVariantIter* iter = nullptr;
    const char* propertyName = nullptr;
    GVariant* value = nullptr;

    QSet<QString> mountPoints;
    g_variant_get (changeProperties, "a{sv}", &iter);
    while (g_variant_iter_next (iter, "{&sv}", &propertyName, &value)) {
        if (0 == strcasecmp("MountPoints", propertyName)) {
            if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING)) {
                mountPoints += g_variant_dup_string (value, nullptr);
            } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_BYTESTRING)) {
                mountPoints += g_variant_dup_bytestring(value, NULL);
            } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING_ARRAY)
                       || g_variant_is_of_type(value, G_VARIANT_TYPE_BYTESTRING_ARRAY)
                       || g_variant_is_of_type(value, G_VARIANT_TYPE_OBJECT_PATH_ARRAY)) {
                const gchar** strv = nullptr;

                if (g_variant_is_of_type(value, G_VARIANT_TYPE_BYTESTRING_ARRAY)) {
                    strv = g_variant_get_bytestring_array (value, nullptr);
                } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_OBJECT_PATH_ARRAY)) {
                    strv = g_variant_get_objv(value, nullptr);
                } else {
                    strv = g_variant_get_strv(value, nullptr);
                }

                for (int m = 0; nullptr != strv && strv[m]; ++m) {
                    mountPoints += strv[m];
                }
            } else {
                qDebug() << "unKnow";
            }
        }
        g_variant_unref(value);

        // 是否需要获取挂载参数：UserspaceMountOptions
    }

    QString path = g_dbus_proxy_get_object_path(interfaceProxy);

    if (mountPoints.size() > 0 && path.startsWith("/org/freedesktop/UDisks2/block_devices/")) {
        QString devName = path.replace("/org/freedesktop/UDisks2/block_devices/", "/dev/");
        if (obj->mBlockDevice.contains(devName)) {
            QSet<QString> v1 = obj->mBlockDevice[devName];
            if (v1.size() != mountPoints.size()) {
                obj->mBlockDevice[devName] = mountPoints;
                QStringList ls = obj->getAllBlockMountPoints();
                Q_EMIT obj->q_ptr->updateBlockDeviceMountPoints(ls);
                qDebug() << "[UPDATE] Block device:" << obj->mBlockDevice;
            }
        }
    }
}


static QStringList getSubdirList (const QString& path)
{
    QStringList res;
    QDir d(path);

    QFileInfoList ls = d.entryInfoList();
    for (QFileInfo& x : ls) {
        if (x.isDir()) {
            QString p = x.filePath();
            if (p == path + "/." || p == path + "/..") {
                continue;
            }
            res << p;
        }
    }

    return res;
}
