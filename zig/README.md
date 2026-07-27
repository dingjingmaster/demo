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

## zig 注释

1. 单行注释
2. 多行注释

## zig 数据类型

Zig 支持多种数据类型，涵盖了整数、浮点数、布尔值、字符、数组、切片、结构体、枚举、联合体和指针等。

下表是 Zig 中各种数据类型的说明：

|数据类型类别|数据类型示例|描述|
|:-----------|:-----------|:---|
|整数类型|i8, i16, i32, i64, isize|由符号整数类型，isize是平台相关的大小|
|无符号整数|u8, u16, u32, u64, usize|无符号整数类型，usize是平台相关的大小|
|浮点数|f16, f32, f64, f128|IEEE浮点数类型|
|布尔类型|bool|布尔类型，值为true或false|
|字符类型|char|unicode标量值|
|符合类型|array, vector|固定大小数组和可变大小数组|
|指针类型|`*T`, `*const T`, `*mut T`|指向 T 类型值的指针，`*const`为只读，`*mut`为可变|
|引用类型|`&T`, `&const T`, `&mut T`|对 T 类型值的引用，`&const`为只读，`&mut`为可变|
|元组类型|(T1, T2, ...)|包含固定数量和类型的值的有序集合|
|可选类型|?T|可以是null或T类型值|
|错误集合类型|error{...}|包含错误值的枚举类型|
|函数类型|fn(T1, T2, T3, ...) ->R |接受参数并返回结果的函数类型|
|结构体类型|struct {...}|包含多个字段的复合数据类型|
|枚举类型|enum {...}|固定数量的命名值的集合|
|联合体类型|union {...}|可以存储多种不同类型值的类型，但一次只能存储一个|
|别名类型|alias T = U|T 是 U 的别名|

### 整数类型

Zig 支持多种数据类型，涵盖了整数、浮点数、布尔值、字符、数组、切片、结构体、枚举、联合体和指针等。

下表是 Zig 中各种数据类型的说明：

```zig
const std = @import("std");

pub fn main() void {
    const a: i8 = -128; // 8-bit signed integer
    const b: u8 = 255;  // 8-bit unsigned integer
    const c: i32 = -2147483648; // 32-bit signed integer
    const d: u64 = 18446744073709551615; // 64-bit unsigned integer

    std.debug.print("a: {}, b: {}, c: {}, d: {}\n", .{a, b, c, d});
}
```

### 浮点数类型

Zig 支持 f32 和 f64 两种浮点数类型。

```zig
const std = @import("std");

pub fn main() void {
    const pi: f32 = 3.14;   // 32-bit floating point
    const e: f64 = 2.71828; // 64-bit floating point

    std.debug.print("pi: {}, e: {}\n", .{pi, e});
}
```

### 布尔类型

布尔类型使用 bool 表示，取值可以是 true 或 false。

```zig
const std = @import("std");

pub fn main() void {
    const is_true: bool = true;
    const is_false: bool = false;

    std.debug.print("is_true: {}, is_false: {}\n", .{is_true, is_false});
}
```

### 字符类型

字符类型使用 u8 来表示单个字符。

```
const std = @import("std");

pub fn main() void {
    const letter: u8 = 'A';

    std.debug.print("letter: {}\n", .{letter});
}
```

### 数组和切片

数组是固定大小的，切片则是动态大小的数组。

```
const std = @import("std");

pub fn main() void {
    const array: [5]i32 = [5]i32{1, 2, 3, 4, 5}; // 固定大小数组
    const slice: []const i32 = array[1..4]; // 切片

    std.debug.print("array: {}, slice: {}\n", .{array, slice});
}
```

### 结构体

结构体用 struct 定义，允许你创建复杂的数据类型。

```
const std = @import("std");

const Point = struct {
    x: i32,
    y: i32,
};

pub fn main() void {
    const p = Point{ .x = 10, .y = 20 };

    std.debug.print("Point: ({}, {})\n", .{p.x, p.y});
}
```

### 枚举

枚举用 enum 定义，允许你创建有命名值的类型。

```
const std = @import("std");

const Color = enum {
    Red,
    Green,
    Blue,
};

pub fn main() void {
    const color: Color = Color.Green;

    std.debug.print("Color: {}\n", .{color});
}
```

### 联合体

联合体用 union 定义，允许你创建一个可以存储不同类型值的变量。

```
const std = @import("std");

const Number = union(enum) {
    Int: i32,
    Float: f32,
};

pub fn main() void {
    const num: Number = Number{ .Int = 10 };

    switch (num) {
        Number.Int => std.debug.print("Integer: {}\n", .{num.Int}),
        Number.Float => std.debug.print("Float: {}\n", .{num.Float}),
    }
}
```

### 指针

指针用 * 定义，可以指向特定类型的变量。

```
const std = @import("std");

pub fn main() void {
    var a: i32 = 10;
    const p: *i32 = &a; // 指向 a 的指针

    std.debug.print("Value: {}, Pointer: {}\n", .{a, p.*});
}
```

## zig变量和常量

在 Zig 中，常量使用 const 关键字定义，而变量使用 var 关键字定义。

### 变量

变量必须在定义时显式指定类型，或者通过初始化值让编译器推断类型。

### 变量声明

在 Zig 中，变量的声明需要指定类型，变量的声明语法如下：

```
var variable_name: type = value;
```

