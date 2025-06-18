/*************************************************************************
> FileName: test-html-tooltip.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月18日 星期三 17时39分34秒
 ************************************************************************/
#include "ipc.h"


int main (void)
{
    const char* data = "<a href='https://www.baidu.com'>提示测试</a>";
    const size_t dataLen = strlen (data);

    IpcMessage msg;
    msg.type = IPC_TYPE_SHOW_SPECIAL_HTML_MESSAGE;
    msg.dataLen = dataLen;

    char buffer[4096] = {0};

    memcpy(buffer, &msg, sizeof(IpcMessage));
    memcpy(buffer + sizeof(IpcMessage), data, dataLen);

    sendDataToClient(buffer, dataLen + sizeof (IpcMessage));

    return 0;
}

