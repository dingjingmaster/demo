#include "table-model.h"

#include <QFile>
#include <QDebug>
#include <QIODevice>
#include <QTextStream>

void TableModel::onLoadData()
{
    for (int i = 0; i < 100000; ++i) {
        mDataLocker.lock ();
        mData.append(QString("i:%1").arg (i));
        mDataLocker.unlock ();

        if (i < mTotalRows) {
            this->setItem(i, 0, new QStandardItem(QString("%1 - 0").arg (i)));
            this->setItem(i, 1, new QStandardItem(QString("%1 - 1").arg (i)));
            this->setItem(i, 2, new QStandardItem(QString("%1 - 2").arg (i)));
            this->setItem(i, 3, new QStandardItem(QString("%1 - 3").arg (i)));
            this->setItem(i, 4, new QStandardItem(QString("%1 - 4").arg (i)));
            this->setItem(i, 5, new QStandardItem(QString("%1 - 5").arg (i)));
            this->setItem(i, 6, new QStandardItem(QString("%1 - 6").arg (i)));
            this->setItem(i, 7, new QStandardItem(QString("%1 - 7").arg (i)));
        }
    }
    mCurrentRow = 0;
    qDebug()<<"Data Loading complete!"<<"\n";
}

void TableModel::onVerscrollBar(double value)
{
    mDataLocker.lock ();
    int dataSize = mData.size ();
    mCurrentRow = value * dataSize;
    mDataLocker.unlock ();

    if (mCurrentRow > dataSize - 2) {
        mCurrentRow = dataSize - 2;
    }

    updateViewData();
}
int TableModel::getCurrentRow()
{
    return mCurrentRow;
}
void TableModel::onReceiverChanged(int row)
{
    mCurrentRow += row;

    mDataLocker.lock ();
    int dataSize = mData.size ();
    mDataLocker.unlock ();

    if (mCurrentRow < 0 || mCurrentRow > dataSize - 2) {
        mCurrentRow -= row;
        return;
    }
    updateViewData();

}
void TableModel::updateViewData()
{
    removeRows (0, rowCount());
    setRowCount (mTotalRows);

    int k = 0;
    for(int i = mCurrentRow; i < mCurrentRow + mTotalRows; i++) {
        mDataLocker.lock ();
        if (i == mData.size()) {
            mDataLocker.unlock();
            break;
        }
        QString ls = mData[i];
        mDataLocker.unlock ();

        this->setItem(k, 0, new QStandardItem(ls));
        this->setItem(k, 1, new QStandardItem(ls));
        this->setItem(k, 2, new QStandardItem(ls));
        this->setItem(k, 3, new QStandardItem(ls));
        this->setItem(k, 4, new QStandardItem(ls));
        this->setItem(k, 5, new QStandardItem(ls));
        this->setItem(k, 6, new QStandardItem(ls));
        this->setItem(k, 7, new QStandardItem(ls));

        ++k;
    }
    qDebug()<<"CurrentRow::"<<mCurrentRow<<"\n";
}

TableModel::TableModel(QObject *parent)
    : QStandardItemModel(parent)
{

}
