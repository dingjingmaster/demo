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

## 变量

