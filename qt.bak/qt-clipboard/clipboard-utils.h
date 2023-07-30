#ifndef CLIPBOARDUTILS_H
#define CLIPBOARDUTILS_H

#include <QObject>
#include <QWidget>

class ClipboardUtils : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardUtils (QObject *parent = nullptr);
    QStringList getUrlsByX11 ();

protected:
    bool eventFilter (QObject* obj, QEvent* ev) override;

private:

public Q_SLOTS:
    void onClipboardDataChanged ();

signals:

private:
    bool isRemoteUri = false;
    int remoteCurrentCount = 0;


};

#endif // CLIPBOARDUTILS_H
