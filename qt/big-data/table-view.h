#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <qglobal.h>
#include <QTableView>

class TableView : public QTableView
{
    Q_OBJECT
public:
    explicit TableView(QWidget *parent = nullptr);
    void wheelEvent(QWheelEvent *event);

public slots:
    void onVerscrollBarChange(int value);

signals:
    void verticalbarValue(double value);
    void rowchanged(int row);

signals:

};

#endif // TABLEVIEW_H
