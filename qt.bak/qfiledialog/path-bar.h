#ifndef PATHBAR_H
#define PATHBAR_H

#include <QLineEdit>

class PathBar : public QLineEdit
{
    Q_OBJECT
public:
    explicit PathBar(QWidget *parent = nullptr);

Q_SIGNALS:
    void goToUriRequest(const QString &uri, bool addToHistory = true);

public Q_SLOTS:
    void updatePath(const QString &uri);
};
#endif // PATHBAR_H
