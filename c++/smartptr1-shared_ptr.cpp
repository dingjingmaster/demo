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

    // create
    shared_ptr<string> pNico(new string("nico"));
    shared_ptr<string> pJutta(new string("jutta"));

    vector<shared_ptr<string>> v;
    v.push_back(pNico);
    v.push_back(pJutta);
    v.push_back(pNico);

    // 打印所有元素
    for (auto pt : v) {
        cout << *pt << "  ";
    }
    cout << endl;

    // 修改指针值
    *pNico = "Nicolai";

    // 再次打印
    for (auto pt : v) {
        cout << *pt << "  ";
    }
    cout << endl;

    // 打印引用计数
    for (auto pt = v.begin(); pt != v.end(); ++pt) {
        cout << *pt << " 'use count:" << pt->use_count() << endl;
    }

    // c++11 书里说编译会失败，然而实际并非如此
    std::shared_ptr<int[]> p1 (new int[10]);

    // shared_ptr 不提供 operator[]，此处编译错误
    //cout << "pNico[0]:" << pNico[0] << endl;


    return 0;
}
