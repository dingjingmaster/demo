#include <QFileDialog>
#include <QApplication>
#include <cstdio>
#include <QDebug>

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    QFileDialog dlg;

    dlg.exec();

    printf("dlg obj: %p\n", &dlg);

    qDebug() << &dlg;

    qInfo() << "selected files: " << dlg.selectedFiles();

    return 0;
}
