#!/bin/lua

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

-- 赋值语句
a = "hello" .. "world"
t.n = t.n + 1

-- Lua 可以对多个变量同时赋值，变量列表和值列表的各个元素用逗号分开，赋值语句右边的值会依次赋给左边的变量。
a, b = 10, 2*x

-- 遇到赋值语句Lua会先计算右边所有的值然后再执行赋值操作，所以我们可以这样进行交换变量的值：
x, y = y, x                     -- swap 'x' for 'y'
a[i], a[j] = a[j], a[i]         -- swap 'a[i]' for 'a[j]'

-- 当变量个数和值的个数不一致时，Lua会一直以变量个数为基础采取以下策略：
--  a. 变量个数 > 值的个数             按变量个数补足nil
--  b. 变量个数 < 值的个数             多余的值会被忽略

a, b, c = 0, 1
print(a,b,c)             --> 0   1   nil
 
a, b = a+1, b+1, b+2     -- value of b+2 is ignored
print(a,b)               --> 1   2
 
a, b, c = 0
print(a,b,c)             --> 0   nil   nil

a, b, c = 0, 0, 0
print(a,b,c)             --> 0   0   0

-- 多值赋值经常用来交换变量，或将函数调用返回给变量：
a, b = f()


-- 索引
t[i]
t.i                 -- 当索引为字符串类型时的一种简化写法
gettable_event(t,i) -- 采用索引访问本质上是一个类似这样的函数调用

site = {}
site["key"] = "www.runoob.com"
print(site["key"])  -- 输出: www.runoob.com

print(site.key)     -- 输出: www.runoob.com
