#include <QApplication>
#include <QMessageBox>
#include <qglobal.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMessageBox box;
    box.exec();
    return a.exec();
}
