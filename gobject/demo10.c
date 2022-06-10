/*************************************************************************
> FileName: demo10.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月20日 星期五 17时16分50秒
 ************************************************************************/
#include <gjs/gjs.h>
#include <girepository.h>

int main (int argc, char* argv[])
{
    /*
    g_type_init();
    kb_bibtex_t* e = g_object_new (KB_TYPE_BIBTEX, "title", "书名", "author", "作者名", "publisher", "出版社", "year", 20191201, NULL);
    kb_bibtex_printf (e);
    g_object_unref (e);
    */

    g_type_init();
    gchar* js_path[]={"./demo10/"};
    gchar* gir_path = "./demo10/";

    GOptionContext* ctx = g_option_context_new (NULL);
    g_option_context_add_group (ctx, g_irepository_get_option_group());
    g_option_context_parse (ctx, &argc, &argv, NULL);

    // 设置 typelib 文件查询路径
    g_irepository_prepend_search_path (gir_path);

    // 设置 javascript 文件路径并解析执行指定的 javascript 脚本
    GjsContext* gjs_ctx = gjs_context_new_with_search_path (js_path);
    gjs_context_eval (gjs_ctx, "const Main = imports.main; Main.start();", /* 要执行的代码 */
            -1, "<main>", /*main.js文件名*/NULL, NULL);

    return 0;
}
