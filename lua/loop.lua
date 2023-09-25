#!/bin/lua

-- 循环
--  while
--  for
--  repeat .. until
--  嵌套循环

-- 控制语句
--  break
--  goto


-- while(condition)
-- do
--  statements
-- end

a=10
while( a < 20 )
do
   print("a 的值为:", a)
   a = a+1
end

-- for var=exp1,exp2,exp3 do
--    <执行体>
-- end
for i=10,1,-1 do
    print(i)
end

function f(x)  
    print("function")  
    return x*2  
end  
for i=1,f(5) do print(i)  
end

-- 泛型for
print("泛型 for")
a = {"one", "two", "three"}
for i, v in ipairs(a) do
    print(i, v)
end 

print("days:")
days = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"}  
for i,v in ipairs(days) do  print(v) end


-- repeat
--    statements
-- until( condition )
--[ 变量定义 --]
a = 10
--[ 执行循环 --]
repeat
   print("a的值为:", a)
   a = a + 1
until( a > 15 )

