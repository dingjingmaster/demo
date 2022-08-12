#include "main-window.h"

#include "table-view.h"
#include "table-model.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mModel = new TableModel;
    mView = new TableView;

    mView->setModel (mModel);

    setCentralWidget (mView);
}

MainWindow::~MainWindow()
{
}

