#include "mainwindow.h"
#include <QtWidgets>

Window::Window()
{
    proxyModel = new QSortFilterProxyModel;

    sourceView = new QTreeView;
    sourceView->setRootIsDecorated(false);
    sourceView->setAlternatingRowColors(true);

    proxyView = new QTreeView;
    proxyView->setRootIsDecorated(false);
    proxyView->setAlternatingRowColors(true);
    proxyView->setModel(proxyModel);
    proxyView->setSortingEnabled(true);

    sortCaseSensitivityCheckBox = new QCheckBox(tr("Case sensitive sorting"));
    filterCaseSensitivityCheckBox = new QCheckBox(tr("Case sensitive filter"));

    filterPatternLineEdit = new QLineEdit;
    filterPatternLabel = new QLabel(tr("&Filter pattern:"));
    filterPatternLabel->setBuddy(filterPatternLineEdit);

    filterSyntaxComboBox = new QComboBox;
    filterSyntaxComboBox->addItem(tr("Regular expression"), RegularExpression);
    filterSyntaxComboBox->addItem(tr("Wildcard"), Wildcard);
    filterSyntaxComboBox->addItem(tr("Fixed string"), FixedString);
    filterSyntaxLabel = new QLabel(tr("Filter &syntax:"));
    filterSyntaxLabel->setBuddy(filterSyntaxComboBox);

    filterColumnComboBox = new QComboBox;
    filterColumnComboBox->addItem(tr("Subject"));
    filterColumnComboBox->addItem(tr("Sender"));
    filterColumnComboBox->addItem(tr("Date"));
    filterColumnLabel = new QLabel(tr("Filter &column:"));
    filterColumnLabel->setBuddy(filterColumnComboBox);

    connect(sortCaseSensitivityCheckBox, &QAbstractButton::toggled, this, &Window::sortChanged);
//    connect(filterPatternLineEdit, &QLineEdit::textChanged, this, &Window::filterRegularExpressionChanged);
    connect(filterColumnComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filterColumnChanged(int)));
    connect(filterCaseSensitivityCheckBox, &QAbstractButton::toggled, this, &Window::filterRegularExpressionChanged);
    connect(filterSyntaxComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filterRegularExpressionChanged(int)));

    sourceGroupBox = new QGroupBox(tr("Original Model"));
    proxyGroupBox = new QGroupBox(tr("Sorted/Filtered Model"));

    QHBoxLayout *sourceLayout = new QHBoxLayout;
    sourceLayout->addWidget(sourceView);
    sourceGroupBox->setLayout(sourceLayout);

    QGridLayout *proxyLayout = new QGridLayout;
    proxyLayout->addWidget(proxyView, 0, 0, 1, 3);
    proxyLayout->addWidget(filterPatternLabel, 1, 0);
    proxyLayout->addWidget(filterPatternLineEdit, 1, 1, 1, 2);
    proxyLayout->addWidget(filterSyntaxLabel, 2, 0);
    proxyLayout->addWidget(filterSyntaxComboBox, 2, 1, 1, 2);
    proxyLayout->addWidget(filterColumnLabel, 3, 0);
    proxyLayout->addWidget(filterColumnComboBox, 3, 1, 1, 2);
    proxyLayout->addWidget(filterCaseSensitivityCheckBox, 4, 0, 1, 2);
    proxyLayout->addWidget(sortCaseSensitivityCheckBox, 4, 2);
    proxyGroupBox->setLayout(proxyLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(sourceGroupBox);
    mainLayout->addWidget(proxyGroupBox);

    setLayout(mainLayout);

    setWindowTitle(tr("Basic Sort/Filter Model"));
    resize(500, 450);

    proxyView->sortByColumn(1, Qt::AscendingOrder);
    filterColumnComboBox->setCurrentIndex(1);

    filterPatternLineEdit->setText("Andy|Grace");
    filterCaseSensitivityCheckBox->setChecked(true);
    sortCaseSensitivityCheckBox->setChecked(true);
}

void Window::setSourceModel(QAbstractItemModel *model)
{
    proxyModel->setSourceModel(model);
    sourceView->setModel(model);
}

void Window::filterRegularExpressionChanged(int)
{
    Syntax s = Syntax(filterSyntaxComboBox->itemData(filterSyntaxComboBox->currentIndex()).toInt());
    QString pattern = filterPatternLineEdit->text();
    switch (s) {
    case Wildcard:
        pattern = QRegularExpression::wildcardToRegularExpression(pattern);
        break;
    case FixedString:
        pattern = QRegularExpression::escape(pattern);
        break;
    default:
        break;
    }

    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    if (!filterCaseSensitivityCheckBox->isChecked())
        options |= QRegularExpression::CaseInsensitiveOption;
    QRegularExpression regularExpression(pattern, options);
    proxyModel->setFilterRegularExpression(regularExpression);
}

void Window::filterColumnChanged(int)
{
    proxyModel->setFilterKeyColumn(filterColumnComboBox->currentIndex());
}

void Window::sortChanged()
{
    proxyModel->setSortCaseSensitivity(sortCaseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
}


