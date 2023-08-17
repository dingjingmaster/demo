/*************************************************************************
> FileName: x-all-window.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年04月03日 星期五 08时13分52秒
 ************************************************************************/
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>

#define MAXSTR  500000

int main (int argc, char* argv[])
{
    Display         *display = NULL;
    Window          rootWindow;
    unsigned int    winNum;
    Window          rootReturn, parentReturn;
    Window*         childList = NULL;

    display = XOpenDisplay (0);
    rootWindow = DefaultRootWindow (display);

    XQueryTree (display, rootWindow, &rootReturn, &parentReturn, &childList, &winNum);

    printf ("window num: %d\n", winNum);
    for (unsigned int i = 0; i < winNum; ++i) {
        printf ("window ID: 0x%1x\n", childList[i]);
        // 属性
        int nProps = 0;
        Atom* proLists = XListProperties (display, childList[i], &nProps);
        for (int j = 0; j < nProps; ++j) {
            Atom propertyAtom;
            Atom actualType;
            int actualFormat;
            unsigned long numItems, bytesAfter;
            unsigned char* propertyValue;
            char* name = XGetAtomName (display, proLists[j]);
            printf ("  property[%d]=%s ", j, name);
            // 属性值
            if (Success == XGetWindowProperty(display, childList[i], proLists[j], 0, (MAXSTR + 3) / 4, False, AnyPropertyType, &actualType, &actualFormat, &numItems, &bytesAfter, &propertyValue)) {
                printf ("\t");
                for (int k = 0; k < numItems;) {
                    if (actualType == XA_STRING && actualFormat == 8) {
                        printf ("%c", *(propertyValue + k));
                        k += sizeof(char);
                    }
                    else if (actualType == 32) {
                        printf ("%d ", (long)*(propertyValue + k));
                        k += sizeof(long);
                    }
                    else if (actualType == 16) {
                        printf ("%d ", (short)*(propertyValue + k));
                        k += sizeof(short);
                    }
                    else {
                        //printf (" ");
                        ++k;
                    }
                }
                printf ("\n");
                XFree (propertyValue);
            }
        }
    }

    XFree(childList);
    XCloseDisplay(display);

    return 0;
}
