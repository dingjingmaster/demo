//
// Created by dingjing on 23-6-1.
//

#include "hook-qstyle.h"

#include <stdio.h>

#include <QDebug>
#include <QWidget>
#include <QAction>
#include <QPushButton>


QStyle *HookPlugin::create(const QString &key)
{
    return new HookStyle("chameleon");
    //return new HookStyle("fusion");
}

HookPlugin::HookPlugin(QObject *parent)
    : QStylePlugin (parent)
{

}

HookStyle::HookStyle(const QString &key)
    : QProxyStyle (key)
{
    printf ("Hook style OK!\n");
}

void HookStyle::polish(QWidget *w)
{
    qDebug() << w->metaObject()->className();
    qWarning() << w->metaObject()->className();
    printf ("windows name: %s\n", w->metaObject()->className());

    if (QString(w->metaObject()->className()) == "QMenu") {
        printf ("found %s======\n", w->metaObject()->className());
        auto ls = w->children();
        for (auto l : ls) {
            if (auto a = qobject_cast<QAction*>(l)) {
                printf ("action: %s, icon text: %s\n", a->text().toUtf8().constData(), a->iconText().toUtf8().constData());
                if ((a->text() == "File(&F)") || (a->text().contains ("文件(&F)")) || (a->iconText().contains("文件(F)"))) {
                    printf ("======>文件Action：开始禁用!\n");
                    a->setEnabled (false);
                    //a->setVisible (false);
                    //w->removeAction (a);
                }
            }
        }
    }
    else if (QString(w->metaObject()->className()) == "BrowserMenu") {
        printf ("found BrowserMenu=======\n");
        auto ls = w->children();
        for (auto l : ls) {
            if (auto a = qobject_cast<QAction*>(l)) {
                printf ("action: %s, icon text: %s\n", a->text().toUtf8().constData(), a->iconText().toUtf8().constData());
                if ((a->text() == "Save As") || (a->text().contains ("另存为")) || (a->iconText().contains ("另存为"))) {
                    printf ("Save As .........\n");
                    a->setEnabled (false);
                    a->setVisible (false);
                    w->removeAction (a);
                }
                else if ((a->text() == "Print") || (a->text().contains ("打印"))) {
                    a->setEnabled (false);
                    a->setVisible (false);
                    w->removeAction (a);
                }
            }
        }
    }
    else if (QString(w->metaObject()->className()) == "Dtk::Widget::DDialog") {
        printf ("found DDialog========\n");
        auto ls = w->children();
        for (auto l : ls) {
            if (auto a = qobject_cast<QPushButton*>(l)) {
                printf ("text: %s\n", a->text().toUtf8().constData());
                if ((a->text().contains ("保 存"))) {
                    printf ("Save As .........\n");
                    a->setEnabled (false);
                    a->setVisible (false);
//                    w->removeAction (a);
                }
            }
        }
    }

    QProxyStyle::polish (w);
}
