#include "combox-style.h"

#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>

#include <QComboBox>
#include <QHBoxLayout>
#include <QWidget>
#else
#include <QtGui/QApplication>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    //a.setStyle (new ComboxStyle());
    
    QWidget w;
    QHBoxLayout layout;
    
    QComboBox cb;
    cb.setStyle (new ComboxStyle());
    cb.addItem ("aaa");
    cb.addItem ("bbb");
    cb.addItem ("ccc");
    cb.addItem ("ddd");
    cb.addItem ("eee");
    cb.addItem ("fff");
    layout.addWidget (&cb);
    
    w.setLayout (&layout);
    w.show();
    
    return a.exec();
}
