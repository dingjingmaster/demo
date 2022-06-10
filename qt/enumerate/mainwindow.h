#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace Peony {
class DirectoryViewContainer;
}

class SideBar;
class PathBar; // location change
class ToolBar; // view switch
class NavigationBar; // go back/forward

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getCurrentUri();

public Q_SLOTS:
    void goToUri(const QString &uri, bool addToHistory = true);
    void goBack();
    void goForward();
    void cdUp();
    void refresh();

    void switchView(const QString &viewId);

    void updateWindowState();

    void updateStatusBar();

private:
    Ui::MainWindow *ui;

    SideBar *m_sideBar;
    PathBar *m_pathBar;
    NavigationBar *m_navigationBar;
    ToolBar *m_toolBar;

    Peony::DirectoryViewContainer *m_container;
};
#endif // MAINWINDOW_H
