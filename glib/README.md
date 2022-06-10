# glib例子

### glib线程安全

- 除了数据结构操作函数之外，所有函数都是不可见的线程安全的
- 如果有两个线程操作相同的数据结构，那么它们必须使用一个锁来同步操作
- GLib为自己的目的创建一个工作线程，因此GLib应用程序将始终拥有至少两个线程

### glib提升权限后安全

《linux和unix安全编程指南》

### 编译glib程序

- 仅用glib：`pkg-config --cflags glib-2.0`
- 使用glib+gobject `pkg-config --cflags --libs gobject-2.0`
- 使用了模块 `pkg-config --cflags --libs gmodule-2.o`
- 推荐使用GLib时总是包含如下头文件 `glib.h`、`glib-object.h`、`gio.h`

### glib 基础功能

- 版本函数和宏
- 基本类型：为了便于使用和可移植性定义的标准GLib类型
- 标准宏
- 类型转换宏
- 不同字节序之间转换的宏
- 边界检查：做无符号数计算的时候看是否越界的检查
- 数值定义：pi、ln2、log10之类的
- 不常用的专用宏：自动释放指针之类的
- 基本的原子操作整数和指针函数

### glib 核心功能

- Main Event Loop：管理所有可用资源和事件；可以使用很多事件源，还可以手动添加新的事件源。
  可以设置事件优先级，高优先级一定先于低优先级处理

- Threading：可移植的线程库
- Thread Pools：线程池
- Asynchronous Queues： 线程间的异步通信
- Dynamic Loading of Modules：模块的动态加载
- Memory Allocation：内存分配
- Memory Slices：内存块，分配大小的内存块
- IO Channels：支持使用文件、管道、套接字，并可添加到主事件循环中
- Error Reporting：错误报告系统。`GError`应该只用于报告可恢复的运行时错误，而不是报告编程错误。如果程序员搞砸了，那么应该使用`g_warning()`、`g_return_if_fail()`、`g_assert()`、`g_error()`或其他类似的工具
- Warnings And Assertions：警告和断言
- 信息输出和调试

### glib 使用功能

- 字符串
- 字符编码转换
- 操作unicode和utf8的函数
- base64 编解码
- Data Checksums 数据校验
- Secure HMAC Digests 安全摘要
- Internationalization 国际化
- Date and Time Function 日历和时间功能
- GTimeZone 时区
- GDateTime 日期和时间的结构
- Random Numbers 随机数和伪随机数生成
- Hook Function 钩子函数
- Miscellaneous Utility Function 杂项可移植函数
- Lexical Scanner 词汇扫描器
- Timers 定时器
- Spawning Processes 生成进程，进程启动
- File Utilities 文件工具，各种文件相关功能
- URI Functions 操作URI的函数
- Hostname Utilities 
- 类似于shell的命令行处理
- Commandline Option parser 命令行配置解析
- Glob-style pattern matching 模式匹配
- Perl-compatible expressions 与perl兼容的正则匹配
- Regular expression sync GRegex支持的正则表达式的语法和语义
- Simple XML Subset Parser XML的解析器
- Key-value file parser K-V 类型的配置文件解析
- Bookmark file parser 数千文件解析
- Testing 测试框架
- Unix-specific utilities and intergration UNIX 管道和信号的处理
- WIndows Compatibility Function 在window上模拟linux
- Guuid 通用的唯一标识符 RFC4122

### glib数据类型

- Doubly-Linked Lists 双向链表
- Singly-Linked List 单向链表
- Double-ended Queues 双端队列
- Sequences 可伸缩列表
- Trash Stacks 垃圾堆栈——维护一个未使用的已分配内存块的堆栈
- Hash Tbale 键值关联
- Strings 可增长的字符串缓存区
- String Chunks 字符串组的有效存储
- Array 随元素增加而增长的数组
- Pointer Arrays 指针数组，自增
- Byte Arrays
- Balanced Binary Trees 自平衡二叉树搜索树
- N-ary Trees 具有任意数目分支的数据树
- Quarks 夸克，字符串和唯一整数标识符之间的双向关联
- Keyed Data Lists 键控数据列表——可以通过字符串或QGuark标识符访问的数据元素列表
- Datasets 将数据元素组成与特定内存位置相关联
- GVariantType GVariant 类型系统，基于D-Bus类型的系统
- GVariant 
- GVariant Format Strings
- GVariant Text Format
- Reference counting
- Reference counted data
- Atomically reference counted data 原子引用计数的数据分配内存与原子引用计数语义
- Reference counted strings 引用计数字符串——字符串与引用计数内存管理


### glib-dbus 接口描述的type域数据类型

|     |     |
| --- | --- |
| a   | array 数组 |
| b   | bollean 布尔值 |
| d   | double 双精度浮点 |
| g   | SIGNATURE 类型签名 |
| i   | INT32 |
| n   | INT16 |
| o   | `object_path` 对象路径 |
| q   | UINT16 16位无符号整数 |
| s   | string 0 结尾的UTF-8字符串 |
| t   | UINT64 |
| u   | UINT32 |
| v   | VARIANT 可放任意类型的容器，数据中包含类型信息。例如glib中的GValue |
| x   | INT64 64位由符号整数 |
| y   | BYTE 8位无符号整数 |
| ()  | 定义结构时使用。例如 "(i(ii))" |
| {}  | 定义键值对时使用。例如 "a{us}" |
