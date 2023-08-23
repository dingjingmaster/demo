/*************************************************************************
> FileName: x-get-mouse-win.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 23 Aug 2023 03:49:55 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

int main (int argc, char* argv[])
{
    Display* dsp = XOpenDisplay(NULL);
    Window win = DefaultRootWindow(dsp);

    Window rootRet, childRet;
    int rootX, rootY, winX, winY;
    unsigned int maskRet;

    XQueryPointer(dsp, win, &rootRet, &childRet, &rootX, &rootY, &winX, &winY, &maskRet);

    char* title = NULL;
    XFetchName(dsp, childRet, &title);

    printf ("mouse Window ID: %8lu(0x%-8X), title: %s\n", childRet, childRet, title?title:"<title>");

    unsigned int    winNum;
    Window          rootReturn, parentReturn;
    Window*         childList = NULL;
    XQueryTree (dsp, childRet, &rootReturn, &parentReturn, &childList, &winNum);
    printf ("child window num: %d\n", winNum);
    for (unsigned int i = 0; i < winNum; ++i) {
        printf ("window ID: 0x%1x\n", childList[i]);
        // 属性
        int nProps = 0;
        Atom* proLists = XListProperties (dsp, childList[i], &nProps);
        for (int j = 0; j < nProps; ++j) {
            Atom propertyAtom;
            Atom actualType;
            int actualFormat;
            unsigned long numItems, bytesAfter;
            unsigned char* propertyValue;
            char* name = XGetAtomName (dsp, proLists[j]);
            printf ("  property[%d]=%s ", j, name);
            // 属性值
            if (Success == XGetWindowProperty(dsp, childList[i], proLists[j], 0, (5000 + 3) / 4, False, AnyPropertyType, &actualType, &actualFormat, &numItems, &bytesAfter, &propertyValue)) {
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



    int nProps = 0;
    Atom* proLists = XListProperties (dsp, childRet, &nProps);
    for (int j = 0; j < nProps; ++j) {
        Atom propertyAtom;
        Atom actualType;
        int actualFormat;
        unsigned long numItems, bytesAfter;
        unsigned char* propertyValue;
        char* name = XGetAtomName (dsp, proLists[j]);
        printf ("  property[%d]=%s ", j, name);
        // 属性值
        if (Success == XGetWindowProperty(dsp, childRet, proLists[j], 0, (500 + 3) / 4, False, AnyPropertyType, &actualType, &actualFormat, &numItems, &bytesAfter, &propertyValue)) {
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

    XCloseDisplay(dsp);

    return 0;
}
