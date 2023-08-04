#ifndef MAINHEADER_H
#define MAINHEADER_H

#include "header-button.h"

#include <QLabel>
#include <QWidget>
#include <QDateTime>
#include <QMouseEvent>
#include <QHBoxLayout>

class MainHeader : public QWidget
{
    Q_OBJECT
public:
    explicit MainHeader(QWidget *parent = nullptr);
    void setTitle (const char* title);

    void hideMin();
    void showMin();

    void hideMax();
    void showMax();


Q_SIGNALS:
    void windowMin();
    void windowMax();
    void windowClose();
    void setCloseButtonEnable(bool e);

public Q_SLOTS:
    void onApplyData ();
    void onApplyDataFinished ();

protected:
    void mouseDoubleClickEvent(QMouseEvent*) override;

private:
    const int               mMaxHeight = 46;

    QLayout*                mLeftLayout;
    QLayout*                mRightLayout;
    QHBoxLayout*            mMainLayout;

    QLabel*                 mHeaderName;

    HeaderButton*           mMinBtn{};
    HeaderButton*           mMaxBtn{};
    HeaderButton*           mCloseBtn;

    bool                    mCloseBtnEnable = true;

    // data
    bool                    mDrag{};
    QPoint                  mMouseStartPoint;
    QPoint                  mWindowTopLeftPoint;
};

#endif // MAINHEADER_H
