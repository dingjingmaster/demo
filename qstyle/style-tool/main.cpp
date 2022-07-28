#include "style.h"

#include <QDebug>
#include <QWidget>
#include <qglobal.h>
#include <QComboBox>
#include <QHBoxLayout>
#include <QApplication>


void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageOutput);
    
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    #endif
    
    QApplication a(argc, argv);
    a.setStyle (new Style);
    
    QWidget main;
    QHBoxLayout mainLayout;
    
    
    QComboBox comboBox;
    comboBox.addItem("item1");
    comboBox.addItem("item2");
    
    mainLayout.addWidget(&comboBox);
    
    
    main.setLayout(&mainLayout);
    
    main.show();
    
    return a.exec();
}


void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type)
    
    QByteArray localMsg = msg.toLocal8Bit();

    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    
    fprintf(stdout, "\033[32m[%s - %s:%d]\033[39m %s\n", file, function, context.line, localMsg.constData());
}