variable_name 为变量名，type 为类型，value 为变量值。

```
var x: i32 = 42; // 定义一个 i32 类型的变量 x，初始值为 42
var y = 10;      // 编译器推断 y 的类型为 comptime_int
```

变量的值可以在程序运行期间修改：

```
const std = @import("std");

pub fn main() void {
    var b: i32 = 20; // 定义一个整数变量 b，初始值为 20
    b = 30; // 修改变量 b 的值
    std.debug.print("b: {}\n", .{b});
}
```

### 变量特点

1、类型必须明确：
- Zig 是强类型语言，变量的类型必须在定义时明确指定，或者通过初始化值推断。
- 如果没有初始化值，必须显式指定类型。

```
var x: i32 = 10; // 显式指定类型
var y = 20;      // 编译器推断类型为 comptime_int
```

2. 可变性：
- 使用 var 定义的变量是可变的，可以在后续代码中修改其值。

```
var x: i32 = 10;
x = 20; // 修改 x 的值
```

3. 作用域
- 变量的作用域是块级作用域（block scope），即在定义它的代码块内有效。

```
{
    var x: i32 = 10;
    std.debug.print("x = {}\n", .{x}); // 输出：x = 10
}
// 这里 x 已经超出作用域，无法访问
```

4. 未初始化变量
- Zig 不允许使用未初始化的变量。如果变量未初始化，编译器会报错

```
var x: i32; // 错误：变量 x 未初始化
x = 10;    // 必须先初始化
```

## 类型推断

Zig 支持类型推断。如果变量在定义时初始化，编译器可以根据初始值推断变量的类型

```
var x = 10;      // 编译器推断 x 的类型为 comptime_int
var y = 3.14;    // 编译器推断 y 的类型为 comptime_float
var z = "Hello"; // 编译器推断 z 的类型为 *const [5:0]u8
```

## 作用域

变量的作用域由其定义的位置决定。

在 Zig 中，变量可以在全局作用域、局部作用域和块作用域中定义。

全局作用域 - 全局变量可以在程序的任何地方访问。

```
const std = @import("std");

const g: i32 = 90; // 全局常量

pub fn main() void {
    std.debug.print("g: {}\n", .{g});
}
```

局部作用域 - 局部变量只能在其定义的函数或代码块内访问

```
const std = @import("std");

pub fn main() void {
    var h: i32 = 100; // 局部变量
    {
        var i: i32 = 110; // 块作用域变量
        std.debug.print("i: {}\n", .{i});
    }
    // std.debug.print("i: {}\n", .{i}); // 这行代码会导致编译错误，因为 i 不在 main 函数的作用域内
    std.debug.print("h: {}\n", .{h});
}
```

## 类型转换

Zig 提供了类型转换函数来将一种类型转换为另一种类型

```
const std = @import("std");

pub fn main() void {
    const j: i32 = 120;
    const k: f64 = @intToFloat(f64, j); // 将整数 j 转换为浮点数 k
    std.debug.print("j: {}, k: {}\n", .{j, k});
}
```

## 默认值

变量在定义时必须被初始化，否则会导致编译错误。

Zig 不允许使用未初始化的变量。

```
const std = @import("std");

pub fn main() void {
    var l: i32 = 0; // 初始化变量 l
    std.debug.print("l: {}\n", .{l});
}
```

## 变量的使用示例

以下是一个完整的 Zig 程序，演示了变量的定义和使用：

```
const std = @import("std");

pub fn main() void {
    // 定义变量
    var x: i32 = 10;
    var y = 20; // 类型推断为 comptime_int

    // 修改变量的值
    x = 30;
    y = 40;

    // 输出变量的值
    std.debug.print("x = {}\n", .{x}); // 输出：x = 30
    std.debug.print("y = {}\n", .{y}); // 输出：y = 40

    // 块级作用域
    {
        var z: i32 = 50;
        std.debug.print("z = {}\n", .{z}); // 输出：z = 50
    }
    // 这里 z 已经超出作用域，无法访问
}
```
## 常量

在 Zig 中，常量使用 const 关键字定义。

常量一旦定义，其值不可更改。

```
const std = @import("std");

pub fn main() void {
    const a: i32 = 10; // 定义一个整数常量 a，值为 10
    std.debug.print("a: {}\n", .{a});
}
```

常量特点：
- 不可变性：常量的值在定义后不可修改。
- 编译时确定：常量的值必须在编译时确定，不能是运行时计算的结果。
- 类型推断：如果常量的类型未显式指定，编译器会根据初始值推断类型。
- 命名规范：常量的命名通常使用全大写字母和下划线（如 MAX_SIZE），以区别于变量。

## 编译时常量

Zig 支持编译时常量（comptime constants），这些常量的值在编译时计算，并且可以用于编译时的逻辑。

定义编译时常量
使用 comptime 关键字定义编译时常量

```
comptime const constant_name: type = value;
```

例如：

```
comptime const MAX_SIZE: usize = 100; // 编译时常量
```

特点：
- 编译时计算： 编译时常量的值在编译时计算，可以用于编译时的逻辑（如数组大小、类型计算等）。
- 类型安全： 编译时常量的类型必须在编译时确定。
- 性能优化： 使用编译时常量可以避免运行时的计算开销。

