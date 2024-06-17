/*************************************************************************
> FileName: glib-variant.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 11 Jan 2023 03:17:46 PM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <glib/gi18n.h>

int main (int argc, char* argv[])
{
    GVariantType* intI32 = g_variant_type_new ("i");
    GVariantType* arrI32 = g_variant_type_new_array (intI32);


    printf ("peek_string: %s\n", g_variant_type_peek_string(intI32));
    printf ("peek_string: %s\n", g_variant_type_peek_string(arrI32));

    GVariantBuilder builder;

    g_variant_builder_init (&builder, arrI32);

    GVariant* variant = g_variant_builder_end (&builder);
    printf ("peek_string: %s\n", g_variant_get_type_string(variant));



    if (intI32)     g_variant_type_free (intI32);
    if (arrI32)     g_variant_type_free (arrI32);
    if (variant)    g_variant_unref (variant);

    return 0;
}
