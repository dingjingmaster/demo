/*************************************************************************
> FileName: x-drop.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 02 Nov 2023 02:23:01 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <X11/Xlib.h>

#define XDND_PROTOCOL_VERSION       5

typedef struct
{
    bool                xdndExchangeStarted;
    bool                xdndPositionReceived;
    bool                xdndStatusReceived;
    bool                xdndStatusSend;
    bool                xdndDropReceived;
    Time                xdndDropTimestamp;
    Time                xdndLastPositionTimestamp;
    bool                amISource;
    int                 pRootX;
    int                 pRootY;
    Window              otherWindow;
    Atom                proposedAction;
    Atom                proposedType;
} XDNDStateMachine;

static bool doWeAcceptAtom(Atom a);
static const char* getEventType(XEvent* event);
static char* getCopiedData(Display* disp, Window source);
static Atom getSupportedType(Display *disp, Window source);
static void sendXdndDrop(Display *disp, Window source, Window target);
static void sendXdndLeave(Display *disp, Window source, Window target);
static void sendXdndFinished(Display *disp, Window source, Window target);
static void printClientMessage(Display *disp, XClientMessageEvent *message);
static void sendXdndStatus(Display *disp, Window source, Window target, Atom action);
static void sendSelectionNotify(Display *disp, XSelectionRequestEvent *selectionRequest, const char *pathStr);

static Atom XdndAware, XA_ATOM, 
            XdndEnter, XdndPosition, XdndActionCopy, 
            XdndLeave, XdndStatus, XdndDrop, XdndSelection,
            XDND_DATA, XdndTypeList, XdndFinished,
            WM_PROTOCOLS, WM_DELETE_WINDOW, typesWeAccept[6];

static XDNDStateMachine xdndState;

int main (int argc, char* argv[])
{
    bool continueEventLoop = true;
    bool clickedStillInWindow = false;

    Display* disp = NULL;
    int screen, screenWidth, screenHeight, x, y;
    Window win;
    XEvent ev;
    GC gCtx;
    unsigned int xdndVersion = XDND_PROTOCOL_VERSION;
    Atom* propList;
    int numOfProperties;

    disp = XOpenDisplay(NULL);
    if (NULL == disp) {
        printf ("XOpenDisplay error!\n");
        goto out;
    }

	// Define atoms
	XdndAware = XInternAtom(disp, "XdndAware", False);
	XA_ATOM = XInternAtom(disp, "XA_ATOM", False);
	XdndEnter = XInternAtom(disp, "XdndEnter", False);
	XdndPosition = XInternAtom(disp, "XdndPosition", False);
	XdndActionCopy = XInternAtom(disp, "XdndActionCopy", False);
	XdndLeave = XInternAtom(disp, "XdndLeave", False);
	XdndStatus = XInternAtom(disp, "XdndStatus", False);
	XdndDrop = XInternAtom(disp, "XdndDrop", False);
	XdndSelection = XInternAtom(disp, "XdndSelection", False);
	XDND_DATA = XInternAtom(disp, "XDND_DATA", False);
	XdndTypeList = XInternAtom(disp, "XdndTypeList", False);
	XdndFinished = XInternAtom(disp, "XdndFinished", False);
	WM_PROTOCOLS = XInternAtom(disp, "WM_PROTOCOLS", False);
	WM_DELETE_WINDOW = XInternAtom(disp, "WM_DELETE_WINDOW", False);

	// Define type atoms we will accept for file drop
	typesWeAccept[0] = XInternAtom(disp, "text/uri-list", False);
	typesWeAccept[1] = XInternAtom(disp, "UTF8_STRING", False);
	typesWeAccept[2] = XInternAtom(disp, "TEXT", False);
	typesWeAccept[3] = XInternAtom(disp, "STRING", False);
	typesWeAccept[4] = XInternAtom(disp, "text/plain;charset=utf-8", False);
	typesWeAccept[5] = XInternAtom(disp, "text/plain", False);

	// Get screen dimensions
	screen = DefaultScreen(disp);
	screenWidth = DisplayWidth(disp, screen);
	screenHeight = DisplayHeight(disp, screen);
	printf("screen width: %d, screen height: %d\n", screenWidth, screenHeight);

    // 创建窗口
    x = 10 * 200;
    y = 0;
    win = XCreateSimpleWindow(disp, RootWindow(disp, screen), x, y, 200, 200, 1, (0xFF<<8), WhitePixel(disp, screen));
    if (0 == win) {
        printf ("XCreateSimpleWindow error\n");
        goto out;
    }

    // 设置窗口标题
    if (XStoreName(disp, win, "drag-drop") == 0) {
        printf ("XStoreName error\n");
        goto out;
    }

    // 事件
    if (0 == XSelectInput (disp, win, PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ExposureMask | EnterWindowMask | LeaveWindowMask)) {
        printf ("XSelectInput error\n");
        goto out;
    }

    // 添加 XdndAware 属性以支持拖拽
    XChangeProperty (disp, win, XdndAware, XA_ATOM, 32, PropModeReplace, (void*) &xdndVersion, 1);

    // FIXME://修改
    XSetWMProtocols (disp, win, &WM_DELETE_WINDOW, 1);

    // 显示 window
    if (XMapWindow (disp, win) == 0) {
        printf ("XMapWindow\n");
        goto out;
    }

    gCtx = XCreateGC(disp, win, 0, NULL);
    if (0 == gCtx) {
        printf ("XCreateGC\n");
        goto out;
    }

    while (continueEventLoop) {
        XNextEvent(disp, &ev);
        switch (ev.type) {
            case SelectionRequest: {
                if (xdndState.xdndExchangeStarted && xdndState.amISource) {
                    // add data to target window
                    sendSelectionNotify(disp, &ev.xselectionrequest, "test");
                }
                break;
            }
            case SelectionNotify: {
                if (ev.xselection.property != XDND_DATA) {
                    break;
                }
                Window rootRet, childRet;
                int rootXRet, rootYRet, winXRet, winYRet;
                unsigned int maskRet;

                char* pathStr = getCopiedData(disp, win);

                // 删除属性
                XDeleteProperty(disp, win, XDND_DATA);

                // 修改 正方形 属性
                printf ("path: %s\n", pathStr);
                free (pathStr);

                // 设置新属性
                XQueryPointer (disp, win, &rootRet, &childRet, &rootXRet, &rootYRet, &winXRet, &winYRet, &maskRet);

                // Send XdndFinished message
			    printf("sending XdndFinished\n");
			    sendXdndFinished(disp, win, xdndState.otherWindow);
			    memset(&xdndState, 0, sizeof(xdndState));
			    break;
            }
            case MotionNotify: {
                break;
            }
            case KeyRelease: {
                break;
            }
            case ButtonPress: {
			    break;
            }
            case ButtonRelease: {
			    if (xdndState.xdndExchangeStarted && xdndState.amISource && xdndState.xdndStatusReceived) {
				    // Send XdndDrop message
				    printf("sending XdndDrop to target window\n");
				    sendXdndDrop(disp, win, xdndState.otherWindow);
			    }
			    break;
            }
            case Expose: {
			    break;
            }
            case EnterNotify: {
			    break;
            }
            case LeaveNotify: {
			    break;
            }
            case ClientMessage: {
			    if (ev.xclient.message_type != XdndEnter &&
				    ev.xclient.message_type != XdndPosition &&
				    ev.xclient.message_type != XdndLeave &&
				    ev.xclient.message_type != XdndStatus &&
				    ev.xclient.message_type != XdndDrop &&
				    ev.xclient.message_type != XdndFinished &&
				    ev.xclient.message_type != WM_PROTOCOLS) {
				    printf("received %s message\n", getEventType(&ev));
				    printClientMessage(disp, &ev.xclient);
			    }
			    // Check if we are being closed
			    if (ev.xclient.message_type == WM_PROTOCOLS) {
				    if (ev.xclient.data.l[0] == WM_DELETE_WINDOW) {
					    // End event loop
					    continueEventLoop = false;
					    break;
				    }
			    }

			    // Check if already in XDND protocol exchange
			    if (!xdndState.xdndExchangeStarted) {
				    // Only handle XdndEnter messages here
				    if (ev.xclient.message_type == XdndEnter) {
					    printf("receiving XdndEnter\n");

					    // Update state
					    xdndState.xdndExchangeStarted = true;
					    xdndState.amISource = false;
					    xdndState.otherWindow = ev.xclient.data.l[0];

					    // Determine type to ask for
					    if (ev.xclient.data.l[1] & 0x1) {
						    // More than three types, look in XdndTypeList
						    xdndState.proposedType = getSupportedType(disp, xdndState.otherWindow);
					    } else {
						    // Only three types, check three in turn and stop when we find
						    // one we support
						    xdndState.proposedType = None;
						    for (int i = 2; i < 5; ++i) {
							    if (doWeAcceptAtom(ev.xclient.data.l[i])) {
								    xdndState.proposedType = ev.xclient.data.l[i];
								    break;
							    }
						    }
					    }
				    }
				    break;
			    } else {
				    // Check whether we are source or target
				    if (xdndState.amISource) {
					    // Check for XdndStatus message
					    if (ev.xclient.message_type == XdndStatus) {
						    xdndState.xdndStatusReceived = true;

						    // Check if target will accept drop
						    if ((ev.xclient.data.l[1] & 0x1) != 1) {
							    // Won't accept, break exchange and wipe state
							    printf("sending XdndLeave message to target window as it won't accept drop\n");
							    sendXdndLeave(disp, win, xdndState.otherWindow);
							    memset(&xdndState, 0, sizeof(xdndState));
							    break;
						    }
					    }
					    else if (ev.xclient.message_type == XdndFinished) {
						    printf("receiving XdndFinished message\n");
						    memset(&xdndState, 0, sizeof(xdndState));
					    }
				    } else {
					    // Check for XdndPosition message
					    if (ev.xclient.message_type == XdndPosition) {
						    printf("receiving XdndPosition\n");

						    // Ignore if not for our window and sent erroneously
						    if (xdndState.xdndPositionReceived && ev.xclient.data.l[0] != xdndState.otherWindow) {
							    printf("receiving XdndPosition from erroneous window, ignoring\n");
							    break;
						    }

						    // Update state
						    xdndState.xdndPositionReceived = true;
						    xdndState.pRootX = ev.xclient.data.l[2] >> 16;
						    xdndState.pRootY = ev.xclient.data.l[2] & 0xFFFF;
						    xdndState.proposedAction = ev.xclient.data.l[4];
						    xdndState.xdndLastPositionTimestamp = ev.xclient.data.l[3];

						    // Now check if we should send XdndStatus message
						    if (!xdndState.xdndStatusSend) {
							    printf("sending XdndStatus\n");
							    xdndState.xdndStatusSend = true;
							    sendXdndStatus(disp, win, xdndState.otherWindow, xdndState.proposedAction);
						    }
					    }

					    // Check for XdndLeave message
					    if (ev.xclient.message_type == XdndLeave) {
						    printf("receiving XdndLeave, clearing state\n");
						    memset(&xdndState, 0, sizeof(xdndState));
					    }

					    // Check for XdndDrop message
					    if (ev.xclient.message_type == XdndDrop) {
						    printf("receiving XdndDrop, processing selection\n");
						    // Ignore if not for our window and/or sent erroneously
						    if (!xdndState.xdndPositionReceived || ev.xclient.data.l[0] != xdndState.otherWindow) {
							    printf("receiving XdndDrop from erroneous window, ignoring\n");
							    break;
						    }

						    // Update state
						    xdndState.xdndDropReceived = true;
						    xdndState.xdndDropTimestamp = ev.xclient.data.l[2];

						    // Call XConvertSelection
						    XConvertSelection(disp, XdndSelection, xdndState.proposedType, XDND_DATA, win, xdndState.xdndDropTimestamp);
					    }
				    }
			    }
			    break;
            }
        }
    }

out:
    if (gCtx)   XFreeGC(disp, gCtx);
    if (win)    XDestroyWindow(disp, win);
    if (disp)   XCloseDisplay(disp);


    return 0;
}

// Read copied path string from our window property
static char* getCopiedData(Display* disp, Window source)
{
	// Declare return value
	char *retVal = NULL;

	// Try to get PRIMARY property
	Atom actualType = None;
	int actualFormat;
	unsigned long numOfItems, bytesAfterReturn;
	unsigned char *data = NULL;
	if (XGetWindowProperty(disp, source, XDND_DATA, 0, 1024, False, AnyPropertyType,
		&actualType, &actualFormat, &numOfItems, &bytesAfterReturn, &data) == Success) {
		// Allocate temporary buffer
		char *tempBuffer = malloc(numOfItems + 1);
		if (!tempBuffer) {
			printf ("malloc\n");
        }

		// Copy all data from X buffer then add null-byte to create proper string, then
		// dispose of X buffer
		memcpy(tempBuffer, data, numOfItems);
		tempBuffer[numOfItems] = '\0';
		XFree(data);

		// Copy from beyond 'file://' prefix if present
		char *tempPtr;
		if ((tempPtr = strstr(tempBuffer, "file://")) != NULL) {
			tempPtr = tempBuffer + 7;
		} else {
			tempPtr = tempBuffer;
		}

		// Check if cr/nl ending is present and terminate string
		// before this if so
		if (tempPtr[strlen(tempPtr)-2] == 0xD && tempPtr[strlen(tempPtr)-1] == 0xA)
			tempPtr[strlen(tempPtr)-2] = '\0';

		// Allocate return buffer
		retVal = malloc(strlen(tempPtr) + 1);
		if (!retVal) {
			printf ("malloc\n");
        }

		// Copy data from temp buffer to it, then free temp buffer
		memcpy(retVal, tempPtr, strlen(tempPtr));
		retVal[strlen(tempPtr)] = '\0';
		free(tempBuffer);
	}

	// Return malloc allocated buffer - caller must free
	return retVal;
}

// This is sent by the target when the exchange has completed
static void sendXdndFinished(Display *disp, Window source, Window target)
{
	if (xdndState.xdndExchangeStarted && !xdndState.amISource) {
		// Declare message struct and populate its values
		XEvent message;
		memset(&message, 0, sizeof(message));
		message.xclient.type = ClientMessage;
		message.xclient.display = disp;
		message.xclient.window = target;
		message.xclient.message_type = XdndFinished;
		message.xclient.format = 32;
		message.xclient.data.l[0] = source;
		message.xclient.data.l[1] = 1;
		message.xclient.data.l[2] = XdndActionCopy;

		// Send it to target window
		if (XSendEvent(disp, target, False, 0, &message) == 0) {
			printf ("XSendEvent");
        }
	}
}

// This is sent by the source to the target to say it can call XConvertSelection
static void sendXdndDrop(Display *disp, Window source, Window target)
{
	if (xdndState.xdndExchangeStarted && xdndState.amISource) {
		// Declare message struct and populate its values
		XEvent message;
		memset(&message, 0, sizeof(message));
		message.xclient.type = ClientMessage;
		message.xclient.display = disp;
		message.xclient.window = target;
		message.xclient.message_type = XdndDrop;
		message.xclient.format = 32;
		message.xclient.data.l[0] = source;
		//message.xclient.data.l[1] reserved
		message.xclient.data.l[2] = xdndState.xdndLastPositionTimestamp;

		// Send it to target window
		if (XSendEvent(disp, target, False, 0, &message) == 0) {
			printf("XSendEvent\n");
        }
	}
}

// This function prints the contents of ClientMessage events
static void printClientMessage(Display *disp, XClientMessageEvent *message)
{
	// Get atom type as string
	char *messageTypeStr = XGetAtomName(disp, message->message_type);
	printf("Message type: %s\n", messageTypeStr);
	XFree(messageTypeStr);

	// Handle format
	printf("Message word size: %d bits\n", message->format);
	printf("Message ");
	switch (message->format) {
	case 8:
		printf("bytes: ");
		for (int i = 0; i < 20; ++i)
			printf("%d ", message->data.b[i]);
		break;
	case 16:
		printf("16-bit shorts: ");
		for (int i = 0; i < 10; ++i)
			printf("%d ", message->data.s[i]);
		break;
	case 32:
		printf("32-bit longs: ");
		for (int i = 0; i < 5; ++i)
			printf("%d ", (int32_t)message->data.l[i]);
		break;
	}
	printf("\n");
}

// This is sent by the target to the source to say whether or not it will accept the drop
static void sendXdndStatus(Display *disp, Window source, Window target, Atom action)
{
	if (xdndState.xdndExchangeStarted && !xdndState.amISource) {
		// Declare message struct and populate its values
		XEvent message;
		memset(&message, 0, sizeof(message));
		message.xclient.type = ClientMessage;
		message.xclient.display = disp;
		message.xclient.window = target;
		message.xclient.message_type = XdndStatus;
		message.xclient.format = 32;
		message.xclient.data.l[0] = source;
		message.xclient.data.l[1] = 1; // Sets accept and want position flags

		// Send back window rectangle coordinates and width
		message.xclient.data.l[2] = 0;
		message.xclient.data.l[3] = 0;

		// Specify action we accept
		message.xclient.data.l[4] = action;

		// Send it to target window
		if (XSendEvent(disp, target, False, 0, &message) == 0) {
			printf ("XSendEvent\n");
        }
	}
}

// This is sent by the source when the exchange is abandoned
static void sendXdndLeave(Display *disp, Window source, Window target)
{
	if (xdndState.xdndExchangeStarted && xdndState.amISource) {
		// Declare message struct and populate its values
		XEvent message;
		memset(&message, 0, sizeof(message));
		message.xclient.type = ClientMessage;
		message.xclient.display = disp;
		message.xclient.window = target;
		message.xclient.message_type = XdndLeave;
		message.xclient.format = 32;
		message.xclient.data.l[0] = source;
		// Rest of array members reserved so not set

		// Send it to target window
		if (XSendEvent(disp, target, False, 0, &message) == 0) {
			printf ("XSendEvent");
        }
	}
}

// Test whether the supplied atom is in our types list
static bool doWeAcceptAtom(Atom a)
{
	for (int i = 0; i < sizeof(typesWeAccept) / sizeof(Atom); ++i) {
		if (a == typesWeAccept[i]) {
			return true;
		}
	}

	return false;
}

// This gets the XdndTypeList from the source window when we need it, and then determines the
// type we will ask for
static Atom getSupportedType(Display *disp, Window source)
{
	// Try to get XdndTypeList property
	Atom retVal = None;
	Atom actualType = None;
	int actualFormat;
	unsigned long numOfItems, bytesAfterReturn;
	unsigned char *data = NULL;
	if (XGetWindowProperty(disp, source, XdndTypeList, 0, 1024, False, AnyPropertyType,
		&actualType, &actualFormat, &numOfItems, &bytesAfterReturn, &data) == Success) {
		if (actualType != None) {
			Atom *supportedAtoms = (Atom *)data;
			for (int i = 0; i < numOfItems; ++i) {
				if (doWeAcceptAtom(supportedAtoms[i])) {
					retVal = supportedAtoms[i];
					break;
				}
			}
			
			XFree(data);
		}
	}

	return retVal;
}


// This is sent by the source to the target to say the data is ready
static void sendSelectionNotify(Display *disp, XSelectionRequestEvent *selectionRequest, const char *pathStr)
{
	if (xdndState.xdndExchangeStarted && xdndState.amISource) {
		// Allocate buffer (two bytes at end for CR/NL and another for null byte)
		size_t sizeOfPropertyData = strlen("file://") + strlen(pathStr) + 3;
		char *propertyData = malloc(sizeOfPropertyData);
		if (!propertyData) {
			printf ("malloc\n");
        }

		// Copy data to buffer
		strcpy(propertyData, "file://");
		strcat(propertyData, pathStr);
		propertyData[sizeOfPropertyData-3] = 0xD;
		propertyData[sizeOfPropertyData-2] = 0xA;
		propertyData[sizeOfPropertyData-1] = '\0';

		// Set property on target window - do not copy end null byte
		XChangeProperty(disp, selectionRequest->requestor, selectionRequest->property,
			typesWeAccept[0], 8, PropModeReplace, (unsigned char *)propertyData, sizeOfPropertyData-1);

		// Free property buffer
		free(propertyData);

		// Declare message struct and populate its values
		XEvent message;
		memset(&message, 0, sizeof(message));
		message.xselection.type = SelectionNotify;
		message.xselection.display = disp;
		message.xselection.requestor = selectionRequest->requestor;
		message.xselection.selection = selectionRequest->selection;
		message.xselection.target = selectionRequest->target;
		message.xselection.property = selectionRequest->property;
		message.xselection.time = selectionRequest->time;

		// Send it to target window
		if (XSendEvent(disp, selectionRequest->requestor, False, 0, &message) == 0) {
			printf ("XSendEvent\n");
        }
	}
}

static const char* getEventType(XEvent* event)
{
    const char* retVal = NULL;

    switch (event->type) {
    case 2:
        retVal = "KeyPress";
        break;
    case 3:
        retVal = "KeyRelease";
        break;
    case 4:
        retVal = "ButtonPress";
        break;
    case 5:
        retVal = "ButtonRelease";
        break;
    case 6:
        retVal = "MotionNotify";
        break;
    case 7:
        retVal = "EnterNotify";
        break;
    case 8:
        retVal = "LeaveNotify";
        break;
    case 9:
        retVal = "FocusIn";
        break;
    case 10:
        retVal = "FocusOut";
        break;
    case 11:
        retVal = "KeymapNotify";
        break;
    case 12:
        retVal = "Expose";
        break;
    case 13:
        retVal = "GraphicsExpose";
        break;
    case 14:
        retVal = "NoExpose";
        break;
    case 15:
        retVal = "VisibilityNotify";
        break;
    case 16:
        retVal = "CreateNotify";
        break;
    case 17:
        retVal = "DestoryNotify";
        break;
    case 18:
        retVal = "UnmapNotify";
        break;
    case 19:
        retVal = "MapNotify";
        break;
    case 20:
        retVal = "MapRequest";
        break;
    case 21:
        retVal = "ReparentNotify";
        break;
    case 22:
        retVal = "ConfigureNotify";
        break;
    case 23:
        retVal = "ConfigureRequest";
        break;
    case 24:
        retVal = "GravityNotify";
        break;
    case 25:
        retVal = "ResizeRequest";
        break;
    case 26:
        retVal = "CirculateNotify";
        break;
    case 27:
        retVal = "CirculateRequest";
        break;
    case 28:
        retVal = "PropertyNotify";
        break;
    case 29:
        retVal = "SelectionClear";
        break;
    case 30:
        retVal = "SelectionRequest";
        break;
    case 31:
        retVal = "SelectionNotify";
        break;
    case 32:
        retVal = "ColormapNotify";
        break;
    case 33:
        retVal = "ClientMessage";
        break;
    case 34:
        retVal = "MappingNotify";
        break;
    case 35:
        retVal = "GenericEvent";
        break;
    default:
        retVal = "Unknown";
        break;
    }

    return retVal;
}