```
const std = @import("std");

pub fn main() void {
    comptime const SIZE: usize = 10; // 编译时常量
    var arr: [SIZE]i32 = undefined;  // 使用编译时常量定义数组大小
    std.debug.print("Array size = {}\n", .{SIZE}); // 输出：Array size = 10
}
```

## 变量与常量的区别

|特性|变量 (var)|常量 (const)|编译时常量(comptime const)|
|:---|:---------|:-----------|:-------------------------|
|可变性|可变|不可变|不可变|
|定义关键字|var|const|comptime const|
|初始化要求|必须初始化|必须初始化|必须初始化|
|类型推断|支持|支持|支持|
|作用域|块级作用域|块级作用域|块级作用域|
|使用场景|需要修改的值|不需要修改的值|编译时计算的值|

```
const std = @import("std");

pub fn main() void {
    // 变量
    var x: i32 = 10;
    x = 20;
    std.debug.print("x = {}\n", .{x}); // 输出：x = 20

    // 常量
    const PI: f64 = 3.14159;
    std.debug.print("PI = {}\n", .{PI}); // 输出：PI = 3.14159

    // 编译时常量
    comptime const SIZE: usize = 5;
    var arr: [SIZE]i32 = undefined;
    std.debug.print("Array size = {}\n", .{SIZE}); // 输出：Array size = 5
}
```

## 循环

### while 循环

```
while (condition) : (increment) {
    // code block
}
```
- condition：一个布尔表达式，只要它为 true，循环就会继续执行。
- increment（可选）：一个在每次循环结束后执行的表达式。

```
const std = @import("std");

pub fn main() void {
    var i: i32 = 0;
    while (i < 5) : (i += 1) {
        std.debug.print("i: {}\n", .{i});
    }
}
```

### for 循环

```
for (collection) |item, index| {
    // code block
}
```

- collection：一个数组、切片或其他可迭代的集合。
- item：在每次迭代中，集合的当前元素。
- index（可选）：当前元素的索引。

```
const std = @import("std");

pub fn main() void {
    const array = [5]i32{ 1, 2, 3, 4, 5 };
    var index: usize = 0; // 索引变量需要声明类型
    for (array) |item| {
        std.debug.print("index: {}, item: {}\n", .{ index, item });
        index += 1; // 更新索引
    }
}
```

解析：
- 定义了一个包含 5 个整数的数组。
- 使用 for 循环遍历数组中的每个元素，并将元素值赋给 item，索引赋给 index。
- 在循环体中，打印每个元素的值和索引。

### continue和break

- continue：跳过当前迭代并继续下一次迭代。
- break：终止循环。

```
const std = @import("std");

pub fn main() void {
    var i: i32 = 0;
    while (i < 10) : (i += 1) {
        if (i == 5) {
            continue; // 跳过 i 等于 5 的那次迭代
        }
        if (i == 8) {
            break; // 终止循环
        }
        std.debug.print("i: {}\n", .{i});
    }
}
```

- 当 i 等于 5 时，continue 跳过该次迭代。
- 当 i 等于 8 时，break 终止循环。

### 嵌套循环

```
for (outer_collection) |outer_item| {
    for (inner_collection) |inner_item| {
        // code block
    }
}
```

例子：

```
const std = @import("std");

pub fn main() void {
    // 声明一个包含三个字符串的数组
    const letters = [_][]const u8{ "A", "B", "C" };
    // 遍历数组
    for (letters) |letter| {
        var count: i32 = 0; // 声明计数器
        // 使用 while 循环来打印每个字母和计数
        while (count < 3) : (count += 1) {
            std.debug.print("{s} - {}\n", .{ letter, count });
        }
    }
}
```

### 更多循环

```
pub fn main() void {
    while (true) {
        // 代码块
        // 必须包含某种退出机制，否则程序将永远运行
    }
}
```

范围循环

```
pub fn main() void {
    for (0..10) |i| {
        std.debug.print("i: {}\n", .{i});
    }
}
```

标签循环

```
pub fn main() void {
    loop: while (true) {
        while (true) {
            std.debug.print("Inside nested loop\n");
            break :loop; // 退出外部循环
        }
    }
}
```

## zig 流程控制

### if 语句

```
if (<condition) {
    // 如果 condition 为 true，执行这里的代码
}

if (<condition>) {
    // 如果 condition 为 true，执行这里的代码
} else {
    // 如果 condition 为 false，执行这里的代码
}
```

例子：
```
const std = @import("std");

pub fn main() void {
    const x: i32 = 10;
    if (x > 5) {
        std.debug.print("x is greater than 5\n", .{});
    } else {
        std.debug.print("x is not greater than 5\n", .{});
    }
}
```

例子：
```
if (condition1) {
    // 如果 condition1 为 true，执行这里的代码
} else if (condition2) {
    // 如果 condition2 为 true，执行这里的代码
} else {
    // 如果所有条件为 false，执行这里的代码
}
```
例子：

```
const std = @import("std");

pub fn main() void {
    const x: i32 = 10;
    if (x > 10) {
        std.debug.print("x is greater than 10\n", .{});
    } else if (x == 10) {
        std.debug.print("x is equal to 10\n", .{});
    } else {
        std.debug.print("x is less than 10\n", .{});
    }
}
```

## 运算符

### 算术运算符

|运算符|描述|
|:-----|:---|
|`+`|加法|
|`-`|减法|
|`*`|乘法|
|`/`|除法|
|`%`|取余(模运算)|

