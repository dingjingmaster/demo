#include "storage.h"

#include <QDir>
#include <QSet>
#include <QFile>
#include <QMutex>
#include <QDebug>
#include <QDateTime>
#include <QHostInfo>
#include <QSettings>
#include <QLockFile>
#include <QSharedMemory>
#include <QStandardPaths>

/* 保存内存 -- start */
#define ST_FILE_SERVER_URL                          "ST_FILE_SERVER_URL"
#define ST_FILE_SERVER_USER                         "ST_FILE_SERVER_USER"
#define ST_FILE_SERVER_PASSWD                       "ST_FILE_SERVER_PASSWD"
/* 保存内存 -- end */

/* 保存配置文件 -- start */
#define FC_DECRYPT_FLOW_GROUP                       "DecryptionFlow"
/* 保存配置文件 -- end */

/* 系统语言 -- start */
#define SYS_LANGUAGE_CN                             "Chinese"
#define SYS_LANGUAGE_EN                             "English"
/* 系统语言 -- end */

#define LOGIN_STATUS_ONLINE                         "1"
#define LOGIN_STATUS_LOGOUT	                        "0"
#define LOGIN_STATUS_OFFLINE                        "2"
#define CIPHERTEXT_MODE                             "0"         // 密文模式
#define PLAINTEXT_MODE                              "1"         // 明文模式
#define WEBSOCKET_PORT 		                        9099
#define PATH_FILE_POLICY                            "/usr/policy/policy.dat"
#define PATH_CLIENTID                               "/usr/local/.clientid.dat"

bool            is_ubuntu               ();
float           get_ubuntu_version      ();
QByteArray      config_value_enc        (QByteArray& val);
QByteArray      config_value_dec        (QByteArray& val);

Storage* Storage::gInstance = nullptr;


class StoragePrivate
{
#ifdef Q_OS_DARWIN

#define CONFIG_RW_PATH         DATA_RW_PATH
#define SP_CONFIG_FILE         PATH_CONFIG_INI
#define SP_CONFIG_VERSION      DATA_RW_PATH+"/andsec_version.ini"
#define SP_CONFIG_OFFLINE      DATA_RW_PATH+"/andsec_offline.ini"
#define SP_CONFIG_TEMP_FILE    DATA_RW_PATH+"/andsec_flow-config.ini"
#else
#define SP_CONFIG_FILE         "/usr/local/andsec/config.ini"
#define SP_CONFIG_VERSION      "/usr/local/andsec/version.ini"
#define SP_CONFIG_OFFLINE      "/usr/local/andsec/offline.ini"
#define SP_CONFIG_TEMP_FILE    "/usr/local/andsec/flow-config.ini"
#define SP_CONFIG_SANDBOX_NET  "/usr/local/andsec/sandbox-net.conf"
#endif
public:
    ~StoragePrivate();
    StoragePrivate(Storage* obj);

    /* 配置文件读写 */
    void lockConfig();
    void unlockConfig();
    QVariant getConfigValue(const QString& group, const QString& key, const QVariant& defaultValue=QVariant());
    void setConfigValue(const QString& group, const QString& key, const QString& value);

    /* 流程相关配置 */
    void lockFlowConfig();
    void unlockFlowConfig();
    void delFlowConfig(const QString& group, const QString& key);
    QVariant getFlowConfigValue(const QString& group, const QString& key);
    void setFlowConfigValue(const QString& group, const QString& key, const QVariant& value);

    /* 版本相关配置 */
    void lockVersionConfig();
    void unlockVersionConfig();
    void delVersionConfig(const QString& group, const QString& key);
    QVariant getVersionConfigValue(const QString& group, const QString& key);
    void setVersionConfigValue(const QString& group, const QString& key, const QVariant& value);

    /* 离线相关配置 */
    void lockOfflineConfig();
    void unlockOfflineConfig();
    void delOfflineConfigValue(const QString& group, const QString& key);
    QVariant getOfflineConfigValue(const QString& group, const QString& key);
    void setOfflineConfigValue(const QString& group, const QString& key, const QVariant& value);

    /* 沙盒网络例外相关配置 */
    void lockSandboxNetConfig();
    void loadSandboxNetConfig();
    void syncSandboxNetConfig();
    void clearSandboxNetConfig();
    void setSandboxEnable(bool enable);
    void addSandboxNetConfig(const QString& proc, const QString& ip, const QString& port);
    bool checkSandboxNetAllowed(const QString& proc, const QString& ip, const QString& port);
    void unlockSandboxNetConfig();

public:
    Storage*                        q_ptr = nullptr;
    QMutex                          mMutexLocker;
    QSharedMemory*                  mSharedMem = nullptr;
    QSettings*                      mSettings = nullptr;

    QMutex                          mFlowConfigLocker;
    QSettings*                      mFlowConfigSettings = nullptr;

    QMutex                          mVersionConfigLocker;
    QSettings*                      mVersionConfigSettings = nullptr;

    QMutex                          mOfflineConfigLocker;
    QSettings*                      mOfflineConfigSettings = nullptr;

    std::atomic_bool                mIsNeedUpdateWebgui;

    bool                            mHaveProcX;                                 // 是否配有 *
    QMap<QString, QSet<QString>>    mSandboxNetIp;                              // <进程名, ip1|ip2|ip3>, 没有设置，意味着例外
    QMap<QString, QSet<QString>>    mSandboxNetPort;                            // <进程名, port1|port2>,
    QMutex                          mSandboxNetConfigLocker;
};

StoragePrivate::StoragePrivate(Storage *obj)
    : q_ptr(obj),

    mSharedMem(new QSharedMemory(SP_CONFIG_FILE)),
    mSettings(new QSettings(SP_CONFIG_FILE, QSettings::IniFormat)),
    mFlowConfigSettings(new QSettings(SP_CONFIG_TEMP_FILE, QSettings::IniFormat)),
    mVersionConfigSettings(new QSettings(SP_CONFIG_VERSION, QSettings::IniFormat)),
    mOfflineConfigSettings(new QSettings(SP_CONFIG_OFFLINE, QSettings::IniFormat))
{
#ifdef Q_OS_DARWIN
    qDebug() << "Using writableLocation path:" << CONFIG_RW_PATH;
#endif

    if (!mSharedMem->attach(QSharedMemory::ReadWrite)) {
        // TODO:// 后续配置文件太过频繁访问是否要根据此flag实现读写锁类似的功能，以提高性能？
        mSharedMem->create(8);
    }
}

StoragePrivate::~StoragePrivate()
{
    mSharedMem->detach();
    delete mSharedMem;
    delete mSettings;
    delete mFlowConfigSettings;
    delete mVersionConfigSettings;
    delete mOfflineConfigSettings;
}

void StoragePrivate::lockConfig()
{
    mMutexLocker.lock();
    mSharedMem->lock();
}

void StoragePrivate::unlockConfig()
{
    mSharedMem->unlock();
    mMutexLocker.unlock();
}

QVariant StoragePrivate::getConfigValue(const QString &group, const QString &key, const QVariant &defaultValue)
{
    lockConfig();
    mSettings->sync();
    mSettings->beginGroup(group);
    auto val = mSettings->value(key,defaultValue);
    mSettings->endGroup();
    mSettings->sync();
    unlockConfig();

    return val;
}

void StoragePrivate::setConfigValue(const QString &group, const QString &key, const QString &value)
{
    lockConfig();
    mSettings->sync();
    mSettings->beginGroup(group);
    mSettings->setValue(key, value);
    mSettings->endGroup();
    mSettings->sync();
    unlockConfig();
}

void StoragePrivate::lockFlowConfig()
{
    mFlowConfigLocker.lock();
}

void StoragePrivate::unlockFlowConfig()
{
    mFlowConfigLocker.unlock();
}

void StoragePrivate::delFlowConfig(const QString &group, const QString &key)
{
    lockFlowConfig();
    mFlowConfigSettings->sync();
    mFlowConfigSettings->beginGroup(group);
    mFlowConfigSettings->remove(key);
    mFlowConfigSettings->endGroup();
    mFlowConfigSettings->sync();
    unlockFlowConfig();
}

