#include "navigation-bar.h"

#include "peony-qt/controls/navigation-bar/navigation-tool-bar.h"

NavigationBar::NavigationBar(QWidget *parent) : QToolBar(parent)
{
    setMovable(false);
    setFloatable(false);
    setContentsMargins(0, 0, 0, 0);

    mGoBack = addAction(QIcon::fromTheme("go-previous"), tr("Go Back"), [=]() {
        this->onGoBack();
    });

    mGoForward = addAction(QIcon::fromTheme("go-next"), tr("Go Forward"), [=]() {
        this->onGoForward();
    });



//    mCdUp = addAction("Cd Up", this, &NavigationBar::cdUpRequest);
//    mRefresh = adkkdAction("Refresh", this, &NavigationBar::refreshRequest);

    setCanGoBack(false);
    setCanGoForward(false);
}

void NavigationBar::onGoBack()
{
}

void NavigationBar::onGoForward()
{

}

void NavigationBar::setCanGoBack(bool can)
{
    mGoBack->setEnabled(can);
}

void NavigationBar::setCanGoForward(bool can)
{
    mGoForward->setEnabled(can);
}
