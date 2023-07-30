#ifndef DIALOG_H
#define DIALOG_H

#include "singleapplication.h"
#include <QCommandLineParser>
#include <QCommandLineOption>

class PeonyApplication : public SingleApplication
{
    Q_OBJECT
public:
    explicit PeonyApplication(int &argc, char *argv[], const char *applicationName = "peony-qt");

    static qint64 peony_start_time;

protected Q_SLOTS:
    void parseCmd(quint32 id, QByteArray msg);

private:
    QCommandLineParser parser;
    QCommandLineOption quitOption = QCommandLineOption(QStringList()<<"q"<<"quit", tr("Close all peony-qt windows and quit"));
    QCommandLineOption showItemsOption = QCommandLineOption(QStringList()<<"i"<<"show-items", tr("Show items"));
    QCommandLineOption showFoldersOption = QCommandLineOption(QStringList()<<"f"<<"show-folders", tr("Show folders"));
    QCommandLineOption showPropertiesOption = QCommandLineOption(QStringList()<<"p"<<"show-properties", tr("Show properties"));

    bool m_first_parse = true;
};

#endif // DIALOG_H
