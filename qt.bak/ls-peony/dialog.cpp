#include "dialog.h"

#include <QTimer>

qint64 PeonyApplication::peony_start_time = 0;

PeonyApplication::PeonyApplication(int &argc, char *argv[], const char *applicationName) : SingleApplication (argc, argv, applicationName, true)
{
    setApplicationVersion(QString("v%1").arg("1"));
    setApplicationName("peony-qt");

//    QFile file(":/data/libpeony-qt-styled.qss");
//    file.open(QFile::ReadOnly);
//    setStyleSheet(QString::fromLatin1(file.readAll()));
//    //qDebug()<<file.readAll();
//    file.close();

//    QTranslator *t = new QTranslator(this);
//    qDebug()<<"\n\n\n\n\n\n\ntranslate:"<<t->load("/usr/share/libpeony-qt/libpeony-qt_"+QLocale::system().name());
//    QApplication::installTranslator(t);
//    QTranslator *t2 = new QTranslator(this);
//    t2->load("/usr/share/peony-qt/peony-qt_"+QLocale::system().name());
//    QApplication::installTranslator(t2);
//    QTranslator *t3 = new QTranslator(this);
//    t3->load("/usr/share/qt5/translations/qt_"+QLocale::system().name());
//    QApplication::installTranslator(t3);
//    setStyle(Peony::ComplementaryStyle::getStyle());

    parser.addOption(quitOption);
    parser.addOption(showItemsOption);
    parser.addOption(showFoldersOption);
    parser.addOption(showPropertiesOption);

    parser.addPositionalArgument("files", tr("Files or directories to open"), tr("[FILE1, FILE2,...]"));

//    //unmount all ftp node when close all window
//    connect(qApp, &QApplication::lastWindowClosed, this, &PeonyApplication::unmountAllFtpLinks);

    if (this->isSecondary()) {
        parser.addHelpOption();
        parser.addVersionOption();
        if (this->arguments().count() == 2 && arguments().last() == ".") {
            QStringList args;

            qDebug() << "second window";

            auto message = args.join(' ').toUtf8();
            sendMessage(message);
            return;
        }
        parser.process(arguments());
        auto message = this->arguments().join(' ').toUtf8();
        sendMessage(message);
        return;
    }

    if (this->isPrimary()) {
        qDebug() << "firstt window";
        connect(this, &SingleApplication::receivedMessage, this, &PeonyApplication::parseCmd);
    }

//    //parse cmd
    auto message = this->arguments().join(' ').toUtf8();
    parseCmd(this->instanceId(), message);
}


void PeonyApplication::parseCmd(quint32 id, QByteArray msg)
{
    QCommandLineParser parser;
    if (m_first_parse) {
        parser.addHelpOption();
        parser.addVersionOption();
        m_first_parse = false;
    }
    parser.addOption(quitOption);
    parser.addOption(showItemsOption);
    parser.addOption(showFoldersOption);
    parser.addOption(showPropertiesOption);

    const QStringList args = QString(msg).split(' ');

    parser.process(args);
    if (parser.isSet(quitOption)) {
        QTimer::singleShot(1, [=]() {
            qApp->quit();
        });
        return;
    }

    if (!parser.optionNames().isEmpty()) {
        if (parser.isSet(showItemsOption)) {
            qDebug() << "show item option";
        }

        if (parser.isSet(showFoldersOption)) {
            qDebug() << "show foldersOption";
        }
        if (parser.isSet(showPropertiesOption)) {

            qDebug() << "show properties";
        }
    } else {
    }
}