例子：

```
const std = @import("std");

pub fn main() void {
    const a: i32 = 5;
    const b: i32 = 3;

    const add: i32 = a + b;
    const subtract: i32 = a - b;
    const multiply: i32 = a * b;
    const divide: i32 = a / b;
    const remainder: i32 = a % b;

    std.debug.print("a + b = {}\n", .{add});
    std.debug.print("a - b = {}\n", .{subtract});
    std.debug.print("a * b = {}\n", .{multiply});
    std.debug.print("a / b = {}\n", .{divide});
    std.debug.print("a % b = {}\n", .{remainder});
}
```

### 关系运算符

|运算符|描述|
|:-----|:---|
|`==`|等于|
|`!=`|不等于|
|`>`|大于|
|`<`|小于|
|`>=`|大于等于|
|`<=`|小于等于|

例子：

```
const std = @import("std");

pub fn main() void {
    const a: i32 = 5;
    const b: i32 = 3;

    const equal: bool = a == b;
    const not_equal: bool = a != b;
    const greater: bool = a > b;
    const less: bool = a < b;
    const greater_equal: bool = a >= b;
    const less_equal: bool = a <= b;

    std.debug.print("a == b: {}\n", .{equal});
    std.debug.print("a != b: {}\n", .{not_equal});
    std.debug.print("a > b: {}\n", .{greater});
    std.debug.print("a < b: {}\n", .{less});
    std.debug.print("a >= b: {}\n", .{greater_equal});
    std.debug.print("a <= b: {}\n", .{less_equal});
}
```

### 逻辑运算

|运算符|描述|
|:-----|:---|
|`and`|逻辑与|
|`or`|逻辑或|
|`!`|逻辑非|

例子：
```
const std = @import("std");

pub fn main() void {
    const a: bool = true;
    const b: bool = false;

    const and_result: bool = a and b;
    const or_result: bool = a or b;
    const not_result: bool = !a;

    std.debug.print("a and b: {}\n", .{and_result}); // false
    std.debug.print("a or b: {}\n", .{or_result}); // true
    std.debug.print("!a: {}\n", .{not_result}); // false
}
```

### 位运算符

|运算符|描述|
|`&`|按位与|
|`|`|按位或|
|`^`|按位异或|
|`~`|按位取反|
|`<<`|左移|
|`>>`|右移|

例子：
```
const std = @import("std");

pub fn main() void {
    const a: i32 = 5;  // 0101
    const b: i32 = 3;  // 0011

    const bit_and: i32 = a & b;      // 0001
    const bit_or: i32 = a | b;       // 0111
    const bit_xor: i32 = a ^ b;      // 0110
    const bit_not: i32 = ~a;         // 11111111111111111111111111111010
    const left_shift: i32 = a << 1;  // 1010
    const right_shift: i32 = a >> 1; // 0010

    std.debug.print("a & b: {}\n", .{bit_and});
    std.debug.print("a | b: {}\n", .{bit_or});
    std.debug.print("a ^ b: {}\n", .{bit_xor});
    std.debug.print("~a: {}\n", .{bit_not});
    std.debug.print("a << 1: {}\n", .{left_shift});
    std.debug.print("a >> 1: {}\n", .{right_shift});
}
```

### 赋值运算符


|运算符|描述|
|:-----|:---|
|`=`|赋值|
|`+=`|加法赋值|
|`-=`|减法赋值|
|`*=`|乘法赋值|
|`/=`|除法赋值|
|`%=`|取余赋值|
|`&=`|按位与赋值|
|`|=`|按位或赋值|
|`^=`|按位异或赋值|
|`<<=`|左移赋值|
|`>>=`|右移赋值|

例子：

```
const std = @import("std");

pub fn main() void {
    var a: i32 = 5;
    const b: i32 = 3;

    a += b; // 相当于 a = a + b;
    std.debug.print("a += b: {}\n", .{a});

    a -= b; // 相当于 a = a - b;
    std.debug.print("a -= b: {}\n", .{a});

    a *= b; // 相当于 a = a * b;
    std.debug.print("a *= b: {}\n", .{a});

    a = @divTrunc(a, b); // 相当于 a = a / b;
    std.debug.print("a /= b: {}\n", .{a});

    a = @mod(a, b); // 相当于 a = a % b;
    std.debug.print("a %= b: {}\n", .{a});

    a &= b; // 相当于 a = a & b;
    std.debug.print("a &= b: {}\n", .{a});

    a |= b; // 相当于 a = a | b;
    std.debug.print("a |= b: {}\n", .{a});

    a ^= b; // 相当于 a = a ^ b;
    std.debug.print("a ^= b: {}\n", .{a});

    a <<= 1; // 相当于 a = a << 1;
    std.debug.print("a <<= 1: {}\n", .{a});

    a >>= 1; // 相当于 a = a >> 1;
    std.debug.print("a >>= 1: {}\n", .{a});
}
```

### 其他运算符

|运算|符描述|
|:---|:-----|
|`++`|自增|
|`--`|自减|

例子
```
const std = @import("std");

pub fn main() void {
    var a: i32 = 5;

    a += 1; // Zig 中没有 ++ 运算符，可以用 += 1 替代
    std.debug.print("a += 1: {}\n", .{a});

    a -= 1; // Zig 中没有 -- 运算符，可以用 -= 1 替代
    std.debug.print("a -= 1: {}\n", .{a});
}
```

