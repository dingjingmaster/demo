/*************************************************************************
> FileName: hotkey.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月23日 星期三 16时33分57秒
 ************************************************************************/
#include <graceful/QHotkey>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	auto hotkey = new QHotkey(QKeySequence("ctrl+alt+t"), true, &a);
	qDebug() << "Is Registered: " << hotkey->isRegistered();

	QObject::connect(hotkey, &QHotkey::activated, qApp, [&](){
		qDebug() << "Hotkey Activated";
	});

	return a.exec();
}
