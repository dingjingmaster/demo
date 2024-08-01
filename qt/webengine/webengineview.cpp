/*************************************************************************
> FileName: demo1.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 01 Aug 2024 09:39:37 AM CST
 ************************************************************************/
#include <QApplication>
#include <QWebEngineView>

/**
 * @brief
 *  QWebEngineView 继承自 QWidget
 *  属性： selectedText
 *  方法：
 */
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    auto view = new QWebEngineView(nullptr);

    // load
    // 重载：QUrl 或 QWebEngineHttpRequest
    view->load(QUrl("http://www.qt.io/"));

    //
    view->show();

    // 查找页面子串，可以设置查找方式、查找后回调
    view->findText("Qt");

    // history
    // 等价于 view->page()->history();
    QWebEngineHistory* history = view->history();

    // page
    QWebEnginePage* page = view->page();

    // Action
    // 复制、粘贴、前进、后退 等...
    QAction* action = view->pageAction(QWebEnginePage::Reload);

    // content
    // Sets the content of the web view to data. If the mimeType argument is empty, it is assumed that the content is text/plain,charset=US-ASCII.
    // External objects referenced in the content are located relative to baseUrl. For external objects to be loaded, baseUrl cannot be empty.
    // The data is loaded immediately; external objects are loaded asynchronously.
    //view->setContent();
    //view->setHtml();
    //view->setPage(); // 开启新的页面

    // 返回当前 view 中 page 的设置属性
    // 等价于：view->page()->settings();
    QWebEngineSettings* settings = view->settings();

    // showEvent();

    // sizeHint();

    // 复制当前选中内容到粘贴板
    view->triggerPageAction(QWebEnginePage::Copy);

    return app.exec();
}
