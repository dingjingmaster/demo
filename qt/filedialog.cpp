/*************************************************************************
> FileName: filedialog.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 31 May 2023 02:45:46 PM CST
 ************************************************************************/
#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <QApplication>

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    QFileDialog dlg;
    dlg.setFileMode(QFileDialog::DirectoryOnly);
    dlg.setOption(QFileDialog::ShowDirsOnly, true);

    QObject::connect(&dlg, &QFileDialog::filesSelected, [=](const QStringList& selected) ->void {
        qInfo() << selected;
    });

    dlg.exec();

    return 0;
}
