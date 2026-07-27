# zig

Zig 是一个命令式、通用、静态类型、编译的系统编程语言。

Zig 的设计目标是提供高性能、安全、简洁和可移植的编程体验。

## 标识符

在 Zig 语言中，标识符是用来命名变量、函数、类型等的名称。

1. 字母和数字：标识符可以包含字母（A-Z 和 a-z）、数字（0-9）和下划线（_）。
2. 开头字符：标识符必须以字母或下划线开头，不能以数字开头。
3. 大小写敏感：Zig 是一种区分大小写的语言，这意味着 Variable 和 variable 是两个不同的标识符。
4. 关键字和保留字：一些特定的单词在 Zig 中是保留的，不能用作标识符。例如 fn（函数）、struct（结构体）、if（条件语句）等。
5. 命名约定：Zig 官方推荐以下约定：
    1. camelCase（小驼峰）：用于变量名和函数名。例如 myVariable、calculateSum。
    2. PascalCase（大驼峰）：用于类型名和命名空间。例如 Point、ArrayList。
    3. SCREAMING_SNAKE_CASE：用于编译时已知的常量（comptime 常量）。例如 MAX_SIZE。
6. 可选类型：Zig 语言中有一个特殊的类型 ?T，表示一个类型为 T 的可选值。这在处理可能为空的值时非常有用。
7. 编译时常量：在标识符前使用 comptime 关键字，可以表示该标识符是一个编译时常量。
8. 错误类型：使用 error 关键字可以定义错误类型
9. 类型后缀：在类型名称后使用 _t 后缀是 C 语言的习惯，在 Zig 中也可以这样做，但不是必需的。

## 保留关键词

|关键词|说明|
|:-----|:---|
|align|指定变量或类型对齐字节数|
|allowzero|允许指针指向空值|
|and|逻辑与操作|
|asm|内联汇编块|
|break|跳出最近的循环或作用域|
|callconv|调用约定|
|const|定义常量|
|continue|继续下一次循环迭代|
|defer|延迟执行语句，直到作用域退出|
|else|条件语句的否定分支|
|enum|枚举类型|
|errdefer|错误发生时的延迟执行语句|
|error|错误类型定义|
|export|导出符号，供 C 语言等调用|
|fn|函数定义|
|for|遍历循环|
|if|条件语句|
|inline|内联函数或内联循环|
|linksection|指定链接器的节|
|noalias|指针不能被其他指针别名|
|noinline|阻止函数内联|
|null|可选类型的空值|
|or|逻辑或操作|
|packed|取消结构体填充，按位紧密排列|
|pub|公开（public）访问级别|
|return|从函数返回|
|struct|结构体类型定义|
|switch|多路分支选择语句|
|test|测试代码块|
|threadlocal|线程局部变量|
|try|尝试执行表达式，错误时向上传播|
|union|联合体类型定义|
|usingnamespace|将命名空间的所有公开成员引入当前作用域|
|var|定义可变变量|
|void|无类型，常用于函数无返回值|
|while|循环语句|

## 基本语法

1. 变量与常量

在 Zig 中，变量使用 var 关键字定义，常量使用 const 关键字定义。

```
const x: i32 = 10;  // 定义一个整数常量 x，值为 10
var y: f64 = 3.14;  // 定义一个浮点数变量 y，值为 3.14
```

2. 函数

函数使用 fn 关键字定义，并指定返回类型。

```
const std = @import("std");

fn add(a: i32, b: i32) i32 {
    return a + b;
}

pub fn main() !void {
    const result = add(3, 4);
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Result: {}\n", .{result});
}
```

3. 条件语句

```
const std = @import("std");

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    const number = 10;
    if (number > 0) {
        try stdout.print("Number is positive\n", .{});
    } else {
        try stdout.print("Number is not positive\n", .{});
    }
}
```

4. 循环

