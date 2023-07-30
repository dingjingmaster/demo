#ifndef DIALOG_H
#define DIALOG_H

#include "filter-bar.h"
#include "location-bar.h"
#include "view-type-menu.h"

#include <QDialog>
#include <QDockWidget>
#include <QLayout>
#include <QMainWindow>

namespace Peony
{
class DirectoryViewContainer;
}

class SideBar;
class PathBar;
class ToolBar;
class NavigationBar;


class Dialog : public QMainWindow
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    QString getCurrentUri();

public Q_SLOTS:
    void cdUp();
    void goBack();
    void refresh();
    void goForward();
    void updateStatusBar();
    void updateWindowState();
    void switchView(const QString &viewId);
    void goToUri(const QString &uri, bool addToHistory = true);

private:
    SideBar*                            mSideBar;
    PathBar*                            mPathBar;
    ToolBar*                            mToolBar;
    FilterBar*                          mFilterBar;
    NavigationBar*                      mNavigationBar;
    Peony::DirectoryViewContainer*      mContainer;
    ViewTypeMenu*                       mViewTypeMenu;
    QLayout*                            mNavLayout;
    QLayout*                            mMainContent;
};
#endif // DIALOG_H
