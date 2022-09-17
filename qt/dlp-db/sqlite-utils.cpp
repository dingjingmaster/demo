#include "sqlite-utils.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sqlite3.h>
#include <sys/file.h>

#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QDateTime>

#define LOCK_FILE1                      "/usr/local/ultrasec/dat/.est.eoa.db.lock"
#define DB_TABLE_EOA_PATH               "/usr/local/ultrasec/dat/db_task/EstDlpEoa.db"
#if 0
#define DB_SQLITE3                      "/usr/local/ultrasec/policyfilter/bin/sqlite3"
#else
#define DB_SQLITE3                      "/usr/bin/sqlite3"
#endif
#define DB_TABLE_EOA_FILE_INFO          ""                                  \
    "CREATE TABLE `eoa_file_info` ("                                        \
        "`md5`                   VARCHAR(255)    DEFAULT \"\"    NOT NULL," \
        "`start_time`            DATETIME        DEFAULT 0       NOT NULL," \
        "`stop_time`             DATETIME        DEFAULT 0       NOT NULL," \
        "`channel`               VARCHAR(255)    DEFAULT \"\"    NOT NULL," \
        "PRIMARY KEY(`md5`)"                                                \
    ");"                                                                    \
""


class SqliteUtilsPrivate
{
    friend SqliteUtils;
public:
    SqliteUtilsPrivate();
    ~SqliteUtilsPrivate();

    void lock ();
    void unlock ();

private:
    FILE*           mLocker;
    sqlite3*        mDB;
};

SqliteUtils::SqliteUtils(QObject *parent)
    : QObject{parent}, d_ptr(new SqliteUtilsPrivate)
{

}

bool SqliteUtils::insert(FileInfo &fileInfo)
{
    Q_D(SqliteUtils);

    char* errorMsg = nullptr;

    QString sql = QString("INSERT INTO `eoa_file_info` (`md5`, `start_time`, `stop_time`, `channel`) VALUES ('%1', %2, %3, '%4');")
            .arg(fileInfo.mMD5).arg(fileInfo.mStartTime).arg(fileInfo.mStopTime).arg(fileInfo.mChannel);

    qInfo() << "sql: " << sql;

    d->lock();
    int ret = sqlite3_exec(d->mDB, sql.toUtf8().constData(), nullptr, nullptr, &errorMsg);
    if (SQLITE_OK != ret) {
        qWarning() << "execute sql error: " << errorMsg;
        sqlite3_free(errorMsg);
    }
    d->unlock();

    return ret == SQLITE_OK;
}

bool SqliteUtils::insert(QList<FileInfo> &fileInfos)
{
    bool ret = true;

    for (auto f : fileInfos) {
        if (!insert (f)) {
            ret = false;
            qWarning() << "insert error, md5: " << f.mMD5;
        }
    }

    return ret;
}

void SqliteUtils::clearAll()
{
    Q_D(SqliteUtils);

    char* errorMsg = nullptr;
    QString sql = QString("DELETE FROM TABLE `eoa_file_info`;");

    qInfo() << "sql: " << sql;

    d->lock();
    int ret = sqlite3_exec(d->mDB, sql.toUtf8().constData(), nullptr, nullptr, &errorMsg);
    if (SQLITE_OK != ret) {
        qWarning() << "execute sql error: " << errorMsg;
        sqlite3_free(errorMsg);
    }
    d->unlock();
}

void SqliteUtils::deleteByMD5(QString &md5)
{
    Q_D(SqliteUtils);

    char* errorMsg = nullptr;
    QString sql = QString("DELETE FROM TABLE `eoa_file_info` WHERE md5='%1';").arg(md5);

    qInfo() << "sql: " << sql;

    d->lock();
    int ret = sqlite3_exec(d->mDB, sql.toUtf8().constData(), nullptr, nullptr, &errorMsg);
    if (SQLITE_OK != ret) {
        qWarning() << "execute sql error: " << errorMsg;
        sqlite3_free(errorMsg);
    }
    d->unlock();
}

bool SqliteUtils::contains(QString &md5, Channel c)
{
    Q_D(SqliteUtils);

    bool isOK = false;
    sqlite3_stmt* stmt = nullptr;
    qint64 time = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QString sql = QString("SELECT `start_time`, `stop_time`, `channel` FROM `eoa_file_info` WHERE md5='%1';").arg(md5);

    qInfo() << "sql: " << sql;

    d->lock();
    int ret = sqlite3_prepare_v2(d->mDB, sql.toUtf8().constData(), -1, &stmt, nullptr);
    if (SQLITE_OK != ret) {
        qWarning() << "execute sql error";
        goto end;
    }

    while (SQLITE_DONE != sqlite3_step(stmt)) {
        qint64 startTime = sqlite3_column_int64(stmt, 0);
        qint64 stopTime = sqlite3_column_int64(stmt, 1);
        QString channel = QString(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));

        QStringList channels = channel.split("|");

        qDebug() << startTime;
        qDebug() << stopTime;
        qDebug() << channel;
        qDebug() << time;

        if (time >= startTime && time <= stopTime && channels.contains(QString("%1").arg(c))) {
            isOK = true;
            break;
        }
    }

end:

    if (stmt)           { sqlite3_finalize(stmt); stmt = nullptr;}

    d->unlock();

    return isOK;
}

FileInfo::FileInfo(QString &md5, qint64 startTime, qint64 stopTime, QString& channel)
    : mMD5(md5), mStartTime(startTime), mStopTime(stopTime), mChannel(channel)
{
}

SqliteUtilsPrivate::SqliteUtilsPrivate()
{
    if (access (LOCK_FILE1, F_OK)) {
        if (creat(LOCK_FILE1, S_IRWXU | S_IRWXG | S_IRWXO)) {
            qWarning() << "create lock file: '" LOCK_FILE1 "' error!";
            exit(-1);
        }
    }

    mLocker = fopen(LOCK_FILE1, "a+");
    if (!mLocker) {
        qWarning() << "open file: '" LOCK_FILE1 "' error!";
        exit(-1);
    }

    if (!QFile::exists(DB_TABLE_EOA_PATH)) {
        QProcess::execute(DB_SQLITE3, QStringList() << DB_TABLE_EOA_PATH << DB_TABLE_EOA_FILE_INFO);
    }

    int rc = sqlite3_open (DB_TABLE_EOA_PATH, &mDB);
    if (SQLITE_OK != rc) {
        qCritical() << "connect to database: " << DB_TABLE_EOA_PATH << " failed!";
        exit(-1);
    }
}

SqliteUtilsPrivate::~SqliteUtilsPrivate()
{
    if (mDB)            { sqlite3_close(mDB); mDB = nullptr;}
    if (mLocker)        { fclose(mLocker); mLocker = nullptr;}
}

void SqliteUtilsPrivate::lock()
{
    while (true) {
        if (flock (mLocker->_fileno, LOCK_EX | LOCK_NB) == 0) {
            qDebug() << "locked";
            break;
        }
        usleep(100);
        qDebug() << "lock failed!";
    }
}

void SqliteUtilsPrivate::unlock()
{
    flock (mLocker->_fileno, LOCK_UN);
}
