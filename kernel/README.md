# 内核模块

## 依赖
- 头文件

## 知识点

- 安全引导：参看 `man kernel lockdown`

## hello-1

1. 模块中必须至少有两个函数：一个在模块被插入时候调用（叫：`init_module`）；另一个在模块被移出时候调用（叫：`cleanup_module`）。早期内核必须使用这两个函数，目前可以使用任意函数名，然后通过`module_init`和`module_exit`宏设置模块的初始化调用函数和移除调用函数。

