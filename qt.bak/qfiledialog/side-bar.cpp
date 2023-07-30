#include "side-bar.h"

#include <QTimer>
#include <side-bar-model.h>
#include <PeonySideBarModel>
#include <PeonySideBarProxyModel>

SideBar::SideBar(QWidget *parent) : QTreeView(parent)
{
    auto model = new Peony::SideBarModel(this);
    auto proxyModel = new Peony::SideBarProxyFilterSortModel(this);
    proxyModel->setSourceModel(model);
    setModel(proxyModel);

    setAttribute(Qt::WA_TranslucentBackground);

    connect(this, &SideBar::clicked, this, [=](const QModelIndex &index){
        auto item = proxyModel->itemFromIndex(index);
        auto uri = item->uri();
        if (!uri.isEmpty())
            Q_EMIT goToUriRequest(uri);
    });

    connect(this, &SideBar::expanded, this, [=](const QModelIndex &index){
        auto item = proxyModel->itemFromIndex(index);
        item->findChildrenAsync();
    });

    connect(this, &SideBar::collapsed, this, [=](const QModelIndex &index){
        auto item = proxyModel->itemFromIndex(index);
        item->clearChildren();
    });

    expandAll();
}
