/*************************************************************************
> FileName: demo7.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月19日 星期四 15时32分13秒
 ************************************************************************/
#include <math.h>
#include <glib-object.h>

/**
 * gobject 版闭包
 */
void g_closure_user_marshal_INT__VOID_VOID (GClosure* closure, GValue* return_value,
        guint n_param_values, const GValue* param_values, gpointer invocation_hint, gpointer marshal_data)
{
    typedef gint (*GMarshalFunc_INT__VOID_VOID) (gpointer data1, gpointer data2);

    register GMarshalFunc_INT__VOID_VOID callback;
    register GCClosure* cc = (GCClosure*) closure;
    register gpointer data1, data2;
    gint v_return;

    g_return_if_fail (return_value != NULL);
    g_return_if_fail (n_param_values == 1);

    if (G_CCLOSURE_SWAP_DATA(closure)) {
        data1 = closure->data;
        data2 = g_value_peek_pointer (param_values + 0);
    } else {
        data1 = g_value_peek_pointer (param_values + 0);
        data2 = closure->data;
    }
    callback = (GMarshalFunc_INT__VOID_VOID) {marshal_data ? marshal_data : cc->callback};
    v_return = callback (data1, data2);
    g_value_set_int (return_value, v_return);
}

static void compare (GClosure* closure, void* b)
{
    GValue return_value = {0};
    GValue param_value = {0};
    g_value_init (&return_value, G_TYPE_INT);
    g_value_init (&param_value, G_TYPE_POINTER);

    g_value_set_pointer (&param_value, b);

    g_closure_invoke (closure, &return_value, 1, &param_value, NULL);
    gint r = g_value_get_int (&return_value);

    if (r == -1) {
        printf ("a < b\n");
    } else if (r == 0) {
        printf ("a = b\n");
    } else {
        printf ("a > b\n");
    }
    g_value_unset (&return_value);
    g_value_unset (&param_value);
}

static gint float_compare (void* a, void* b)
{
    gfloat* f1 = a;
    gfloat* f2 = b;

    if (*f1 > *f2) {
        return 1;
    } else if (fabs (*f1 - *f2) <= 10E-6) {
        return 0;
    } else {
        return -1;
    }
}

static gint str_compare (void* a, void* b)
{
    size_t len1 = g_utf8_strlen ((gchar*)a, -1);
    size_t len2 = g_utf8_strlen ((gchar*)b, -1);

    if (len1 > len2) {
        return 1;
    } else if (len1 == len2) {
        return 0;
    } else {
        return -1;
    }
}

int main (void)
{
    g_type_init ();

    gfloat a = 123.567;
    gfloat b = 222.222;
    GClosure* closure = g_cclosure_new (G_CALLBACK(float_compare), &a, NULL);
    g_closure_set_marshal (closure, g_closure_user_marshal_INT__VOID_VOID);
    compare (closure, &b);
    g_closure_unref (closure);

    gchar* s1 = "Hello World!\n";
    gchar* s2 = "Hello!\n";
    closure = g_cclosure_new (G_CALLBACK(str_compare), s1, NULL);
    g_closure_set_marshal (closure, g_closure_user_marshal_INT__VOID_VOID);
    compare (closure, s2);
    g_closure_unref (closure);

    return 0;
}