QVariant StoragePrivate::getFlowConfigValue(const QString &group, const QString &key)
{
    lockFlowConfig();
    mFlowConfigSettings->sync();
    mFlowConfigSettings->beginGroup(group);
    QVariant val = mFlowConfigSettings->value(key);
    mFlowConfigSettings->endGroup();
    mFlowConfigSettings->sync();
    unlockFlowConfig();

    return val;
}

void StoragePrivate::setFlowConfigValue(const QString &group, const QString &key, const QVariant& value)
{
    lockFlowConfig();
    mFlowConfigSettings->sync();
    mFlowConfigSettings->beginGroup(group);
    mFlowConfigSettings->setValue(key, value);
    mFlowConfigSettings->endGroup();
    mFlowConfigSettings->sync();
    unlockFlowConfig();
}

void StoragePrivate::lockVersionConfig()
{
    mVersionConfigLocker.lock();
}

void StoragePrivate::unlockVersionConfig()
{
    mVersionConfigLocker.unlock();
}

void StoragePrivate::delVersionConfig(const QString &group, const QString &key)
{
    lockVersionConfig();
    mVersionConfigSettings->sync();
    mVersionConfigSettings->beginGroup(group);
    mVersionConfigSettings->remove(key);
    mVersionConfigSettings->endGroup();
    mVersionConfigSettings->sync();
    unlockVersionConfig();
}

QVariant StoragePrivate::getVersionConfigValue(const QString &group, const QString &key)
{
    lockVersionConfig();
    mVersionConfigSettings->sync();
    mVersionConfigSettings->beginGroup(group);
    QVariant val = mVersionConfigSettings->value(key);
    mVersionConfigSettings->endGroup();
    mVersionConfigSettings->sync();
    unlockVersionConfig();

    return val;
}

void StoragePrivate::setVersionConfigValue(const QString &group, const QString &key, const QVariant &value)
{
    lockVersionConfig();
    mVersionConfigSettings->sync();
    mVersionConfigSettings->beginGroup(group);
    mVersionConfigSettings->setValue(key, value);
    mVersionConfigSettings->endGroup();
    mVersionConfigSettings->sync();
    unlockVersionConfig();
}

void StoragePrivate::lockOfflineConfig()
{
    mOfflineConfigLocker.lock();
}

void StoragePrivate::unlockOfflineConfig()
{
    mOfflineConfigLocker.unlock();
}

void StoragePrivate::delOfflineConfigValue(const QString &group, const QString &key)
{
    lockOfflineConfig();
    mOfflineConfigSettings->sync();
    mOfflineConfigSettings->beginGroup(group);
    mOfflineConfigSettings->remove(key);
    mOfflineConfigSettings->endGroup();
    mOfflineConfigSettings->sync();
    unlockOfflineConfig();
}

QVariant StoragePrivate::getOfflineConfigValue(const QString &group, const QString &key)
{
    lockOfflineConfig();
    mOfflineConfigSettings->sync();
    mOfflineConfigSettings->beginGroup(group);
    QVariant val = mOfflineConfigSettings->value(key);
    mOfflineConfigSettings->endGroup();
    mOfflineConfigSettings->sync();
    unlockOfflineConfig();

    return val;
}

void StoragePrivate::setOfflineConfigValue(const QString &group, const QString &key, const QVariant &value)
{
    lockOfflineConfig();
    mOfflineConfigSettings->sync();
    mOfflineConfigSettings->beginGroup(group);
    mOfflineConfigSettings->setValue(key, value);
    mOfflineConfigSettings->endGroup();
    mOfflineConfigSettings->sync();
    unlockOfflineConfig();
}

void StoragePrivate::lockSandboxNetConfig()
{
    mSandboxNetConfigLocker.lock();
}

void StoragePrivate::loadSandboxNetConfig()
{
#ifdef Q_OS_DARWIN
    //TODO
#else
    lockSandboxNetConfig();

    QFile file(SP_CONFIG_SANDBOX_NET);
    do {
        if (!file.open(QFile::ReadOnly)) {
            break;
        }
        mHaveProcX = false;
        mSandboxNetIp.clear();
        mSandboxNetPort.clear();
        QByteArray line;
        while (!(line = file.readLine()).isEmpty()) {
            line = line.trimmed();
            auto lineD = QString(config_value_dec(line));
            qInfo() << "[SN] " << lineD;
            auto arr = lineD.split("{]");
            if (arr.length() != 3) {
                qInfo() << "[SN] error " << arr;
                continue;
            }
            auto proc = arr[0];
            auto ip = arr[1];
            auto port = arr[2];
            if ("*" == proc) { mHaveProcX = true; }
            mSandboxNetIp[proc] = ip.split("|").toSet();
            mSandboxNetPort[proc] = port.split("|").toSet();
        }
    } while (false);

    file.close();

    unlockSandboxNetConfig();
#endif
}

void StoragePrivate::syncSandboxNetConfig()
{

#ifdef Q_OS_DARWIN
    //TODO
#else
    lockSandboxNetConfig();

    QFile file(SP_CONFIG_SANDBOX_NET);
    do {
        if (!file.open(QFile::ReadWrite | QFile::Truncate)) {
            break;
        }

        auto procs = mSandboxNetIp.keys();
        for (auto& p : procs) {
            auto& ips = mSandboxNetIp[p];
            auto& ports = mSandboxNetPort[p];

            QStringList ip1;
            QStringList port1;

            for (auto ip : ips) {
                ip1 << ip;
            }
            for (auto port : ports) {
                port1 << port;
            }

            auto line = QString("%1{]%2{]%3{]").arg(p, ip1.join("|"), port1.join("|")).toUtf8();
            auto lineE = config_value_enc(line);
            file.write(lineE);
            file.write("\n");
        }
    } while (false);

    file.close();

    unlockSandboxNetConfig();
#endif
}

void StoragePrivate::clearSandboxNetConfig()
{
#ifdef Q_OS_DARWIN
    //TODO
#else
    lockSandboxNetConfig();

    QFile file(SP_CONFIG_SANDBOX_NET);
    do {
        if (!file.open(QFile::ReadWrite | QFile::Truncate)) {
            break;
        }
        mHaveProcX = false;
        mSandboxNetIp.clear();
        mSandboxNetPort.clear();
    } while (false);

    file.close();

    unlockSandboxNetConfig();
#endif
}

void StoragePrivate::setSandboxEnable(bool enable)
{
    lockSandboxNetConfig();

    unlockSandboxNetConfig();
}

void StoragePrivate::addSandboxNetConfig(const QString &proc, const QString &ip, const QString &port)
{
    // 空则退出
    if ((proc.isNull() || proc.isEmpty()) && (ip.isNull() || ip.isEmpty()) && (port.isNull() || port.isEmpty())) {
        return;
    }

    lockSandboxNetConfig();

    auto format = [=] (const QString& str) ->QSet<QString> {
        QString tmp = str.trimmed();
        QSet<QString> ls;

        while (tmp.startsWith("|"))   { tmp = tmp.remove(0, 1); }
        while (tmp.endsWith("|"))     { tmp = tmp.remove(tmp.size() - 1, 1); }

        if (tmp.isNull() || tmp.isEmpty()) {
            ls << "*";
        }
        else {
            auto parr = tmp.split("|");
            int idx = parr.size();
            for (int i = 0; i < idx; ++i) {
                // qInfo() << parr;
                auto ele = parr.at(i).trimmed();
                if (ele.isNull() || ele.isEmpty()) {
                    continue;
                }
                ls << ele;
            }
        }

        return ls;
    };

    bool needUpdate = false;
    auto mip = format(ip);
    auto mproc = format(proc);
    auto mport = format(port);

    for (auto& p : mproc) {
        if (mSandboxNetIp.contains(p)) {
            int curIpNum = mSandboxNetIp[p].count();
            int curPortNum = mSandboxNetPort[p].count();

            mSandboxNetIp[p] += mip;
            mSandboxNetPort[p] += mport;

            if (mSandboxNetIp[p].count() >= curIpNum || mSandboxNetPort[p].count() >= curPortNum) {
                needUpdate = true;
            }
        }
        else {
            QString pp = p;
            if (pp.isNull() || pp.isEmpty()) { pp = "*"; }
            if (pp == "*") {
                mHaveProcX = true;
            }
            mSandboxNetIp[pp] = mip;
            mSandboxNetPort[pp] = mport;
            needUpdate = true;
        }
    }

    unlockSandboxNetConfig();

    if (needUpdate) {
        syncSandboxNetConfig();
    }
}

