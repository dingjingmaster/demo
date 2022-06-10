#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;


private:
    // widget size
    float                       mFixWidth = 550;
    float                       mFixHeight = 192;
    float                       mMarginl = 12;
    float                       mMargint = 6;

    // header bar icon
    float                       mHeaderIconTop = 8;
    float                       mHeaderIconLeft = 16;

    float                       mHeaderBtnSize = 26;
    float                       mHeaderIconSize = 20;

};
#endif // DIALOG_H
