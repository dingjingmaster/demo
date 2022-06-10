#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);

Q_SIGNALS:
    void switchViewRequest(const QString &viewId);

public Q_SLOTS:
    void updateViewType(const QString &viewId);

private:
    QAction *m_iconView;
    QAction *m_listView;
};
#endif // TOOLBAR_H
