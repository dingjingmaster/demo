#!/bin/lua

print(type("Hello world"))      --> string
print(type(10.4*3))             --> number
print(type(print))              --> function
print(type(type))               --> function
print(type(true))               --> boolean
print(type(nil))                --> nil
print(type(type(X)))            --> string

print("----")
print(type(M)==nil)             -- false
print(type(M)=="nil")           -- true

-- type 命令返回的是一个字符串，因此 type(M) 返回的是字符串 "nil"
