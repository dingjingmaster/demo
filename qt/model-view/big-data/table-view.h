#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QWidget>
#include <qglobal.h>

class TableView : public QTableView
{
    Q_OBJECT
public:
    explicit TableView(QWidget *parent = nullptr);

signals:

};

#endif // TABLEVIEW_H