## zig 函数

```
fn 函数名(参数列表) 返回类型 {
    // 函数体
}
```

例子：
```
const std = @import("std");

pub fn main() void {
    printHello();
}

fn printHello() void {
    std.debug.print("Hello, World!\n", .{});
}
```

参数传递
函数可以接受参数，这些参数可以是各种类型的值。

参数在函数定义时列出，并在调用时传递给函数，例如：
```
const std = @import("std");

pub fn main() void {
    greet("Alice");
}

fn greet(name: []const u8) void {
    std.debug.print("Hello, {s}!\n", .{name}); // 使用 {s} 格式符来打印字符串切片
}
```

### 返回值
函数可以返回值，返回类型在函数定义中指定，使用 return 关键字。

例如，定义一个计算两个整数和的函数：

```
const std = @import("std");

pub fn main() void {
    const result = add(3, 5);
    std.debug.print("3 + 5 = {}\n", .{result});
}

fn add(a: i32, b: i32) i32 {
    return a + b;
}
```

### 递归

函数可以调用自身，即递归调用。

```
const std = @import("std");

pub fn main() void {
    const result = factorial(5);
    std.debug.print("5! = {}\n", .{result});
}

fn factorial(n: i32) i32 {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}
```

### 函数重载

Zig 不支持传统意义上的函数重载（即同名但参数不同的多个函数）。

Zig 提供了泛型函数的功能，通过使用编译时常量参数，可以实现类似于函数重载的效果，例如：

```
const std = @import("std");

pub fn main() void {
    printValue(i32, 5); // 显式指定类型为 i32
    printValue([]const u8, "Hello"); // 显式指定类型为 []const u8
}

fn printValue(comptime T: type, value: T) void {
    std.debug.print("{any}\n", .{value}); // 使用 {any} 来处理不同类型的值
}
```

在这个例子中，printValue 函数可以接受任何类型的参数，并根据参数类型进行打印。

### 内核函数

内联函数是指在编译时将函数的代码直接插入到调用点，从而避免函数调用的开销。

在 Zig 中，可以使用 inline 关键字定义内联函数，例如：

```
const std = @import("std");

// 定义一个内联函数
inline fn square(x: i32) i32 {
    return x * x;
}

pub fn main() void {
    const result = square(5); // 在调用点插入内联函数代码
    std.debug.print("Square of 5 is {}\n", .{result});
}
```

## 数组和切片

在 Zig 编程语言中，数组和切片（slice）是用于存储和操作一组相同类型数据的基本结构。

- 数组：用于存储固定大小的一组相同类型的数据，定义时指定大小，存储在栈上。
- 切片：用于引用数组或其他连续内存区域的一部分，大小可动态调整，更灵活，通常引用堆内存。

数组（Array）：
- 数组是一种固定长度的序列，它在编译时大小就已经确定。
- 数组的类型是[T]，其中T是数组中元素的类型。
- 数组的内存是连续的，这使得它们在性能上很有优势，尤其是在处理大量数据时。

切片（Slice）：
- 切片是一种动态长度的序列，它允许在运行时改变大小。
- 切片的类型是[]T，其中T是切片中元素的类型。
- 切片实际上是对数组的引用，它包含指向数组的指针和切片的长度。
- 切片可以更灵活地处理数据，因为它们可以轻松地在不同的数组之间共享和传递。

### 数组

可以使用 [] 语法来定义数组，并指定数组的大小。

数组的类型定义为 [T; N]，其中 T 是数组元素的类型，N 是数组的长度。

数组的元素可以通过索引访问，索引从 0 开始。

```
const arrayName: [size]ElementType = [size]ElementType{element1, element2, ...};
```

参数说明：
- arrayName：数组的名称。
- size：数组的大小（元素数量），是一个编译时常量。
- ElementType：数组中元素的类型。
- element1, element2, ...：数组中的元素。

```
var myArray: [10]u8 = [10]u8{0} ** 10; // 定义并初始化一个大小为 10 的 u8 类型数组
```

例子：
```
const std = @import("std");

pub fn main() void {
    // 定义一个包含 5 个 i32 类型元素的数组
    const arr: [5]i32 = [5]i32{1, 2, 3, 4, 5};

    // 通过索引访问数组元素
    std.debug.print("First element: {}\n", .{arr[0]});
    std.debug.print("Third element: {}\n", .{arr[2]});

    // 数组的大小是固定的
    const size: usize = arr.len;
    std.debug.print("Array size: {}\n", .{size});
}
```

### 遍历数组

```
const std = @import("std");

pub fn main() void {
    const arr: [5]i32 = [5]i32{ 1, 2, 3, 4, 5 };

    var index: usize = 0;

    // 遍历数组
    for (arr) |item| {
        std.debug.print("Index: {}, Item: {}\n", .{ index, item });
        index += 1;
    }
}
```

### 切片

切片是对数组或其他连续内存区域的一部分的引用。

切片可以动态调整大小，并且比数组更灵活，但其元素存储在堆上。

定义和初始化：
- 切片是对数组或其他连续内存区域的一部分的引用。
- 切片是动态的，可以改变其大小，通常用于表示数组的一部分或动态分配的内存块。

