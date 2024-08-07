#ifndef WEB_GUI_H
#define WEB_GUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QWebChannel>
#include <QWebEngineView>

#include "../qtwebgui/WebExportCore.h"


class WebGui : public QWidget
{
    Q_OBJECT
    enum Direction {UP = 0, DOWN, LEFT, RIGHT, LEFT_TOP, LEFT_BOTTOM, RIGHT_BOTTOM, RIGHT_TOP, NONE};

public:
    enum WebPage {
        WP_WORKBENCH,
    };

public:
    static WebGui* getInstance ();

    /* 页面显示 -- 开始 */

    /**/
    bool loadPage(WebPage page);

    /* 页面显示 -- 结束 */

protected:
    void showEvent(QShowEvent*) override;
    void resizeEvent (QResizeEvent*) override;
    void mouseMoveEvent (QMouseEvent*) override;
    void mousePressEvent (QMouseEvent*) override;
    void mouseReleaseEvent (QMouseEvent*) override;

    static bool deleteDir(const QString &path);

private:
    WebGui(){}
    ~WebGui(){}
    QUrl getBaseUrl(WebPage page);
    explicit WebGui(QWidget *parent = nullptr);
    void region (const QPoint& cursorGlobalPoint);
    bool eventFilter(QObject* watched, QEvent* ev) override;

private:
    WebExportCore* 					core;
    QPoint                          mOffset;
    bool                            mDrag = false;
    bool                            mIsPress = false;
    QPoint                          mDragPos;
    Direction                       mDirection = NONE;
    QHBoxLayout* 					mMainLayout = nullptr;
    QWebChannel* 					mWebChannel = nullptr;
    QWebEngineView* 				mWebEngineView = nullptr;


    static WebGui* 					gInstance;
};

#endif // WEB-GUI_H
