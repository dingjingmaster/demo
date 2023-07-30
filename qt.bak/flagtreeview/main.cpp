#include <QApplication>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QTreeView>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QStandardItemModel model(4, 2);
    QTreeView treeView;
    treeView.setModel(&model);
    treeView.setRootIsDecorated(false);
    treeView.header()->setFirstSectionMovable(true);
    treeView.header()->setStretchLastSection(true);

    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 2; ++column) {
            QModelIndex index = model.index(row, column, QModelIndex());
            model.setData(index, QVariant((row + 1) * (column + 1)));
        }
    }

    treeView.setWindowTitle(QObject::tr("Flat Tree View"));
    treeView.show();
    return app.exec();
}
