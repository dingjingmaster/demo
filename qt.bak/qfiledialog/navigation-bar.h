#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include "location-bar.h"
#include "path-bar.h"

#include <QToolBar>

class QAction;
namespace Peony {
class NavigationToolBar;
}

class NavigationBar : public QToolBar
{
    Q_OBJECT
public:
    explicit NavigationBar(QWidget *parent = nullptr);

Q_SIGNALS:
    void goBackRequest();
    void goForwardRequest();
    void cdUpRequest();
    void refreshRequest();

public Q_SLOTS:
    void onGoBack();
    void onGoForward();

    void setCanGoBack(bool can);
    void setCanGoForward(bool can);

private:
    QAction*                        mGoBack;
    QAction*                        mGoForward;
//    QAction*                        mCdUp;
//    QAction*                        mRefresh;
    PathBar*                        mPathBar;

    Peony::NavigationToolBar*       mNavLeft;
};
#endif // NAVIGATIONBAR_H
