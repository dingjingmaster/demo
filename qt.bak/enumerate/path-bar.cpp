#include "path-bar.h"

#include <PeonyPathCompleter>
#include <PeonyPathBarModel>

PathBar::PathBar(QWidget *parent) : QLineEdit(parent)
{
    auto completer = new Peony::PathCompleter(this);
    completer->setModel(new Peony::PathBarModel(this));
    setCompleter(completer);

    connect(this, &PathBar::returnPressed, this, [=](){
        auto uri = this->text();
        if (!uri.isEmpty()) {
            goToUriRequest(uri);
        }
    });
}

void PathBar::updatePath(const QString &uri)
{
    setText(uri);
}
