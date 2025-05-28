/*************************************************************************
> FileName: concurrent.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月28日 星期三 16时13分19秒
 ************************************************************************/
#include <QDebug>
#include <QString>
#include <QThread>
#include <QApplication>
#include <QtConcurrent>


using namespace QtConcurrent;


void func1(QString name)
{
    qDebug() << name << "from" << QThread::currentThread();
}


int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    // QtConcurrent::run 函数将 func1 分别运行在两个不同的线程中
    QFuture<void> f1 = run(func1, QString("Thread 1"));
    QFuture<void> f2 = run(func1, QString("Thread 2"));

    f1.waitForFinished();
    f2.waitForFinished();

    // 将线程运行在指定线程池中
    QThreadPool pool;
    QFuture<void> f3 = run(&pool, func1, QString("Thread 3"));
    
    f3.waitForFinished();

    // 常量成员函数, 类的实例引用或指针作为第一个参数传入，
    // 常量成员函数一百年传递常量引用
    QByteArray ba = "Hello, world!";
    QFuture<QList<QByteArray>> f4 = run(ba, &QByteArray::split, ',');
    f4.waitForFinished();
    qInfo() << "F4: " << f4.result();

    QFuture<QList<QByteArray>> f5 = run(&ba, &QByteArray::split, ',');
    f5.waitForFinished();
    qInfo() << "F5: " << f5.result();

    return 0;
}
