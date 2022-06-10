#include "xatom-helper.h"

#include <limits.h>

#include <QX11Info>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

static XatomHelper *global_instance = nullptr;

XatomHelper *XatomHelper::getInstance()
{
    if (!global_instance)
        global_instance = new XatomHelper;
    return global_instance;
}

bool XatomHelper::isFrameLessWindow(int winId)
{
    auto hints = getInstance()->getWindowMotifHint(winId);
    if (hints.flags == MWM_HINTS_DECORATIONS && hints.functions == 1) {
        return true;
    }
    return false;
}

bool XatomHelper::isWindowDecorateBorderOnly(int winId)
{
    return isWindowMotifHintDecorateBorderOnly(getInstance()->getWindowMotifHint(winId));
}

bool XatomHelper::isWindowMotifHintDecorateBorderOnly(const MotifWmHints &hint)
{
    bool isDeco = false;
    if (hint.flags & MWM_HINTS_DECORATIONS && hint.flags != MWM_HINTS_DECORATIONS) {
        if (hint.decorations == MWM_DECOR_BORDER)
            isDeco = true;
    }
    return isDeco;
}

bool XatomHelper::isUKUICsdSupported()
{
    // fixme:
    return false;
}

bool XatomHelper::isUKUIDecorationWindow(int winId)
{
    if (mDecorationAtion == None)
        return false;

    Atom type;
    int format;
    ulong nitems;
    ulong bytes_after;
    uchar *data;

    bool isUKUIDecoration = false;

    XGetWindowProperty(QX11Info::display(), winId, mDecorationAtion, 0, LONG_MAX, false, mDecorationAtion, &type, &format, &nitems, &bytes_after, &data);

    if (type == mDecorationAtion) {
        if (nitems == 1) {
            isUKUIDecoration = data[0];
        }
    }

    return isUKUIDecoration;
}

UnityCorners XatomHelper::getWindowBorderRadius(int winId)
{
    UnityCorners corners;

    Atom type;
    int format;
    ulong nitems;
    ulong bytes_after;
    uchar *data;

    if (mUnityBorderRadiusAtom != None) {
        XGetWindowProperty(QX11Info::display(), winId, mUnityBorderRadiusAtom, 0, LONG_MAX, false, XA_CARDINAL, &type, &format, &nitems, &bytes_after, &data);

        if (type == XA_CARDINAL) {
            if (nitems == 4) {
                corners.topLeft = static_cast<ulong>(data[0]);
                corners.topRight = static_cast<ulong>(data[1*sizeof (ulong)]);
                corners.bottomLeft = static_cast<ulong>(data[2*sizeof (ulong)]);
                corners.bottomRight = static_cast<ulong>(data[3*sizeof (ulong)]);
            }
            XFree(data);
        }
    }

    return corners;
}

void XatomHelper::setWindowBorderRadius(int winId, const UnityCorners &data)
{
    if (mUnityBorderRadiusAtom == None)
        return;

    ulong corners[4] = {data.topLeft, data.topRight, data.bottomLeft, data.bottomRight};

    XChangeProperty(QX11Info::display(), winId, mUnityBorderRadiusAtom, XA_CARDINAL, 32, XCB_PROP_MODE_REPLACE, (const unsigned char *) &corners, sizeof (corners)/sizeof (corners[0]));
}

void XatomHelper::setWindowBorderRadius(int winId, int topLeft, int topRight, int bottomLeft, int bottomRight)
{
    if (mUnityBorderRadiusAtom == None)
        return;

    ulong corners[4] = {(ulong)topLeft, (ulong)topRight, (ulong)bottomLeft, (ulong)bottomRight};

    XChangeProperty(QX11Info::display(), winId, mUnityBorderRadiusAtom, XA_CARDINAL, 32, XCB_PROP_MODE_REPLACE, (const unsigned char *) &corners, sizeof (corners)/sizeof (corners[0]));
}

void XatomHelper::setUKUIDecoraiontHint(int winId, bool set)
{
    if (mDecorationAtion == None)
        return;

    XChangeProperty(QX11Info::display(), winId, mDecorationAtion, mDecorationAtion, 32, XCB_PROP_MODE_REPLACE, (const unsigned char *) &set, 1);
}

void XatomHelper::setWindowMotifHint(int winId, const MotifWmHints &hints)
{
    if (mUnityBorderRadiusAtom == None)
        return;

    XChangeProperty(QX11Info::display(), winId, mMotifWMHintsAtom, mMotifWMHintsAtom, 32, XCB_PROP_MODE_REPLACE, (const unsigned char *)&hints, sizeof (MotifWmHints)/ sizeof (ulong));
}

MotifWmHints XatomHelper::getWindowMotifHint(int winId)
{
    MotifWmHints hints;

    if (mUnityBorderRadiusAtom == None)
        return hints;

    uchar *data;
    Atom type;
    int format;
    ulong nitems;
    ulong bytes_after;

    XGetWindowProperty(QX11Info::display(), winId, mMotifWMHintsAtom, 0, sizeof (MotifWmHints)/sizeof (long), false, AnyPropertyType, &type, &format, &nitems, &bytes_after, &data);
    if (type == None) {
        return hints;
    } else {
        hints = *(MotifWmHints *)data;
        XFree(data);
    }
    return hints;
}

XatomHelper::XatomHelper(QObject *parent) : QObject(parent)
{
    if (!QX11Info::isPlatformX11())
        return;

    mMotifWMHintsAtom = XInternAtom(QX11Info::display(), "_MOTIF_WM_HINTS", true);
    mUnityBorderRadiusAtom = XInternAtom(QX11Info::display(), "_UNITY_GTK_BORDER_RADIUS", false);
    mDecorationAtion = XInternAtom(QX11Info::display(), "_KWIN_UKUI_DECORAION", false);
}

Atom XatomHelper::registerUKUICsdNetWmSupportAtom()
{
    // fixme:
    return None;
}

void XatomHelper::unregisterUKUICsdNetWmSupportAtom()
{
    // fixme:
}
