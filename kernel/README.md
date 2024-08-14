# 内核模块

## 依赖
- 头文件
- 指定gcc版本：`cat /proc/version`

## 知识点

- 安全引导：参看 `man kernel lockdown`
- 内核模块解析命令行参数

## hello

1. 模块中必须至少有两个函数：一个在模块被插入时候调用（叫：`init_module`）；另一个在模块被移出时候调用（叫：`cleanup_module`）。早期内核必须使用这两个函数，目前可以使用任意函数名，然后通过`module_init`和`module_exit`宏设置模块的初始化调用函数和移除调用函数。
2. `__init` 宏确保初始化函数被调用后，函数被丢弃，其内存被释放（对集成到内核的模块生效，对模块没作用）。
3. `__initdata` 工作方式和`__init`一样，但是适用于变量。
4. `__exit`宏在内核模块被编译到内核中也没用（毕竟不会执行卸载操作）。
5. 内核模块命令行参数：定义变量，使用`module_param()`、`module_param_array()`、`module_param_string()`
6. 一个模块由多源文件组成
7. 模块可使用的函数在`/proc/kallsyms`
8. 区分库函数和系统调用：库函数更高级，完全运行于操作系统用户空间；系统调用则代码在内核空间运行。man 2 里有系统调用，man 3 里则是库调用。
9. 分为两种设备：块设备、字符设备
10. 设备类型
11. `seq_file`：简化读写`/proc`文件的复杂度。有三个API`start()`、`next()`、`stop()`，当读取一个 `/proc` 文件时候，开始调用`start()`，如果返回非`NULL`，则继续调用`next()`，否则调用`stop()`，每次调用`next()`之后，就会调用`show()`。
12. ioctl
13. completions：代码同步（比如：一个进程完成后再执行另一个进程），completions有三个主要部分：初始化completion同步结构体部分、通过`wait_for_completion()`等待完成、通过`complete()`发送完成信号。
14. 锁：互斥锁、自旋锁
