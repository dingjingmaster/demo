#include "view-type-menu.h"

#include "peony-qt/controls/tool-bar/view-factory-sort-filter-model.h"
#include "peony-qt/controls/directory-view/directory-view-factory/directory-view-factory-manager.h"
#include "directory-view-plugin-iface2.h"

#include <QActionGroup>

#include <QDebug>

ViewTypeMenu::ViewTypeMenu(QWidget *parent) : QMenu(parent)
{
    m_model = new Peony::ViewFactorySortFilterModel2(this);
    m_view_actions = new QActionGroup(this);
    m_view_actions->setExclusive(true);

    connect(m_view_actions, &QActionGroup::triggered, this, [=](QAction *action) {
        auto viewId = action->data().toString();
        setCurrentView(viewId);
    });

    connect(this, &QMenu::aboutToShow, this, [=]() {
        qDebug()<<"show menu";
        updateMenuActions();
    });

    setCurrentDirectory("file:///");
}

void ViewTypeMenu::setCurrentView(const QString &viewId, bool blockSignal)
{
    if (viewId == m_current_view_id)
        return;

    if (isViewIdValid(viewId)) {
        m_current_view_id = viewId;
    }

    for (auto action : m_view_actions->actions()) {
        if (action->text() == viewId) {
            action->setChecked(true);
        }
    }


    Q_EMIT this->switchViewRequest(viewId, m_model->iconFromViewId(viewId));

    if (!blockSignal) {
        auto factoryManager = Peony::DirectoryViewFactoryManager2::getInstance();
        auto factory = factoryManager->getFactory(viewId);
        int zoomLevelHint = factory->zoom_level_hint();
        Q_EMIT this->updateZoomLevelHintRequest(zoomLevelHint);
    }
}

void ViewTypeMenu::setCurrentDirectory(const QString &uri)
{
    m_current_uri = uri;
    m_model->setDirectoryUri(uri);
}

bool ViewTypeMenu::isViewIdValid(const QString &viewId)
{
    return m_model->supportViewIds().contains(viewId);
}

void ViewTypeMenu::updateMenuActions()
{
    auto supportViews = m_model->supportViewIds();
    for (auto action : m_view_actions->actions()) {
        removeAction(action);
        m_view_actions->removeAction(action);
        action->deleteLater();
    }
    for (auto id : supportViews) {
        auto action = new QAction(this);
        auto text = m_model->getViewDisplayNameFromId(id);
        action->setText(text);
        action->setData(id);
        action->setIcon(m_model->iconFromViewId(id));
        m_view_actions->addAction(action);
        addAction(action);

        action->setCheckable(true);
        if (m_current_view_id == id) {
            action->setChecked(true);
        }
    }
}
