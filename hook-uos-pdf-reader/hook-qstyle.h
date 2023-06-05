//
// Created by dingjing on 23-6-1.
//

#ifndef QSTYLE_HOOK_STYLE_H
#define QSTYLE_HOOK_STYLE_H
#include <QProxyStyle>
#include <QStylePlugin>

class HookStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit HookStyle (const QString &key);

    void polish (QWidget* w) override;
};

class HookPlugin : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "hookstyleplugin.json")
public:
    explicit HookPlugin(QObject *parent = 0);

    QStyle *create(const QString &key) override;

};

#endif //QSTYLE_HOOK_STYLE_H
