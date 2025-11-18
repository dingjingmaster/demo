/*************************************************************************
> FileName: ipc.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月18日 星期三 17时31分03秒
 ************************************************************************/
#ifndef _IPC_H
#define _IPC_H
#include <stdlib.h>
#include <string.h>


#define IPC_CLIENT_SOCKET_PATH              "/usr/local/andsec/config/andsec_status.sock"
#define IPC_DAEMON_SOCKET_PATH              "/usr/local/andsec/config/sec_daemon.sock"

typedef enum
{
    IPC_TYPE_STATUS_CHANGE = 1,                     // 1  用户状态改变
    IPC_TYPE_UPDATE_POLICY_SUCCESS,                 // 2  更新策略成功
    IPC_TYPE_UPDATE_POLICY_FAILED,                  // 3  更新策略失败
    IPC_TYPE_SHOW_INFO_MESSAGE,                     // 4  显示提示信息
    IPC_TYPE_SHOW_WARN_MESSAGE,                     // 5  显示提示信息
    IPC_TYPE_FM_MENU_DECRYPT_FILE,                  // 6  右键添加文件加入解密申请列表
    IPC_TYPE_DISKSCAN_MESSAGE,                      // 7
    IPC_TYPE_UPDATE_WEBGUI,                         // 8  更新webgui
    IPC_TYPE_SHOW_SPECIAL_HTML_MESSAGE,             // 9  显示特殊 HTML 信息
} IpcClientType;

typedef enum
{
    IPC_TYPE_LOGIN = 1,                             // 1  登录服务器
    IPC_TYPE_LOGOUT,                                // 2  登出
    IPC_TYPE_UPDATE_POLICY,                         // 3  更新策略
    IPC_TYPE_TEST_CONNECT_SERVICE,                  // 4  测试是否可以与服务器连通
    IPC_TYPE_DECRYPT_FILE_SYNC,                     // 5  文件解密
    IPC_TYPE_ENCRYPT_FILE_SYNC,                     // 6  文件加密
    IPC_TYPE_TEST_ENCRYPT_FILE_SYNC,                // 7  是否是密文
    IPC_TYPE_PRIVILEGED_DECRYPT_FILES_SYNC,         // 8  文件特权解密
    IPC_TYPE_PRIVILEGED_ENCRYPT_FILES_SYNC,         // 9  文件特权加密
    IPC_TYPE_CHANGE_USER_PASSWD,                    // 10 修改用户密码
    IPC_TYPE_HAS_LEAK_PROCESS,                      // 11 leak
    IPC_TYPE_KILL_LEAK_PROCESS,                     // 12 杀掉打开密文的进程
    IPC_TYPE_DRIVER_ISWORK,                         // 13 测试驱动是否加载
    IPC_TYPE_CHECK_PRIVILEGED,                      // 14 检查当前账号是否有特权
    IPC_TYPE_UMOUNT_USB_DEVICE,                     // 15 卸载USB设备
    IPC_TYPE_CONFIG_CTRL_INSERT,                    // 16 插入管控
    IPC_TYPE_CONFIG_CTRL_COPY,                      // 17 复制管控
    IPC_TYPE_CONFIG_CTRL_SAVEAS,                    // 18 另存为管控
    IPC_TYPE_CONFIG_CTRL_TERMINAL,                  // 19 终端管控
    IPC_TYPE_CONFIG_MOUSE_MANUAL_ENCRYPT,           // 20 手动加密
    IPC_TYPE_FLOW_DECRYPT_FILES_SAVE_FILES,         // 21 流程解密 —— 保存文件
    IPC_TYPE_ENC_FILE_BACKUP_BACKUP,                // 22 加密文件备份
    IPC_TYPE_ENC_FILE_BACKUP_RENAME,                // 23 加密文件重命名
    IPC_TYPE_ENC_FILE_BACKUP_DELETE,                // 24 加密文件删除
    IPC_TYPE_ENC_FILE_BACKUP_RESTORE,               // 25 加密文件还原
    IPC_TYPE_ENC_FILE_BACKUP_RESTORE_TO_DIR,        // 26 加密文件还原到指定目录

    IPC_TYPE_SANDBOX_IS_MOUNTED                     = 100,  // 100 检测沙盒是否挂载, 不能修改
    IPC_TYPE_SANDBOX_CHECK_IS_OK                    = 101,  // 101 检测沙盒分区是否完成. 不能修改
    IPC_TYPE_SANDBOX_GET_KEY                        = 102,  // 102 获取 sandbox 加密密钥
    IPC_TYPE_SANDBOX_QUIT                           = 103,  // 103 退出沙盒
    IPC_TYPE_SANDBOX_IS_ALLOWED_PROCESS_IP_PORT     = 104,  // 104 沙盒中的进程/IP/Port是否允许访问
    IPC_TYPE_SANDBOX_READ_HOST_FILE                 = 105,  // 105 沙盒中进程通过此 ipc 读取文件内容
    IPC_TYPE_SANDBOX_WRITE_HOST_FILE                = 106,  // 106 沙盒中进程通过此 ipc 写文件内容

    IPC_TYPE_TEST_ENCRYPT_FILE_ASYNC                = 200,  // 200 异步是否是密文
    IPC_TYPE_PRIVILEGED_DECRYPT_FILES_ASYNC,                // 201 异步文件特权解密
    IPC_TYPE_PRIVILEGED_ENCRYPT_FILES_ASYNC,                // 202 异步文件特权加密

} IpcServerType;


typedef struct __attribute__((packed)) _IpcMessage
{
    unsigned int        type;                       // 处理类型：IpcServerType、IpcClientType
    unsigned long       dataLen;
    char                data[];
} IpcMessage;


void sendDataToClient(const char* data, size_t dataLen);
void sendDataToDaemon(const char* data, size_t dataLen);


#endif
