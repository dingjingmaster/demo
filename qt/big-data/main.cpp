#include "table-model.h"
#include "table-view.h"

#include <QThread>
#include <QWidget>
#include <qglobal.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TableView tw;
    TableModel tm;

    tw.connect (&tw, &TableView::rowchanged, &tm, &TableModel::onReceiverChanged);
    tw.connect (&tw, &TableView::verticalbarValue, &tm, &TableModel::onVerscrollBar);

    tw.setModel (&tm);

#if 0
    QThread thread;
    tm.moveToThread (&thread);
    tw.connect (&thread, &QThread::started, &tm, &TableModel::onLoadData);
    thread.start ();
#else
    tm.onLoadData();

#endif

    tw.show ();


    return a.exec();
}
