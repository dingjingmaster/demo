/*************************************************************************
> FileName: kb-bibtex.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月20日 星期五 21时03分38秒
 ************************************************************************/
#ifndef _KB_BIBTEX_H
#define _KB_BIBTEX_H
#include <glib-object.h>

#define KB_TYPE_BIBTEX (kb_bibtex_get_type())
#define KB_BIBTEX(obj)\
    G_TYPE_CHECK_INSTANCE_CAST((obj), KB_TYPE_BIBTEX, kb_bibtex)
#define KB_IS_BIBTEX(obj)\
    G_TYPE_CHECK_INSTANCE_TYPE((obj), KB_TYPE_BIBTEX)

#define KB_BIBTEX_CLASS(kclass)\
    (G_TYPE_CHECK_CLASS_CAST ((kclass), KB_TYPE_BIBTEX, kb_bibtexClass))
#define KB_IS_BIBTEX_CLASS(kclass)\
    (G_TYPE_CHECK_CLASS_TYPE ((kclass), KB_TYPE_BIBTEX))
#define KB_BIBTEX_GET_CLASS(obj)\
    (G_TYPE_INSTANCE_GET_CLASS ((obj), KB_TYPE_BIBTEX, kb_bibtexClass))

typedef struct _kb_bibtex kb_bibtex;
typedef struct _kb_bibtexClass kb_bibtexClass;

struct _kb_bibtex 
{
    GObject parent;
};

struct _kb_bibtexClass
{
    GObjectClass parent_class;
};

GType kb_bibtex_get_type (void);

void kb_bibtex_printf (kb_bibtex* self);

#endif
