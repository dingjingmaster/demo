#!/bin/lua

s1 = "this is string1"
s2 = 'this is string2'
s3 = [[
<html>
<head></head>
<body>
</body>
</html>
]]

n4 = "1" + "2"
-- n5 = "1" + "23a" -- 引发异常，导致程序终止

print(s1)
print(s2)
print(s3)
print(type(n4) .. " " .. n4)
-- print(type(n5) .. " " .. n5)

print("'" .. s1 .. "'" .. " len: " .. #s1)
