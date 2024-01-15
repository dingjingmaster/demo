/*************************************************************************
> FileName: map2.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 15 Jan 2024 02:31:35 PM CST
 ************************************************************************/
#include <map>
#include <string>
#include <iostream>
#include <string_view>
 
int main ()
{
    std::map<std::string, int> m { {"CPU", 10}, {"GPU", 15}, {"RAM", 20}, };

    for (auto it = m.begin(); it != m.end();) {
        std::cout << it->first << " " << it->second << std::endl;
        it = m.erase(it);
    }
}