```
const sliceName: []ElementType = array[start..end];
```
参数说明：
- sliceName：切片的名称。
- ElementType：切片中元素的类型。
- array[start..end]：从 array 中提取一个子切片，start 和 end 是索引。

切片可以通过数组的子集来创建，也可以通过指针和长度来创建：

```
var myArray: [10]u8 = ...; // 假设已经初始化
var mySlice = myArray[2..7]; // 创建一个切片，包含索引2到6的元素

// 或者使用指针和长度
var mySlicePtr = myArray[2..]; // 创建一个切片，从索引2开始到数组末尾
```

切片提供了一些内置的方法来操作切片，例如：
- len：获取切片的长度。
- ptr：获取切片的指针。
- capacity：获取切片的容量，即它能够引用的数组部分的最大长度。

例子：

```
const std = @import("std");

pub fn main() void {
    var arr: [5]i32 = [5]i32{ 1, 2, 3, 4, 5 };

    // 从数组创建切片
    const slice: []i32 = arr[1..4];

    // 通过索引访问切片元素
    std.debug.print("First element of slice: {}\n", .{slice[0]});
    std.debug.print("Second element of slice: {}\n", .{slice[1]});

    // 切片的长度
    const length: usize = slice.len;
    std.debug.print("Slice length: {}\n", .{length});
}
```

### 遍历切片

```
const std = @import("std");

pub fn main() void {
    var arr: [5]i32 = [5]i32{ 1, 2, 3, 4, 5 };
    const slice: []i32 = arr[1..4];
    var index: usize = 1;

    // 遍历切片
    for (slice) |item| {
        std.debug.print("Index: {}, Item: {}\n", .{ index, item });
        index += 1;
    }
}
```

数组与切片的区别
- 大小：数组的大小是固定的，定义时即确定；切片的大小可以动态调整。
- 存储位置：数组通常存储在栈上，而切片引用的内存可以在堆上。
- 灵活性：切片更灵活，可以引用数组的一部分或动态分配的内存。

|特性|数组|切片|
|:---|:---|:---|
|大小|固定，编译时确定|动态，可以改变大小|
|元素类型|相同|相同|
|内存位置|通常在栈上（局部变量）|引用的内存可能在堆上或栈上|
|访问|通过索引|通过切片的起始和结束索引|
|创建|直接定义|从数组或其他切片中创建|

以下实例中，printArray 函数接受一个固定大小的数组作为参数，而 printSlice 函数接受一个切片作为参数。通过这些函数，可以看到数组和切片在传递和使用上的差异。

```
const std = @import("std");

fn printArray(arr: [5]i32) void {
    for (arr) |item| {
        std.debug.print("Array item: {}\n", .{item});
    }
}

fn printSlice(slice: []const i32) void {
    for (slice) |item| {
        std.debug.print("Slice item: {}\n", .{item});
    }
}

pub fn main() void {
    const arr: [5]i32 = [5]i32{ 1, 2, 3, 4, 5 };
    const slice: []const i32 = arr[1..4];

    printArray(arr);
    printSlice(slice);
}
```

## 结构体和枚举

### 结构体

```
const structName = struct {
    field1: FieldType1,
    field2: FieldType2,
    // 其他字段
};
```

例子
```
const std = @import("std");

// 定义一个结构体
const Person = struct {
    name: []const u8,
    age: u32,
};

pub fn main() void {
    // 创建结构体实例
    const person = Person{
        .name = "Alice",
        .age = 30,
    };

    // 访问结构体字段并正确格式化
    std.debug.print("Name: {s}\n", .{person.name}); // 使用 {s} 格式化切片
    std.debug.print("Age: {}\n", .{person.age});
}
```

### 方法
在 Zig 中，结构体方法通过 fn 关键字定义，类似于其他编程语言中的类方法。
```
const std = @import("std");

const Rectangle = struct {
    width: u32,
    height: u32,

    // 计算面积的方法
    fn area(self: Rectangle) u32 {
        return self.width * self.height;
    }
};

pub fn main() void {
    var rect = Rectangle{
        .width = 10,
        .height = 5,
    };

    // 调用结构体方法
    std.debug.print("Area: {}\n", .{rect.area()});
}
```

### 枚举

```
const enumName = enum {
    Variant1,
    Variant2,
    // 其他变体
};
```

例子：

```
const std = @import("std");

// 定义一个枚举
const Color = enum {
    Red,
    Green,
    Blue,
};

pub fn main() void {
    // 使用枚举
    const favoriteColor = Color.Green;

    switch (favoriteColor) {
        Color.Red => std.debug.print("Red\n", .{}),
        Color.Green => std.debug.print("Green\n", .{}),
        Color.Blue => std.debug.print("Blue\n", .{}),
    }
}
```

带值的枚举
Zig 允许为枚举的每个变体指定具体的值，这可以用来表示更多的信息或进行比较。

```
const std = @import("std");

// 定义一个带值的枚举
const Status = enum(u32) {
    Pending = 1,
    InProgress = 2,
    Completed = 3,
};

pub fn main() void {
    const taskStatus = Status.InProgress;

    switch (taskStatus) {
        Status.Pending => std.debug.print("Pending\n", .{}),
        Status.InProgress => std.debug.print("InProgress\n", .{}),
        Status.Completed => std.debug.print("Completed\n", .{}),
    }
}
```

### 使用枚举作为字段

