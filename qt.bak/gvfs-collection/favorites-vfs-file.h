#ifndef FAVORITESVFSFILE_H
#define FAVORITESVFSFILE_H

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define VFS_TYPE_FAVORITES_FILE vfs_favorites_file_get_type()

G_DECLARE_FINAL_TYPE(FavoritesVFSFile, vfs_favorites_file, VFS, FAVORITES_FILE, GObject)

FavoritesVFSFile* vfs_favorites_file_new(void);

typedef struct _FavoritesVFSFilePrivate FavoritesVFSFilePrivate;

struct _FavoritesVFSFilePrivate
{
    gchar* uri;
};

struct _FavoritesVFSFile
{
    GObject parent_instance;
    FavoritesVFSFilePrivate* priv;
};

G_END_DECLS

extern "C"
{
    GFile* vfs_favorites_file_new_for_uri (const char* uri);
    static GFileEnumerator* vfs_favorites_file_enumerate_children_internal (GFile* file, const char* attribute, GFileQueryInfoFlags flags, GCancellable* cancellable, GError** error);
}


#endif // FAVORITESVFSFILE_H
