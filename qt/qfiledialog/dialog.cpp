#include "dialog.h"

#include "path-bar.h"
#include "tool-bar.h"
#include "side-bar.h"
#include "navigation-bar.h"
#include "path-bar.h"
#include "view-type-menu.h"

#include <QUrl>
#include <QLayout>
#include <QDockWidget>
#include <PeonyFileInfo>
#include <QDesktopServices>
#include <QMenuBar>

#include <peony-qt/controls/directory-view/directory-view-widget.h>
#include <peony-qt/controls/directory-view/directory-view-container.h>
#include <peony-qt/controls/menu/directory-view-menu/directory-view-menu.h>

Dialog::Dialog(QWidget *parent) : QMainWindow(parent)
{
//    new QGridLayout(this);
    layout()->setSpacing(0);
    layout()->setContentsMargins(0, 0, 0, 0);

    mContainer = new Peony::DirectoryViewContainer;
    mContainer->switchViewType("Icon View");
    mContainer->goToUri("file:///", false);

    mNavigationBar = new NavigationBar;
    mNavLayout = new QHBoxLayout;
    mMainContent = new QHBoxLayout;
    mSideBar = new SideBar;
    mFilterBar = new FilterBar;
    mPathBar = new PathBar;
    mViewTypeMenu = new ViewTypeMenu;

    QToolBar* menuBar = new QToolBar;
    QMenuBar* mb = new QMenuBar;

    mViewTypeMenu->setBaseSize(QSize(200,200));
    mSideBar->setFixedWidth(180);
    mNavigationBar->setFixedHeight(50);

    mMainContent->addWidget(mSideBar);
    mMainContent->addWidget(mContainer);
    mContainer->setContentsMargins(0, 0, 0, 0);

    mNavLayout->addWidget(mNavigationBar);
    mNavLayout->addWidget(mPathBar);

    mb->addMenu(mViewTypeMenu);
//    mNavLayout->addWidget(menu);

    mViewTypeMenu->setBaseSize(QSize(200, 20));


    menuBar->addWidget(mb);


    addToolBarBreak();
    menuBar->addWidget(mPathBar);
    menuBar->addWidget(mViewTypeMenu);
    addToolBar(menuBar);


//    qobject_cast<QGridLayout*>(layout())->addLayout(mNavLayout, 0, 0, 1, 1);
//    qobject_cast<QGridLayout*>(layout())->addWidget(menuBar, 0, 1, 1, 1);
//    qobject_cast<QGridLayout*>(layout())->addLayout(mMainContent, 1, 0, 1, 1);
//    qobject_cast<QGridLayout*>(layout())->addWidget(mFilterBar, 2, 0, 1, 1);


//    connect(menuBar, &ToolBar::switchViewRequest, mContainer, &Peony::DirectoryViewContainer::switchViewType);
}

Dialog::~Dialog()
{
}

void Dialog::cdUp()
{

}

void Dialog::goBack()
{

}

void Dialog::refresh()
{

}

void Dialog::goForward()
{

}

void Dialog::updateStatusBar()
{

}

void Dialog::updateWindowState()
{

}

void Dialog::switchView(const QString &viewId)
{

}

void Dialog::goToUri(const QString &uri, bool addToHistory)
{

}

