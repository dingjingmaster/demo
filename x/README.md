# XLib

X11书籍：https://tronche.com/gui/x/

[Xlib 介绍](#user-content-xlib-介绍)
[Xlib 显示相关函数](#user-content-显示相关函数)


## Xlib 介绍

- X窗口系统概述
- 错误
- 标准头文件
- 常见类型和值
- Xlib中的命名和参数约定
- 编程注意事项
- 字符集和编码
- 格式规范

### X窗口系统概述

1. 分清楚哪些属于xserver、哪些属于xclient；了解根窗口、父窗口、子窗口。
2. 窗口座标系统、窗口边框；X不保留窗口的内容，当窗口某部分由隐藏变为显示则发送expose事件到xclient。
3. X提供图形对象的屏幕外存储，称为 pixmap；窗口和pixmap一起被称为可绘制图。
4. xlib中大多数函数是将请求添加到输出缓存区，这些请求稍后在xserver 上异步执行。返回存储在服务器上信息值的函数在接收到显示回复或发生错误前不会返回（这些是阻塞调用）
5. xclient 如果不希望请求异步执行，它可以在请求之后调用 XSync 阻塞等待所有异步事件被发送或执行完毕。
6. 许多Xlib函数将返回一个整数资源ID，它允许您引用存储在X服务器上的对象。这些类型可以是Window、Font、Pixmap、Colormap、Cursor和GContext（在文件<X11/X.h>中定义）。
7. 输入事件(例如，按下键或指针移动)从服务器异步到达，并排队，直到显式调用(例如，XNextEvent或XWindowEvent)请求它们。此外，一些库函数(例如XRaiseWindow)会生成Expose和ConfigureRequest事件。这些事件也是异步到达的，但是客户机可能希望在调用一个函数之后通过调用XSync显式地等待它们，该函数会导致服务器生成事件。

### 错误

一些函数返回Status，一个整数错误指示。如果函数失败，则返回0。如果函数返回状态为0，则表示它没有更新返回参数。由于C语言不提供多个返回值，许多函数必须通过写入客户端传递的存储来返回结果。默认情况下，错误由标准库函数或您提供的函数处理。如果字符串不存在，返回指向字符串指针的函数返回NULL指针。

X服务器在检测到协议错误时报告它们。如果一个给定的请求可以生成多个错误，服务器可以报告其中的任何一个。

### 标准头文件

1. `<X11/Xlib.h>`：这是Xlib的主要头文件。大多数Xlib符号都是通过包含这个文件来声明的。该文件还包含预处理器符号`XlibSpecificationRelease`。此符号在本标准中定义为6。
2. `<X11/X.h>`：该文件声明了应用程序将使用的X协议的类型和常量。它从`<X11/Xlib.h>`自动包含，因此应用程序代码不需要直接引用该文件。
3. `<X11/Xcms.h>`：这个文件包含了许多颜色管理工具的符号。所有以“Xcms”为前缀的函数、类型和符号，以及颜色转换上下文宏，都在这个文件中声明。在包含该文件之前必须包含`<X11/Xlib.h>`。
4. `<X11/Xutil.h>`：该文件声明了用于客户端间通信和应用程序实用程序的各种函数、类型和符号，在包含该文件之前必须包含`<X11/Xlib.h>`。
5. `<X11/Xresource.h>`：该文件声明了资源管理器设施的所有函数、类型和符号。必须在包含此文件之前包含`<X11/Xlib.h>`。
6. `<X11/Xatom.h>`：该文件声明了所有预定义的原子，这些原子是前缀为“XA_”的符号。
7. `<X11/cursorfont.h>`：该文件声明了标准游标字体的游标符号，列在附录b中。所有的游标符号都有前缀“XC_”。
8. `<X11/keysymdef.h>`：该文件声明了所有标准KeySym值，这些值是前缀为“XK_”的符号。键盘系统按组排列，预处理器符号控制每组的包含。必须在包含文件之前定义预处理器符号以获得相关值。预处理器符号是xk_miscellaneous, XK_XKB_KEYS, XK_3270, XK_LATIN1, XK_LATIN2, XK_LATIN3, XK_LATIN4, XK_KATAKANA, XK_ARABIC, XK_CYRILLIC, XK_GREEK, XK_TECHNICAL, XK_SPECIAL, XK_PUBLISHING, XK_APL, XK_HEBREW, XK_THAI和XK_KOREAN。
9. `<X11/keysym.h>`：这个文件定义了预处理器符号xk_miscellaneous, XK_XKB_KEYS, XK_LATIN1, XK_LATIN2, XK_LATIN3, XK_LATIN4和XK_GREEK，然后包含了`<X11/keysymdef.h>`。
10. `<X11/Xlibint.h>`：该文件声明了用于扩展的所有函数、类型和符号，它们是该文件自动包含`<X11/Xlib.h>`。
11. `<X11/Xproto.h>`：该文件声明了基本X协议的类型和符号，以便在实现扩展时使用。它从`<X11/xlibin.h>`自动包含，因此应用程序和扩展代码不需要直接引用该文件。
12. `<X11/Xprotostr.h>`：该文件声明了基本X协议的类型和符号，以便在实现扩展时使用。它从`<X11/xproto1.h>`自动包含，因此应用程序和扩展代码不需要直接引用该文件。
13. `<X11/X10.h>`：该文件声明了用于X10兼容性函数的所有函数、类型和符号，这些在附录D中有描述。

### 常见类型和值

以下符号由Xlib定义，并在整个手册中使用:
- Xlib定义Bool类型和布尔值True和False。
- None是通用的空资源ID或原子。
- 类型XID用于通用资源id。
- 类型XPointer定义为char*，并用作指向数据的泛型不透明指针

### Xlib中的命名和参数约定

Xlib在函数的命名和语法方面遵循许多约定。假设您记住了函数需要的信息，这些约定旨在使函数的语法更加可预测。
- 为了将X符号与其他符号区分开来，库对外部符号使用混合大小写。按照现有约定，它为变量保留小写字母，为用户宏保留全部大写字母。
- 所有Xlib函数都以大写X开头。
- 所有函数名和符号的开头都大写。
- 所有用户可见的数据结构都以大写X开头。更一般地说，用户可以解引用的任何内容都以大写X开头。
- 宏和其他符号不以大写x开头。为了区别于所有用户符号，宏中的每个单词都大写。
- 数据结构中的所有元素或变量都是小写的。必要时，复合词用下划线(_)构成。
- 当使用display参数时，它总是在参数列表的第一个。
- 所有使用的资源对象都出现在参数列表的开头，紧跟在display参数之后。
- 当图形上下文与另一种类型的资源(最常见的是可绘制的资源)一起出现时，图形上下文出现在参数列表中，位于其他资源之后。可抽取资源的排名高于所有其他资源。
- 在参数列表中，源参数总是位于目标参数之前。
- 在参数列表中，x参数总是位于y参数之前。
- 在参数列表中，宽度参数总是位于高度参数之前。
- 当x、y、width和height参数一起使用时，x和y参数总是在width和height参数之前。
- 当掩码与结构体同时出现时，掩码总是位于参数列表中指向该结构体的指针之前。

### 编程注意事项

- X中的坐标和大小实际上是16位的量。这个决定是对的限制给定性能级别所需的带宽。坐标通常是在接口中声明为int型。大于16位的值将被静默截断。大小(宽度和高度)被声明为无符号整型。
- 键盘是不同制造商工作站之间最大的变量。如果你希望您的程序是可移植的，您应该在这里特别保守。
- 许多显示系统的屏幕外内存有限。如果可以，你应该去做尽量减少像素图和后备存储的使用。
- 用户应该能够控制自己的屏幕空间。因此，你应该写你的应用程序对窗口管理作出反应，而不是假定控制整个窗口屏幕上。但是，在顶层窗口内执行的操作取决于应用程序。

### 字符集和编码

### 格式规范


## 显示相关函数

在您的程序可以使用显示器之前，您必须建立到X服务器的连接。一旦建立了连接，就可以使用本章讨论的Xlib宏和函数来返回有关显示的信息。本章讨论如何:

- 连接到xserver
- 获取有关Display、图像格式或屏幕的信息
- 生成一个NoOperation协议请求
- 释放xclient创建的数据
- 关闭到xserver的连接
- 使用X服务器关闭连接操作
- 使用带有线程的Xlib
- 使用内部连接

### 连接到xserver（XOpenDisplay）

```c
Display* XOpenDisplay(display_name);


char* display_name; // 指定硬件显示名称；如果为空则从环境变量中获取
```

显示名的编码和解释取决于实现。支持主机可移植字符编码中的字符串;对其他字符的支持取决于实现。在符合posix的系统上，显示名称或display环境变量可以是如下格式的字符串:
```
protocol/hostname:number.screen_number
```
- protocol：
- hostname：
- number：
- screen_number：

`XOpenDisplay`函数返回一个`Display`结构，该结构用作到X服务器的连接，并且包含关于该X服务器的所有信息。`XOpenDisplay`通过`TCP`或`DECnet`通信协议，或通过`某些本地进程间通信协议`，将应用程序连接到X服务器。如果协议指定为“tcp”、“inet”或“inet6”，或者没有指定协议，主机名是主机名，主机名和显示号之间用一个冒号分隔，`XOpenDisplay`使用tcp流连接。(如果指定的协议为“inet”，则使用TCP over IPv4。如果指定协议为inet6，则使用TCP over IPv6。否则，实现将决定使用哪个IP版本。)如果没有指定主机名和协议，Xlib将使用它认为最快的传输方式。如果主机名是主机名，用双冒号(::)分隔主机名和显示号，`XOpenDisplay`使用`DECnet`进行连接。单个X服务器可以同时支持任何或所有这些传输机制。特定的Xlib实现可以支持更多这样的传输机制。

如果成功，`XOpenDisplay`返回一个指向`Display`结构的指针，该结构定义在。如果`XOpenDisplay`不成功，它返回`NULL`。在成功调用XOpenDisplay之后，客户机可以使用显示中的所有屏幕。display_name参数中指定的屏幕编号由DefaultScreen宏(或XDefaultScreen函数)返回。只能通过使用信息宏或函数来访问Display和Screen结构的元素。有关使用宏和函数从Display结构中获取信息的信息。

### 获取有关Display、图像格式或屏幕的信息

Xlib库提供了许多有用的宏和相应的函数，它们从Display结构返回数据。这些宏用于C编程，它们对应的函数等价于其他语言绑定。本节讨论:
- 显示宏
- 图像格式函数和宏
- 屏幕信息宏

Display结构的所有其他成员(即没有为其定义宏的成员)都是Xlib私有的，不能使用。应用程序绝对不能直接修改或检查Display结构的这些私有成员。

#### 显示宏（Display Macros）

应用程序不应该直接修改`Display`和`Screen`结构的任何部分。这些成员应该被认为是只读的，尽管它们可能会由于对显示的其他操作而改变。下面列出了C语言的宏，它们对应的用于其他语言绑定的等价函数，以及它们都可以返回的数据。

##### AllPlanes
```c
unsigned long XAllPlanes()
```

AllPlanes宏返回一个所有位设置为1的值，适合用于过程的plane参数。

##### BlackPixel

```c
unsigned long XBlackPixel (display, screen_number);
```
返回指定屏幕的黑色像素值。

##### WhitePixel

```c
unsigned long XWhitePixel (display, screen_number)
```

返回指定屏幕的白色像素值。

##### ConnectionNumber
```c
int XConnectionNumber(display)
```
返回指定显示的连接号。

##### DefaultColormap
```c
Colormap XDefaultColormap (display, screen_number)
```
两者都返回在指定屏幕上分配的默认colormap ID。大多数例行的颜色分配都应该从这个颜色图中进行。

##### DefaultDepth

```c
int XDefaultDepth (display, screen_number)
```
两者都返回指定屏幕的默认根窗口的深度。其他深度也可以在这个屏幕上支持(见XMatchVisualInfo)。

##### XListDepths
```c
int *XListDepths(display, screen_number, count_return);

Display *display;
int screen_number;
int *count_return;
```
xlistdepth函数返回指定屏幕上可用的深度数组。如果指定的screen_number是有效的，并且可以为数组分配足够的内存，则xlistdepth将count_return设置为可用深度的数量。否则，它不设置count_return并返回NULL。释放为深度数组分配的内存，使用XFree。

##### DefaultGC

```c
GC XDefaultGC (display, screen_number);

Display *display;
int screen_number;
```

两者都返回指定屏幕的根窗口的默认图形上下文。这个GC是为了方便简单的应用程序而创建的，它包含默认的GC组件，其中前景和背景像素值分别初始化为屏幕的黑色和白色像素。您可以自由地修改它的内容，因为它不在任何Xlib函数中使用。这个GC永远不应该被释放。

##### DefaultRootWindow

```c
Window XDefaultRootWindow(display);
```

两者都返回默认屏幕的根窗口。

##### DefaultScreenOfDisplay

```c
Screen *XDefaultScreenOfDisplay (display);
```

两者都返回一个指向默认屏幕的指针。

##### ScreenOfDisplay

```c
Screen *XScreenOfDisplay(display, screen_number);

Display *display;
int screen_number;
```

##### DefaultScreen
```c
int XDefaultScreen (display);

Display *display;
```

两者都返回由XOpenDisplay函数引用的默认屏幕数。这个宏或函数应该用于在只使用单个屏幕的应用程序中检索屏幕号。

##### DefaultVisual

```c
Visual *XDefaultVisual (display, screen_number);

Display *display;
int screen_number;
```

两者都返回指定屏幕的默认可视类型。有关视觉的更多信息类型

##### DisplayCells

```c
int XDisplayCells(display, screen_number);

Display *display;
int screen_number;
```

两者都返回默认颜色映射中的条目数。
