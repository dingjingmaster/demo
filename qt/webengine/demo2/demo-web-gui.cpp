/*************************************************************************
> FileName: demo-web-gui.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 07 Aug 2024 02:53:17 PM CST
 ************************************************************************/
#include <QApplication>

#include "web-gui.h"

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    WebGui* gui = WebGui::getInstance();

    gui->loadPage(WebGui::WP_WORKBENCH);
    gui->show();

    return app.exec();
}
