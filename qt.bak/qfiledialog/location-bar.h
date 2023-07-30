#ifndef LOCATIONBAR_H
#define LOCATIONBAR_H
#include <QToolBar>
#include <memory>

class QMenu;
class QLineEdit;
class QHBoxLayout;
class QToolButton;

namespace Peony {
class FileInfo;
}

class LocationBar : public QWidget
{
    Q_OBJECT
public:
    ~LocationBar() override;
    const QString getCurentUri();
    explicit LocationBar(QWidget *parent = nullptr);

Q_SIGNALS:
    void blankClicked();
    void groupChangedRequest(const QString &uri);

public Q_SLOTS:
    void setRootUri(const QString &uri);

protected:
    void clearButtons();
    void addButton(const QString &uri, bool setIcon = false, bool setMenu = true);

    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void resizeEvent(QResizeEvent* event) override;

    void doLayout();

private:
    QString                                                 mCurrentUri;
    QLineEdit*                                              mStyledEdit;
    QHBoxLayout*                                            mLayout;

    QMap<QString, QToolButton*>                             mButtons;
    QToolButton*                                            mIndicator;
    QMenu*                                                  mIndicatorMenu;

    const int                                               ELIDE_TEXT_LENGTH = 16;

    std::shared_ptr<Peony::FileInfo>                               mCurrentInfo;
    QList<std::shared_ptr<Peony::FileInfo>>                        mButtonsInfo;
    QList<std::shared_ptr<Peony::FileInfo>>                        mQueryingButtonsInfo;
    QHash<QString, QList<std::shared_ptr<Peony::FileInfo>>>        mInfosHash;
};
#endif // LOCATIONBAR_H
