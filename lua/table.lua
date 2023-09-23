#!/usr/bin/lua

table = { k1 = "v1", k2 = "v2", k3 }

-- 并不会输出 k3
for k, v in pairs(table) do
    print (k .. " - " .. v)
end

print("---")
table.k1 = nil  -- 相当于删除 k1 键
for k, v in pairs(table) do
    print (k .. " - " .. v)
end
