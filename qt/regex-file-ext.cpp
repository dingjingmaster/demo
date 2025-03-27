/*************************************************************************
> FileName: regex-file-ext.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年03月27日 星期四 19时13分43秒
 ************************************************************************/
#include <QDebug>
#include <QString>
#include <QRegExp>

int main ()
{
	const QString str1 = "/aa/bb/vv/dd.c";
	const QString str2 = "/aa/bb/vv/dd.C";
	const QString str3 = "/aa/bb/vv/dd.txt";
	const QString str4 = "/aa/bb/vv/dd.tXt";
	const QString str5 = "/aa/bb/vv/dd.java";
	const QString str6 = "/aa/bb/vv/dd.m";

	QRegExp regex (".(c|txt|java)$", Qt::CaseInsensitive);

	qDebug() << str1 << "    " << regex.indexIn(str1) << "   " << regex.cap(0);
	qDebug() << str2 << "    " << regex.indexIn(str2) << "   " << regex.cap(0);
	qDebug() << str3 << "    " << regex.indexIn(str3) << "   " << regex.cap(0);
	qDebug() << str4 << "    " << regex.indexIn(str4) << "   " << regex.cap(0);
	qDebug() << str5 << "    " << regex.indexIn(str5) << "   " << regex.cap(0);
	qDebug() << str6 << "    " << regex.indexIn(str6) << "   " << regex.cap(0);

	return 0;
}

