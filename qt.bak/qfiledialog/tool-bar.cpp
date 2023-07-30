#include "tool-bar.h"

ToolBar::ToolBar(QWidget *parent) : QToolBar(parent)
{
    mIconView = addAction("Icon View", this, [=](){
        Q_EMIT this->switchViewRequest("Icon View");
    });
    mListView = addAction("List View", this, [=](){
        Q_EMIT this->switchViewRequest("List View");
    });

    mIconView->setCheckable(true);
    mListView->setCheckable(true);
}

void ToolBar::updateViewType(const QString &viewId)
{
    if (viewId == "List View") {
        mIconView->setChecked(false);
        mListView->setChecked(true);
    } else {
        mIconView->setChecked(true);
        mListView->setChecked(false);
    }
}
