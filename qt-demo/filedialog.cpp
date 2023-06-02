/*************************************************************************
> FileName: filedialog.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 31 May 2023 02:45:46 PM CST
 ************************************************************************/
#include <QFileDialog>
#include <QApplication>

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    QFileDialog dlg;

    dlg.exec();

    return 0;
}
