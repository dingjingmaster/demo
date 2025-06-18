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


#define IPC_CLIENT_SOCKET_PATH              "/usr/local/andsec/start/andsec_status.sock"

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

typedef struct __attribute__((packed)) _IpcMessage
{
    unsigned int        type;                       // 处理类型：IpcServerType、IpcClientType
    unsigned long       dataLen;
    char                data[];
} IpcMessage;


void sendDataToClient(const char* data, size_t dataLen);


#endif
