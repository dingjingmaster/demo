#include "table-model.h"

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    return Qt::NoItemFlags;
}

QModelIndex TableModel::sibling(int row, int column, const QModelIndex &idx) const
{
    return QModelIndex();
}

QModelIndex TableModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return 1000000;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole != role) {
        return QVariant();
    }

    return QVariant (QString("%1 - %2").arg(index.row()).arg (index.column ()));
}
