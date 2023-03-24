/*************************************************************************
> FileName: any-of.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 20 Mar 2023 04:59:56 PM CST
 ************************************************************************/
#include <stdio.h>

#include <vector>
#include <algorithm>
#include <functional>


int main (int argc, char* argv[])
{
    std::vector<int> ev = {1, 3, 1, 5, 7, 9};

    if (std::any_of(ev.begin(), ev.end(), [=] (int a) -> bool {
        printf("%s\n", (!(a % 2) ? "true" : "false"));
        return !(a % 2 ? true : false);
    })) printf("ok\n");


    return 0;
}
