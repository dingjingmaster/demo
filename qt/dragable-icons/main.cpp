#include "drag-widget.h"

#include <QApplication>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget mainWidget;
    QHBoxLayout *horizontalLayout = new QHBoxLayout(&mainWidget);
    horizontalLayout->addWidget(new DragWidget);
    horizontalLayout->addWidget(new DragWidget);

    mainWidget.setWindowTitle(QObject::tr("Draggable Icons"));
    mainWidget.show();

    return app.exec();
}
