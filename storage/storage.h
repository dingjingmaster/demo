#ifndef STORAGE_H
#define STORAGE_H

#include <QMap>
#include <QMutex>
#include <QString>
#include <QVariant>


/**
 * @brief
 * 	保存前端传过来的所有配置信息
 * 	后续从此处提供所有配置信息的访问和设置功能
 *
 * 配置分为两部分：
 * 	1. 系统相关的配置，每次在登陆、注销等有限的几个时间点读取和保存信息，使用 ini 保存配置（后续加入加密功能）
 * 	2. 策略相关配置，策略相关配置相对复杂：存取频繁、多线程操作、多进程操作、多用户操作，后续使用 sqlite + 锁保存（先实现sqlite保存，再引入加密功能）
 *
 * 此类封装上述两种配置
 *
 * @ note 仅作为单例配置相关操作使用，不加任何其它功能
 */
class StoragePrivate;
class Storage : private QObject
{
    Q_OBJECT
public:
    static Storage* getInstance();

    /**
     * @brief Cookies token
     * @note 保存到本地
     */
    void delTokenId();
    QString getTokenId();
    void setTokenId(const QString& token);

    /**
     * @brief Cookies device id
     * @note 保存配置文件
     */
    void delDeviceId();
    QString getDeviceId();
    void setDeviceId(const QString& id);

    /**
     * @brief 升级包下载地址
     * @note 保存内存
     */
    void delFileServerUrl();
    QString getFileServerUrl();
    void setFileServerUrl(const QString& url);

    /**
     * @brief 升级包下载 用户名
     * @note 保存内存
     */
    void delFileServerUser();
    QString getFileServerUser();
    void setFileServerUser(const QString& user);

    /**
     * @brief 升级包下载密码
     * @note 保存内存
     */
    void delFileServerPasswd();
    QString getFileServerPasswd();
    void setFileServerPasswd(const QString& passwd);

    /**
     * @brief 是否支持特权解密
     * @note 线程安全
     */
    bool getPrivilegedDecryptEnable();
    void setPrivilegedDecryptEnable(bool enable);

    /**
     * @brief 是否支持手动加密
     */
    bool getManualEncryptIsEnable();
    void setManualEncryptEnable(bool enable);

    /**
     * @brief 是否支持特权打印
     */
    bool getPrivilegedPrintEnable();
    void setPrivilegedPrintEnable(bool enable);

    /**
     * @brief 是否支持特权移除水印
     */
    bool getPrivilegedRemoveWatermarkEnable();
    void setPrivilegedRemoveWatermarkEnable(bool enable);

    bool getPrivilegedRestoreEnable();
    void setPrivilegedRestoreEnable(bool enable);

    /**
     * @brief 解密流程相关数据保存与获取
     */
    void delDecryptionFlowInfo(const QString& flowId);
    QStringList getDecryptionFlowInfo(const QString& flowId);
    void setDecryptionFlowInfo(const QString& flowId, const QStringList& files);

    /**
     * @brief ip
     * @note 保存到配置文件
     */
    QString getServerIp();
    void setServerIp(const QString& ip);

    /**
     * @brief UI ip
     * @note 保存到配置文件
     */
    QString getUiServerIp();
    void setUiServerIp(const QString& ip);

    /**
     * @brief port
     * @note 保存配置文件
     */
    int getServerPort();
    QString getServerPortStr();
    void setServerPort(int port);
    void setServerPort(const QString& port);

    /**
     * @brief UI port
     * @note 保存配置文件
     */
    int getUiServerPort();
    QString getUiServerPortStr();
    void setUiServerPort(const QString& port);

    /**
     * @brief serverFlag
     */
    QString getServerFlag();
    void setServerFlag(const QString& flag);

    /**
     * @brief driver version
     * @note 保存配置文件
     */
    QString getDriverVersion();
    void setDriverVersion(const QString& driverVersion);

    /**
     * @brief client version
     * @note 保存配置文件
     */
    QString getClientVersion();
    void setClientVersion(const QString& clientVersion);

    /**
     * @brief client id
     * @note 保存配置文件
     */
    QString getClientId();
    void setClientId(const QString& id);

    /**
     * @brief user id
     */
    QString getUserId();
    void setUserId(const QString& userId);

    /**
     * @brief 保存用户名
     */
    QString getUserName();
    void setUserName(const QString& usrName);

    /**
     * @brief 保存用户登录密码
     * @todo 要加密（配置文件里提供加解密接口）
     */
    QString getUserPasswd();
    void setUserPasswd(const QString& passwd);

    /**
     * @brief 获取当前语言
     */
    QString getCurrentLanguage();
    bool getCurrentLanguageIsChinese();
    void setCurrentLanguageChinese();
    void setCurrentLanguageEnglish();

    /**
     * @brief 获取当前客户端状态
     *  LOGIN_STATUS_ONLINE     "1"
     *  LOGIN_STATUS_LOGOUT	    "0"
     *  LOGIN_STATUS_OFFLINE    "2"
     */
    bool getLoginStatusIsOnline();
    bool getLoginStatusIsLogout();
    bool getLoginStatusIsOffline();

