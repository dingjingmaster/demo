#include <QCoreApplication>

#include "log.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    log_add_fp(fopen("/home/dingjing/.local/log/test.log", "w+"), LOG_INFO);
    log_trace("Hello %s", "world");

    return a.exec();
}
