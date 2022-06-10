#include "location-bar.h"

#include <QUrl>
#include <QMenu>
#include <QPainter>
#include <QLineEdit>
#include <QClipboard>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QToolButton>
#include <QProxyStyle>
#include <QApplication>
#include <QStandardPaths>
#include <QStyleOptionFrame>
#include <QStyleOptionFocusRect>
#include <QStyleOptionToolButton>
#include <file-info.h>
#include <file-utils.h>
#include <file-info-job.h>
#include <file-enumerator.h>
#include <PeonyPathBarModel>

using namespace Peony;

class LocationBarButtonStyle;
static LocationBarButtonStyle *buttonStyle = nullptr;

class LocationBarButtonStyle : public QProxyStyle
{
public:
    explicit LocationBarButtonStyle() : QProxyStyle() {}
    static LocationBarButtonStyle *getStyle () {
        if (!buttonStyle) {
            buttonStyle = new LocationBarButtonStyle;
        }
        return buttonStyle;
    }

    void polish(QWidget *widget) override;
    void unpolish(QWidget *widget) override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

LocationBar::~LocationBar()
{
    mStyledEdit->deleteLater();
}

const QString LocationBar::getCurentUri ()
{
    return mCurrentUri;
}

LocationBar::LocationBar(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);

    setToolTip(tr("click the blank area for edit"));

    setStyleSheet("padding-right: 15;margin-left: 2");
    mStyledEdit = new QLineEdit;

    mLayout = new QHBoxLayout;
    setLayout(mLayout);

    mIndicator = new QToolButton(this);
    mIndicator->setFocusPolicy(Qt::FocusPolicy(mIndicator->focusPolicy() & ~Qt::TabFocus));
    mIndicator->setAutoRaise(true);
    mIndicator->setStyle(LocationBarButtonStyle::getStyle());
    mIndicator->setPopupMode(QToolButton::InstantPopup);
    mIndicator->setArrowType(Qt::RightArrow);
    mIndicator->setCheckable(true);
    mIndicator->setFixedSize(this->height() - 2, this->height() - 2);
    mIndicator->move(-2, 1);

    mIndicatorMenu = new QMenu(mIndicator);
    mIndicator->setMenu(mIndicatorMenu);
    mIndicator->setArrowType(Qt::RightArrow);

    connect(mIndicatorMenu, &QMenu::aboutToShow, this, [=](){
        mIndicator->setArrowType(Qt::DownArrow);
    });

    connect(mIndicatorMenu, &QMenu::aboutToHide, this, [=](){
        mIndicator->setArrowType(Qt::RightArrow);
    });
}

void LocationBar::setRootUri(const QString &uri)
{
    if (mCurrentUri == uri)
        return;

    mCurrentUri = uri;

    //clear buttons
    clearButtons();

    if (mCurrentUri.startsWith("search://")) {
        mIndicator->setArrowType(Qt::NoArrow);
        addButton(mCurrentUri, false, false);
        return;
    }

    mCurrentInfo = Peony::FileInfo::fromUri(uri);
    mButtonsInfo.clear();
    auto tmpUri = uri;
    while (!tmpUri.isEmpty() && tmpUri != "") {
        mButtonsInfo.prepend(FileInfo::fromUri(tmpUri));
        tmpUri = FileUtils::getParentUri(tmpUri);
    }

    mQueryingButtonsInfo = mButtonsInfo;

    for (auto info : mButtonsInfo) {
        auto infoJob = new FileInfoJob(info);
        infoJob->setAutoDelete();
        connect(infoJob, &FileInfoJob::queryAsyncFinished, this, [=](){
            // enumerate buttons info directory
            auto enumerator = new FileEnumerator;
            enumerator->setEnumerateDirectory(info.get()->uri());
            enumerator->setEnumerateWithInfoJob();

            connect(enumerator, &FileEnumerator::enumerateFinished, this, [=](bool successed){
                if (successed) {
                    auto infos = enumerator->getChildren();
                    mInfosHash.insert(info.get()->uri(), infos);
                    mQueryingButtonsInfo.removeOne(info);
                    if (mQueryingButtonsInfo.isEmpty()) {
                        // add buttons
                        clearButtons();
                        for (auto info : mButtonsInfo) {
                            addButton(info.get()->uri(), true, true);
                        }
                        doLayout();
                    }
                }

                enumerator->deleteLater();
            });

            enumerator->enumerateAsync();
        });
        infoJob->queryAsync();
    }

    return;
}

void LocationBar::clearButtons()
{
    for (auto button : mButtons) {
        button->hide();
        button->deleteLater();
    }

    mButtons.clear();
}

