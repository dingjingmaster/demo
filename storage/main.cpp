/*************************************************************************
> FileName: main.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年03月27日 星期四 16时54分45秒
 ************************************************************************/
#include <QDebug>
#include <QDateTime>
#include "storage.h"

int main (int argc, char* argv[])
{
	// backup
	qInfo() << "[backup]";
	qInfo() << "    允许本地备份: " << Storage::getInstance()->getBackupLocalEnable();
	qInfo() << "    允许远程备份: " << Storage::getInstance()->getBackupRpcEnable();
	qInfo() << "远程备份文件类型: " << Storage::getInstance()->getBackupFileExtName();
	qInfo() << "上次远程备份时间: " << QDateTime::fromMSecsSinceEpoch(Storage::getInstance()->getLastUpdateTime()).toString("yyyy-MM-dd HH:mm:ss");

	return 0;
}
