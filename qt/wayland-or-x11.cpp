/*************************************************************************
> FileName: wayland-or-x11.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月27日 星期日 16时39分18秒
 ************************************************************************/
#include <QGuiApplication>

#include <QDebug>

int main (int argc, char* argv[])
{
	QGuiApplication app(argc, argv);
	qDebug() << "Platform: " << QGuiApplication::platformName();

	return 0;
}
