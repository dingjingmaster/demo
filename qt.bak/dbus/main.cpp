#include <QDebug>
#include <QCoreApplication>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusReply>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDBusInterface interface("com.kylin.softwarecenter.getsearchresults",
                             "/com/kylin/softwarecenter/getsearchresults",
                             "com.kylin.getsearchresults",
                             QDBusConnection::sessionBus());

    // Calls DBus method
    QDBusReply<QList<QString>> result = interface.call("get_search_result", "文件的 url");
    if (result.isValid()) {
        for (auto d : result.value()) {
            qDebug() << d;
        }
    } else {
        qDebug() << "reply not invalid!!!" << result.error();
    }


    return a.exec();
}
