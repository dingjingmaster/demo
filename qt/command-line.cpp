/*************************************************************************
> FileName: command-line.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 22 Mar 2023 02:10:15 PM CST
 ************************************************************************/
#include <QCoreApplication>
#include <QCommandLineParser>

 int main(int argc, char *argv[])
 {
     QCoreApplication app(argc, argv);
     QCoreApplication::setApplicationName("my-copy-program");
     QCoreApplication::setApplicationVersion("1.0");

     QCommandLineParser parser;
     parser.setApplicationDescription("Test helper");
     parser.addHelpOption();
     parser.addVersionOption();
     parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to copy."));
     parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));

     // A boolean option with a single name (-p)
     QCommandLineOption showProgressOption("p", QCoreApplication::translate("main", "Show progress during copy"));
     parser.addOption(showProgressOption);

     // A boolean option with multiple names (-f, --force)
     QCommandLineOption forceOption(QStringList() << "f" << "force",
             QCoreApplication::translate("main", "Overwrite existing files."));
     parser.addOption(forceOption);

     // An option with a value
     QCommandLineOption targetDirectoryOption(QStringList() << "t" << "target-directory",
             QCoreApplication::translate("main", "Copy all source files into <directory>."),
             QCoreApplication::translate("main", "directory"));
     parser.addOption(targetDirectoryOption);

     // Process the actual command line arguments given by the user
     parser.process(app);

     const QStringList args = parser.positionalArguments();
     // source is args.at(0), destination is args.at(1)

     bool showProgress = parser.isSet(showProgressOption);
     bool force = parser.isSet(forceOption);
     QString targetDir = parser.value(targetDirectoryOption);
     // ...
 }

