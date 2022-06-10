#include "mainwindow.h"

#include "mysortfilterproxymodel.h"
#include "filterwidget.h"

#include <QtWidgets>

Window::Window()
{
    proxyModel = new MySortFilterProxyModel(this);

    sourceView = new QTreeView;
    sourceView->setRootIsDecorated(false);
    sourceView->setAlternatingRowColors(true);

    QHBoxLayout *sourceLayout = new QHBoxLayout;

    sourceLayout->addWidget(sourceView);
    sourceGroupBox = new QGroupBox(tr("Original Model"));
    sourceGroupBox->setLayout(sourceLayout);

    filterWidget = new FilterWidget;
    filterWidget->setText(tr("Grace|Sports"));
    connect(filterWidget, &FilterWidget::filterChanged, this, &Window::textFilterChanged);

    filterPatternLabel = new QLabel(tr("&Filter pattern:"));
    filterPatternLabel->setBuddy(filterWidget);

    fromDateEdit = new QDateEdit;
    fromDateEdit->setDate(QDate(1970, 01, 01));
    fromLabel = new QLabel(tr("F&rom:"));
    fromLabel->setBuddy(fromDateEdit);

    toDateEdit = new QDateEdit;
    toDateEdit->setDate(QDate(2099, 12, 31));
    toLabel = new QLabel(tr("&To:"));
    toLabel->setBuddy(toDateEdit);

    connect(filterWidget, &QLineEdit::textChanged, this, &Window::textFilterChanged);
    connect(toDateEdit, &QDateTimeEdit::dateChanged, this, &Window::dateFilterChanged);
    connect(fromDateEdit, &QDateTimeEdit::dateChanged, this, &Window::dateFilterChanged);

    proxyView = new QTreeView;
    proxyView->setRootIsDecorated(false);
    proxyView->setAlternatingRowColors(true);
    proxyView->setModel(proxyModel);
    proxyView->setSortingEnabled(true);
    proxyView->sortByColumn(1, Qt::AscendingOrder);

    QGridLayout *proxyLayout = new QGridLayout;
    proxyLayout->addWidget(proxyView, 0, 0, 1, 3);
    proxyLayout->addWidget(filterPatternLabel, 1, 0);
    proxyLayout->addWidget(filterWidget, 1, 1);
    proxyLayout->addWidget(fromLabel, 3, 0);
    proxyLayout->addWidget(fromDateEdit, 3, 1, 1, 2);
    proxyLayout->addWidget(toLabel, 4, 0);
    proxyLayout->addWidget(toDateEdit, 4, 1, 1, 2);

    proxyGroupBox = new QGroupBox(tr("Sorted/Filtered Model"));
    proxyGroupBox->setLayout(proxyLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(sourceGroupBox);
    mainLayout->addWidget(proxyGroupBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Custom Sort/Filter Model"));
    resize(500, 450);
}

void Window::setSourceModel(QAbstractItemModel *model)
{
    proxyModel->setSourceModel(model);
    sourceView->setModel(model);

    for (int i = 0; i < proxyModel->columnCount(); ++i) {
        proxyView->resizeColumnToContents(i);
    }

    for (int i = 0; i < model->columnCount(); ++i) {
        sourceView->resizeColumnToContents(i);
    }
}

void Window::textFilterChanged()
{
    FilterWidget::PatternSyntax s = filterWidget->patternSyntax();
    QString pattern = filterWidget->text();
    switch (s) {
    case FilterWidget::Wildcard:
        pattern = QRegularExpression::wildcardToRegularExpression(pattern);
        break;
    case FilterWidget::FixedString:
        pattern = QRegularExpression::escape(pattern);
        break;
    default:
        break;
    }

    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    if (filterWidget->caseSensitivity() == Qt::CaseInsensitive) {
        options |= QRegularExpression::CaseInsensitiveOption;
    }
    QRegularExpression regularExpression(pattern, options);
    proxyModel->setFilterRegularExpression(regularExpression);
}

void Window::dateFilterChanged()
{
    proxyModel->setFilterMinimumDate(fromDateEdit->date());
    proxyModel->setFilterMaximumDate(toDateEdit->date());
}
