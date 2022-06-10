/*************************************************************************
> FileName: gf-log.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月23日 星期三 21时04分39秒
 ************************************************************************/

#include <QCoreApplication>
#include <graceful/Logger.h>
#include <graceful/FileAppender.h>

int main(int argc, char* argv[])
{
  QCoreApplication app(argc, argv);
  FileAppender* consoleAppender = new FileAppender("/home/dingjing/.local/log/");
  consoleAppender->setFormat("[%{type:-7}] <%{Function}> %{message}\n");
  consoleAppender->setFileName("test.log");
  cuteLogger->registerAppender(consoleAppender);
  LOG_INFO("Starting the application");
  int result = app.exec();
  if (result)
    LOG_WARNING() << "Something went wrong." << "Result code is" << result;
  return result;
}
