/*************************************************************************
> FileName: ss.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 24 Mar 2023 10:53:31 AM CST
 ************************************************************************/
#include <QApplication>
#include <QLabel>
#include <QMessageBox>
#include "server-header.h"
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QLabel       text("teststts");
    Server       s;
    if (!s.init("localserver-test")){
        // 初使化失败, 说明已经有一个在运行了
        QMessageBox::information(&text, "info", "There is already exist one!");
        return 1;
    }
    QObject::connect(&s, SIGNAL(newMessage(const QString &)),&text, SLOT(setText(const QString &)));
    text.show();
    return app.exec();
}
