/*************************************************************************
> FileName: smartptr-mult-thread.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 19 Jun 2023 02:19:37 PM CST
 ************************************************************************/

#include <stdio.h>

#include <memory>
#include <atomic>
#include <vector>
#include <thread>
#include <cstring> 
#include <iostream>


struct/*class*/ ShowThread
{
private:
    std::atomic<bool> m_stop = false;
    std::vector<std::shared_ptr<std::thread>> spthArray;
    void showmsg(std::string msg) {
        while(!m_stop){
            std::cout << msg << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
public:
    ShowThread() {
        for (int i = 0; i < 10; i++) {
            char str[12];
            memset(str, '\0', sizeof(str)); 
            snprintf(str, sizeof(str), "%d", i);
            std::string str_s = str;
            std::shared_ptr<std::thread> spth = std::make_shared<std::thread>(&ShowThread::showmsg, this, str_s);
            spthArray.push_back(spth);
        }
    }
    ~ShowThread() {
        //调用析构函数时，会先执行析构函数内的内容
        m_stop = true;
        for (auto it = spthArray.begin(); it != spthArray.end(); it++) {
            it->get()->join();
        }
        //执行完析构函数内容后，会执行释放智能指针的操作
    }
};
int main()
{
    ShowThread ST;
    char a;
    std::cin >> a;
    std::cout << "Hello World!\n";
}
 
