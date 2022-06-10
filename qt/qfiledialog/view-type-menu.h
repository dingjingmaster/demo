#ifndef VIEWTYPEMENU_H
#define VIEWTYPEMENU_H
#include <QMenu>

namespace Peony {
class ViewFactorySortFilterModel2;
}

class ViewTypeMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ViewTypeMenu(QWidget *parent = nullptr);

Q_SIGNALS:
    void switchViewRequest(const QString &viewId, const QIcon &icon, bool resetToZoomLevelHint = false);
    void updateZoomLevelHintRequest(int zoomLevelHint);

public Q_SLOTS:
    void setCurrentView(const QString &viewId, bool blockSignal = false);
    void setCurrentDirectory(const QString &uri);

protected:
    bool isViewIdValid(const QString &viewId);
    void updateMenuActions();

private:
    QString m_current_uri;

    QString m_current_view_id;
    Peony::ViewFactorySortFilterModel2 *m_model;

    QActionGroup *m_view_actions;
};


#endif // VIEWTYPEMENU_H
