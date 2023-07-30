#include <QApplication>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QScreen>
#include <QTreeView>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    const QString rootPath = "/";

    QFileSystemModel model;
    model.setRootPath("");
    QTreeView tree;
    tree.setModel(&model);
    if (!rootPath.isEmpty()) {
        const QModelIndex rootIndex = model.index(QDir::cleanPath(rootPath));
        if (rootIndex.isValid()) {
            tree.setRootIndex(rootIndex);
        }
    }

    // Demonstrating look and feel features
    tree.setAnimated(true);
    tree.setIndentation(20);
    tree.setSortingEnabled(true);
    const QSize availableSize = QSize(1000, 900);
    tree.resize(availableSize / 2);
    tree.setColumnWidth(0, tree.width() / 3);

    tree.setWindowTitle(QObject::tr("Dir View"));
    tree.show();

    return app.exec();
}

