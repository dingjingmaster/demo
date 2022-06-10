/*************************************************************************
> FileName: demo2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年01月02日 星期四 15时03分05秒
 ************************************************************************/
#include <locale.h> 
#include <libintl.h> 
#include <stdio.h> 
 
#define PACKAGE "demo2"
#define LOCALEDIR "po"
#define _(msgid) gettext(msgid) 
 
int main (int argc, char *argv[]) 
{ 
   setlocale (LC_CTYPE, "zh_CN.UTF-8"); 
   setlocale (LC_MESSAGES, "zh_CN.UTF-8"); 
 
   bindtextdomain (PACKAGE, LOCALEDIR); 
   textdomain (PACKAGE); 
 
   printf (_("Are you ok?\n")); 
 
   return 0; 
}
