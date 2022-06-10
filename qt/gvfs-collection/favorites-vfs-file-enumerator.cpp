#include "favorites-vfs-file-enumerator.h"


G_DEFINE_TYPE_WITH_PRIVATE(FavoritesVFSFileEnumerator, vfs_favorites_file_enumerator, G_TYPE_FILE_ENUMERATOR);

void vfs_favorites_file_enumerator_dispose(GObject *object);
static gboolean enumerator_close (GFileEnumerator *enumerator, GCancellable *cancellable, GError **error);
static GFileInfo *enumerate_next_file (GFileEnumerator *enumerator, GCancellable *cancellable, GError **error);

static void vfs_favorites_file_enumerator_init (FavoritesVFSFileEnumerator* self)
{
    FavoritesVFSFileEnumeratorPrivate* priv = (FavoritesVFSFileEnumeratorPrivate*) vfs_favorites_file_enumerator_get_instance_private(self);

    self->priv = priv;
    self->priv->enumerate_queue = new QQueue<QString>;
}

static void vfs_favorites_file_enumerator_class_init (FavoritesVFSFileEnumeratorClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GFileEnumeratorClass *enumerator_class = G_FILE_ENUMERATOR_CLASS(klass);

    gobject_class->dispose = vfs_favorites_file_enumerator_dispose;
    enumerator_class->next_file = enumerate_next_file;
    enumerator_class->close_fn = enumerator_close;
}

void vfs_favorites_file_enumerator_dispose(GObject *object)
{
    FavoritesVFSFileEnumerator *self = VFS_FAVORITES_FILE_ENUMERATOR(object);


}

static GFileInfo *enumerate_next_file (GFileEnumerator *enumerator, GCancellable *cancellable, GError **error)
{
    static bool all = true;
    printf("\nenumerate_next_file\n");

    auto ve = VFS_FAVORITES_FILE_ENUMERATOR(enumerator);
    auto eq = ve->priv->enumerate_queue;

    printf("\nenumerate_next_file  1\n");
    if (all) {
        eq->enqueue("file:///");
        eq->enqueue("file:///home/");
        eq->enqueue("file:///home/dingjing");
        all = false;
    }

    while (!eq->isEmpty()) {
        printf("\nenumerate_next_file  2\n");
        auto uri = eq->dequeue();
        GFileInfo *tmp = g_file_info_new();
        QString realUriSuffix = "real-uri:" + uri;

        g_file_info_set_name(tmp, realUriSuffix.toUtf8().constData());

        return tmp;
    }

    return nullptr;
}

static gboolean enumerator_close(GFileEnumerator *enumerator, GCancellable *cancellable, GError **error)
{
    FavoritesVFSFileEnumerator *self = VFS_FAVORITES_FILE_ENUMERATOR(enumerator);

    return true;
}
