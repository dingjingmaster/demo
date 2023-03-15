/*************************************************************************
> FileName: glib-hmac.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 15 Mar 2023 10:53:27 AM CST
 ************************************************************************/
#include <glib.h>
#include <glib/gi18n.h>


/**
 * @brief
 *  根据数据和键生成 cookies 或 散列时候可以使用
 */
int main (int argc, char* argv[])
{
    const char* key = "test-key";

    GHmac* mac = g_hmac_new(G_CHECKSUM_MD5, key, strlen(key));

    g_hmac_update(mac, "1", 1);

    printf ("mac: %s\n", g_hmac_get_string(mac));

    return 0;
}