枚举可以用作结构体字段，使得结构体更加灵活和功能强大

```
const std = @import("std");

const Status = enum {
    Active,
    Inactive,
    Suspended,
};

const User = struct {
    name: []const u8,
    status: Status,
};

pub fn main() void {
    // 创建 User 实例
    const user = User{
        .name = "Alice",
        .status = Status.Active,
    };

    // 输出 User 的 name 字段
    std.debug.print("User: {s}\n", .{user.name}); // 使用 {s} 格式化切片

    // 使用 switch 语句根据 status 输出不同的状态
    switch (user.status) {
        Status.Active => std.debug.print("Status: Active\n", .{}),
        Status.Inactive => std.debug.print("Status: Inactive\n", .{}),
        Status.Suspended => std.debug.print("Status: Suspended\n", .{}),
    }
}
```

## zig 错误处理

在 Zig 中处理错误是一种常见的任务，特别是在进行系统级编程时。

Zig 提供了一种灵活且显式的错误处理机制，使得开发人员能够清晰地管理和处理错误。

Zig 使用显式的错误处理机制，通过 ! 符号和 try 语句来处理错误。

错误处理在 Zig 中不像异常那样隐式，而是显式地表示在代码中，使得错误的处理更加透明和可控。

以下是一些基本的错误处理策略和技巧：
- 错误类型：Zig 中的错误通常被定义为 error 类型。你可以定义自己的错误类型来处理特定的错误情况。
- 返回错误：函数可以返回一个 error 类型的值来表示错误。调用者需要检查返回值并相应地处理错误。
- 错误检查：调用者需要检查函数返回的错误，并在发现错误时采取适当的行动。这通常涉及到使用 if 语句或 switch 语句。
- 错误传播：如果一个函数接收到一个错误，它可以决定处理这个错误或将错误传播到调用者。这可以通过返回错误或抛出异常来实现。
- 错误处理函数：Zig 允许你定义错误处理函数，这些函数可以在程序中被调用来处理错误。
- 使用 try 关键字：在 Zig 中，try 关键字用于尝试执行一个可能失败的操作，并捕获任何发生的错误。
- 错误代码：Zig 允许你定义错误代码来表示不同的错误情况。这可以通过 enum 或 error 类型来实现。
- 错误日志：在处理错误时，记录错误日志是一种常见的做法。这可以帮助开发者了解错误发生的原因和上下文。
- 资源清理：在处理错误时，确保释放或清理所有已分配的资源是很重要的。这可以通过 defer 语句来实现。
- 错误恢复：在某些情况下，你可能希望在发生错误后恢复程序的执行。这可以通过重新尝试操作或回退到安全状态来实现。

### 错误类型

在 Zig 中，错误类型通常用 ! 符号表示，它是一个泛型类型，表示可能发生错误的值。

```
const std = @import("std");

pub fn mightFail() !void {
    return error.SomeError;
}
```

### 错误处理机制

1. 使用 try 语句

try 语句用于在函数调用中自动处理错误。如果函数返回一个错误，try 会使得外层函数立即返回该错误。

```
const std = @import("std");

// 定义可能失败的函数
pub fn mightFail() !void {
    return error.SomeError; // 返回一个示例错误
}

// 使 main 函数允许返回错误
pub fn main() !void {
    // 尝试调用 mightFail 函数，如果失败，main 也会返回该错误
    try mightFail();

    // 如果没有错误，继续执行
    std.debug.print("Success!\n", .{});
}
```

2. 使用catch 语句

```
const std = @import("std");

pub fn mightFail() !void {
    return error.SomeError;
}

pub fn main() void {
    // 直接处理错误，避免未使用的变量
    _ = mightFail() catch |err| {
        std.debug.print("Error occurred: {}\n", .{err});
        return; // 处理错误后退出
    };

    std.debug.print("Success!\n", .{});
}
```

3. 使用catch捕获特定错误

```
const std = @import("std");

const Error = error{
    NotFound,
    PermissionDenied,
};

pub fn mightFail() !void {
    return Error.NotFound; // 返回一个示例错误
}

pub fn main() void {
    // 直接处理错误，不需要将结果存储到变量中
    _ = mightFail() catch |err| {
        switch (err) {
            Error.NotFound => std.debug.print("Not found error occurred\n", .{}),
            Error.PermissionDenied => std.debug.print("Permission denied error occurred\n", .{}),
        }
        return; // 处理错误后退出
    };

    std.debug.print("Success!\n", .{}); // 如果没有错误，继续执行
}
```

4. 使用 defer 语句
defer 语句用于在函数退出时执行一些清理操作，无论是正常返回还是因为错误返回。它类似于其他编程语言中的 finally 语句。

```
const std = @import("std");

pub fn someFunction() void {
    defer std.debug.print("Cleanup code executed\n", .{});

    // Function logic
    std.debug.print("Function logic\n", .{});
    // 可以在这里发生错误，defer 代码依然会执行
}

pub fn main() void {
    someFunction();
}
```

## 内存管理

Zig 语言是一种系统编程语言，其内存管理方式与 C 语言类似，由程序员显式控制，没有垃圾回收机制。这种设计使得 Zig 能够在多种环境中高效运行，如实时软件、操作系统内核、嵌入式设备和低延迟服务器等。

