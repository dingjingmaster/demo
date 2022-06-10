#include "dialog.h"

#include <QFileDialog>
#include <QPushButton>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    mps = new QPushButton(this);

    mps->setGeometry(0, 0, 100, 60);

    mps->setText("ssssssssssss");

    connect(mps, &QPushButton::clicked, this, [=] () {
        QFileDialog dl;
        dl.exec();
    });

    mps->show();
}

Dialog::~Dialog()
{
}

