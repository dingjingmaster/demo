#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class Dialog : public QWidget //QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
};
#endif // DIALOG_H
