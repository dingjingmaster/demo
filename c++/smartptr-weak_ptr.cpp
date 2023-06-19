/*************************************************************************
> FileName: smartptr-weak_ptr.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 19 Jun 2023 02:07:22 PM CST
 ************************************************************************/
#include <memory>
#include <iostream>

std::weak_ptr<int> p1;

void observe()
{
    std::cout << "gw.use_count() == " << p1.use_count() << "; ";
    // we have to make a copy of shared pointer before usage:
    if (std::shared_ptr<int> spt = p1.lock()) {
        std::cout << "*spt == " << *spt << '\n';
    }
    else {
        std::cout << "gw is expired\n";
    }
}

int main (int argc, char* argv[])
{
    {
        auto sp = std::make_shared<int>(42);
        p1 = sp;
        observe();
    }

    observe();

    return 0;
}
