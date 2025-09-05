/*************************************************************************
> FileName: regex-file-ext.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年03月27日 星期四 19时13分43秒
 ************************************************************************/
#include <QDebug>
#include <QString>
#include <QRegExp>
#include <QRegularExpression>

int main ()
{
	const QString str = "/aa/bb/vv/dd.java";
	//const QString str = "dd.java";

	//QRegularExpression regex ("*\\.java");
    //qInfo() << "str: " << str << ", contains: " << regex.match(str).hasMatch();

	QRegExp regex ("*.java", Qt::CaseInsensitive);
    regex.setPatternSyntax(QRegExp::Wildcard);
    qInfo() << "str: " << str << ", contains: " << regex.exactMatch(str);

//	qDebug() << str << "    " << regex.indexIn(str) << "   " << regex.cap(0);

	return 0;
}