    void setLoginStatusIsOnline();
    void setLoginStatusIsLogout();
    void setLoginStatusIsOffline();

    QString getLoginStatus();

    /**
     * @brief -- 未知
     */
    QString getPackageNum();
    void setPackageNum(int num);
    void setPackageNum(const QString& num);

    /**
     * @brief 离线时长相关配置
     * 离线时长：策略默认配置、离线申请获取、当前剩余时间
     */
    QString getOfflineTimeBase();
    QString getOfflineTimeByApply();
    QString getOfflineTimeRemain();
    void resetOfflineTimeRemain();

    void setOfflineTimeBase(const QString& tim);
    void setOfflineTimeByApply(const QString& tim);
    void setOfflineTimeRemain(const QString& tim);

    void setOfflineTimeBase(long tim);
    void setOfflineTimeByApply(long tim);
    void setOfflineTimeRemain(long tim);

    void delOfflineTimeByApply(const QString& flowId);
    qint64 getOfflineTimeByApply(const QString& flowId);
    void saveOfflineTimeByApply(const QString& flowId, qint64 tim);

    /**
     * @brief 获取当前模式
     */
    QString getCurrentMode();
    void setCurrentModePlainText();
    void setCurrentModeCipherText();

    /**
     * @brief 是否控制 save ...
     */
    void setControlSaveEnable();
    void setControlSaveDisable();
    bool getControlSaveIsEnable();

    /**
     * @brief 是否控制终端 拷贝
     */
    int getControlTerminalCopyMax();
    void setControlTerminalCopyEnable();
    void setControlTerminalCopyDisable();
    bool getControlTerminalCopyIsEnable();
    void setControlTerminalCopyMax(int max);

    /**
     * @brief 复制管控
     */
    int getControlCopyMax();
    void setControlCopyEnable();
    void setControlCopyDisable();
    bool getControlCopyIsEnable();
    void setControlCopyMax(int max);

    /**
     * @brief 插入管控
     */
    void setControlInsertEnable();
    void setControlInsertDisable();
    bool getControlInsertIsEnable();

    /**
     * @brief 托盘菜单 -- 登出 是否可用
     */
    bool getTrayMenuLogoutIsEnable();
    void setTrayMenuLogoutEnable();
    void setTrayMenuLogoutDisable();

    /**
     * @brief 保存密码
     */
    bool getLoginIsSavePasswd();
    void setLoginSavePasswdEnable();
    void setLoginSavePasswdDisable();

    /**
     * @brief
     *  #define ANDSEC_FS_MOUNTED_FILE "/usr/local/andsec/fs_mnt.cfg"
     */


    /**
     * @brief 沙盒密钥
     */
    bool getSandboxEnable();
    QString getSandboxPasswd();
    void setSandboxEnable(bool enable);
    void setSandboxPasswd(const QString& passwd);

    /**
     * @brief 沙盒网络管控
     */
    bool getSandboxControlNetwork();
    void setSandboxControlNetwork(bool enableControl);

    /**
     * @brief 加解密密钥
     */
    QByteArray getMasterKey();
    void setMasterKey(const QByteArray& key);

    /**
     * @brief 沙盒策略文件
     *  配置文件中记录的进程、ip、端口是沙盒允许其联网的，其它默认不允许联网。
     *  #define ANDSEC_SANDBOX_CONFIG   "/usr/local/andsec/sandbox-net.conf"
     */
    void cleanSandboxNetConf();
    bool allowSandboxNetConf(const QString& process, const QString& ip, const QString& port);
    void addSandboxNetConf(const QString& process, const QString& ip, const QString& port);

    void setSurName(const QString &surName);
    QString getSurName();

    QString getDeptName();
    void setDeptName(const QString &deptname);

    QString getServerScheme();
    int getServerWsPort();

    /**
     * @brief 检查策略更新后是否需要更新webgui
     */
    void setNeedUpdateWebgui();
    bool getIsNeedUpdateWebgui();
    void setNoNeedUpdateWebgui();

    /**
     * @brief 是否允许修改密码
     */
    void setEnableChangePasswd();
    void setDisableChangePasswd();
    bool getEnableChangePasswd();

    /**
     * @brief 备份相关
     */
    void setBackupValue(const QString& bp);
    bool getBackupLocalEnable();
    bool getBackupRpcEnable();
    void setBackupRound(const QString& str);
    bool getNeedRpcBackup();
    void updateRpcBackupTime();
    qint64 getLastUpdateTime();
    void setRpcBackupMaxFileSize(int size, int unit);
    qint64 getRpcBackupMaxFileSize();

    QString getBackupRpcFileExtName();
    void setBackupRpcFileExtName(const QString& extName);

    void clearBackupLocalExtName();
    QString getBackupLocalExtName();
    QRegExp getBackupLocalExtNameReg();
    bool getNeedLocalBackup(const QString& filePath);
    void addBackupLocalExtName(const QStringList& ls);

private:
    Storage(QObject* parent=nullptr);
    ~Storage();

private:
    StoragePrivate*             d_ptr;
    static Storage*             gInstance;
    QMap<QString, QVariant>     mStore;
    QMutex                      mLocker;

    Q_DECLARE_PRIVATE(Storage)
};

#endif // STORAGE_H
