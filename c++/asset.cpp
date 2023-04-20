/*************************************************************************
> FileName: asset.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 20 Apr 2023 09:59:33 AM CST
 ************************************************************************/
#include <stdio.h>

#include <vector>
#include <assert.h>
#include <iostream>


int main (int argc, char* argv[])
{
    printf ("1\n");
    assert(1==2);           // 运行时候断言，core
    static_assert(1==2);    // 编译时候断言
    printf ("2\n");

    return 0;
}
