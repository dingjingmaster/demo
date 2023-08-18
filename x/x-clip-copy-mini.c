/*************************************************************************
> FileName: x-clip-copy-mini.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 18 Aug 2023 01:45:46 PM CST
 ************************************************************************/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>

#define XCLIB_XCIN_NONE     0
#define XCLIB_XCIN_SELREQ   1
#define XCLIB_XCIN_INCR     2

struct requestor
{
    Window cwin;
    Atom pty;
    unsigned int context;
    unsigned long sel_pos;
    int finished;
    long chunk_size;
    struct requestor *next;
};


char* xcnamestr(Display* display, Window w);
static struct requestor* get_requestor(Window win);
static void del_requestor(struct requestor* requestor);
int xcfetchname(Display *display, Window w, char **namep);

Display* display = NULL;
Atom target = XA_STRING;

/**
 * @brief
 *  去掉命令行解析，直接等价于 xclip 命令行：
 *      xclip -selection secondary -loops 1 -i
 *
 * @note
 *  为了减少代码程序中没有对内存进行判断，代码不能用于实际生产
 */
int main (int argc, char* argv[])
{
    struct timeval      tv;
    fd_set              inFD;
    XEvent              event;
    unsigned long       selLen = 0;
    unsigned long       bufLen = 10240;
    char*               recType = NULL;
    Window              window;
    
    display = XOpenDisplay(NULL);
    window = XCreateSimpleWindow (display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);

    XSelectInput(display, window, PropertyChangeMask);      // 属性改变时候获取时间

    /* FIXME: 错误处理与打印 */

    /* 开始复制功能 */
    int x11Fd = ConnectionNumber(display);                  // 获取与X11的连接号（X客户端与X服务器连接的编号，用于与X服务端交互）

    unsigned char* buf = (unsigned char*) malloc (bufLen * sizeof(char));

    while (!feof(stdin)) {
        if (selLen == bufLen) {
            bufLen *= 2;
            buf = (unsigned char*) realloc (buf, bufLen * sizeof (char));
        }
        selLen += fread (buf + selLen, sizeof(char), bufLen - selLen, stdin);
    }

    XSetSelectionOwner (display, XA_SECONDARY, window, CurrentTime);

    Window owner = XGetSelectionOwner(display, XA_SECONDARY);
    if (owner != window) {
        fprintf (stderr, "获取所有权出错!\n");
        return -1;
    }

    if (-1 == chdir("/")) {
        fprintf (stderr, "切换目录出错!\n");                // 为了避免占用当前目录，所以切换到根目录（非主要逻辑）
        return -1;
    }

    fprintf(stdout, "读取数据结束!\n");

    goto start;

    while (1) {
        struct requestor* req;
        Window reqID;
        int finished;

        // 获取从X获取到但没有处理的事件数量
        if (!XPending(display)) {
            tv.tv_sec = 0;
            tv.tv_usec = 0;

            FD_ZERO(&inFD);
            FD_SET(x11Fd, &inFD);
            if (!select (x11Fd + 1, &inFD, 0, 0, &tv)) {
                XSetSelectionOwner (display, XA_SECONDARY, None, CurrentTime);
                memset (buf, 0, sizeof(buf));
                return 0;
            }
        }

start:
        XNextEvent(display, &event);
        switch (event.type) {
            case SelectionRequest: {
                fprintf(stdout, "===>SelectionRequest\n");
                reqID = event.xselectionrequest.requestor;
                req = get_requestor(reqID);
                break;
            }
            case PropertyNotify: {
                fprintf(stdout, "===>PropertyNotify\n");
                reqID = event.xproperty.window;
                req = get_requestor(reqID);
                break;
            }
            case SelectionClear: {
                fprintf(stdout, "===>SelectionClear\n");
                break;
            }
            default: {
                fprintf(stdout, "===>continue\n");
                continue;
            }
        }

        fprintf (stdout, "event was sent by: %s\n", xcnamestr(display, reqID));
        reqID = 0;

        if (xcin (display, &(req->cwin), event, &(req->pty), target, buf, bufLen, &(req->sel_pos), &(req->context), &(req->chunk_size))) {
            del_requestor(req);
            req = NULL;
            continue;
        }

        if (req && req->cwin == 0) {
            del_requestor(req);
            req = NULL;
            continue;
        }
    }

    /* 最后获取粘贴板并删除数据 */


    XCloseDisplay(display);

    return 0;
}

