#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QList>
#include <QMutex>
#include <QObject>
#include <QStringList>
#include <QStandardItemModel>

class TableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject* parent=nullptr);

    int getCurrentRow();
    void updateViewData();

public Q_SLOT:
    void onLoadData();
    void onReceiverChanged(int row);
    void onVerscrollBar(double value);

public:
    QList<QString>          mData;
    QMutex                  mDataLocker;

    int                     mCurrentRow = 0;
    int                     mTotalRows = 100;
};

#endif // TABLEMODEL_H
