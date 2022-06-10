#include "mainwindow.h"

#include <QtWidgets>

Window::Window(QWidget *parent) : QWidget(parent)
{
    setupModel();

    nameLabel = new QLabel(tr("Na&me:"));
    nameEdit = new QLineEdit();
    addressLabel = new QLabel(tr("&Address:"));
    addressEdit = new QTextEdit();
    typeLabel = new QLabel(tr("&Type:"));
    typeComboBox = new QComboBox();
    nextButton = new QPushButton(tr("&Next"));
    previousButton = new QPushButton(tr("&Previous"));

    nameLabel->setBuddy(nameEdit);
    addressLabel->setBuddy(addressEdit);
    typeLabel->setBuddy(typeComboBox);

    typeComboBox->setModel(typeModel);

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->addMapping(nameEdit, 0);
    mapper->addMapping(addressEdit, 1);
    mapper->addMapping(typeComboBox, 2, "currentIndex");

    connect(nextButton, &QAbstractButton::clicked, mapper, &QDataWidgetMapper::toNext);
    connect(mapper, &QDataWidgetMapper::currentIndexChanged, this, &Window::updateButtons);
    connect(previousButton, &QAbstractButton::clicked, mapper, &QDataWidgetMapper::toPrevious);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(nameLabel, 0, 0, 1, 1);
    layout->addWidget(nameEdit, 0, 1, 1, 1);
    layout->addWidget(previousButton, 0, 2, 1, 1);
    layout->addWidget(addressLabel, 1, 0, 1, 1);
    layout->addWidget(addressEdit, 1, 1, 2, 1);
    layout->addWidget(nextButton, 1, 2, 1, 1);
    layout->addWidget(typeLabel, 3, 0, 1, 1);
    layout->addWidget(typeComboBox, 3, 1, 1, 1);
    setLayout(layout);

    setWindowTitle(tr("Delegate Widget Mapper"));
    mapper->toFirst();
}

void Window::setupModel()
{
    QStringList items;
    items << tr("Home") << tr("Work") << tr("Other");
    typeModel = new QStringListModel(items, this);

    model = new QStandardItemModel(5, 3, this);
    QStringList names;
    names << "Alice" << "Bob" << "Carol" << "Donald" << "Emma";
    QStringList addresses;
    addresses << "<qt>123 Main Street<br/>Market Town</qt>"
              << "<qt>PO Box 32<br/>Mail Handling Service"
                 "<br/>Service City</qt>"
              << "<qt>The Lighthouse<br/>Remote Island</qt>"
              << "<qt>47338 Park Avenue<br/>Big City</qt>"
              << "<qt>Research Station<br/>Base Camp<br/>Big Mountain</qt>";

    QStringList types;
    types << "0" << "1" << "2" << "0" << "2";

    for (int row = 0; row < 5; ++row) {
        QStandardItem *item = new QStandardItem(names[row]);
        model->setItem(row, 0, item);
        item = new QStandardItem(addresses[row]);
        model->setItem(row, 1, item);
        item = new QStandardItem(types[row]);
        model->setItem(row, 2, item);
    }
}

void Window::updateButtons(int row)
{
    previousButton->setEnabled(row > 0);
    nextButton->setEnabled(row < model->rowCount() - 1);
}
