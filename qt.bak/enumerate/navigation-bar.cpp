#include "navigation-bar.h"

NavigationBar::NavigationBar(QWidget *parent) : QToolBar(parent)
{
    m_goBack = addAction("Go Back", this, &NavigationBar::goBackRequest);
    m_goForward = addAction("Go Forward", this, &NavigationBar::goForwardRequest);
    m_cdUp = addAction("Cd Up", this, &NavigationBar::cdUpRequest);
    m_refresh = addAction("Refresh", this, &NavigationBar::refreshRequest);

    setCanGoBack(false);
    setCanGoForward(false);
    setCanCdUp(false);
}

void NavigationBar::setCanGoBack(bool can)
{
    m_goBack->setEnabled(can);
}

void NavigationBar::setCanGoForward(bool can)
{
    m_goForward->setEnabled(can);
}

void NavigationBar::setCanCdUp(bool can)
{
    m_cdUp->setEnabled(can);
}
