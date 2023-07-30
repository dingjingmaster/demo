#include <QDebug>
#include <QWindow>
#include <QX11Info>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>

extern "C" {
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <gdk/gdkx.h>
}

// 失败！！！

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);
    QApplication app(argc, argv);

    qDebug() << "-1";
    QWidget* mMainWidget = new QWidget;
    qDebug() << "0";
    QWidget* container = QWidget::createWindowContainer (mMainWidget->windowHandle());
    container->setBackgroundRole(QPalette::Dark);

    qDebug() << "1";


    GtkWidget* widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget* button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(widget), button);
    gtk_widget_show_all(widget);

    qDebug() << "2";
    QWidget* gtkWindow = QWidget::find(GDK_WINDOW_XID(gtk_widget_get_window(widget)));

    gtkWindow->show();

//    qDebug() << "3";
//    mMainWidget->layout()->addWidget(gtkWindow);
//    mMainWidget->show();
    qDebug() << "4";






//    gtk_init(&argc, NULL);
//    GtkWidget* widget = gtk_window_new( GTK_WINDOW_TOPLEVEL );
//    GtkWidget* socket = gtk_socket_new();
//    gtk_widget_show( socket );
//    gtk_container_add( GTK_CONTAINER ( widget ), socket );
//    gtk_widget_realize( socket );


//    QWidget* qwidget = new QWidget;
//    qwidget->setLayout(new QVBoxLayout);
//    QPushButton* ps = new QPushButton;
//    ps->setText("QT按钮");
//    qwidget->layout()->addWidget(ps);
//    qwidget->connect(ps, &QPushButton::clicked, [=] () {
//        qDebug() << "按钮按下";
//    });

//    qDeleteAll (qwidget->layout()->children());
//    gtk_socket_add_id(GTK_SOCKET(socket), qwidget->winId());
//    gtk_widget_show(widget);
//    gtk_main();
    app.exec();


    return 0;
}
