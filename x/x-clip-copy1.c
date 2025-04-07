/*************************************************************************
> FileName: x-clip-copy1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 18 Aug 2023 11:10:03 AM CST
 ************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>

int main (int argc, char* argv[])
{
    char*               recType = NULL;
    Display*            display = XOpenDisplay(NULL);

    XrmValue            recVal;
    XrmDatabase         optDB = NULL;

    // 支持的命令行参数
    XrmOptionDescRec    optTable[] = {
        {
            .option = "-loops",
            .specifier = ".loops",
            .argKind = XrmoptionSepArg,
            .value = NULL
        },
        {
            .option = "-selection",
            .specifier = ".selection",
            .argKind = XrmoptionSepArg,
            .value = NULL
        },
        {
            .option = "-in",
            .specifier = ".direction",
            .argKind = XrmoptionNoArg,
            .value = "I"
        }
    };

    // 初始化资源管理器
    XrmInitialize();

    // 命令行参数解析到资源数据库
    XrmParseCommand(&optDB, optTable, sizeof(optTable)/sizeof(optTable[0]), "xclip", &argc, argv);

    if (XrmGetResource (optDB, "xclip.selection", "Xclip.Selection", &recType, &recVal)) {
        switch (tolower(recVal.addr[0])) {
        case 'p':
            printf ("XA_PRIMARY\n");
            break;
        case 's':
            printf ("XA_SECONDARY\n");
            break;
        case 'c':
            printf ("XA_CLIPBOARD\n");
            break;
        case 'b':
            printf ("XA_STRING");
            break;
        }
    }

    XCloseDisplay(display);

    return 0;
}

