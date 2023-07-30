#include <QDir>
#include <QDebug>
#include <QFileDialog>

int main(void)
{
    QFileDialog dlg;


    QDir dir("/home/dingjing/test`");

    QStringList dirCtx = dir.entryList();

    qDebug() << dir.exists();
    qDebug() << dirCtx;
}
