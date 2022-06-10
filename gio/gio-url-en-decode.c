/*************************************************************************
> FileName: gio-url-en-decode.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年11月03日 星期三 16时57分41秒
 ************************************************************************/
#include <gio/gio.h>

char* url_encode (const char* url);
char* url_decode (const char* url);

char* strs[] = {
    "file:///home/啊三大苏打水/bug%A9-12/",
    "file:///home/啊三大苏打水/%三大/",
    "file:///home/啊三大苏打水/三%大/",
    "file:///home/啊三大苏打水/三大%/",
    "file:///home/啊三大苏打水/三大%1/",
    "file:///home/啊三大苏打水/三大%1/%21啊三大萨斯",
    "file:///home/啊三大苏打水/三大%1/%21啊三大萨斯%asd",
    "file:///home/啊三大苏打水/三大%1/%21啊三大萨斯%%",
    "file:///home/啊三大苏打水/三大d%/1%20%20%E5%89%AF%E6%9C%AC(2).txt",
};

int main (int argc, char* argv[])
{
    for (int i = 0; i < sizeof (strs) / sizeof (strs[0]); ++i) {
        char* str = strs[i];

        g_autofree char* encode = url_encode (str);
        g_autofree char* eencode = url_encode (encode);
        printf ("str: %s  ==>  encode: %s ==> eencode %s\n", str, encode, eencode);

        g_autofree char* decode = url_decode (encode);
        g_autofree char* ddecode = url_decode (decode);
        printf ("str: %s  ==>  decode: %s ==> ddecode %s\n", encode, decode, ddecode);
        printf ("========================\n");
    }

    return 0;
}

char* url_encode (const char* url)
{
    g_return_val_if_fail (url, NULL);

    g_autofree char* decodeUrl = url_decode (url);
    if (decodeUrl) {
        g_autofree char* encode = g_uri_escape_string (decodeUrl, ":/", TRUE);
        return g_strdup (encode);
    }
    
    g_autofree char* encode = g_uri_escape_string (url, ":/", TRUE);

    return g_strdup (encode);
}

/**
 * 此处为了兼容 windows 编码, 做如下特殊处理
 */
char* url_decode (const char* url)
{
    g_return_val_if_fail (url, NULL);

    g_autofree char* decode = g_uri_unescape_string (url, ":/");
    if (decode) {
        return g_strdup (decode);
    }

    return g_strdup (url);
}
