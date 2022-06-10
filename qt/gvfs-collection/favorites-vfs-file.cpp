#include "favorites-vfs-file.h"
#include "favorites-vfs-file-enumerator.h"

#include <QDebug>

static void vfs_favorites_file_g_file_iface_init(GFileIface *iface);

char* vfs_favorites_file_get_uri(GFile *file);
GFile* vfs_favorites_file_get_parent(GFile *file);
GFile* vfs_favorites_file_new_for_uri(const char *uri);
static void vfs_favorites_file_dispose(GObject *object);
static GFile* vfs_favorites_file_dup(GFile *file);
static gboolean vfs_favorites_file_is_native(GFile *file);
char* vfs_favorites_file_get_path(GFile *file);
GFile* vfs_favorites_file_resolve_relative_path(GFile *file, const char *relative_path);
GFileInfo* vfs_favorites_file_query_info(GFile *file, const char *attributes, GFileQueryInfoFlags flags, GCancellable *cancellable, GError **error);
GFileEnumerator* vfs_favorites_file_enumerate_children(GFile *file, const char *attribute, GFileQueryInfoFlags flags, GCancellable *cancellable, GError **error);
GFileEnumerator *vfs_favorites_file_enumerate_children_internal(GFile *file, const char *attribute, GFileQueryInfoFlags flags, GCancellable *cancellable, GError **error);


G_DEFINE_TYPE_EXTENDED(FavoritesVFSFile, vfs_favorites_file, G_TYPE_OBJECT, 0, G_ADD_PRIVATE(FavoritesVFSFile) G_IMPLEMENT_INTERFACE(G_TYPE_FILE, vfs_favorites_file_g_file_iface_init));


static void vfs_favorites_file_init(FavoritesVFSFile* self)
{
    FavoritesVFSFilePrivate *priv = (FavoritesVFSFilePrivate*)vfs_favorites_file_get_instance_private(self);
    self->priv = priv;
    priv->uri = nullptr;
}

static void vfs_favorites_file_class_init (FavoritesVFSFileClass* kclass)
{
    GObjectClass* gobject_class = G_OBJECT_CLASS (kclass);

    gobject_class->dispose = vfs_favorites_file_dispose;
}

static void vfs_favorites_file_dispose(GObject *object)
{
    auto vfs_file = VFS_FAVORITES_FILE (object);
    if (vfs_file->priv->uri) {
        g_free(vfs_file->priv->uri);
    }
}

static void vfs_favorites_file_g_file_iface_init(GFileIface *iface)
{
    iface->dup = vfs_favorites_file_dup;
    iface->get_parent = vfs_favorites_file_get_parent;
    iface->is_native = vfs_favorites_file_is_native;
    iface->enumerate_children = vfs_favorites_file_enumerate_children;
    iface->query_info = vfs_favorites_file_query_info;
    iface->get_uri = vfs_favorites_file_get_uri;
    iface->get_path = vfs_favorites_file_get_path;
    iface->resolve_relative_path = vfs_favorites_file_resolve_relative_path;
}


char* vfs_favorites_file_get_uri(GFile *file)
{
    auto vfs_file = VFS_FAVORITES_FILE (file);

    return g_strdup(vfs_file->priv->uri);
}

GFile* vfs_favorites_file_get_parent(GFile *file)
{
    Q_UNUSED(file);

    return nullptr;
}

gboolean vfs_favorites_file_is_native(GFile *file)
{
    Q_UNUSED(file);

    return false;
}

GFile* vfs_favorites_file_dup(GFile *file)
{
    if (!VFS_FAVORITES_FILE(file)) {
        return g_file_new_for_uri("favorite:///");
    }
    auto vfs_file = VFS_FAVORITES_FILE(file);
    auto dup = VFS_FAVORITES_FILE(g_object_new(VFS_TYPE_FAVORITES_FILE, nullptr));
    dup->priv->uri = g_strdup(vfs_file->priv->uri);

    return G_FILE(dup);
}

GFileEnumerator* vfs_favorites_file_enumerate_children_internal(GFile *file, const char *attribute, GFileQueryInfoFlags flags, GCancellable *cancellable, GError **error)
{
//    auto vfs_file = VFS_FAVORITES_FILE(file);
    auto enumerator = VFS_FAVORITES_FILE_ENUMERATOR(g_object_new(VFS_TYPE_FAVORITES_FILE_ENUMERATOR, "container", file, nullptr));
//    vfs_favorites_file_enumerator_parse_uri(enumerator, vfs_file->priv->uri);

    Q_UNUSED(flags)
    Q_UNUSED(error)
    Q_UNUSED(attribute)
    Q_UNUSED(cancellable)

    return G_FILE_ENUMERATOR(enumerator);
}

GFile* vfs_favorites_file_new_for_uri(const char *uri)
{
    auto search_vfs_file = VFS_FAVORITES_FILE(g_object_new(VFS_TYPE_FAVORITES_FILE, nullptr));
    search_vfs_file->priv->uri = g_strdup(uri);

    return G_FILE(search_vfs_file);
}

char* vfs_favorites_file_get_path(GFile *file)
{
    Q_UNUSED(file);

    return nullptr;
}

GFile* vfs_favorites_file_resolve_relative_path(GFile *file, const char *relative_path)
{
    Q_UNUSED(file);

    QString tmp = relative_path;
    if (tmp.contains("real-uri:")) {
        tmp = tmp.remove("real-uri:");
        return g_file_new_for_uri(tmp.toUtf8().constData());
    }
    return g_file_new_for_uri("favorite:///");
}

GFileInfo* vfs_favorites_file_query_info(GFile *file, const char *attributes, GFileQueryInfoFlags flags, GCancellable *cancellable, GError **error)
{
    auto vfs_file = VFS_FAVORITES_FILE(file);
    GFileInfo *info = g_file_info_new();
    g_file_info_set_name(info, vfs_file->priv->uri);
    auto icon = g_themed_icon_new("favorite");
    g_file_info_set_icon(info, icon);
    g_object_unref(icon);
    g_file_info_set_display_name(info, "");
    g_file_info_set_file_type(info, G_FILE_TYPE_DIRECTORY);

    Q_UNUSED(flags)
    Q_UNUSED(error)
    Q_UNUSED(attributes)
    Q_UNUSED(cancellable)

    return info;
}

GFileEnumerator* vfs_favorites_file_enumerate_children(GFile *file, const char *attribute, GFileQueryInfoFlags flags, GCancellable *cancellable, GError **error)
{
    auto vf = VFS_FAVORITES_FILE(file);

    return vfs_favorites_file_enumerate_children_internal(file, attribute, flags, cancellable, error);
}
