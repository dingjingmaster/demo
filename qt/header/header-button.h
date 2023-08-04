#ifndef HEADERBUTTON_H
#define HEADERBUTTON_H

#include <QPushButton>

class HeaderButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(Type type MEMBER mType READ getType WRITE setType NOTIFY typeChanged)
public:
    enum Type { MIN, MAX, CLOSE, RESTORE }; Q_ENUM(Type)

public:
    explicit HeaderButton(QWidget *parent = nullptr, Type type = MIN);

    Type getType ();
    void setType (Type type);

protected:
    void enterEvent (QEvent *event) override;
    void leaveEvent (QEvent *event) override;
    void paintEvent (QPaintEvent* event) override;

Q_SIGNAL void typeChanged(Type);

public Q_SLOTS:
    void onTypeChanged(Type type);

private:
    QPixmap transColor (const QPixmap& icon);

private:
    Type                mType;
};

#endif // HEADERBUTTON_H