```
const std = @import("std");

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();

    // while 循环，: (i += 1) 是每次迭代后执行的更新表达式
    var i: i32 = 0;
    while (i < 5) : (i += 1) {
        try stdout.print("i: {}\n", .{i});
    }

    // for 循环，遍历数组
    const array = [5]i32{ 1, 2, 3, 4, 5 };
    for (array) |item| {
        try stdout.print("item: {}\n", .{item});
    }
}
```

5. 结构体

```
const std = @import("std");

const Point = struct {
    x: i32,
    y: i32,
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    const p = Point{ .x = 10, .y = 20 };
    try stdout.print("Point: ({}, {})\n", .{ p.x, p.y });
}
```

6. 错误处理

```
const std = @import("std");

const FileError = error{
    FileNotFound,
};

fn readFile(path: []const u8) !void {
    // 模拟一个可能失败的操作
    if (std.mem.eql(u8, path, "invalid")) {
        return FileError.FileNotFound;
    }
    // 其他操作...
}

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();

    readFile("invalid") catch |err| {
        switch (err) {
            FileError.FileNotFound => {
                try stdout.print("Error: File not found\n", .{});
            },
            else => return err,
        }
        return;
    };

    try stdout.print("File read successfully\n", .{});
}
```

代码解析：
- catch |err|：捕获错误并绑定到变量 err。
- switch (err)：对错误类型进行匹配，分别处理不同的错误情形。
- else => return err：对未预期的错误直接向上传播，不静默忽略。
- 如果 readFile 执行成功（没有触发 catch），则继续执行后面的打印语句。

## 项目管理文件

|文件|说明|
|:---|:---|
|build.zig|定义如何构建项目|
|build.zig.zon|定义项目是什么、依赖有哪些|

### build.zig

是一个真正的zig程序，定义执行 `zig build` 时候，zig首先会编译并运行 `build.zig`，由它决定：
- 编译哪些源文件
- 生成哪些可执行程序
- 是否生成动态库/静态库
- 是否运行测试
- 是否安装
- 是否链接 libc
- 是否链接第三方库
- 是否执行自定义步骤

```zig
const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "hello",
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
    });

    b.installArtifact(exe);
}
```
执行

```Zig
zig build
```
生成：`zig-out/bin/hello`

### 它能做什么

由于它本身是 Zig 程序，因此几乎可以做任何事情，例如：
- 读取文件
- 扫描目录
- 生成代码
- 执行外部程序
- 下载资源
- 生成版本号
- 根据平台选择源码
- 生成 config.h
- 生成 version.zig
- 扫描 plugins/

例如：
```
if (target.result.os.tag == .linux) {
    // Linux
} else if (target.result.os.tag == .windows) {
    // Windows
}
```

## build.zig.zon

这是 Zig 的项目清单（Manifest）。

它采用 Zig Object Notation（ZON）格式，语法类似 Zig 字面量。

例如：
```
.{
    .name = "myproject",
    .version = "0.1.0",

    .dependencies = .{
        .foo = .{
            .url = "https://github.com/xxx/foo/archive/main.tar.gz",
            .hash = "...",
        },
    },
}
```

主要描述：
- 项目名称
- 项目版本
- 最低 Zig 版本
- 第三方依赖
- 包信息

它不会告诉 Zig 如何编译。

`.zon` 是 Zig Object Notation。

### 二者如何配合

假设项目：

```
myapp/

├── build.zig
├── build.zig.zon
├── src/
│   └── main.zig
└── zig-cache/
```

执行：

```
zig build
```

流程大致如下：

```
读取 build.zig.zon
        │
        ▼
下载依赖（如果需要）
        │
        ▼
运行 build.zig
        │
        ▼
编译源码
        │
        ▼
生成 zig-out/
```

因此：
build.zig.zon 提供项目和依赖信息。
build.zig 根据这些信息组织实际的构建过程。

> 注意：可以没有 `build.zig.zon`，甚至也可以没有 `build.zig`。只有当需要：包管理、第三方依赖、发布项目时候才添加`build.zig.zon`