在 Zig 中，内存管理是通过以下几个关键概念来实现的：
- 手动内存管理： Zig 强调手动内存管理，允许开发者对内存分配和释放进行完全控制。这与许多自动内存管理（如垃圾回收）的语言不同。
- 分配器（Allocator）： Zig 提供了分配器接口 (Allocator) 用于内存分配。标准库中有几种预定义的分配器，例如 std.heap.page_allocator 和 std.heap.general_purpose_allocator。
- 内存安全： 虽然 Zig 不提供自动垃圾回收，但它通过严格的编译时检查和运行时检查来提高内存安全性。比如，Zig 不允许使用悬挂指针和未初始化的内存。
- 内存泄漏： Zig 没有内置的垃圾回收机制，开发者需要小心处理内存泄漏问题。确保分配的内存最终被释放是开发者的责任。

### 手动内存管理

在 Zig 中，内存分配和释放是通过分配器来完成的。以下是分配内存的用法示例：

```
const std = @import("std");

pub fn main() void {
    // 获取分配器
    var allocator = std.heap.page_allocator;

    // 使用分配器分配内存
    const size: usize = 1024;
    const ptr = allocator.alloc(u8, size) catch |err| {
        std.debug.print("Memory allocation failed: {}\n", .{err});
        return;
    };

    // 使用分配的内存
    ptr[0] = 42;
    std.debug.print("First byte: {}\n", .{ptr[0]});

    // 释放内存
    allocator.free(ptr);
}
```

### 内存泄漏

内存泄漏发生在分配了内存但未释放的情况下。

为了避免内存泄漏，你可以使用 defer 关键字在函数退出时自动释放内存：

```
const std = @import("std");

pub fn main() void {
    var allocator = std.heap.page_allocator;

    // 使用 defer 确保内存在函数结束时被释放
    const size: usize = 1024;
    const ptr = allocator.alloc(u8, size) catch |err| {
        std.debug.print("Memory allocation failed: {}\n", .{err});
        return;
    };
    defer allocator.free(ptr);

    // 使用分配的内存
    ptr[0] = 42;
    std.debug.print("First byte: {}\n", .{ptr[0]});
}
```

在上面的例子中，defer allocator.free(ptr); 确保了无论 main 函数如何退出（正常退出或因错误退出），内存都会被释放。


### 使用标准库进行内存管理

在 Zig 中，内存管理是通过使用标准库提供的分配器（Allocator）接口来进行的。

Zig 标准库提供了多种分配器实现，可以根据需求选择合适的分配器来进行内存分配和管理。

以下是使用 Zig 标准库进行内存管理的详细说明和示例。

Zig 标准库中的 std 模块提供了几种常用的分配器：
- `std.heap.page_allocator`：提供按页分配的分配器，适用于较大的内存块分配。
- `std.heap.GeneralPurposeAllocator`：通用分配器，适用于中小型内存块分配。
- `std.heap.FixedBufferAllocator`：固定缓冲区分配器，适用于在固定大小的缓冲区内进行内存分配。

内存分配和释放

使用 page_allocator 分配内存

page_allocator 通常用于分配较大的内存块。下面是一个使用 page_allocator 的示例：

```
const std = @import("std");

pub fn main() void {
    var allocator = std.heap.page_allocator;

    // 使用分配器分配内存
    const size: usize = 1024;
    const ptr = allocator.alloc(u8, size) catch |err| {
        std.debug.print("Memory allocation failed: {}\n", .{err});
        return;
    };

    // 使用分配的内存
    ptr[0] = 42;
    std.debug.print("First byte: {}\n", .{ptr[0]});

    // 释放内存
    allocator.free(ptr);
}
```

使用 FixedBufferAllocator 分配内存
FixedBufferAllocator 是一个固定缓冲区分配器，适用于在预分配的固定大小缓冲区内进行内存分配。下面是一个使用 FixedBufferAllocator 的示例：

```
const std = @import("std");

const BUFFER_SIZE: usize = 1024;

pub fn main() void {
    var buffer: [BUFFER_SIZE]u8 = undefined;
    var fixed_buffer_allocator = std.heap.FixedBufferAllocator.init(&buffer);

    // 获取内存分配器
    var allocator = fixed_buffer_allocator.allocator();

    // 使用分配器分配内存
    const size: usize = 256;
    const ptr = allocator.alloc(u8, size) catch |err| {
        std.debug.print("Memory allocation failed: {}\n", .{err});
        @panic("Allocation failed");
    };

    // 使用分配的内存
    ptr[0] = 42;
    std.debug.print("First byte: {}\n", .{ptr[0]});

    // 无需释放内存，因为使用的是固定缓冲区
}
```

分配器接口

所有分配器都实现了 Allocator 接口，该接口定义了以下方法：
- alloc：分配指定大小的内存块。返回一个 []T 类型的指针，如果分配失败，则返回错误。
- free：释放之前分配的内存块。

### 处理内存错误
```
const std = @import("std");

pub fn main() void {
    var allocator = std.heap.page_allocator;

    // 尝试分配内存
    const size: usize = 1024;
    const ptr = allocator.alloc(u8, size) catch |err| {
        std.debug.print("Memory allocation failed: {}\n", .{err});
        return;
    };

    // 使用分配的内存
    ptr[0] = 42;
    std.debug.print("First byte: {}\n", .{ptr[0]});

    // 释放内存
    allocator.free(ptr);
}
```
