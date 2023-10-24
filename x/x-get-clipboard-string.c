/*************************************************************************
> FileName: x-get-clipboard-string.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 24 Oct 2023 02:23:41 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>

static size_t mach_item_size(int format);
static void print_buf (FILE* fout, Atom selType, unsigned char* selBuf, size_t selLen);
static int xcout (Display* dsp, Window win, XEvent event, Atom sel, Atom target, Atom* type, unsigned char** txt, unsigned long* len, unsigned int* ctx);

#define F 0
#define T 1

#define SF "F"
#define ST "T"

#define XCLIB_XCOUT_NONE                0   // no context
#define XCLIB_XCOUT_SENTCONVSEL         1   // sent a request
#define XCLIB_XCOUT_INCR                2   // in an incr loop
#define XCLIB_XCOUT_BAD_TARGET          3   // given target failed

static int fsecm = F;
static Display* dsp = NULL;

int main (int argc, char* argv[])
{
    dsp = XOpenDisplay(NULL);
    if (!dsp) {
        printf ("XOpenDisplay error\n");
        return -1;
    }

    do {
        Window win = XCreateSimpleWindow(dsp, DefaultRootWindow (dsp), 0, 0, 1, 1, 0, 0, 0);
        XSelectInput(dsp, win, PropertyChangeMask);

        Atom selType = None;
        unsigned char* selBuf = NULL;
        unsigned long selLen = 0;
        XEvent event;
        unsigned int context = XCLIB_XCOUT_NONE;

        Atom target = XA_UTF8_STRING(dsp);
        Atom clipboard = XInternAtom (dsp, "CLIPBOARD", False);

        while (1) {
            if (context != XCLIB_XCOUT_NONE) {
                XNextEvent(dsp, &event);
            }

            xcout (dsp, win, event, clipboard, target, &selType, &selBuf, &selLen, &context);
            if (XCLIB_XCOUT_BAD_TARGET == context) {
                if (XA_UTF8_STRING(dsp) == target) {
                    context = XCLIB_XCOUT_NONE;
                    target = XA_STRING;
                    continue;
                }
                else {
                    if (fsecm) {
                        XSetSelectionOwner (dsp, selLen, None, CurrentTime);
                        memset (selBuf, 0, selLen);
                    }
 
                    free (selBuf);
                }
            }
            if (XCLIB_XCOUT_NONE == context) break;
        }

        if (selLen > 0) {
            print_buf (stdout, selType, selBuf, selLen);
            free (selBuf);
        }
    } while (0);


    if (dsp) {
        XFlush(dsp);
        XCloseDisplay(dsp);
    }

    (void) argc;
    (void) argv;

    return 0;
}

static size_t mach_item_size(int format)
{
    if (format == 8)
	return sizeof(char);
    if (format == 16)
	return sizeof(short);
    if (format == 32)
	return sizeof(long);
    return 0;
}

static int xcout (Display* dsp, Window win, XEvent event, Atom sel, Atom target, Atom* type, unsigned char** txt, unsigned long* len, unsigned int* ctx)
{
    static Atom pty;
    static Atom inc;

    int ptyFormat;

    // 将 XGetWindowProperty 的数据 dump 到下边 buffer 中
    unsigned char* buffer;
    unsigned long ptySize, ptyItems, ptyMachsize;

    unsigned char* lTxt = *txt;

    if (!pty) {
        pty = XInternAtom (dsp, "XCLIP_OUT", False);
    }

    if (!inc) {
        inc = XInternAtom (dsp, "INCR", False);
    }

    switch (*ctx) {
        case XCLIB_XCOUT_NONE: {
            if (*len > 0) {
                free (*txt);
                *len = 0;
            }
            XConvertSelection (dsp, sel, target, pty, win, CurrentTime);
            *ctx = XCLIB_XCOUT_SENTCONVSEL;
            return 0;
        }
        case XCLIB_XCOUT_SENTCONVSEL: {
            if (event.type != SelectionNotify) {
                return 0;
            }
            if (event.xselection.property == None) {
                *ctx = XCLIB_XCOUT_BAD_TARGET;
                return 0;
            }
            XGetWindowProperty (dsp, win, pty, 0, 0, False, AnyPropertyType, type, &ptyFormat, &ptyItems, &ptySize, &buffer);
            XFree (buffer);
            if (*type == inc) {
                printf ("[debug] 需要多次读取数据!\n");
                XDeleteProperty(dsp, win, pty);
                XFlush(dsp);
                *ctx = XCLIB_XCOUT_INCR;
                return 0;
            }
            XGetWindowProperty(dsp, win, pty, 0, (long) ptySize, False, AnyPropertyType, type, &ptyFormat, &ptyItems, &ptySize, &buffer);
            XDeleteProperty(dsp, win, pty);
            ptyMachsize = ptyItems * mach_item_size(ptyFormat);

            // FIXME://
            lTxt = (unsigned char*) malloc (ptyMachsize);
            if (!lTxt) {
                printf ("malloc error!\n");
                exit(-1);
            }
            memset (lTxt, 0, ptyMachsize);
            memcpy (lTxt, buffer, ptyMachsize);

            *len = ptyMachsize;
            *txt = lTxt;
            XFree (buffer);
            *ctx = XCLIB_XCOUT_NONE;
            return 1;
        }
        case XCLIB_XCOUT_INCR: {
            if (event.type != PropertyNotify) {
                return 0;
            }
            if (event.xproperty.state != PropertyNewValue) {
                return 0;
            }
            XGetWindowProperty (dsp, win, pty, 0, 0, False, AnyPropertyType, type, &ptyFormat, &ptyItems, &ptySize, (unsigned char**) &buffer);
            if (0 == ptySize) {
                printf ("[debug] INCR 传输完成!\n");
                XFree(buffer);
                XDeleteProperty (dsp, win, pty);
                *ctx = XCLIB_XCOUT_NONE;
                return 1;
            }
            XFree(buffer);
            XGetWindowProperty (dsp, win, pty, 0, (long) ptySize, False, AnyPropertyType, type, &ptyFormat, &ptyItems, &ptySize, (unsigned char**) &buffer);
            ptyMachsize = ptyItems * mach_item_size (ptyFormat);
            if (0 == *len) {
                *len = ptyMachsize;
                lTxt = (unsigned char*) malloc (*len);
            }
            else {
                *len += ptyMachsize;
                // FIXME://
                lTxt = (unsigned char*) realloc (lTxt, *len);
                if (!lTxt) {
                    printf ("malloc error\n");
                    exit(-1);
                }

                memcpy (&lTxt[*len - ptyMachsize], buffer, ptyMachsize);
                *txt = lTxt;
                XFree (buffer);

                XDeleteProperty(dsp, win, pty);
                XFlush (dsp);
                return 0;
            }
        }
    }

    return 0;
}

static void print_buf (FILE* fout, Atom selType, unsigned char* selBuf, size_t selLen)
{
    if (selType == XA_INTEGER) {
        long* longBuf = (long*) selBuf;
        size_t longLen = selLen / sizeof (long);
        while (longLen--) {
            fprintf (fout, "%ld\n", *longBuf++);
        }
        return;
    }

    if (selType == XA_ATOM) {
        Atom* atomBuf = (Atom*) selBuf;
        size_t atomLen = selLen / sizeof(Atom);
        while (atomLen--) {
            char* atomName = XGetAtomName (dsp, *atomBuf++);
            fprintf(fout, "%s\n", atomName);
            XFree(atomName);
        }
        return;
    }

    fwrite (selBuf, sizeof(char), selLen, fout);
}
