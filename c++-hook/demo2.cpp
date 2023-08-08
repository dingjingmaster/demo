/*************************************************************************
> FileName: demo2.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 04 Aug 2023 02:43:54 PM CST
 ************************************************************************/
#include <QApplication>
#include <DFileDialog>


int main(int argc, char* argv[]) 
{
    QApplication a(argc, argv);
    Dtk::Widget::DFileDialog fd(nullptr);

    printf ("%d\n", sizeof(fd));

    return 0;
}