static struct requestor* requestors;
static struct requestor* get_requestor(Window win)
{
    struct requestor *requestor;

    if (requestors) {
        for (requestor = requestors; requestor != NULL; requestor = requestor->next) {
            if (requestor->cwin == win) {
                fprintf(stdout, "== Reusing requestor for %s\n", xcnamestr(display, win) );
                return requestor;
            }
        }
    }

    fprintf(stdout, "+ Creating new requestor for %s\n", xcnamestr(display, win));

    requestor = (struct requestor*) calloc (1, sizeof(struct requestor));
    if (!requestor) {
        fprintf(stderr, "error calloc\n");
    }
    else {
        requestor->context = XCLIB_XCIN_NONE;
    }

    if (!requestors) {
        requestors = requestor;
    }
    else {
        requestor->next = requestors;
        requestors = requestor;
    }

    return requestor;
}

static void del_requestor(struct requestor* requestor)
{
    struct requestor *reqitr;

    if (!requestor) {
        fprintf(stderr, "del requestor input: NULL\n");
        return;
    }

    fprintf(stdout, "- Deleting requestor for %s\n", xcnamestr(display, requestor->cwin));

    if (requestors == requestor) {
        requestors = requestors->next;
    }
    else {
        for (reqitr = requestors; reqitr != NULL; reqitr = reqitr->next) {
            if (reqitr->next == requestor) {
                reqitr->next = reqitr->next->next;
                break;
            }
        }
    }

    free(requestor);
}

char xcname[4096];
char* xcnamestr(Display* display, Window w) 
{
    char* window_name;
    xcfetchname(display, w, &window_name);
    if (window_name && window_name[0]) {
        snprintf(xcname, sizeof(xcname)-1, "'%s' (0x%lx)", window_name, w);
    }
    else {
        snprintf(xcname, sizeof(xcname)-1, "window id 0x%lx", w);
    }

    if (window_name) {
        XFree(window_name);
    }

    xcname[sizeof(xcname) - 1] = '\0'; /* Ensure NULL termination */

    return xcname;
}

int xcfetchname(Display *display, Window w, char **namep)
{
    *namep = NULL;
    
    if (w == None) {
        return 1;       /* No window, no name. */
    }

    XFetchName(display, w, namep);
    if (*namep) {
        return 0;       /* Hurrah! It worked on the first try. */
    }

    /* Otherwise, recursively try the parent windows */
    Window p = w;
    Window dummy, *dummyp;
    unsigned int n;
    while (!*namep && p != None) {
        if (!XQueryTree(display, p, &dummy, &p, &dummyp, &n)) {
            break;
        }

        if (p != None) {
            XFetchName(display, p, namep);
        }
    }

    return (*namep == NULL);
}

