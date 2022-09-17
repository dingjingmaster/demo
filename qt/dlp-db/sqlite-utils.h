#ifndef SQLITEUTILS_H
#define SQLITEUTILS_H

#include <QObject>
#include <qglobal.h>

class SqliteUtils;

class FileInfo
{
    friend SqliteUtils;
public:
    /**
     * @brief 要保存的数据
     *
     * @param md5: 外发文件的 MD5 值
     * @param startTime: 文件审批时间的时间戳——开始有效时间
     * @param stopTime: 文件审批时间的时间戳——截止有效时间
     * @param channel: 通道标识，标识以 '|' 分割，1: USB；2: 打印机； 3:邮件
     */
    FileInfo (QString& md5, qint64 startTime, qint64 stopTime, QString& channel);

private:
    QString             mMD5;
    qint64              mStartTime;
    qint64              mStopTime;
    QString             mChannel;
};


class SqliteUtilsPrivate;
class SqliteUtils : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SqliteUtils);
public:
    enum Channel { CHANNEL_USB=1, CHANNEL_PRINTER, CHANNEL_EMAIL };

    explicit SqliteUtils(QObject *parent = nullptr);

    /**
     * @brief 插入一条数据或多条数据
     */
    bool insert (FileInfo& fileInfo);
    bool insert (QList<FileInfo>& fileInfos);

    /**
     * @brief 清空数据库
     */
    void clearAll ();

    /**
     * @brief 根据文件 md5 值删除数据库中相应记录
     */
    void deleteByMD5 (QString& md5);

    /**
     * @brief 检查是否包含指定文件的 MD5 值
     */
    bool contains (QString& md5, Channel channel);

private:
    SqliteUtilsPrivate*         d_ptr;
};

#endif // SQLITEUTILS_H
