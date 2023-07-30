#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "path-bar.h"
#include "navigation-bar.h"
#include "tool-bar.h"
#include "side-bar.h"

#include <QDockWidget>

#include <peony-qt/controls/directory-view/directory-view-container.h>
#include <peony-qt/controls/menu/directory-view-menu/directory-view-menu.h>

#include <peony-qt/controls/directory-view/directory-view-widget.h>

#include <QDesktopServices>
#include <QUrl>

#include <PeonyFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // view
    m_container = new Peony::DirectoryViewContainer(this);
    m_container->switchViewType("Icon View");
    m_container->goToUri("file:///", false);
    setCentralWidget(m_container);
    m_container->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_container, &Peony::DirectoryViewContainer::directoryChanged, this, &MainWindow::updateWindowState);
    connect(m_container, &Peony::DirectoryViewContainer::menuRequest, this, [=](){
        Peony::DirectoryViewMenu menu(m_container->getView());
        menu.exec(QCursor::pos());
    });
    connect(m_container, &Peony::DirectoryViewContainer::viewDoubleClicked, this, [=](const QString &uri){
        auto info = Peony::FileInfo::fromUri(uri);
        if (info->isDir() || info->isVolume()) {
            goToUri(uri);
        } else {
            QDesktopServices::openUrl(QUrl(uri));
        }
    });
    connect(m_container, &Peony::DirectoryViewContainer::viewTypeChanged, this, [=](){
        m_toolBar->updateViewType(m_container->getView()->viewId());
    });
    connect(m_container, &Peony::DirectoryViewContainer::updateWindowLocationRequest, this, [=](const QString &uri, bool addToHistory){
        goToUri(uri, addToHistory);
    });

    connect(m_container, &Peony::DirectoryViewContainer::selectionChanged, this, &MainWindow::updateStatusBar);

    // navigation bar
    m_navigationBar = new NavigationBar(this);
    addToolBar(m_navigationBar);

    connect(m_navigationBar, &NavigationBar::goBackRequest, this, &MainWindow::goBack);
    connect(m_navigationBar, &NavigationBar::goForwardRequest, this, &MainWindow::goForward);
    connect(m_navigationBar, &NavigationBar::cdUpRequest, this, &MainWindow::cdUp);
    connect(m_navigationBar, &NavigationBar::refreshRequest, this, &MainWindow::refresh);

    // tool bar
    m_toolBar = new ToolBar(this);
    addToolBar(m_toolBar);

    connect(m_toolBar, &ToolBar::switchViewRequest, m_container, &Peony::DirectoryViewContainer::switchViewType);

    // path bar
    m_pathBar = new PathBar(this);

    addToolBarBreak();
    auto t1 = new QToolBar(this);
    t1->addWidget(m_pathBar);

    addToolBar(t1);

    connect(m_pathBar, &PathBar::goToUriRequest, this, &MainWindow::goToUri);

    // side bar
    m_sideBar = new SideBar(this);
    QDockWidget *dock = new QDockWidget(this);

    dock->setWidget(m_sideBar);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    connect(m_sideBar, &SideBar::goToUriRequest, this, &MainWindow::goToUri);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getCurrentUri()
{
    return m_container->getCurrentUri();
}

void MainWindow::goToUri(const QString &uri, bool addToHistory)
{
    m_container->goToUri(uri, addToHistory);
}

void MainWindow::goBack()
{
    m_container->goBack();
}

void MainWindow::goForward()
{
    m_container->goForward();
}

void MainWindow::cdUp()
{
    m_container->cdUp();
}

void MainWindow::refresh()
{
    m_container->refresh();
}

void MainWindow::switchView(const QString &viewId)
{
    m_container->switchViewType(viewId);
}

void MainWindow::updateWindowState()
{
    auto uri = getCurrentUri();
    m_pathBar->updatePath(uri);

    m_navigationBar->setCanGoBack(m_container->canGoBack());
    m_navigationBar->setCanGoForward(m_container->canGoForward());
    m_navigationBar->setCanCdUp(m_container->canCdUp());

    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    ui->statusbar->clearMessage();
    auto selections = m_container->getCurrentSelections();
    ui->statusbar->showMessage(selections.isEmpty()? m_container->getCurrentUri(): QString("%1 selected").arg(selections.count()));
}


