/*************************************************************************
> FileName: kb-bibtex.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月20日 星期五 21时31分13秒
 ************************************************************************/
#include "kb-bibtex.h"

G_DEFINE_TYPE (kb_bibtex, kb_bibtex, G_TYPE_OBJECT);

#define KB_BIBTEX_GET_PRIVATE(obj)\
    G_TYPE_INSTANCE_GET_PRIVATE ((obj), KB_TYPE_BIBTEX, kb_bibtex_private_t)

typedef struct _kb_bibtex_private_t kb_bibtex_private_t;

struct _kb_bibtex_private_t
{
    GString* title;
    GString* author;
    GString* publisher;
    guint year;
};

enum PRIPERTY_BIBTEX 
{
    PROPERTY_0,
    PROPERTY_TITLE,
    PROPERTY_AUTHOR,
    PROPERTY_PUBLISHER,
    PROPERTY_YEAR,
    N_PROPERTIES,
};

static void kb_bibtex_set_property (GObject* obj, guint property_id, const GValue* value, GParamSpec* pspec)
{
    kb_bibtex* self = KB_BIBTEX(obj);
    kb_bibtex_private_t* priv = KB_BIBTEX_GET_PRIVATE(self);

    switch (property_id) {
    case PROPERTY_TITLE:
        if (NULL != priv->title) g_string_free (priv->title, TRUE);
        priv->title = g_string_new (g_value_get_string(value));
        break;
    case PROPERTY_AUTHOR:
        if (NULL != priv->author) g_string_free (priv->author, TRUE);
        priv->author = g_string_new (g_value_get_string(value));
        break;
    case PROPERTY_PUBLISHER:
        if (NULL != priv->publisher) g_string_free (priv->publisher, TRUE);
        priv->publisher = g_string_new (g_value_get_string(value));
        break;
    case PROPERTY_YEAR:
        priv->year = g_value_get_uint (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property_id, pspec);
    }
}

static void kb_bibtex_get_property (GObject* obj, guint property_id, GValue* value, GParamSpec* spec)
{
    kb_bibtex* self = KB_BIBTEX(obj);
    kb_bibtex_private_t* priv = KB_BIBTEX_GET_PRIVATE(self);
    GString* similar = NULL;

    switch (property_id) {
    case PROPERTY_TITLE:
        g_value_set_string (value, priv->title->str);
        break;
    case PROPERTY_AUTHOR:
        g_value_set_string (value, priv->author->str);
        break;
    case PROPERTY_PUBLISHER:
        g_value_set_string (value, priv->publisher->str);
        break;
    case PROPERTY_YEAR:
        g_value_set_uint (value, priv->year);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property_id, spec);
        break;
    }
}

static void kb_bibtex_init (kb_bibtex* self)
{

}

static void kb_bibtex_class_init (kb_bibtexClass* kclass)
{
    g_type_class_add_private (kclass, sizeof (kb_bibtex_private_t));

    GObjectClass* base_class = G_OBJECT_CLASS (kclass);
    base_class->set_property = kb_bibtex_set_property;
    base_class->get_property = kb_bibtex_get_property;
    GParamSpec* properties[N_PROPERTIES] = {NULL,};

    properties[PROPERTY_TITLE] = g_param_spec_string ("title", "Title", "...", NULL, G_PARAM_READWRITE);
    properties[PROPERTY_AUTHOR] = g_param_spec_string ("author", "Author", "...", NULL, G_PARAM_READWRITE);
    properties[PROPERTY_PUBLISHER] = g_param_spec_string ("publisher", "Publisher", "...", NULL, G_PARAM_READWRITE);
    properties[PROPERTY_YEAR] = g_param_spec_uint ("year", "Year", "...", 0, G_MAXUINT, 0, G_PARAM_READWRITE);

    g_object_class_install_properties (base_class, N_PROPERTIES, properties);
}

void kb_bibtex_printf (kb_bibtex* self)
{
    gchar* title, *author, *publisher;
    guint year;

    g_object_get (G_OBJECT (self), "title", &title, "author", &author, "publisher", &publisher, "year", &year, NULL);
    g_print ("      Title:%s\n"
              "     Author:%s\n"
              "  Publisher:%s\n"
              "       Year:%d\n", title, author, publisher, year);

    g_free (title);
    g_free (author);
    g_free (publisher);
}
