/*************************************************************************
> FileName: demo1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年01月02日 星期四 14时40分07秒
 ************************************************************************/
#include <locale.h>
#include <libintl.h> 
#include <stdio.h>

#define PACKAGE "demo1"
#define LOCALEDIR "po"
#define _(msgid) gettext(msgid) 
 
int main (int argc, char* argv[])
{
   setlocale (LC_CTYPE, ""); 
   setlocale (LC_MESSAGES, ""); 

    bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);

    char* str = "english";

    // _("") 必须是字符串，不能是指针
    printf ("%s -> %s\n", str, _("english"));

    return 0;
}