bool StoragePrivate::checkSandboxNetAllowed(const QString &proc, const QString &ip, const QString &port)
{
    if ((proc.isNull() || proc.isEmpty()) && (ip.isNull() || ip.isEmpty()) && (port.isNull() || port.isEmpty())) {
        return false;
    }

    auto format = [=] (const QString& str) ->QString {
        QString tmp = str.trimmed();
        while (tmp.startsWith("|"))   { tmp = tmp.remove(0, 1); }
        while (tmp.endsWith("|"))     { tmp = tmp.remove(tmp.size() - 1, 1); }

        if (tmp.isNull() || tmp.isEmpty()) {
            tmp = "*";
        }

        return tmp;
    };


    QString pp = format(proc);
    QString ii = format(ip);
    QString oo = format(port);

    // 检查是否有配置
    if (mSandboxNetIp.contains(pp)) {
        auto ips = mSandboxNetIp[pp];
        auto oos = mSandboxNetPort[pp];

        qDebug() << "contain: " << pp << " -- " << ips << " -- " << oos;

        // 针对域名进行转换
        // 不管控 ip, 且端口包含例外
        if (ips.contains("*") && (oos.contains(oo) || oos.contains("*"))) {
            return true;
        }

        // 管控 ip, 依次检测ip
        if (oos.contains(oo)) {
            QRegExp rxp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
            for (auto& ipTT : ips) {
                if (rxp.exactMatch(ipTT)) {
                    if (ipTT == ii) {
                        return true;
                    }
                }
                else {
                    QHostInfo info = QHostInfo::fromName(ipTT);
                    if (QHostInfo::NoError == info.error()) {
                        for (auto& addrTT : info.addresses()) {
                            if (addrTT.toString() == ii) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        else {
            qDebug() << "port " << oo << " not found!";
        }
    }
    else {
        qDebug() << "proc " << pp << " not found!";
    }

    return false;
}

void StoragePrivate::unlockSandboxNetConfig()
{
    mSandboxNetConfigLocker.unlock();
}

Storage *Storage::getInstance()
{
    static bool inited = false;
    if (!inited) {
        static QMutex locker;
        locker.lock();
        if (!gInstance) {
            gInstance = new Storage();
        }
        locker.unlock();
    }

    return gInstance;
}

void Storage::delTokenId()
{
    Q_D(Storage);

    d->setConfigValue("client", "token", "");
}

QString Storage::getTokenId()
{
    Q_D(Storage);

    return d->getConfigValue("client", "token").toString();
}

void Storage::setTokenId(const QString &token)
{
    Q_D(Storage);

    if ("" == token || token.isEmpty()) {
        return;
    }

    d->setConfigValue("client", "token", token);
}

void Storage::delDeviceId()
{
    Q_D(Storage);

    d->setConfigValue("client", "id", "");
}

QString Storage::getDeviceId()
{
    Q_D(Storage);

    return d->getConfigValue("client", "id").toString();
}

void Storage::setDeviceId(const QString &id)
{
    if ("" == id || id.isEmpty()) {
        return;
    }

    Q_D(Storage);

    d->setConfigValue("client", "id", id);
}

void Storage::delFileServerUrl()
{
    QMutexLocker locker(&mLocker);
    mStore.remove(ST_FILE_SERVER_URL);
}

QString Storage::getFileServerUrl()
{
    QMutexLocker locker(&mLocker);
    if (mStore.contains(ST_FILE_SERVER_URL)) {
        return mStore[ST_FILE_SERVER_URL].toString();
    }

    return "";
}

void Storage::setFileServerUrl(const QString &url)
{
    if ("" == url || url.isEmpty()) {
        return;
    }

    QMutexLocker locker(&mLocker);
    mStore[ST_FILE_SERVER_URL] = url;
}

void Storage::delFileServerUser()
{
    QMutexLocker locker(&mLocker);
    mStore.remove(ST_FILE_SERVER_USER);
}

QString Storage::getFileServerUser()
{
    QMutexLocker locker(&mLocker);
    if (mStore.contains(ST_FILE_SERVER_USER)) {
        return mStore[ST_FILE_SERVER_USER].toString();
    }

    return "";
}

void Storage::setFileServerUser(const QString &user)
{
    if ("" == user || user.isEmpty()) {
        return;
    }

    QMutexLocker locker(&mLocker);
    mStore[ST_FILE_SERVER_USER] = user;
}

void Storage::delFileServerPasswd()
{
    QMutexLocker locker(&mLocker);
    mStore.remove(ST_FILE_SERVER_PASSWD);
}

QString Storage::getFileServerPasswd()
{
    QMutexLocker locker(&mLocker);
    if (mStore.contains(ST_FILE_SERVER_PASSWD)) {
        return mStore[ST_FILE_SERVER_PASSWD].toString();
    }

    return "";
}

void Storage::setFileServerPasswd(const QString &passwd)
{
    if ("" == passwd || passwd.isEmpty()) {
        return;
    }

    QMutexLocker locker(&mLocker);
    mStore[ST_FILE_SERVER_PASSWD] = passwd;
}

bool Storage::getPrivilegedDecryptEnable()
{
    Q_D(Storage);
    QString group = QString("%1%2").arg(d->getConfigValue("user", "username").toString());

    return (0 == d->getConfigValue(group, "privileged_decryption").toInt());
}

void Storage::setPrivilegedDecryptEnable(bool enable)
{
    Q_D(Storage);

    QString group = QString("%1%2").arg(d->getConfigValue("user", "username").toString());
    d->setConfigValue(group, "privileged_decryption", enable ? "1" : "0");
}

bool Storage::getManualEncryptIsEnable()
{
    Q_D(Storage);

    return "1" == d->getConfigValue("MouseMenu", "manualEncrypt").toString();
}

void Storage::setManualEncryptEnable(bool enable)
{
    Q_D(Storage);

    d->setConfigValue("MouseMenu", "manualEncrypt", enable ? "1" : "0");
}

bool Storage::getPrivilegedPrintEnable()
{
    Q_D(Storage);
    QString group = QString("%1%2").arg(d->getConfigValue("user", "username").toString());

    return (1 == d->getConfigValue(group, "privileged_print").toInt());
}

void Storage::setPrivilegedPrintEnable(bool enable)
{
    Q_D(Storage);
    QString group = QString("%1%2").arg(d->getConfigValue("user", "username").toString());
    d->setConfigValue(group, "privileged_print", enable ? "1" : "0");
}

bool Storage::getPrivilegedRemoveWatermarkEnable()
{
    Q_D(Storage);
    QString group = QString("%1%2").arg(d->getConfigValue("user", "username").toString());

    return (1 == d->getConfigValue(group, "privileged_remove_watermark").toInt());
}

void Storage::setPrivilegedRemoveWatermarkEnable(bool enable)
{
    Q_D(Storage);
    QString group = QString("%1%2").arg(d->getConfigValue("user", "username").toString());
    d->setConfigValue(group, "privileged_remove_watermark", enable ? "1" : "0");
}

bool Storage::getPrivilegedRestoreEnable()
{
    Q_D(Storage);
    QString group = QString("%1%2").arg(d->getConfigValue("user", "username").toString());
    return (1 == d->getConfigValue(group, "privileged_restore").toInt());
}

void Storage::setPrivilegedRestoreEnable(bool enable)
{
    Q_D(Storage);
    QString group = QString("%1%2").arg(d->getConfigValue("user", "username").toString());
    d->setConfigValue(group, "privileged_restore", enable ? "1" : "0");
}

void Storage::delDecryptionFlowInfo(const QString &flowId)
{
    Q_D(Storage);
    d->delFlowConfig(FC_DECRYPT_FLOW_GROUP, flowId);
}

void Storage::setDecryptionFlowInfo(const QString &flowId, const QStringList &files)
{
    Q_D(Storage);
    d->setFlowConfigValue(FC_DECRYPT_FLOW_GROUP, flowId, files);
}

QString Storage::getServerIp()
{
    Q_D(Storage);

    return d->getConfigValue("server", "address").toString();
}

void Storage::setServerIp(const QString &ip)
{
    Q_D(Storage);

    d->setConfigValue("server", "address", ip);
}

QString Storage::getUiServerIp()
{
    Q_D(Storage);

    auto ip = d->getConfigValue("server", "uiAddress").toString();
    if (ip.isEmpty()) {
        ip = getServerIp();
    }

    return ip;
}

void Storage::setUiServerIp(const QString &ip)
{
    Q_D(Storage);

    d->setConfigValue("server", "uiAddress", ip);
}

int Storage::getServerPort()
{
    Q_D(Storage);

    return d->getConfigValue("server", "port").toInt(0);
}

QString Storage::getServerPortStr()
{
    Q_D(Storage);

    return d->getConfigValue("server", "port").toString();
}

void Storage::setServerPort(int port)
{
    Q_D(Storage);

    d->setConfigValue("server", "port", QString("%1").arg(port));
}

void Storage::setServerPort(const QString &port)
{
    Q_D(Storage);

    d->setConfigValue("server", "port", port);
}

int Storage::getUiServerPort()
{
    Q_D(Storage);

    int port = d->getConfigValue("server", "uiPort").toInt();
    if (0 == port) {
        port = getServerPort();
    }

    return port;
}

QString Storage::getUiServerPortStr()
{
    Q_D(Storage);

    QString port = d->getConfigValue("server", "uiPort").toString();
    if (port.isEmpty()) {
        port = getServerPortStr();
    }

    return port;
}

void Storage::setUiServerPort(const QString &port)
{
    Q_D(Storage);

    d->setConfigValue("server", "uiPort", port);
}

QString Storage::getServerFlag()
{
    Q_D(Storage);

    return d->getConfigValue("server", "serverFlag").toString();
}

void Storage::setServerFlag(const QString &flag)
{
    Q_D(Storage);

    d->setConfigValue("server", "serverFlag", flag);
}

QString Storage::getDriverVersion()
{
    Q_D(Storage);

    return d->getVersionConfigValue("ver", "driver").toString();
}

void Storage::setDriverVersion(const QString &driverVersion)
{
    Q_D(Storage);

    d->setVersionConfigValue("ver", "driver", driverVersion);
}

QString Storage::getClientVersion()
{
    Q_D(Storage);

    return d->getVersionConfigValue("ver", "client").toString();
}

void Storage::setClientVersion(const QString &clientVersion)
{
    Q_D(Storage);

    d->setVersionConfigValue("ver", "client", clientVersion);
}

QString Storage::getClientId()
{
    Q_D(Storage);

    QFile file(PATH_CLIENTID);
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            auto value = file.readAll();
            if (!value.isEmpty()) {
                setClientId(value);
            }
        }
    }

    return d->getConfigValue("client", "id").toString();
}

void Storage::setClientId(const QString &id)
{
    Q_D(Storage);

    d->setConfigValue("client", "id", id);
}

QString Storage::getUserId()
{
    Q_D(Storage);

    return d->getConfigValue("user", "id").toString();
}

void Storage::setUserId(const QString &userId)
{
    Q_D(Storage);

    d->setConfigValue("user", "id", userId);
}


QString Storage::getUserName()
{
    Q_D(Storage);

    return d->getConfigValue("user", "username").toString();
}

QString Storage::getSurName()
{
    Q_D(Storage);

    return d->getConfigValue("user", "surname").toString();
}

void Storage::setSurName(const QString& surName)
{
    Q_D(Storage);

    d->setConfigValue("user", "surname", surName);
}

QString Storage::getDeptName()
{
    Q_D(Storage);

    return d->getConfigValue("user", "deptname").toString();
}

void Storage::setDeptName(const QString& deptname)
{
    Q_D(Storage);

    d->setConfigValue("user", "deptname", deptname);
}

QString Storage::getServerScheme()
{
    Q_D(Storage);
    return d->getConfigValue("server","scheme","https").toString();
}

int Storage::getServerWsPort()
{
    Q_D(Storage);
    return d->getConfigValue("server","websocketport",WEBSOCKET_PORT).toInt();
}

void Storage::setNeedUpdateWebgui()
{
    Q_D(Storage);
    d->mIsNeedUpdateWebgui = true;
}

bool Storage::getIsNeedUpdateWebgui()
{
    Q_D(Storage);

    return d->mIsNeedUpdateWebgui;
}

void Storage::setNoNeedUpdateWebgui()
{
    Q_D(Storage);
    d->mIsNeedUpdateWebgui = false;
}

void Storage::setEnableChangePasswd()
{
    Q_D(Storage);

    if (!getEnableChangePasswd()) {
        setNeedUpdateWebgui();
    }

    d->setConfigValue("TrayMenu", "changePasswd", "1");
}

void Storage::setDisableChangePasswd()
{
    Q_D(Storage);

    if (getEnableChangePasswd()) {
        setNeedUpdateWebgui();
    }

    d->setConfigValue("TrayMenu", "changePasswd", "0");
}

bool Storage::getEnableChangePasswd()
{
    Q_D(Storage);

    return d->getConfigValue("TrayMenu", "changePasswd").toString().toInt() == 1;
}

void Storage::setBackupValue(const QString& bp)
{
    Q_D(Storage);

    QByteArray val = bp.toUtf8();
    auto valE = config_value_enc(val);
    d->setConfigValue("backup", "status", valE);
}

bool Storage::getBackupLocalEnable()
{
    Q_D(Storage);

    auto passwdE = d->getConfigValue("backup", "status").toByteArray();
    auto pe = config_value_dec(passwdE);
    if (!pe.isEmpty()) {
        return pe.contains("1");
    }

    return false;
}

bool Storage::getBackupRpcEnable()
{
    Q_D(Storage);

    auto passwdE = d->getConfigValue("backup", "status").toByteArray();
    auto pe = config_value_dec(passwdE);
    if (!pe.isEmpty()) {
        return pe.contains("2");
    }

    return false;
}

QString Storage::getBackupRpcFileExtName()
{
    Q_D(Storage);

    auto passwdE = d->getConfigValue("backup", "rpcFileType").toByteArray();

    return config_value_dec(passwdE);
}

void Storage::setBackupRpcFileExtName(const QString &extName)
{
    Q_D(Storage);

    QByteArray val = extName.toUtf8();
    auto valE = config_value_enc(val);
    d->setConfigValue("backup", "rpcFileType", valE);
}

void Storage::setBackupRound(const QString &str)
{
    Q_D(Storage);

    QByteArray val = str.toUtf8();
    auto valE = config_value_enc(val);

    d->setConfigValue("backup", "round", valE);
}

bool Storage::getNeedRpcBackup()
{
    Q_D(Storage);

    auto passwdE = d->getConfigValue("backup", "round").toByteArray();
    QString val = config_value_dec(passwdE);
    auto valLst = val.split(";");
    if (valLst.size() != 3) {
        qWarning() << "Not support backup round format...";
        return false;
    }

    auto pv = valLst.at(2).split(":");

    auto lastBackupTime = QDateTime::fromMSecsSinceEpoch(getLastUpdateTime());
    auto lm = lastBackupTime.toString("d").toInt(0);
    auto lw = lastBackupTime.date().dayOfWeek();
    auto lh = lastBackupTime.toString("H").toInt(0);

    auto currentTime = QDateTime::currentDateTime();
    auto cm = currentTime.toString("d").toInt(0);
    auto cw = currentTime.date().dayOfWeek();
    auto ch = currentTime.toString("H").toInt(0);

    auto pm = valLst.at(1).toInt();
    auto pw = pm;
    auto ph = pv.at(0).toInt();

    auto parseDay = [=] () ->bool {
        if (lh > ph || ch < ph) {
            return false;
        }
        return true;
    };

    auto parseWeek = [=] () ->bool {
        if (lw > pw || cw < pw) {
            return false;
        }
        return parseDay();
    };

    auto parseMonth = [=] () ->bool {
        if (lm > pm || cm < pm) {
            return false;
        }
        return parseDay();
    };

    switch(valLst.at(0).toInt(0)) {
    case 1:
        return parseDay();
    case 2:
        return parseWeek();
    case 3:
        return parseMonth();
    }

    return false;
}

void Storage::updateRpcBackupTime()
{
    Q_D(Storage);

    QByteArray val = QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch()).toUtf8();
    auto valE = config_value_enc(val);
    d->setConfigValue("backup", "lastUpdateTime", valE);
}

qint64 Storage::getLastUpdateTime()
{
    Q_D(Storage);

    auto passwd = d->getConfigValue("backup", "lastUpdateTime").toByteArray();

    return QString(config_value_dec(passwd)).toLongLong(0);
}

void Storage::setRpcBackupMaxFileSize(int size, int unit)
{
    Q_D(Storage);

    qint64 maxSize = 0;

    switch (unit) {
    case 1:
        maxSize = size * 1024 * 1024;
        break;
    case 2:
        maxSize = size * 1024 * 1024 * 1024;
        break;
    }

    QByteArray val = QString("%1").arg(maxSize).toUtf8();
    auto valE = config_value_enc(val);
    d->setConfigValue("backup", "maxFileSize", valE);
}

qint64 Storage::getRpcBackupMaxFileSize()
{
    Q_D(Storage);

    auto val = d->getConfigValue("backup", "maxFileSize").toByteArray();

    return QString(config_value_dec(val)).toLongLong();
}

void Storage::clearBackupLocalExtName()
{
    Q_D(Storage);

    d->setConfigValue("backup", "localFileType", "");
}

QRegExp Storage::getBackupLocalExtNameReg()
{
    auto extsT = getBackupLocalExtName().split("|");
    std::for_each(extsT.begin(), extsT.end(), [=] (QString& str) { if (str.startsWith("*.")) str = str.remove(0, 2); });
    QRegExp extName(QString(".(%1)$").arg(extsT.join("|")), Qt::CaseInsensitive);

    return extName;
}

bool Storage::getNeedLocalBackup(const QString &filePath)
{
    return getBackupLocalExtNameReg().indexIn(filePath) > 0;
}

void Storage::addBackupLocalExtName(const QStringList& ls)
{
    Q_D(Storage);
    if (ls.isEmpty() || (ls.count() == 1 && ls.at(0).isEmpty())) { return; }

    auto passwdE = d->getConfigValue("backup", "localFileType").toByteArray();
    QString old = config_value_dec(passwdE);

    auto extList = old.split("|");
    extList += ls;

    QSet<QString> filter(extList.begin(), extList.end());

    auto newStr = QList<QString>(filter.begin(), filter.end()).join("|").toUtf8();
    auto encStr = config_value_enc(newStr);

    d->setConfigValue("backup", "localFileType", encStr);
}

QString Storage::getBackupLocalExtName()
{
    Q_D(Storage);

    auto passwdE = d->getConfigValue("backup", "localFileType").toByteArray();

    return config_value_dec(passwdE);
}


void Storage::setUserName(const QString &usrName)
{
    Q_D(Storage);

    d->setConfigValue("user", "username", usrName);
}

QString Storage::getUserPasswd()
{
    Q_D(Storage);

    auto passwd = d->getConfigValue("user", "userpasswd").toByteArray();
    auto pe = QString(config_value_dec(passwd));
    if (!pe.isEmpty()) {
        return pe;
    }

    // qWarning() << "get passwd error :" << SP_CONFIG_FILE;

    return "";
}

void Storage::setUserPasswd(const QString &passwd)
{
    Q_D(Storage);

    auto passwdE = passwd.toUtf8();
    auto pe = config_value_enc(passwdE);

    if (!pe.isEmpty()) {
        d->setConfigValue("user", "userpasswd", pe);
        return;
    }
    qWarning() << "set passwd error";
}

QString Storage::getCurrentLanguage()
{
    Q_D(Storage);

    return d->getConfigValue("Language", "current_language").toString();
}

bool Storage::getCurrentLanguageIsChinese()
{
    Q_D(Storage);

    return (SYS_LANGUAGE_CN == d->getConfigValue("Language", "current_language").toString());
}

void Storage::setCurrentLanguageChinese()
{
    Q_D(Storage);

    d->setConfigValue("Language", "current_language", SYS_LANGUAGE_CN);
}

void Storage::setCurrentLanguageEnglish()
{
    Q_D(Storage);

    d->setConfigValue("Language", "current_language", SYS_LANGUAGE_EN);
}

bool Storage::getLoginStatusIsOnline()
{
    Q_D(Storage);

    return (LOGIN_STATUS_ONLINE == d->getConfigValue("user", "online_status"));
}

bool Storage::getLoginStatusIsLogout()
{
    Q_D(Storage);

    return (LOGIN_STATUS_LOGOUT == d->getConfigValue("user", "online_status"));
}

bool Storage::getLoginStatusIsOffline()
{
    Q_D(Storage);

    return (LOGIN_STATUS_OFFLINE == d->getConfigValue("user", "online_status"));
}

void Storage::setLoginStatusIsOnline()
{
    Q_D(Storage);
    qInfo() << "Set LoginStatus Is Online";
    d->setConfigValue("user", "online_status", LOGIN_STATUS_ONLINE);
}

void Storage::setLoginStatusIsLogout()
{
    Q_D(Storage);

    d->setConfigValue("user", "online_status", LOGIN_STATUS_LOGOUT);
}

void Storage::setLoginStatusIsOffline()
{
    Q_D(Storage);

    d->setConfigValue("user", "online_status", LOGIN_STATUS_OFFLINE);
}

QString Storage::getLoginStatus()
{
    Q_D(Storage);

    return d->getConfigValue("user", "online_status").toString();
}

QString Storage::getPackageNum()
{
    Q_D(Storage);

    return d->getConfigValue("package", "num").toString();
}

void Storage::setPackageNum(const QString& num)
{
    Q_D(Storage);

    d->setConfigValue("package", "num", num);
}

QString Storage::getOfflineTimeBase()
{
    Q_D(Storage);

    auto group = QString("%1offline").arg(getUserName());

    auto val = d->getOfflineConfigValue(group, "val1").toByteArray();

    return QByteArray::fromBase64(val, QByteArray::Base64Encoding);
}

QString Storage::getOfflineTimeByApply()
{
    Q_D(Storage);

    auto group = QString("%1offline").arg(getUserName());

    auto val = d->getOfflineConfigValue(group, "val2").toByteArray();

    return QByteArray::fromBase64(val, QByteArray::Base64Encoding);
}

QString Storage::getOfflineTimeRemain()
{
    Q_D(Storage);

    auto group = QString("%1offline").arg(getUserName());

    auto val = d->getOfflineConfigValue(group, "val3").toByteArray();
    if (val.isEmpty()) {
        auto val1 = getOfflineTimeBase().toInt();
        auto val2 = getOfflineTimeByApply().toInt();
        setOfflineTimeRemain(val1 + val2);
        val = d->getOfflineConfigValue(group, "val3").toByteArray();
    }

    return QByteArray::fromBase64(val, QByteArray::Base64Encoding);
}

void Storage::resetOfflineTimeRemain()
{
    Q_D(Storage);

    auto group = QString("%1offline").arg(getUserName());
    d->setOfflineConfigValue(group, "val3", "");
}

void Storage::setOfflineTimeBase(const QString &tim)
{
    Q_D(Storage);

    auto group = QString("%1offline").arg(getUserName());

    d->setOfflineConfigValue(group, "val1", tim.toUtf8().toBase64(QByteArray::Base64Encoding));
}

void Storage::setOfflineTimeByApply(const QString &tim)
{
    Q_D(Storage);

    auto group = QString("%1offline").arg(getUserName());

    d->setOfflineConfigValue(group, "val2", tim.toUtf8().toBase64(QByteArray::Base64Encoding));
}

void Storage::setOfflineTimeRemain(const QString &tim)
{
    Q_D(Storage);

    auto group = QString("%1offline").arg(getUserName());

    d->setOfflineConfigValue(group, "val3", tim.toUtf8().toBase64(QByteArray::Base64Encoding));
}

void Storage::setOfflineTimeBase(long tim)
{
    setOfflineTimeBase(QString("%1").arg(tim));
}

void Storage::setOfflineTimeByApply(long tim)
{
    setOfflineTimeByApply(QString("%1").arg(tim));
}

void Storage::setOfflineTimeRemain(long tim)
{
    setOfflineTimeRemain(QString("%1").arg(tim));
}

void Storage::delOfflineTimeByApply(const QString &flowId)
{
    Q_D(Storage);

    auto group = QString("%1offline").arg(getUserName());

    d->delOfflineConfigValue(group, flowId);
}

qint64 Storage::getOfflineTimeByApply(const QString &flowId)
{
    Q_D(Storage);

    auto group = QString("%1offline").arg(getUserName());

    return d->getOfflineConfigValue(group, flowId).toInt();
}

void Storage::saveOfflineTimeByApply(const QString &flowId, qint64 tim)
{
    Q_D(Storage);

    auto group = QString("%1offline").arg(getUserName());

    d->setOfflineConfigValue(group, flowId, QString("%1").arg(tim));
}

QString Storage::getCurrentMode()
{
    Q_D(Storage);

    return d->getConfigValue("mode", "current_mode").toString();
}

void Storage::setCurrentModePlainText()
{
    Q_D(Storage);

    d->setConfigValue("mode", "current_mode", PLAINTEXT_MODE);
}

void Storage::setCurrentModeCipherText()
{
    Q_D(Storage);

    d->setConfigValue("mode", "current_mode", CIPHERTEXT_MODE);
}

void Storage::setControlSaveEnable()
{
    Q_D(Storage);

    d->setConfigValue("control", "savecontrol", "1");
}

void Storage::setControlSaveDisable()
{
    Q_D(Storage);

    d->setConfigValue("control", "savecontrol", "0");
}

bool Storage::getControlSaveIsEnable()
{
    Q_D(Storage);

    return "1" == d->getConfigValue("control", "savecontrol").toString();
}

int Storage::getControlTerminalCopyMax()
{
    Q_D(Storage);

    return d->getConfigValue("control", "tmncontrolMax").toInt(0);
}

void Storage::setControlTerminalCopyEnable()
{
    Q_D(Storage);

    d->setConfigValue("control", "tmncontrol", "1");
}

void Storage::setControlTerminalCopyDisable()
{
    Q_D(Storage);

    d->setConfigValue("control", "tmncontrol", "0");
}

bool Storage::getControlTerminalCopyIsEnable()
{
    Q_D(Storage);

    return "1" == d->getConfigValue("control", "tmncontrol").toString();
}

void Storage::setControlTerminalCopyMax(int max)
{
    Q_D(Storage);

    d->setConfigValue("control", "tmncontrolMax", QString("%1").arg(max));
}

int Storage::getControlCopyMax()
{
    Q_D(Storage);

    return d->getConfigValue("control", "copycontrolMax").toInt(0);
}

void Storage::setControlCopyEnable()
{
    Q_D(Storage);

    d->setConfigValue("control", "copycontrol", "1");
}

void Storage::setControlCopyDisable()
{
    Q_D(Storage);

    d->setConfigValue("control", "copycontrol", "0");
}

bool Storage::getControlCopyIsEnable()
{
    Q_D(Storage);

    return "1" == d->getConfigValue("control", "copycontrol").toString();
}

void Storage::setControlCopyMax(int max)
{
    Q_D(Storage);

    d->setConfigValue("control", "copycontrolMax", QString("%1").arg(max));
}

void Storage::setControlInsertEnable()
{
    Q_D(Storage);

    d->setConfigValue("control", "insertcontrol", "1");
}

void Storage::setControlInsertDisable()
{
    Q_D(Storage);

    d->setConfigValue("control", "insertcontrol", "0");
}

bool Storage::getControlInsertIsEnable()
{
    Q_D(Storage);

    return d->getConfigValue("control", "insertcontrol").toString().toInt(0) ? true : false;
}

bool Storage::getTrayMenuLogoutIsEnable()
{
    Q_D(Storage);

    return d->getConfigValue("TrayMenu", "logout").toString().toInt() == 1;
}

void Storage::setTrayMenuLogoutEnable()
{
    Q_D(Storage);

    if (!getTrayMenuLogoutIsEnable()) {
        setNeedUpdateWebgui();
    }

    d->setConfigValue("TrayMenu", "logout", "1");
}

void Storage::setTrayMenuLogoutDisable()
{
    Q_D(Storage);

    if (getTrayMenuLogoutIsEnable()) {
        setNeedUpdateWebgui();
    }

    d->setConfigValue("TrayMenu", "logout", "0");
}

bool Storage::getLoginIsSavePasswd()
{
    Q_D(Storage);

    return (d->getConfigValue("user", "save_passwd").toString().toInt() == 1);
}

void Storage::setLoginSavePasswdEnable()
{
    Q_D(Storage);

    d->setConfigValue("user", "save_passwd", "1");
}

void Storage::setLoginSavePasswdDisable()
{
    Q_D(Storage);

    d->setConfigValue("user", "save_passwd", "0");
}

bool Storage::getSandboxEnable()
{
#ifndef Q_OS_DARWIN
    Q_D(Storage);

    // 沙盒不支持 ubuntu 18.04 以前的系统
    if (is_ubuntu()) {
        auto ver = get_ubuntu_version();
        if (ver < 18.04) {
            qWarning() << "NOT supported sandbox. Current ubuntu version is: " << ver;
            return false;
        }
    }

    auto passwdE = d->getConfigValue("sandbox", "isEnable").toByteArray();
    auto pe = config_value_dec(passwdE);
    if (!pe.isEmpty()) {
        return (pe.toInt(0) == 1);
    }

    qWarning() << "set sandbox enable error";
#endif

    return false;
}

void Storage::setSandboxEnable(bool enable)
{
    Q_D(Storage);

    QByteArray val = QString("%1").arg((enable ? 1 : 0)).toUtf8();
    auto valE = config_value_enc(val);
    d->setConfigValue("sandbox", "isEnable", valE);
}

void Storage::setSandboxPasswd(const QString &passwd)
{
    Q_D(Storage);

    auto passwdE = passwd.toUtf8();
    auto pe = config_value_enc(passwdE);
    if (!pe.isEmpty()) {
        d->setConfigValue("sandbox", "passwd", pe);
        return;
    }

    qWarning() << "set sandbox passwd error";
}

bool Storage::getSandboxControlNetwork()
{
    Q_D(Storage);

    auto passwdE = d->getConfigValue("sandbox", "isNetControl").toByteArray();
    auto pe = config_value_dec(passwdE);
    if (!pe.isEmpty()) {
        return (pe.toInt(0) == 1);
    }

    qWarning() << "set sandbox network control enable error";

    return false;
}

void Storage::setSandboxControlNetwork(bool enableControl)
{
    Q_D(Storage);

    QByteArray val = QString("%1").arg((enableControl ? 1 : 0)).toUtf8();
    auto valE = config_value_enc(val);
    d->setConfigValue("sandbox", "isNetControl", valE);
}

QByteArray Storage::getMasterKey()
{
    Q_D(Storage);

    auto passwd = d->getConfigValue("client", "masterKey").toByteArray();
    auto pe = config_value_dec(passwd);
    if (!pe.isEmpty()) {
        return pe;
    }

    qWarning() << "get MASTER KEY error!";

    return "";
}

void Storage::setMasterKey(const QByteArray &key)
{
    Q_D(Storage);

    auto passwdE = key;
    auto pe = config_value_enc(passwdE);
    if (!pe.isEmpty()) {
        d->setConfigValue("client", "masterKey", pe);
        return;
    }

    qWarning() << "set master key error";
}

QString Storage::getSandboxPasswd()
{
    Q_D(Storage);

    auto passwd = d->getConfigValue("sandbox", "passwd").toByteArray();
    QString pe = config_value_dec(passwd);
    // qInfo() << "PASSWD[GET]: " << pe;
    if (!pe.isEmpty()) {
        return pe;
    }

    // qWarning() << "get sandbox passwd error :" << SP_CONFIG_FILE;

    return "";
}

void Storage::cleanSandboxNetConf()
{
    Q_D(Storage);

    d->clearSandboxNetConfig();
}

bool Storage::allowSandboxNetConf(const QString &process, const QString &ip, const QString &port)
{
    Q_D(Storage);

    return d->checkSandboxNetAllowed(process, ip, port);
}

void Storage::addSandboxNetConf(const QString &process, const QString &ip, const QString &port)
{
    Q_D(Storage);

    d->addSandboxNetConfig(process, ip, port);
}

void Storage::setPackageNum(int num)
{
    Q_D(Storage);

    d->setConfigValue("package", "num", QString("%1").arg(num));
}

QStringList Storage::getDecryptionFlowInfo(const QString &flowId)
{
    Q_D(Storage);

    auto ret = d->getFlowConfigValue(FC_DECRYPT_FLOW_GROUP, flowId).toStringList();

    qInfo() << ret;

    return ret;
}

Storage::Storage(QObject* parent)
    : QObject(parent), d_ptr(new StoragePrivate(this))
{
    Q_D(Storage);

    QString deviceId = d->getConfigValue("client", "id").toString();
    if (!deviceId.isEmpty()) {
        setDeviceId(deviceId);
    }
}

Storage::~Storage()
{
    delete d_ptr;
}


#include <stdint.h>
#include <stdlib.h>

/**
 * @brief 加解密算法
 */

// AES -- START
#ifndef CBC
#define CBC                                     1
#endif

#ifndef ECB
#define ECB                                     1
#endif

#ifndef CTR
#define CTR                                     1
#endif


#define AES128                                  1
#define AES_BLOCKLEN                            16 // Block length in bytes - AES is 128b block only

#if defined(AES256) && (AES256 == 1)
#define AES_KEYLEN 32
#define AES_KEY_EXP_SIZE                        240
#elif defined(AES192) && (AES192 == 1)
#define AES_KEYLEN 24
#define AES_KEY_EXP_SIZE                        208
#else
#define AES_KEYLEN                              16   // Key length in bytes
#define AES_KEY_EXP_SIZE                        176
#endif

#define Nb 4

#if defined(AES256) && (AES256 == 1)
#define Nk 8
#define Nr 14
#elif defined(AES192) && (AES192 == 1)
#define Nk 6
#define Nr 12
#else
#define Nk 4        // The number of 32 bit words in a key.
#define Nr 10       // The number of rounds in AES Cipher.
#endif

// jcallan@github points out that declaring Multiply as a function
// reduces code size considerably with the Keil ARM compiler.
// See this link for more information: https://github.com/kokke/tiny-AES-C/pull/3
#ifndef MULTIPLY_AS_A_FUNCTION
#define MULTIPLY_AS_A_FUNCTION 0
#endif

typedef struct _AesCtx                          AesCtx;


struct _AesCtx
{
    uint8_t     roundKey[AES_KEY_EXP_SIZE];
#if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
    uint8_t     Iv[AES_BLOCKLEN];
#endif
};

typedef uint8_t state_t[4][4];

void aes_init_ctx(AesCtx* ctx, const uint8_t* key);
#if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
void aes_init_ctx_iv(AesCtx* ctx, const uint8_t* key, const uint8_t* iv);
void aes_ctx_set_iv (AesCtx* ctx, const uint8_t* iv);
#endif

#if defined(ECB) && (ECB == 1)
// buffer size is exactly AES_BLOCKLEN bytes;
// you need only AES_init_ctx as IV is not used in ECB
// NB: ECB is considered insecure for most uses
void aes_ecb_encrypt(const AesCtx* ctx, uint8_t* buf);
void aes_ecb_decrypt(const AesCtx* ctx, uint8_t* buf);

#endif // #if defined(ECB) && (ECB == !)

#if defined(CBC) && (CBC == 1)
// buffer size MUST be mutile of AES_BLOCKLEN;
// Suggest https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7 for padding scheme
// NOTES: you need to set IV in ctx via AES_init_ctx_iv() or AES_ctx_set_iv()
//        no IV should ever be reused with the same key
void aes_cbc_encrypt_buffer(AesCtx* ctx, uint8_t* buf, size_t length);
void aes_cbc_decrypt_buffer(AesCtx* ctx, uint8_t* buf, size_t length);

#endif // #if defined(CBC) && (CBC == 1)

#if defined(CTR) && (CTR == 1)

// Same function for encrypting as for decrypting.
// IV is incremented for every block, and used after encryption as XOR-compliment for output
// Suggesting https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7 for padding scheme
// NOTES: you need to set IV in ctx with AES_init_ctx_iv() or AES_ctx_set_iv()
//        no IV should ever be reused with the same key
void aes_ctr_xcrypt_buffer(AesCtx* ctx, uint8_t* buf, size_t length);

#endif // #if defined(CTR) && (CTR == 1)

// AES -- END

bool is_ubuntu()
{
    QSysInfo info;

    if (info.prettyProductName().toLower().startsWith("ubuntu")) {
        return true;
    }

    return false;
}

float get_ubuntu_version()
{
    if (is_ubuntu()) {
        QSysInfo info;
        return info.productVersion().toFloat();
    }

    return 0;
}

const static char* gsKeyBuf = "This is Andsec encryption key";

QByteArray config_value_enc(QByteArray &val)
{
    QByteArray key(gsKeyBuf, 16);

    AesCtx ctx;
    aes_init_ctx_iv(&ctx, (uint8_t*) key.data(), (uint8_t*) key.data());

    int len = val.length();
    int bufLen = len;
    int padLen = AES_BLOCKLEN - (len % AES_BLOCKLEN);

    if (padLen) {
        bufLen = ((len / AES_BLOCKLEN) + 1) * AES_BLOCKLEN;
    }
    bufLen += AES_BLOCKLEN;

    unsigned char* buf = (unsigned char*) malloc (bufLen);
    if (buf) {
        memcpy(buf, val.constData(), len);
        memset(buf + len, 0x00, padLen);

        aes_cbc_encrypt_buffer(&ctx, buf, bufLen);

        QByteArray encStr = QByteArray((char*) buf, bufLen);
        free(buf);
        return encStr.toBase64(QByteArray::Base64Encoding);
    }

    return "";
}

QByteArray config_value_dec(QByteArray &val)
{
    QByteArray key(gsKeyBuf, 16);

    AesCtx ctx;
    aes_init_ctx_iv(&ctx, (uint8_t*) key.data(), (uint8_t*) key.data());

    QByteArray encStr = QByteArray::fromBase64(val, QByteArray::Base64Encoding);

    int len = encStr.length();
    unsigned char* buf = (unsigned char*) malloc (len);
    if (buf) {
        memcpy(buf, encStr.constData(), len);
        aes_cbc_decrypt_buffer(&ctx, buf, len);

        auto str = QByteArray((char*) buf, len);
        free(buf);
        return str;
    }

    return "";
}



//
// The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
// The numbers below can be computed dynamically trading ROM for RAM -
// This can be useful in (embedded) bootloader applications, where ROM is often limited.
static const uint8_t sbox[256] = {
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

#if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)
static const uint8_t rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
#endif

// The round constant word array, Rcon[i], contains the values given by
// x to the power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
static const uint8_t Rcon[11] = { 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

#define getSBoxValue(num) (sbox[(num)])

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states.
static void keyExpansion(uint8_t* RoundKey, const uint8_t* Key)
{
    unsigned i, j, k;
    uint8_t tempa[4]; // Used for the column/row operations

    // The first round key is the key itself.
    for (i = 0; i < Nk; ++i) {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for (i = Nk; i < Nb * (Nr + 1); ++i) {
        {
            k = (i - 1) * 4;
            tempa[0]=RoundKey[k + 0];
            tempa[1]=RoundKey[k + 1];
            tempa[2]=RoundKey[k + 2];
            tempa[3]=RoundKey[k + 3];

        }

        if (i % Nk == 0) {
            // This function shifts the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

            // Function RotWord()
            {
                const uint8_t u8tmp = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = u8tmp;
            }

            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.

            // Function Subword()
            {
                tempa[0] = getSBoxValue(tempa[0]);
                tempa[1] = getSBoxValue(tempa[1]);
                tempa[2] = getSBoxValue(tempa[2]);
                tempa[3] = getSBoxValue(tempa[3]);
            }

            tempa[0] = tempa[0] ^ Rcon[i/Nk];
        }
#if defined(AES256) && (AES256 == 1)
        if (i % Nk == 4)
        {
            // Function Subword()
            {
                tempa[0] = getSBoxValue(tempa[0]);
                tempa[1] = getSBoxValue(tempa[1]);
                tempa[2] = getSBoxValue(tempa[2]);
                tempa[3] = getSBoxValue(tempa[3]);
            }
        }
#endif
        j = i * 4; k=(i - Nk) * 4;
        RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
        RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
        RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
        RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
    }
}

void aes_init_ctx(AesCtx* ctx, const uint8_t* key)
{
    keyExpansion(ctx->roundKey, key);
}
#if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
void aes_init_ctx_iv(AesCtx* ctx, const uint8_t* key, const uint8_t* iv)
{
    keyExpansion(ctx->roundKey, key);
    memcpy (ctx->Iv, iv, AES_BLOCKLEN);
}
void aes_ctx_set_iv(AesCtx* ctx, const uint8_t* iv)
{
    memcpy (ctx->Iv, iv, AES_BLOCKLEN);
}
#endif

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
static void addRoundKey(uint8_t round, state_t* state, const uint8_t* roundKey)
{
    uint8_t i,j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            (*state)[i][j] ^= roundKey[(round * Nb * 4) + (i * Nb) + j];
        }
    }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void subBytes(state_t* state)
{
    uint8_t i, j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            (*state)[j][i] = getSBoxValue((*state)[j][i]);
        }
    }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
static void shiftRows(state_t* state)
{
    uint8_t temp;

    // Rotate first row 1 columns to left
    temp           = (*state)[0][1];
    (*state)[0][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[3][1];
    (*state)[3][1] = temp;

    // Rotate second row 2 columns to left
    temp           = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp           = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to left
    temp           = (*state)[0][3];
    (*state)[0][3] = (*state)[3][3];
    (*state)[3][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[1][3];
    (*state)[1][3] = temp;
}

static uint8_t xtime(uint8_t x)
{
    return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
static void mixColumns(state_t* state)
{
    uint8_t i;
    uint8_t Tmp, Tm, t;
    for (i = 0; i < 4; ++i) {
        t   = (*state)[i][0];
        Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
        Tm  = (*state)[i][0] ^ (*state)[i][1] ; Tm = xtime(Tm);  (*state)[i][0] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][1] ^ (*state)[i][2] ; Tm = xtime(Tm);  (*state)[i][1] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][2] ^ (*state)[i][3] ; Tm = xtime(Tm);  (*state)[i][2] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][3] ^ t ;              Tm = xtime(Tm);  (*state)[i][3] ^= Tm ^ Tmp ;
    }
}

// Multiply is used to multiply numbers in the field GF(2^8)
// Note: The last call to xtime() is unneeded, but often ends up generating a smaller binary
//       The compiler seems to be able to vectorize the operation better this way.
//       See https://github.com/kokke/tiny-AES-c/pull/34
#if MULTIPLY_AS_A_FUNCTION
static uint8_t Multiply(uint8_t x, uint8_t y)
{
    return (((y & 1) * x) ^
            ((y>>1 & 1) * xtime(x)) ^
            ((y>>2 & 1) * xtime(xtime(x))) ^
            ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
            ((y>>4 & 1) * xtime(xtime(xtime(xtime(x)))))); /* this last call to xtime() can be omitted */
}
#else
#define Multiply(x, y)                                \
(  ((y & 1) * x) ^                              \
 ((y>>1 & 1) * xtime(x)) ^                       \
 ((y>>2 & 1) * xtime(xtime(x))) ^                \
 ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
 ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \

#endif

#if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)
/*
static uint8_t getSBoxInvert(uint8_t num)
{
  return rsbox[num];
}
*/
#define getSBoxInvert(num) (rsbox[(num)])

// MixColumns function mixes the columns of the state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
static void invMixColumns(state_t* state)
{
    int i;
    uint8_t a, b, c, d;
    for (i = 0; i < 4; ++i) {
        a = (*state)[i][0];
        b = (*state)[i][1];
        c = (*state)[i][2];
        d = (*state)[i][3];

        (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}


// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void invSubBytes(state_t* state)
{
    uint8_t i, j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            (*state)[j][i] = getSBoxInvert((*state)[j][i]);
        }
    }
}

static void invShiftRows(state_t* state)
{
    uint8_t temp;

    // Rotate first row 1 columns to right
    temp = (*state)[3][1];
    (*state)[3][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[0][1];
    (*state)[0][1] = temp;

    // Rotate second row 2 columns to right
    temp = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to right
    temp = (*state)[0][3];
    (*state)[0][3] = (*state)[1][3];
    (*state)[1][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[3][3];
    (*state)[3][3] = temp;
}
#endif // #if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)

// Cipher is the main function that encrypts the PlainText.
static void cipher(state_t* state, const uint8_t* RoundKey)
{
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    addRoundKey(0, state, RoundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without MixColumns()
    for (round = 1; ; ++round) {
        subBytes(state);
        shiftRows(state);
        if (round == Nr) {
            break;
        }
        mixColumns(state);
        addRoundKey(round, state, RoundKey);
    }
    // Add round key to last round
    addRoundKey(Nr, state, RoundKey);
}

#if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)
static void invCipher(state_t* state, const uint8_t* RoundKey)
{
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    addRoundKey(Nr, state, RoundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without InvMixColumn()
    for (round = (Nr - 1); ; --round) {
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(round, state, RoundKey);
        if (round == 0) {
            break;
        }
        invMixColumns(state);
    }
}
#endif // #if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)

/*****************************************************************************/
/* Public functions:                                                         */
/*****************************************************************************/
#if defined(ECB) && (ECB == 1)


void aes_ecb_encrypt(const AesCtx* ctx, uint8_t* buf)
{
    // The next function call encrypts the PlainText with the Key using AES algorithm.
    cipher((state_t*)buf, ctx->roundKey);
}

void aes_ecb_decrypt(const AesCtx* ctx, uint8_t* buf)
{
    // The next function call decrypts the PlainText with the Key using AES algorithm.
    invCipher((state_t*)buf, ctx->roundKey);
}
#endif // #if defined(ECB) && (ECB == 1)


#if defined(CBC) && (CBC == 1)
static void xorWithIv(uint8_t* buf, const uint8_t* Iv)
{
    uint8_t i;
    // The block in AES is always 128bit no matter the key size
    for (i = 0; i < AES_BLOCKLEN; ++i) {
        buf[i] ^= Iv[i];
    }
}

void aes_cbc_encrypt_buffer(AesCtx*ctx, uint8_t* buf, size_t length)
{
    size_t i;
    uint8_t *Iv = ctx->Iv;
    for (i = 0; i < length; i += AES_BLOCKLEN) {
        xorWithIv(buf, Iv);
        cipher((state_t*)buf, ctx->roundKey);
        Iv = buf;
        buf += AES_BLOCKLEN;
    }
    /* store Iv in ctx for next call */
    memcpy(ctx->Iv, Iv, AES_BLOCKLEN);
}

void aes_cbc_decrypt_buffer(AesCtx* ctx, uint8_t* buf, size_t length)
{
    size_t i;
    uint8_t storeNextIv[AES_BLOCKLEN];
    for (i = 0; i < length; i += AES_BLOCKLEN) {
        memcpy(storeNextIv, buf, AES_BLOCKLEN);
        invCipher((state_t*)buf, ctx->roundKey);
        xorWithIv(buf, ctx->Iv);
        memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
        buf += AES_BLOCKLEN;
    }
}

#endif // #if defined(CBC) && (CBC == 1)



#if defined(CTR) && (CTR == 1)

/* Symmetrical operation: same function for encrypting as for decrypting. Note any IV/nonce should never be reused with the same key */
void aes_ctr_xcrypt_buffer(AesCtx* ctx, uint8_t* buf, size_t length)
{
    uint8_t buffer[AES_BLOCKLEN];

    size_t i;
    int bi;
    for (i = 0, bi = AES_BLOCKLEN; i < length; ++i, ++bi) {
        /* we need to regen xor compliment in buffer */
        if (bi == AES_BLOCKLEN) {
            memcpy(buffer, ctx->Iv, AES_BLOCKLEN);
            cipher((state_t*)buffer,ctx->roundKey);

            /* Increment Iv and handle overflow */
            for (bi = (AES_BLOCKLEN - 1); bi >= 0; --bi) {
                /* inc will overflow */
                if (ctx->Iv[bi] == 255) {
                    ctx->Iv[bi] = 0;
                    continue;
                }
                ctx->Iv[bi] += 1;
                break;
            }
            bi = 0;
        }

        buf[i] = (buf[i] ^ buffer[bi]);
    }
}

#endif // #if defined(CTR) && (CTR == 1)
