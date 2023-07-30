#ifndef FAVORITESVFSFILEENUMERATOR_H
#define FAVORITESVFSFILEENUMERATOR_H

#include <gio/gio.h>
#include <QQueue>
#include <QRegExp>

G_BEGIN_DECLS

#define VFS_TYPE_FAVORITES_FILE_ENUMERATOR vfs_favorites_file_enumerator_get_type()

G_DECLARE_FINAL_TYPE(FavoritesVFSFileEnumerator, vfs_favorites_file_enumerator, VFS, FAVORITES_FILE_ENUMERATOR, GFileEnumerator)

FavoritesVFSFileEnumerator* vfs_favorites_file_enumerator_new(void);

typedef struct _FavoritesVFSFileEnumeratorPrivate       FavoritesVFSFileEnumeratorPrivate;

struct _FavoritesVFSFileEnumeratorPrivate
{
    QQueue<QString> *enumerate_queue;
};

struct _FavoritesVFSFileEnumerator
{
    GFileEnumerator parent_instance;

    FavoritesVFSFileEnumeratorPrivate* priv;
};

G_END_DECLS


#endif // FAVORITESVFSFILEENUMERATOR_H
