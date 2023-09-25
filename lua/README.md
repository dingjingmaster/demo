# Lua

## 注释

### 单行注释
```lua
--
```

### 多行注释
```lua
--[[
多行注释
多行注释
--]]
```

## 标识符

Lua 标识符用于定义一个变量、函数获取其它用户定义的项。

标识符以数字、字母、下划线组成，数字不开头

> 尽量不要使用下划线 + 大写字母的标识符，因为Lua保留字也是这样的。
> Lua 标识符大写小写敏感

## 关键词

- and
- break
- do
- else
- elseif
- end
- false
- for
- function
- if
- in
- local
- nil
- not
- or
- repeat
- return
- then
- true
- until
- while
- goto

> 一般约定，以下划线开头连接一串大写字母的名字(比如：_VERSION)被保留用于Lua内部全局变量。

默认情况下，变量总是认为是全局的。全局变量不需要声明，给一个变量赋值后即创建了这个全局变量，访问一个没有初始化的全局变量也不会出错，只不过得到的结果是：nil。

如果想删除一个全局变量，只需要赋值为 nil 即可。

## 数据类型

|数据类型|描述|
|--------|----|
|nil|表示无效值，在条件表达式中表示 false|
|boolean||
|number||
|string||
|function|由C或Lua编写的函数|
|userdata|表示任务存储在变量中的C数据结构|
|thread|表示执行的独立线路，用于执行协同程序|
|table|Lua中的表，实际上是一个关联数组，最简单的构造表是`{}`|

> 通过使用 type 函数来测试变量或者值的类型：

```lua
print(type("Hello world"))      --> string
print(type(10.4*3))             --> number
print(type(print))              --> function
print(type(type))               --> function
print(type(true))               --> boolean
print(type(nil))                --> nil
print(type(type(X)))            --> string
```

> type(X)==nil 结果为 false 的原因是 type(X) 实质是返回的 "nil" 字符串，是一个 string 类型

### table

```lua
tab1 = { key1 = "val1", key2 = "val2", "val3" }
for k, v in pairs(tab1) do
    print(k .. " - " .. v)
end
 
tab1.key1 = nil
for k, v in pairs(tab1) do
    print(k .. " - " .. v)
end
```

表的构建是通过构建表达式来完成的，最简单的构建表达式就是`{}`

Lua中表里的默认初始索引一般是以 1 开始的。

### bool

**boolean 类型只有两个可选值：true（真） 和 false（假），Lua 把 false 和 nil 看作是 false，其他的都为 true，数字 0 也是 true**

### 数字

所有数字都是 number 类型，确切的说都是 double 类型

### 字符串

字符串由单引号或双引号表示，也可以由两个方括号 `[[ ]]` 表示

对字符串进行算术运算时候，Lua会尝试转为数字，无法转换则导致异常

用 `#` 来计算字符串的长度

### 函数 function

在 Lua 中，函数是被看作是"第一类值（First-Class Value）"，函数可以存在变量里:

函数参数可以传递匿名函数

### 线程(thread)

在 Lua 里，最主要的线程是协同程序（coroutine）。它跟线程（thread）差不多，拥有自己独立的栈、局部变量和指令指针，可以跟其他协同程序共享全局变量和其他大部分东西。

线程跟协程的区别：线程可以同时多个运行，而协程任意时刻只能运行一个，并且处于运行状态的协程只有被挂起（suspend）时才会暂停。

### 用户自定义类型

userdata 是一种用户自定义数据，用于表示一种由应用程序或 C/C++ 语言库所创建的类型，可以将任意 C/C++ 的任意数据类型的数据（通常是 struct 和 指针）存储到 Lua 变量中调用。

## 变量

变量在使用前，需要在代码中进行声明，即创建该变量。

编译程序执行代码之前编译器需要知道如何给语句变量开辟存储区，用于存储变量的值。

Lua 变量有三种类型：全局变量、局部变量、表中的域。

Lua 中的变量全是全局变量，哪怕是语句块或是函数里，除非用 local 显式声明为局部变量。

局部变量的作用域为从声明位置开始到所在语句块结束。

变量的默认值均为 nil。

```lua
a = 5               -- 全局变量
local b = 5         -- 局部变量

function joke()
    c = 5           -- 全局变量
    local d = 6     -- 局部变量
end

joke()
print(c,d)          --> 5 nil

do
    local a = 6     -- 局部变量
    b = 6           -- 对局部变量重新赋值
    print(a,b);     --> 6 6
end

print(a,b)      --> 5 6
```

## 垃圾回收
Lua 提供了以下函数collectgarbage ([opt [, arg]])用来控制自动内存管理:
- collectgarbage("collect"): 做一次完整的垃圾收集循环。通过参数 opt 它提供了一组不同的功能：
- collectgarbage("count"): 以 K 字节数为单位返回 Lua 使用的总内存数。 这个值有小数部分，所以只需要乘上 1024 就能得到 Lua 使用的准确字节数（除非溢出）。
- collectgarbage("restart"): 重启垃圾收集器的自动运行。
- collectgarbage("setpause"): 将 arg 设为收集器的 间歇率。 返回 间歇率 的前一个值。
- collectgarbage("setstepmul"): 返回 步进倍率 的前一个值。
- collectgarbage("step"): 单步运行垃圾收集器。 步长"大小"由 arg 控制。 传入 0 时，收集器步进（不可分割的）一步。 传入非 0 值， 收集器收集相当于 Lua 分配这些多（K 字节）内存的工作。 如果收集器结束一个循环将返回 true 。
- collectgarbage("stop"): 停止垃圾收集器的运行。 在调用重启前，收集器只会因显式的调用运行。

```lua
mytable = {"apple", "orange", "banana"}

print(collectgarbage("count"))

mytable = nil

print(collectgarbage("count"))

print(collectgarbage("collect"))

print(collectgarbage("count"))
```
