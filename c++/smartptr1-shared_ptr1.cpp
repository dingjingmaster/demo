/*************************************************************************
> FileName: smartptr1-shared_ptr.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 29 Aug 2022 02:18:30 PM CST
 ************************************************************************/
#include <string>
#include <memory>
#include <vector>
#include <iostream>

int main (int argc, char* argv[])
{
    using namespace std;

    string* str = new string("aaaaaa");

    shared_ptr<string> p1;

    p1.reset(str);

    cout << *p1 << endl;

    return 0;
}
