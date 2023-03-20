#include <QApplication>
#include <QMetaEnum>
#include <QDebug>
#include <QTimer>
#include <QPalette>
#include <qglobal.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto p = a.palette();

    auto pGI = p.staticMetaObject.indexOfEnumerator("ColorGroup");
    auto pRI = p.staticMetaObject.indexOfEnumerator("ColorRole");

    QMetaEnum pGE = p.staticMetaObject.enumerator(pGI);
    QMetaEnum pRE = p.staticMetaObject.enumerator(pRI);

//    for (auto k = 0; k < pGE.keyCount(); ++k) {
//        qDebug() << pGE.key(k);
//    }

//    for (auto k = 0; k < pRE.keyCount(); ++k) {
//        qDebug() << pRE.key(k);
//    }

    for (auto i = 0; i < pGE.keyCount(); ++i) {
        const char* kI = pGE.key(i);
        int kIV = pGE.value(i);
        for (auto j = 0; j < pRE.keyCount(); ++j) {
            const char* kJ = pRE.key(j);
            int kJV = pRE.value(i);
            auto color = p.brush((QPalette::ColorGroup)kIV, (QPalette::ColorRole)kJV).color();


            qDebug() << kI << " " << kJ << ": " << color;
        }
    }

    qDebug() << p;

//    QTimer::singleShot(200, &a, SLOT(quit()));

    a.exec();

    return 0;
}