int xcin(Display* dpy, Window* win, XEvent evt, Atom* pty, Atom target, unsigned char* txt, unsigned long len, unsigned long *pos, unsigned int *context, long *chunk_size)
{
    unsigned long chunk_len;    // length of current chunk (for incr* transfers only)
    XEvent res;                 // response to event 
    static Atom inc;
    static Atom targets;

    if (!targets) {
        targets = XInternAtom(dpy, "TARGETS", False);
    }

    if (!inc) {
        inc = XInternAtom(dpy, "INCR", False);
    }

    if (!(*chunk_size)) {
        *chunk_size = XExtendedMaxRequestSize(dpy) / 4;     // 获取服务器支持最大请求大小(以4字节为单位)
        if (!(*chunk_size)) {
            *chunk_size = XMaxRequestSize(dpy) / 4;         // 如果不支持扩展长度编码，则上边调用返回0
        }

        fprintf(stdout, "INCR chunk size is %ld\n", (*chunk_size));
    }

    switch (*context) {
        case XCLIB_XCIN_NONE: {
            char *tempstr = XGetAtomName(dpy, evt.xselectionrequest.target);
            fprintf(stdout, "target: %s\n", tempstr);
            XFree(tempstr);

            if (evt.type != SelectionRequest) {
                return (0);
            }
            *win = evt.xselectionrequest.requestor;
            *pty = evt.xselectionrequest.property;
            *pos = 0;

            /* put the data into a property */
            if (evt.xselectionrequest.target == targets) {
                Atom types[2] = { targets, target };
                fprintf(stdout, "sending list of TARGETS\n");

                /* send data all at once (not using INCR) */
                XChangeProperty(dpy, *win, *pty, XA_ATOM, 32, PropModeReplace, (unsigned char *) types, (int) (sizeof(types) / sizeof(Atom)));
            }
            else if (len > *chunk_size) {
                /* send INCR response */
                fprintf (stdout, "Starting INCR response\n");
                XChangeProperty(dpy, *win, *pty, inc, 32, PropModeReplace, 0, 0);
                XSelectInput(dpy, *win, PropertyChangeMask);
                *context = XCLIB_XCIN_INCR;
            }
            else {
                /* send data all at once (not using INCR) */
                fprintf(stdout, "Sending data all at once (%d bytes)\n", (int) len);
                XChangeProperty(dpy, *win, *pty, target, 8, PropModeReplace, (unsigned char *) txt, (int) len);
            }

            /* set values for the response event */
            res.xselection.property = *pty;
            res.xselection.type = SelectionNotify;
            res.xselection.display = evt.xselectionrequest.display;
            res.xselection.requestor = *win;
            res.xselection.selection = evt.xselectionrequest.selection;
            res.xselection.target = evt.xselectionrequest.target;
            res.xselection.time = evt.xselectionrequest.time;

            /* send the response event */
            XSendEvent(dpy, evt.xselectionrequest.requestor, 0, 0, &res);
            XFlush(dpy);

            /* don't treat TARGETS request as contents request */
            if (evt.xselectionrequest.target == targets) {
                return (1);     /* Finished with request */
            }

            if (len > *chunk_size) {
                return (0);
            }
            else {
                return (1);
            }
            break;
        }
        case XCLIB_XCIN_INCR: {
            /* length of current chunk */
            /* ignore non-property events */
            if (evt.type != PropertyNotify) {
                return (0);
            }

            if (evt.xproperty.state != PropertyDelete) {
                if (evt.xproperty.state == 0) {
                    fprintf(stdout, "ignoring PropertyNewValue\n");
                }
                else {
                    fprintf(stdout, "ignoring state %d\n", evt.xproperty.state);
                }
                return (0);
            }
            /* set the chunk length to the maximum size */
            chunk_len = *chunk_size;

            if ((*pos + chunk_len) > len) {
                chunk_len = len - *pos;
            }
            if (*pos > len) {
                chunk_len = 0;
            }

            if (chunk_len) {
                /* put the chunk into the property */
                fprintf(stdout, "Sending chunk of %d bytes\n", (int) chunk_len);
                XChangeProperty(dpy, *win, *pty, target, 8, PropModeReplace, &txt[*pos], (int) chunk_len);
            }
            else {
                fprintf(stdout, "Signalling end of INCR\n");
                XChangeProperty(dpy, *win, *pty, target, 8, PropModeReplace, 0, 0);
            }
            XFlush(dpy);

            /* all data has been sent, break out of the loop */
            if (!chunk_len) {
                fprintf(stdout, "Finished INCR transfer.\n");
                *context = XCLIB_XCIN_NONE;
            }

            *pos += *chunk_size;

            if (chunk_len > 0) {
                return (0);
            }
            else {
                return (1);
            }
            break;
        }
    }
    return (0);
}




