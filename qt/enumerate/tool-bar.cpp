#include "tool-bar.h"

ToolBar::ToolBar(QWidget *parent) : QToolBar(parent)
{
    m_iconView = addAction("Icon View", this, [=](){
        Q_EMIT this->switchViewRequest("Icon View");
    });
    m_listView = addAction("List View", this, [=](){
        Q_EMIT this->switchViewRequest("List View");
    });

    m_iconView->setCheckable(true);
    m_listView->setCheckable(true);
}

void ToolBar::updateViewType(const QString &viewId)
{
    if (viewId == "List View") {
        m_iconView->setChecked(false);
        m_listView->setChecked(true);
    } else {
        m_iconView->setChecked(true);
        m_listView->setChecked(false);
    }
}