void LocationBar::addButton(const QString &uri, bool setIcon, bool setMenu)
{
    setIcon = true;
    QToolButton *button = new QToolButton(this);
    button->setFocusPolicy(Qt::FocusPolicy(button->focusPolicy() & ~Qt::TabFocus));
    button->setAutoRaise(true);
    button->setStyle(LocationBarButtonStyle::getStyle());
    button->setProperty("uri", uri);
    button->setFixedHeight(this->height());
    button->setIconSize(QSize(16, 16));
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button->setPopupMode(QToolButton::MenuButtonPopup);

    auto displayName = FileUtils::getFileDisplayName(uri);
    mButtons.insert(uri, button);

    QUrl url = uri;

    auto parent = FileUtils::getParentUri(uri);
    if (setIcon) {
        QIcon icon = QIcon::fromTheme(Peony::FileUtils::getFileIconName(uri), QIcon::fromTheme("folder"));
        button->setIcon(icon);
    }

    if (!url.fileName().isEmpty()) {
        if (FileUtils::getParentUri(uri).isNull()) {
            setMenu = false;
        }
        button->setText(displayName);
        mCurrentUri = uri.left(uri.lastIndexOf("/") + 1) + displayName;
    } else {
        if (uri == "file:///") {
            auto text = FileUtils::getFileDisplayName("computer:///root.link");
            if (text.isNull()) {
                text = tr("File System");
            }
            button->setText(text);
        } else {
            button->setText(displayName);
        }
    }

    //if button text is too long, elide it
    displayName = button->text();
    if (displayName.length() > ELIDE_TEXT_LENGTH)
    {
        int  charWidth = fontMetrics().averageCharWidth();
        displayName = fontMetrics().elidedText(displayName, Qt::ElideRight, ELIDE_TEXT_LENGTH * charWidth);
    }
    button->setText(displayName);

    connect(button, &QToolButton::clicked, [=]() {
        Q_EMIT this->groupChangedRequest(uri);
    });

    if (setMenu) {
        auto infos = mInfosHash.value(uri);
        QStringList uris;
        for (auto info : infos) {
            if (info.get()->isDir() && !info.get()->displayName().startsWith("."))
                uris<<info.get()->uri();
        }
        if (uris.isEmpty())
            button->setPopupMode(QToolButton::InstantPopup);
        Peony::PathBarModel m;
        m.setStringList(uris);
        m.sort(0);

        auto suburis = m.stringList();
        if (!suburis.isEmpty()) {
            QMenu *menu = new QMenu;
            connect(button, &QToolButton::destroyed, menu, &QMenu::deleteLater);
            const int WIDTH_EXTEND = 5;
            connect(menu, &QMenu::aboutToShow, this, [=](){
                menu->setMinimumWidth(button->width() + WIDTH_EXTEND);
            });
            QList<QAction *> actions;
            for (auto uri : suburis) {
                QString tmp = uri;
                displayName = Peony::FileUtils::getFileDisplayName(uri);
                if (displayName.length() > ELIDE_TEXT_LENGTH)
                {
                    int  charWidth = fontMetrics().averageCharWidth();
                    displayName = fontMetrics().elidedText(displayName, Qt::ElideRight, ELIDE_TEXT_LENGTH * charWidth);
                }
                QIcon icon = QIcon::fromTheme(Peony::FileUtils::getFileIconName(uri), QIcon::fromTheme("folder"));
                QAction *action = new QAction(icon, displayName, this);
                actions<<action;
                connect(action, &QAction::triggered, [=]() {
                    Q_EMIT groupChangedRequest(tmp);
                });
            }
            menu->addActions(actions);

            button->setMenu(menu);
        } else {
            // no subdir directory should not display an indicator arrow.
            button->setPopupMode(QToolButton::InstantPopup);
        }
    }

    button->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(button, &QWidget::customContextMenuRequested, this, [=](){
//        QMenu menu;
//        FMWindowIface *windowIface = dynamic_cast<FMWindowIface*>(this->topLevelWidget());
//        auto copy = menu.addAction(QIcon::fromTheme("edit-copy-symbolic"), tr("&Copy Directory"));

//        menu.addAction(QIcon::fromTheme("tab-new-symbolic"), tr("Open In New &Tab"), [=](){
//            windowIface->addNewTabs(QStringList()<<uri);
//        });

//        menu.addAction(QIcon::fromTheme("window-new-symbolic"), tr("Open In &New Window"), [=](){
//            auto newWindow = windowIface->create(uri);
//            dynamic_cast<QWidget *>(newWindow)->show();
//        });

//        if (copy == menu.exec(QCursor::pos())) {
//            if (uri.startsWith("file://")) {
//                QUrl url = uri;
//                QApplication::clipboard()->setText(url.path());
//            } else {
//                QApplication::clipboard()->setText(uri);
//            }
//        }
    });
}

void LocationBar::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    QStyleOptionFocusRect opt;
    opt.initFrom(this);

    QStyleOptionFrame fopt;
    fopt.initFrom(this);
    fopt.state |= QStyle::State_HasFocus;
    //fopt.state.setFlag(QStyle::State_HasFocus);
    fopt.rect.adjust(-2, 0, 0, 0);
    fopt.palette.setColor(QPalette::Highlight, fopt.palette.base().color());
    fopt.palette.setColor(QPalette::Base, fopt.palette.window().color());

    style()->drawPrimitive(QStyle::PE_PanelLineEdit, &fopt, &p, this);

    style()->drawControl(QStyle::CE_ToolBar, &opt, &p, this);
}

void LocationBar::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        Q_EMIT blankClicked();
    }
}

void LocationBar::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    doLayout();
}

void LocationBar::doLayout()
{
    mIndicator->setVisible(false);

    QList<int> sizeHints;

    mIndicatorMenu->clear();

    for (auto button : mButtons) {
        button->setVisible(true);
        button->resize(button->sizeHint().width(), button->height());
        button->setToolButtonStyle(Qt::ToolButtonTextOnly);
        button->adjustSize();
        sizeHints<<button->sizeHint().width();
        button->setVisible(false);
    }

    int totalWidth = this->width();
    int currentWidth = 0;
    int visibleButtonCount = 0;
    for (int index = sizeHints.count() - 1; index >= 0; index--) {
        int tmp = currentWidth + sizeHints.at(index);
        if (tmp <= totalWidth) {
            visibleButtonCount++;
            currentWidth = tmp;
        } else {
            break;
        }
    }

    int offset = 0;

    bool indicatorVisible = visibleButtonCount < sizeHints.count();
    if (indicatorVisible) {
        mIndicator->setVisible(true);
        offset += mIndicator->width();
    } else {
        mIndicator->setVisible(false);
    }

    for (int index = sizeHints.count() - visibleButtonCount; index < sizeHints.count(); index++) {
        auto button = mButtons.values().at(index);
        button->setVisible(true);
        button->move(offset, 0);
        if (index == sizeHints.count() - visibleButtonCount) {
            button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            button->adjustSize();
        }
        offset += button->width();
    }

    if (visibleButtonCount == 0 && !mButtons.isEmpty()) {
        auto button = mButtons.values().at(sizeHints.count() - 1);
        button->setVisible(true);
        button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        button->resize(totalWidth - 20, button->height());
    }

    int spaceCount = 0;
    QList<QAction *> actions;
    for (auto button : mButtons) {
        if (button->isVisible()) {
            break;
        }
        auto uri = button->property("uri").toString();
        QString space;
        int i = 0;
        while (i < spaceCount) {
            space.append(' ');
            i++;
        }
        auto action = new QAction(space + button->text(), nullptr);
        actions.append(action);

        connect(action, &QAction::triggered, this, [=](){
            Q_EMIT groupChangedRequest(uri);
        });
        spaceCount++;
    }
    mIndicatorMenu->addActions(actions);
    //add some space for switch to edit
    for (int i = 0; i < 10; i++) {
         mIndicatorMenu->addSeparator();
    }
}

void LocationBarButtonStyle::polish(QWidget *widget)
{
    QProxyStyle::polish(widget);

    widget->setProperty("useIconHighlightEffect", true);
    widget->setProperty("iconHighLightEffectMode", 1);
}

void LocationBarButtonStyle::unpolish(QWidget *widget)
{
    QProxyStyle::unpolish(widget);

    widget->setProperty("useIconHighlightEffect", QVariant());
    widget->setProperty("iconHighLightEffectMode", QVariant());
}

void LocationBarButtonStyle::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if (control == QStyle::CC_ToolButton) {
        auto toolButton = qstyleoption_cast<const QStyleOptionToolButton *>(option);
        auto opt = *toolButton;
        if (toolButton->arrowType == Qt::NoArrow)
            opt.rect.adjust(0, 1, 0, -1);
        else
            opt.rect.adjust(-2, 1, 2, -1);
        return QProxyStyle::drawComplexControl(control, &opt, painter, widget);
    }
    return QProxyStyle::drawComplexControl(control, option, painter, widget);
}
